#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActionAttackHelperParams -- polymorphic base of the attack-family
// action params (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI: .?AUI_CombatActionAttackHelperParams@combatmodule@wh@@  (TD 0x184AFAA50; abstract -- no
// standalone vtable; its vtable materialises inside the concrete S_CombatActorActionXxxParams).
// Declared with `struct` (the mangled `U` prefix) even though polymorphic.
//
// This is the KCD2 replacement for KCD1's unified `I_CombatActorActionParamsReflect` reflection base
// (which is GONE in KCD2 -- 0 TDs). It is inherited (single base, +0x00) by exactly the 3 polymorphic
// attack params structs, proved by their RTTI base arrays (each numBaseClasses=2 = {self, this}):
//   S_CombatActorActionAttackParams       (vt 0x183A61540)
//   S_CombatActorActionStaticAttackParams (vt 0x183E3DFB0)  -- adds NO data (is exactly this base, 0x10)
//   S_CombatActorActionSyncAttackParams   (vt 0x183A8AEF0)
//
// The vtable has 23 slots. Attack & StaticAttack share byte-identical handlers for slots [1..22]
// (only the slot-[0] deleting dtor differs), so those 22 are the base-provided getters that read the
// embedded attack-data candidate m_pAttackData; SyncAttack overrides most of them. Method names are
// not statically recoverable from the abstract base -- declared `_vfN` with the default (Attack/Static)
// handler address per slot.
//
// m_pAttackData (+0x08): the selected attack-data candidate every getter dereferences. Written to 0 by
// each attack action's base ctor (obj+0x58); set to the chosen candidate by the creation site
// (e.g. the StaticAttack factory sub_180D4AEB0 stores it into obj+0x58). Exact pointee UNVERIFIED
// (KCD1 called it C_CombatActionAttackData*); typed as the fwd-declared candidate below.

namespace wh::combatmodule {

class C_CombatAttackData;   // selected attack-data candidate (UNVERIFIED exact type; data-class wave)

struct I_CombatActionAttackHelperParams {
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActionAttackHelperParams;
    // ---- 23-slot vtable (addresses = the default Attack/StaticAttack impls) ----
    // (bodies are {} not =0: the game class IS abstract, but C_ParamAction embeds the derived
    //  params BY VALUE, so the replica must be instantiable; layout is identical either way)
    virtual ~I_CombatActionAttackHelperParams() = default;  // [0]  0x182754ED8 (Attack) / 0x182754F68 (Static)
    virtual void _vf1()  {}   // [1]  0x181A7D2D0
    virtual void _vf2()  {}   // [2]  0x181A7D6D0
    virtual void _vf3()  {}   // [3]  0x181A7D270
    virtual void _vf4()  {}   // [4]  0x181A7D250
    virtual void _vf5()  {}   // [5]  0x181A7D230
    virtual void _vf6()  {}   // [6]  0x181A7D280
    virtual void _vf7()  {}   // [7]  0x181A7D620
    virtual void _vf8()  {}   // [8]  0x181A7D600
    virtual void _vf9()  {}   // [9]  0x181A7D5F0
    virtual void _vf10() {}   // [10] 0x181A7D2B0
    virtual void _vf11() {}   // [11] 0x181A7D200
    virtual void _vf12() {}   // [12] 0x181A7D360
    virtual void _vf13() {}   // [13] 0x181A7CFD0
    virtual void _vf14() {}   // [14] 0x181A7D320
    virtual void _vf15() {}   // [15] 0x181A7D300
    virtual void _vf16() {}   // [16] 0x181A7D2F0
    virtual void _vf17() {}   // [17] 0x181A7D6E0
    virtual void _vf18() {}   // [18] 0x181A7D7A0
    virtual void _vf19() {}   // [19] 0x181A7D740
    virtual void _vf20() {}   // [20] 0x181A7D760
    virtual void _vf21() {}   // [21] 0x181A7D710
    virtual void _vf22() {}   // [22] 0x181A7D040

    // ---- data ----
    C_CombatAttackData* m_pAttackData;   // +0x08  selected candidate (init 0); every getter reads it
};
static_assert(sizeof(I_CombatActionAttackHelperParams) == 0x10,
              "I_CombatActionAttackHelperParams = vtable + m_pAttackData");

}  // namespace wh::combatmodule
