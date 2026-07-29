# MCM — Mod Configuration Menu

## Three ways to add settings

You can use any combination of these at once; they merge into one list per
mod, keyed by `modId`:

1. **XML**
2. **Lua**
3. **C++**

If the same `modId`+setting `id` is declared by more than one source, the
**first** definition wins for its label/tooltip/range (merge order every
rebuild is XML → C++ → Lua). All three sources can still call `SetValue`
to update an already-declared setting's live
value regardless of which source originally declared it.

### 1. XML (no code)

Ship `Libs/UI/MCM/<anything>.xml` inside your mod's own pak

```xml
<MCMConfig modid="my_mod" name="My Mod">
    <Toggle id="enable" label="Enable Mod" tooltip="Master toggle." default="1"/>
    <Category label="Combat" tooltip="Combat tuning."/>
    <Dropdown id="preset" label="Difficulty Preset" tooltip="..."
              options="Easy|Normal|Hardcore" default="1"/>
    <Slider id="stamina" label="Stamina Cost" tooltip="..."
            min="0" max="200" step="5" default="100" suffix="%"/>
</MCMConfig>
```

- `modid` (root attribute) is the stable id other sources merge against; if
  omitted, MCM falls back to the filename stem.
- Every element needs `id` + `label` (except `Category`, which needs only
  `label`) — an element missing either is skipped with a log warning.
- `Toggle.default` is truthy/falsy (`"1"`/`"0"`, or `"true"`/`"false"`);
  `Dropdown.default` is a 0-based option index; `Slider.min`/`max`/`step`/
  `default` are numbers, `suffix` is shown verbatim after the value.
- XML has no way to express "live value differs from default" — a plain
  XML setting always opens at its `default`, and MCM's own session-carry
  handles edits made *through the menu* (see below). If your setting mirrors
  state you own elsewhere (a CVar, a save value), you need Lua or C++ to
  report it via `SetValue`.
- A malformed XML file is skipped with a log warning, not a crash.

### 2. Lua

A `MCM` global is exposed.

```lua
local function BuildMenu()
    MCM.AddMod("my_mod", "My Mod")
    MCM.AddToggle("my_mod", "enable", "Enable Mod", "Master toggle.", 1)
    MCM.AddCategory("my_mod", "Combat", "Combat tuning.")
    MCM.AddSlider("my_mod", "stamina", "Stamina Cost", "...", 0, 200, 5, 100, "%")
end

MCM.RegisterBuildSettingsListener(BuildMenu)

MCM.RegisterValueChangeListener("my_mod", function(settingId, value)
    if settingId == "enable" then
        g_enabled = (value ~= 0)
    elseif settingId == "stamina" then
        g_staminaCost = value
    end
end)
```

**Calls, all dot-call only** (colon-call shifts the args by one and raises a
script error):

| Call | Params | Notes |
|---|---|---|
| `MCM.RegisterBuildSettingsListener(callback)` | `callback()` | See below |
| `MCM.RegisterValueChangeListener(modId, callback)` | `callback(settingId, value)` | One registration per `modId`; re-registering the same closure replaces the old entry |
| `MCM.AddMod(modId, displayName)` | | build-callback only |
| `MCM.AddCategory(modId, label, tooltip)` | | build-callback only |
| `MCM.AddToggle(modId, id, label, tooltip, default)` | `default` truthy/falsy | build-callback only |
| `MCM.AddDropdown(modId, id, label, tooltip, options, default)` | `options` = `"A\|B\|C"`, `default` = 0-based index | build-callback only |
| `MCM.AddSlider(modId, id, label, tooltip, min, max, step, default, suffix)` | | build-callback only |
| `MCM.SetValue(modId, id, value)` | | build-callback only — see **Default vs. current** below |

**`RegisterBuildSettingsListener`** fires on *every* menu open (not once at
load) — MCM clears its model and rebuilds from scratch each time, so your
callback must re-declare your full settings set every call, not just deltas.
The `MCM.Add*`/`MCM.SetValue` functions only work while a build callback is
actually running; calling them at any other time is a no-op with a warning
log (`WARN: MCM.AddX outside a BuildSettings callback - ignored`).

**`RegisterValueChangeListener`** fires whenever *any* setting under your
`modId` changes — a menu edit, an X-reset (value = the default), or your own
`SetValue` calls do **not** loop back here (see below). Values always arrive
as Lua numbers (the VM is float-Lua): toggle 0/1, dropdown a 0-based index,
slider the raw value.

