#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_SoulCondition -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum, 6 values (SYNTHETIC name).
// -----------------------------------------------
// The readable Health/Stamina/Hunger/... METER enum -- distinct from both E_SoulStat (base stats)
// and E_DerivedStat (computed values). Recovered from its converter pair: name->id table builder
// sub_1801C5400, id->name sub_180C69810. All 6 code strings VERIFIED both directions. The binary
// enum name was not recovered (no RTTI on enums); "E_SoulCondition" is SYNTHETIC, after the KCD1
// condition/meter concept.

namespace wh::rpgmodule {

enum class E_SoulCondition : uint32_t {
    health     = 0,
    stamina    = 1,
    exhaust    = 2,
    hunger     = 3,
    karma      = 4,
    alcoholism = 5,

    Count      = 6,   // SYNTHETIC enumerator
};

}  // namespace wh::rpgmodule
