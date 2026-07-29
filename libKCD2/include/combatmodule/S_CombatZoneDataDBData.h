#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::S_CombatZoneDataDBData -- KCD2 1.5.6 (kd7u). sizeof 0x80.
// -----------------------------------------------
// Raw combat_zone table row (Libs/Tables/combat/combat_zone.xml). Row type of
// C_ObjectTableDatabase<C_CombatZoneData, S_CombatZoneDataDBData> (vtable 0x183ABD8B0).
// ParseRows sub_180BE7CD0 walks the raw buffer with stride 0x80 (`i << 7`) -> sizeof VERIFIED
// 0x80. Column names/offsets/types from the binder sub_1800C7520 (string columns via helper
// sub_180D16794, float via sub_18197AA9C, int via sub_18194FE60, default_zone via
// sub_1819DA954: type 4 = bool, offset 92). Row->Data ctor sub_180BE7DC8 copies every column
// in order into C_CombatZoneData +0x08..+0x80 (strings become CryStringT; the Data's runtime
// tag-block tail is NOT row-fed).
// This binder RESOLVES the column names that C_CombatZoneData.h had marked tentative:
// mn_tag_28=opp_defense, mn_tag_30=start, mn_tag_38=opp_start, mn_tag_40=end,
// mn_tag_48=finishing, _field68=approx_body_subpart_id, mn_tag_70=approx_bone_name,
// _field78=approx_orig_mat_id, _field7C=symetric_combat_zone_id (sic, table spelling),
// _field80=master_strike_combat_zone_id.
//
// [vs KCD1] KCD1's zone row fed a 0xA0 interleaved Data (name,attack,defense,start,end,riposte
// strings). KCD2 has 8 mn-tag columns (opp_defense/opp_start/finishing NEW vs KCD1's riposte)
// plus the approx_*/symetric/master_strike scalar block.

namespace wh::combatmodule {

struct S_CombatZoneDataDBData {
    int32_t     combat_zone_id;                // +0x00  PK
    int32_t     _pad04;                        // +0x04
    const char* combat_zone_name;              // +0x08
    const char* attack_mn_tag;                 // +0x10
    const char* defense_mn_tag;                // +0x18
    const char* opp_defense_mn_tag;            // +0x20
    const char* start_mn_tag;                  // +0x28
    const char* opp_start_mn_tag;              // +0x30
    const char* end_mn_tag;                    // +0x38
    const char* finishing_mn_tag;              // +0x40
    float       radius_from;                   // +0x48
    float       radius_to;                     // +0x4C
    float       angle_from;                    // +0x50
    float       angle_to;                      // +0x54
    int32_t     cursor_idx;                    // +0x58
    bool        default_zone;                  // +0x5C
    uint8_t     _pad5D[3];                     // +0x5D
    int32_t     approx_body_subpart_id;        // +0x60
    int32_t     _pad64;                        // +0x64
    const char* approx_bone_name;              // +0x68
    int32_t     approx_orig_mat_id;            // +0x70
    int32_t     symetric_combat_zone_id;       // +0x74  (table spelling)
    int32_t     master_strike_combat_zone_id;  // +0x78
    int32_t     _pad7C;                        // +0x7C
};
static_assert(sizeof(S_CombatZoneDataDBData) == 0x80, "S_CombatZoneDataDBData must be 0x80");

}  // namespace wh::combatmodule
