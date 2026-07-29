#pragma once
#include <functional>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::playermodule::I_ActionSets -- the contextual-action registration interface
// (KCD2 WHGame.dll 1.5.6, kd7u).  Secondary base of C_ActionSets at +0x08.
// -----------------------------------------------
// RTTI .?AVI_ActionSets@playermodule@wh@@; live vtable 0x183F60C88, COL 0x18443B358 (coloff 8) --
// exactly 17 slots, ALL validated as function starts (wh:: type, not interfuscator-shuffled).
// Reach: `*(C_PlayerModule+0x60)` IS this subobject (C_PlayerModule ctor stores C_ActionSets+8).
// Full RE: analysis/alchemy_re/action_registration_re.md.
//
// Registration model (one-shot, NEVER per-frame): RegisterAction inserts the callback into the
// DURABLE context-keyed registry (C_ActionSets+0x10) and, when bindLive is set, binds it against
// the live IActionMapManager maps.  Context enable/disable only rebuilds the ACTIVE dispatch
// filter (+0x50) from the registry -- callbacks persist.  Duplicate registration = idempotent
// move-assign overwrite.  The action NAME must already exist in the loaded action-map profile
// (defaultProfile.xml actionmap) -- registration binds, it does not create actions.
// Callbacks are NULLARY std::function<void()> -- dispatch passes no activation mode/value; all
// state is captured at registration.  Dispatch path: input -> C_ActionHintManager -> hint functor
// (sub_181FFEF20) -> C_ActionSets::TriggerAction (0x18085A360) -> functor _Do_call().
//
// Slots [4]-[8]/[10]/[12]/[14]/[15] are typed-capture register overloads (void*/int/float/char/
// by-value-functor variants, same registry machinery); only slots with pinned arity are declared
// callable.  [13] resolved: the CryString-typed channel is the row's HOLD-INFO toast text (impl
// 0x1809F22DC: binder impl-vt 0x183F5FDF0, hint passed BY VALUE -- MSVC callee-destroys); [12] is
// the float twin (impl-vt 0x183F5FD80), value semantics unresolved.
//
// HELPBAR/DISPATCH MODEL (decompiled 0x180A13558/0x1808E65A0/0x18085AA10/0x18085A360): init [1]
// parses libs/config/defaultActionHelp.xml into the +0x10 registry (actionmap -> alternative ->
// 0xE0 action row: action@0, map@8, priority@16, type@20 (2=Hold), text@32, enabled?@40,
// visible@41, order@44, hold_duration@48, hold_info@56, enabled@64, disable_reason@72,
// CALLBACK FUNCTOR@88, aux functor@152).  RegisterAction writes the functor into that row;
// enabling an actionmap (event via OnActionMapEvent 0x18085B2B4) copies the rows of the selected
// alternative into the ACTIVE map +0x50 and installs hint-manager rows (functor at hintrow+256)
// whose trigger wrap (0x181FFEF20) calls TriggerAction -> ACTIVE-map row functor.  A help-XML row
// is REQUIRED: rows create the registry nodes -- registering an action with no help row binds
// nothing (lookup-miss is silently skipped).
// Hold timing (in-game verified): the PROFILE's holdTriggerDelay = how long the key must be
// pressed before hold counting starts; the help row's hold_duration = the actual ring duration.

namespace wh::playermodule {

class I_ActionSets {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ActionSets;

