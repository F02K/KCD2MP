#pragma once

// -----------------------------------------------
// wh::combatmodule::I_CombatComboManager -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Secondary MI base of C_CombatComboManager (RTTI BCD mdisp 0x10; the combo ctor writes the
// `vftable'_0 variant at object+0x10). Pure interface -- vtable only; slot roles beyond the dtor
// thunk not yet enumerated.

namespace wh::combatmodule {

class I_CombatComboManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatComboManager;
    virtual ~I_CombatComboManager() = default;   // [0] (thunk to the combo manager dtor)
};

}  // namespace wh::combatmodule
