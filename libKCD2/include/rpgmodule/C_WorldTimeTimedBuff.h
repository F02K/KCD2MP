#pragma once
#include "C_TimedSoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_WorldTimeTimedBuff : C_TimedSoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xE8.
// -----------------------------------------------
// RTTI .?AVC_WorldTimeTimedBuff@rpgmodule@wh@@  vtable 0x183EFA7F0  make sub_182BDA748
// (alloc 232 = 0xE8, memset, C_TimedSoulBuffInstance ctor sub_18046C4A4) -- adds NO members.
// Timed buff ticking on WORLD/calendar time rather than the base leaf's real-time clock (clock
// semantics beyond the class name were not decoded -- the timer storage is still the inherited
// double pair @0xD8/0xE0). Per-type default-def sentinel qword_18532B638 (RUNTIME-populated;
// all-zero on disk). Derivatives include C_PoisoningWorldTimeTimedBuff (0xF0). New in KCD2.

namespace wh::rpgmodule {

class C_WorldTimeTimedBuff : public C_TimedSoulBuffInstance   // +0x00  (0xE8; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WorldTimeTimedBuff;
};
static_assert(sizeof(C_WorldTimeTimedBuff) == 0xE8, "C_WorldTimeTimedBuff must be 0xE8");

}  // namespace wh::rpgmodule
