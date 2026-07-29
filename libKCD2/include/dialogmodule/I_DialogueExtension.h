#pragma once

// -----------------------------------------------
// wh::dialogmodule::I_DialogueExtension -- pluggable dialog-minigame extension base
// (KCD2 1.5.6, kd7u).  11-slot abstract interface, vptr-only.
// -----------------------------------------------
// RTTI .?AVI_DialogueExtension@dialogmodule@wh@@ (TD 0x184B4E7C0, abstract, no COL).
// Base of the 4 C_Dialogue*Extension classes (haggle / barber / shield painting / map
// player position). vptr-only: the owning-C_DialogInstance ptr and the `active` bool are
// per-concrete members (they coincide at +0x08/+0x10 in the single-inheritance
// extensions). 11-slot contract from the 4 concrete vtables (0x183E67418..0x183E67CF0):
//   [0] dtor                    [6] OnUiEvent/OnInput (haggle: OnPriceEntered(int, bool))
//   [1] OnStart/OnActivate      [7] GetResult
//   [2] OnEnd/OnDeactivate      [8] open-UI action
//       (resets owner+668=256)  [9] identity thunk
//   [3] OnUpdate (mostly nullsub) [10] close-UI action
//   [4] shared query 0x181A7DE10 (IsFinished? UNVERIFIED)
//   [5] Reset
// Slot names coined from behavior; per-slot signatures vary by extension -- mirrored for
// layout/slot count only.

namespace wh::dialogmodule {

class I_DialogueExtension {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DialogueExtension;
    virtual ~I_DialogueExtension();   // [0]
    virtual void _vf1() = 0;          // [1] OnStart/OnActivate
    virtual void _vf2() = 0;          // [2] OnEnd/OnDeactivate
    virtual void _vf3() = 0;          // [3] OnUpdate
    virtual void _vf4() = 0;          // [4] shared query (IsFinished? UNVERIFIED)
    virtual void _vf5() = 0;          // [5] Reset
    virtual void _vf6() = 0;          // [6] OnUiEvent/OnInput
    virtual void _vf7() = 0;          // [7] GetResult
    virtual void _vf8() = 0;          // [8] open-UI action
    virtual void _vf9() = 0;          // [9] identity thunk
    virtual void _vf10() = 0;         // [10] close-UI action
};
static_assert(sizeof(I_DialogueExtension) == 0x08);

}  // namespace wh::dialogmodule
