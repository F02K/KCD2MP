#pragma once
#include "C_TimedSoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_ModifiableTimedBuff : C_TimedSoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xE8.
// -----------------------------------------------
// RTTI .?AVC_ModifiableTimedBuff@rpgmodule@wh@@  vtable 0x183EF9B78  ctor sub_182BC51F0
// = {C_TimedSoulBuffInstance ctor sub_18046C4A4; set vtable} -- adds NO members. Timed buff
// whose duration/parameters can be re-modified after creation (behavior via vtable overrides;
// the m_duration/m_elapsedTime pair lives in the C_TimedSoulBuffInstance layer). New in KCD2.

namespace wh::rpgmodule {

class C_ModifiableTimedBuff : public C_TimedSoulBuffInstance   // +0x00  (0xE8; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ModifiableTimedBuff;
};
static_assert(sizeof(C_ModifiableTimedBuff) == 0xE8, "C_ModifiableTimedBuff must be 0xE8");

}  // namespace wh::rpgmodule
