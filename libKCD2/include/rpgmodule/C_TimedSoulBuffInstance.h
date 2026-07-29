#pragma once
#include "C_SoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_TimedSoulBuffInstance : C_SoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xE8.
// -----------------------------------------------
// RTTI .?AVC_TimedSoulBuffInstance@rpgmodule@wh@@ (TD 0x184CE4380)  vtable 0x183A38E08
// ctor sub_18046C4A4  allocator sub_18046C3D0.  sizeof HARD-CONFIRMED: alloc(232) +
// _InterlockedAdd(&dword_1851DFB18, 0xE8).
//
// The duration-driven buff leaf. Init (vtable[0] = sub_18063A8F0) widens the buff definition's
// float duration @def+0x30 to double (cvtps2pd) and zeroes the accumulator; Update (vtable[1] =
// sub_1811908DC) adds dt and returns 2 (active) while duration < 0 or elapsed < duration, else
// clamps and returns 0 (expired). IMPORTANT (task-hypothesis correction, disasm-verified): the
// timers are C++ double SECONDS accumulators, NOT CTimeValue int64 ticks.
//
// Sibling leaves with their own RTTI (not composed here): C_ConstantSoulBuffInstance
// (TD 0x184CE4DA0), C_TwoStateSoulBuffInstance (TD 0x184CE4D30), C_BleedingBuffInstance, ... --
// they override Update without timer fields.

namespace wh::rpgmodule {

class C_TimedSoulBuffInstance : public C_SoulBuffInstance {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TimedSoulBuffInstance;
    double m_duration;      // +0xD8  seconds (widened from float def+0x30; < 0 = infinite)
    double m_elapsedTime;   // +0xE0  seconds accumulator (Init zeroes; Update += dt)
};
static_assert(sizeof(C_TimedSoulBuffInstance) == 0xE8, "C_TimedSoulBuffInstance must be 0xE8");

}  // namespace wh::rpgmodule
