#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_PotEffectAction -- the persistent cauldron fire/boil driver (KCD2 1.5.6, kd7u).
// sizeof 0xD0.
// -----------------------------------------------
// [SYNTHETIC NAME -- no RTTI TD located; coined from its role.]  ctor sub_1818C29B8 (factory
// sub_1818C2910, alloc 0xD0); primary vtable 0x183C36D30.  Created + enqueued into the director by
// the session-start spawner sub_180D5AE50 (Alchemy.OnUse start), alongside the per-kind prop-entity
// resolution into C_Alchemy+0x150.  Runs PERSISTENTLY: the director's advance-while-running path
// calls its per-frame update sub_180738CBC (primary vtable +0x110 = the OnTick slot [34]), which
//   - ramps the fire intensity toward target and writes C_Alchemy+0x320 (sub_1807391E0; bellows
//     raise the target, StrongBoilFireIntensity=2.0),
//   - computes E_AlchemyPotBoilingState from the intensity vs the thresholds below and stores the
//     byte at C_Alchemy+0x440 (observer C_Alchemy+0x448 notified via sub_180393524),
//   - writes the audio boil intensity to C_Alchemy+0x458 (CVar al_boiling_intensity),
//   - calls the boil-accrual writer sub_18073920C (pot bucket[6]: resource+0x14 += dt weak /
//     +0x18 += dt strong).
// Ctor caches the table-entity Lua constants: m_invHourGlassUnitTime = 1/HourGlassUnitTime,
// m_weakBoilTimeout = WeakBoilTimeout (5), m_strongBoilTimeout = StrongBoilTimeout (10).

namespace wh::playermodule {

class C_PotEffectAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    uint64_t _pad68[8];              // +0x68  effect state [contents UNVERIFIED]
    float    m_invHourGlassUnitTime; // +0xA8  1 / HourGlassUnitTime (0.1 for the 10 s default)
    float    m_weakBoilTimeout;      // +0xAC  WeakBoilTimeout table constant (5)
    float    m_strongBoilTimeout;    // +0xB0  StrongBoilTimeout table constant (10)
    uint32_t _padB4;                 // +0xB4
    float    m_const1;               // +0xB8  1.0f (weak-boil intensity threshold input)
    float    m_const2;               // +0xBC  2.0f (strong-boil intensity threshold input)
    float    m_const5;               // +0xC0  5.0f
    uint32_t _padC4;                 // +0xC4
    uint64_t _padC8;                 // +0xC8
};
static_assert(sizeof(C_PotEffectAction) == 0xD0, "C_PotEffectAction must be 0xD0");
static_assert(offsetof(C_PotEffectAction, m_invHourGlassUnitTime) == 0xA8, "rate at 0xA8");

}  // namespace wh::playermodule
