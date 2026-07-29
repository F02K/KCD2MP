#include "playermodule/C_Keybinds.h"
#include "Offsets/Offsets.h"

// C_Keybinds engine-function forwarders (KCD2 WHGame.dll 1.5.6 RVAs, kd7u).
// CryStringT<char> params are BY VALUE on purpose: the binary callees destroy those args at
// exit (MSVC callee-destroy, e.g. ChangeInput releases a3/a5, ResetSlotToDefault releases a3
// @0x182E18699) -- a by-value C++ signature reproduces that ABI exactly.

namespace wh::playermodule {

bool C_Keybinds::HasConflicts(E_KeybindController::Type controller) const
{
    // 0x182E16AA4 -- scan m_inputToSuperactions[controller]; true on the first input whose
    // users the conflict graph pairs up.  The Apply/Back gates call this with Keyboard (0).
    using Fn = bool (__fastcall*)(const C_Keybinds*, int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(102) };
    return fn(this, controller);
}

void C_Keybinds::ChangeInput(E_KeybindController::Type controller, CryStringT<char> superaction,
                             int32_t slot, CryStringT<char> input)
{
    // 0x182E181A4 -- gate m_type == Writeable; pendingInputs[slot] = input; move the input
    // between m_inputToSuperactions user lists; Clear/SetConflictsForInput for the old/new
    // input; notify m_pChangeListener.  Empty input = unbind.  Takes effect only after
    // CommitBindings.
    using Fn = void (__fastcall*)(C_Keybinds*, int32_t, CryStringT<char>, int32_t, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(103) };
    fn(this, controller, superaction, slot, input);
}

void C_Keybinds::ResetSlotToDefault(E_KeybindController::Type controller,
                                    CryStringT<char> superaction, int32_t slot)
{
    // 0x182E185BC -- guard superaction exists; ChangeInput(controller, superaction, slot,
    // defaultInputs[slot] or empty).
    using Fn = void (__fastcall*)(C_Keybinds*, int32_t, CryStringT<char>, int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(105) };
    fn(this, controller, superaction, slot);
}

void C_Keybinds::ResetToDefaults(E_KeybindController::Type controller)
{
    // 0x182E18448 -- for every superaction: pending = defaults slot-wise (via ChangeInput),
    // per-record conflict refresh (sub_180F68F64), then CommitBindings(controller, true).
    using Fn = void (__fastcall*)(C_Keybinds*, int32_t);
    static REL::Relocation<Fn> fn{ REL::ID(104) };
    fn(this, controller);
}

void C_Keybinds::CommitBindings(E_KeybindController::Type controller, bool saveProfile)
{
    // 0x180F68DC8 -- unregister the applied set from the action maps (sub_180A111BC ..., 0),
    // applied = copy(pending), register (..., 1); saveProfile also persists to the user
    // profile.  This is what the settings-page Apply/Back commit runs (sub_182BA6720 calls
    // it with (Keyboard, true)).
    using Fn = void (__fastcall*)(C_Keybinds*, int32_t, bool);
    static REL::Relocation<Fn> fn{ REL::ID(42) };
    fn(this, controller, saveProfile);
}

void C_Keybinds::SetConflictsForInput(E_KeybindController::Type controller, CryStringT<char> input)
{
    // 0x182E18AC4 -- cross-check the input's users against m_conflictGraph; SET the UI
    // conflict flag (m_pChangeListener->SetConflict) on every conflicting pair.
    using Fn = void (__fastcall*)(C_Keybinds*, int32_t, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(106) };
    fn(this, controller, input);
}

void C_Keybinds::ClearConflictsForInput(E_KeybindController::Type controller, CryStringT<char> input)
{
    // 0x182E19384 -- same walk as SetConflictsForInput; CLEAR the UI conflict flags.
    using Fn = void (__fastcall*)(C_Keybinds*, int32_t, CryStringT<char>);
    static REL::Relocation<Fn> fn{ REL::ID(107) };
    fn(this, controller, input);
}

}  // namespace wh::playermodule
