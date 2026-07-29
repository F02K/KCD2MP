#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatZoneId -- KCD2 WHGame.dll 1.5.6 (kd7u).  4-byte enum.
// -----------------------------------------------
// combat_zone id (directional guard/attack zones of the star combat system). UNLIKE the other
// three combat databases, C_CombatZoneDatabase::Init (sub_180E52640, RVA 0xE52640) registers NO
// zone-name strings at all -- it only allocates 7 empty sorted-index sub-containers and installs
// resolver lambdas. The zone rows (id + names + geometry) come purely from combat_zone.tbl.
//
// [VALUES VERIFIED against KCD2 game data] Ids below are the row ids of
// Libs/Tables/combat/combat_zone.xml extracted from the KCD2 Tables.pak
// (analysis/game_data/combat_tables/, extract_kcd2_combat_tables.py).
// [DELTA vs KCD1] Zone 5 renamed "center" -> "lower" (the thrust/center zone of KCD1's 6-zone
// star becomes a low zone in KCD2's 5-direction layout). The camelCase strings
// "head/upperLeft/..." in the binary (0x183D38xxx) belong to the HUD/reticle subsystem, not
// to C_CombatZoneData.

namespace wh::combatmodule {

enum class E_CombatZoneId : int32_t {
    Undefined  = -1,  // "undefined"
    Head       = 0,   // "head"
    UpperLeft  = 1,   // "upper_left"
    UpperRight = 2,   // "upper_right"
    LowerLeft  = 3,   // "lower_left"
    LowerRight = 4,   // "lower_right"
    Lower      = 5,   // "lower"  (KCD1: "center")
};

}  // namespace wh::combatmodule
