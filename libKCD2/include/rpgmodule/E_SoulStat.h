#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_SoulStat -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum, 10 values.
// -----------------------------------------------
// Recovered from the C_RPGUtils enum converters (singleton @0x1855E7460, vtable 0x183C24F60):
// name->id vtable+0x58 = sub_180BE27B8 (table builder sub_1801C52C0 @0x185330D30), id->name
// sub_180C67780. VERIFIED both directions, all 10 values. The Lua Soul:GetStatLevel handler
// (sub_182CD6820 via thunk sub_182CF7518) rejects id >= 0xA.
//
// This enum indexes S_SoulStatBlock::m_stats[10] (base value = *(uint32*)(block + 0x0C + 8*id),
// i.e. C_Soul + 1388 + 8*id on the live block @C_Soul+0x560).

namespace wh::rpgmodule {

enum class E_SoulStat : uint32_t {
    strength      = 0,
    agility       = 1,
    vitality      = 2,
    speech        = 3,
    vision        = 4,
    hearing       = 5,
    barter        = 6,
    courage       = 7,
    storyProgress = 8,
    prestige      = 9,

    Count         = 10,   // reject threshold in every stat handler (SYNTHETIC enumerator)
};

}  // namespace wh::rpgmodule
