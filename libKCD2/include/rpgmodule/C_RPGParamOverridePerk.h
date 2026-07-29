#pragma once
#include <cstdint>
#include "C_Perk.h"

// -----------------------------------------------
// wh::rpgmodule::C_RPGParamOverridePerk -- perk overriding RPG tuning params
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D53CC8  vtable 0x183BB43C8.  sizeof 0x40 (PROVEN: factory sub_181552C2C
// operator new(64)). Ctor: +0x30 = C_PerkRPGParamOverrideDatabase's sorted-index END pointer
// (qword @db+0xA0; range-begin snapshot [INFERRED]), +0x38 = ctor argument.
// This is the perk type that patches S_RpgParams values while active -- cheat-relevant.

namespace wh::rpgmodule {

class C_RPGParamOverridePerk : public C_Perk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RPGParamOverridePerk;
    S_PerkRPGParamOverride** m_rowsBegin;  // +0x30  = C_PerkRPGParamOverrideDatabase sorted-index end() snapshot (db+0xA0)
    S_PerkRPGParamOverride** m_rowsEnd;    // +0x38  = same end() ptr as m_rowsBegin; empty [begin,end) row range (NOT a ctor arg)
};
static_assert(sizeof(C_RPGParamOverridePerk) == 0x40, "C_RPGParamOverridePerk must be 0x40 (operator new(64))");

}  // namespace wh::rpgmodule
