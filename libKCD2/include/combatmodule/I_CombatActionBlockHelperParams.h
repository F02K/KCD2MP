#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActionBlockHelperParams -- block-family polymorphic params base
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable pointer only; no data).
// -----------------------------------------------
// RTTI: .?AUI_CombatActionBlockHelperParams@combatmodule@wh@@  (struct; TD 0x184AFA4E0, COL 0x184160E20,
// standalone vtable 0x183A8A4C8).
//
// This is one of the per-family "helper params" interfaces that REPLACED the KCD1 unified reflection
// base I_CombatActorActionParamsReflect (0 TDs in KCD2). Every block-side polymorphic params struct
// derives from it:
//   S_CombatActorActionBlockParams        : I_CombatActionBlockHelperParams            (single base @+0x00)
//   S_CombatActorActionPerfectBlockParams : ..., I_CombatActionBlockHelperParams @+0x10, ...
//   S_CombatActorActionSyncPerfectBlockParams : ..., I_CombatActionBlockHelperParams @+0x10, ...
// (proved by decoding each params struct's RTTI BaseClassArray).
//
// The 18-slot vtable is the params "reflection"/serialisation visitor set. In the abstract base the
// working slots are _purecall (leaf params implement them); the tail four carry a shared default. The
// method names are NOT recoverable from the abstract base (the impls live in the leaf params, e.g.
// S_CombatActorActionBlockParams vtable 0x183A8A430 provides concrete handlers), so they are declared
// _vfN with their slot address. Adds NO data members.
//
// Slot map (base vtable 0x183A8A4C8):
//   [0]      0x182754DD0  ~dtor (scalar deleting)
//   [1..13]  0x181D93E5D  _purecall  (13 pure reflect visitors; leaf-implemented)
//   [14..17] 0x181A730F0  base default (4-byte stub; returns a small default)

namespace wh::combatmodule {

struct I_CombatActionBlockHelperParams {
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActionBlockHelperParams;
    virtual ~I_CombatActionBlockHelperParams() = default;   // [0]  0x182754DD0

    // ---- reflection visitors (pure/_purecall in the game; {} here so the replica params
    //      can be embedded BY VALUE in C_ParamAction -- layout identical) ----
    virtual void _vf1()  {}   // [1]  _purecall
    virtual void _vf2()  {}   // [2]  _purecall
    virtual void _vf3()  {}   // [3]  _purecall
    virtual void _vf4()  {}   // [4]  _purecall
    virtual void _vf5()  {}   // [5]  _purecall
    virtual void _vf6()  {}   // [6]  _purecall
    virtual void _vf7()  {}   // [7]  _purecall
    virtual void _vf8()  {}   // [8]  _purecall
    virtual void _vf9()  {}   // [9]  _purecall
    virtual void _vf10() {}   // [10] _purecall
    virtual void _vf11() {}   // [11] _purecall
    virtual void _vf12() {}   // [12] _purecall
    virtual void _vf13() {}   // [13] _purecall

    // ---- tail slots carrying a shared base default (0x181A730F0) ----
    virtual void _vf14() {}       // [14] 0x181A730F0 (base default)
    virtual void _vf15() {}       // [15] 0x181A730F0 (base default)
    virtual void _vf16() {}       // [16] 0x181A730F0 (base default)
    virtual void _vf17() {}       // [17] 0x181A730F0 (base default)
    // No data members.
};
static_assert(sizeof(I_CombatActionBlockHelperParams) == 0x08,
              "I_CombatActionBlockHelperParams is vtable-only (empty polymorphic base)");

}  // namespace wh::combatmodule