    virtual ~I_ActionSets() = default;                          // [0] adjustor -> C_ActionSets dtor 0x182E14244
    virtual void _vf1() = 0;                                    // [1] 0x180C3D5B4 Init: loads libs/config/defaultActionHelp.xml + AddActionMapEventListener [arity UNVERIFIED]
    virtual void _vf2() = 0;                                    // [2] 0x180DA23CC RefreshActionHelpText(mode) [arity UNVERIFIED]
    // [3] 0x18085A5D4 -- nested registry lookup (context -> action present?)
    virtual bool IsRegistered(const CryStringT<char>& context, const CryStringT<char>& action) = 0;
    // [4] 0x180A26028 -- bool-typed register overload; the minigames' enable/disable-action call
    // (live site sub_1809F1F2C @0x1809F1FBB: (ctx, action, enabled, 1))
    virtual void SetActionEnabled(const CryStringT<char>& context, const CryStringT<char>& action,
                                  bool enabled, char flag) = 0;
    // [5] 0x1803F6EBC -- show/hide the action's helpbar row (writes row visible@+41; invoke
    // 0x181FFEEF0).  Refresh is live: any channel change -> sub_18085AABC -> hint-manager rebuild.
    // CAVEAT: visibility only gates the final UI push (0x1808E6C7A).  An INVISIBLE row still
    // occupies its key in the helpbar's candidate list -- the conflict filter (0x181FFD960)
    // drops any later same-type row sharing a key binding (sub_1806480E8) BEFORE visibility is
    // considered, and hidden rows still dispatch.  Swapping two same-key rows therefore needs
    // full uninstall of the inactive one: the hint-install gate (0x1808E6A61) is
    // (enabled@64 || visible@41 || disable_reason@72 nonempty) -- drive enabled+visible false
    // together to remove a row from both display and dispatch, true to reinstall it.
    virtual void SetActionVisible(const CryStringT<char>& context, const CryStringT<char>& action,
                                  char visible, char bindLive) = 0;
    virtual void _vf6() = 0;                                    // [6] 0x181FFE160 register overload (ctx,action,functor,a5,a6)
    // [7] 0x180560D54 -- set the row's disable_reason text (writes @+72; invoke 0x181FFED50).
    // Same by-value/callee-destroys CryString ABI as SetActionHoldInfo.
    virtual void SetActionDisableReason(const CryStringT<char>& context, const CryStringT<char>& action,
                                        CryStringT<char> reason, char bindLive) = 0;
    virtual void _vf8() = 0;                                    // [8] 0x182E18D64 int channel -> row dword @+24 [role UNRESOLVED]
    // [9] 0x18085A96C -> core 0x18085A98C -- THE register: insert into the durable registry;
    // bindLive=1 also resolves the context against IActionMapManager so it dispatches immediately.
    // The game passes the std::function by pointer; identical ABI to this reference.  const&:
    // the callee only COPIES from it (registry assign 0x1808E71DC), so temporaries/lambdas bind.
    virtual void RegisterAction(const CryStringT<char>& context, const CryStringT<char>& action,
                                const std::function<void()>& callback, char bindLive) = 0;
    virtual void _vf10() = 0;                                   // [10] 0x180560FAC RegisterAction twin (internal arg6=1, observationally identical -- use [9])
    // [11] 0x18085A914 -- unbind one (context, action): registers an empty functor over the slot
    virtual void UnregisterAction(const CryStringT<char>& context, const CryStringT<char>& action,
                                  char flag) = 0;
    // [12] 0x1809F2540 -- set the row's hold-ring duration in seconds (writes float @+48; invoke
    // 0x181FFE870) -- the runtime twin of the help-row hold_duration attribute.
    virtual void SetActionHoldDuration(const CryStringT<char>& context, const CryStringT<char>& action,
                                       float seconds, char bindLive) = 0;
    // [13] 0x1809F22DC -- set the action's HOLD-INFO TOAST text (the CryString-typed channel;
    // maps to the defaultActionHelp.xml row's hold_info attr): the center-screen message shown on
    // hold, used by the game to explain disabled actions (e.g. @dlg_alch_resettable_info).  It is
    // NOT the helpbar label -- that is the help row's `text`.  Empty string clears it.
    // `holdInfo` is BY VALUE: under the MSVC x64 ABI the caller materializes a
    // temporary and the CALLEE runs its destructor (impl tail-releases the param on every path
    // @0x1809F235F; game callers construct and never release -- sub_1809F1F2C @0x1809F1FFB..2026).
    // Do NOT redeclare as a reference: the callee-side release would double-free a caller-owned
    // string (or force leaking a heap temp to compensate).
    virtual void SetActionHoldInfo(const CryStringT<char>& context, const CryStringT<char>& action,
                                   CryStringT<char> holdInfo, char flag) = 0;
    virtual void _vf14() = 0;                                   // [14] 0x180D43554 register the SECONDARY functor (row @+152, via 0x1808E72E0)
    virtual void _vf15() = 0;                                   // [15] 0x182E18CC4 char channel -> row byte @+21 [role UNRESOLVED]
    // [16] 0x1808BFFFC -- bulk-clear every callback registered under a context
    virtual void UnregisterContext(const CryStringT<char>& context) = 0;
};
static_assert(sizeof(I_ActionSets) == 8, "I_ActionSets is a vptr-only interface");

}  // namespace wh::playermodule