### 3. C++ (KCSE plugin)

Vendor `api/MCM_API.h` into your plugin (there is no import lib — copy the
header). Register one listener for both message types:

```cpp
#include "MCM_API.h"

static bool g_enabled = true;
static float g_stamina = 100.0f;

KCSE_PLUGIN_LOAD(kcse) {
    MCM::ListenForMessages([](KCSE::Message* msg) {
        if (msg->type == MCM::kMessage_BuildSettings) {
            auto* b = static_cast<MCM::ISettingsBuilder*>(msg->data);
            b->AddMod("my_mod", "My Mod");
            b->AddToggle("my_mod", "enable", "Enable Mod", "Master toggle.", true);
            b->AddSlider("my_mod", "stamina", "Stamina Cost", "...",
                         0.0, 200.0, 5.0, 100.0, "%");
        } else if (msg->type == MCM::kMessage_ValueChanged) {
            auto* v = static_cast<const MCM::ValueChanged*>(msg->data);
            if (std::strcmp(v->modId, "my_mod") != 0) return;
            if (std::strcmp(v->settingId, "enable") == 0) g_enabled = v->value != 0.0;
            else if (std::strcmp(v->settingId, "stamina") == 0) g_stamina = (float)v->value;
        }
    });
    return true;
}
```

This is a **soft dependency**: if MCM.dll isn't installed, nothing ever
broadcasts sender `"MCM"`, so the listener simply never fires and your
compiled-in defaults stand — no null checks needed, no hard link dependency.

- `kMessage_BuildSettings` — fires on every menu open. `msg->data` is an
  `MCM::ISettingsBuilder*`, valid **only** for the duration of this callback;
  call `Add*`/`SetValue` on it synchronously, don't stash the pointer.
- `kMessage_ValueChanged` — fires on every setting mutation under any mod
  (check `v->modId` yourself). `msg->data` is `MCM::ValueChanged*`; its
  pointers are only valid for the callback's duration — copy strings you
  need to keep. A slider drag fires once, on release, not live per-pixel.
- `ISettingsBuilder::AddMod/AddCategory/AddToggle/AddDropdown/AddSlider`
  mirror the Lua calls above exactly (same params, same merge/dedup rules).
- `ISettingsBuilder::SetValue(modId, id, value)` — see below.

## Default vs. current value

Every setting has two numbers: **default** (what an X-reset in the menu
restores) and the value the menu **displays**. `Add*`'s trailing
`default`/`defaultValue` argument sets both at once — for a setting MCM
itself is the only owner of (no CVar, no external state backing it), that's
all you need; MCM carries the user's in-menu edits across every rebuild on
its own via a session snapshot.

If your setting mirrors state *you* own (a CVar, a save value, anything that
can change outside the menu), call `SetValue(modId, id, value)` right after
that setting's `Add*` call, every time your build callback runs:

```lua
MCM.AddSlider("my_mod", "speed", "Speed", "...", 0, 10, 0.5, 1, "x")
MCM.SetValue("my_mod", "speed", GetLiveSpeed())   -- live may differ from the 1 default
```

```cpp
b->AddSlider("my_mod", "speed", "Speed", "...", 0.0, 10.0, 0.5, 1.0, "x");
b->SetValue("my_mod", "speed", g_liveSpeed);
```

This reports "the default is 1, but right now it's actually
`GetLiveSpeed()`" — the slider opens at the live value, but its X-reset
still targets the schema default, not whatever was live. Rules:

- **Only valid during the build callback**, and only for an item that
  `Add*` already added this same round — call it right after, not before.
  Outside a build callback (Lua) or outside the broadcast dispatch (C++,
  where the builder pointer doesn't exist at all) there is no way to call
  it — it does **not** fire your own `RegisterValueChangeListener`/
  `kMessage_ValueChanged` callback (you already know your own state; that
  would just be noise).
- There's no C++/Lua-only asymmetry here beyond call site: `ISettingsBuilder`
  and the Lua `MCM.*` table expose the identical four Add\* + SetValue
  surface.

## Gotchas

- **No persistence** Reopening the menu mid-session shows
  whatever was last set; a game restart shows your declared defaults (or
  whatever your own `SetValue`-reported live state resolves to, if you
  persist it yourself). MCM will never write a config file for you.
- **`RegisterValueChangeListener` is one-per-modId.** Re-registering with a
  different closure for the same `modId` fully replaces the old one, not add
  a second listener — safe to call unconditionally on every script reload.
