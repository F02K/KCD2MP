#pragma once

// -----------------------------------------------
// wh::combatmodule::I_CombatAutomation -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Secondary MI base of C_CombatAutomation (its `vftable'_0 variant is written at object+0x18 by the
// ctor sub_1810EF2DC). Vtable-only; slot roles beyond the dtor thunk not enumerated.

namespace wh::combatmodule {

class I_CombatAutomation {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatAutomation;
    virtual ~I_CombatAutomation() = default;   // [0] (thunk to the automation dtor)
};

}  // namespace wh::combatmodule
