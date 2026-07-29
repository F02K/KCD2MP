#pragma once
#include <cstdint>
#include "I_CombatZoneData.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatZoneData : I_CombatZoneData -- KCD2 1.5.6 (kd7u). sizeof 0x118.
// -----------------------------------------------
// combat_zone lookup row -> Data. POLYMORPHIC. vtable 0x183A83D70. Template row type is
// S_CombatZoneDataDBData (from C_CombatZoneDatabase::Init sub_180E52640). Row->Data ctor
// sub_180BE7DC8 (RVA 0xBE7DC8); Data->Data copy-ctor sub_180BE8038 copies every field and its
// last write is idCopy @+0x110 -> sizeof 0x118 (0x114 + 8-align). dtor sub_180BE8218.
// attack_zone_id / guard_zone_id / end_guard_zone_id all resolve to this type.
//
// [FUNDAMENTAL vs KCD1] KCD1 (0xA0) interleaved each mn-tag string with its 0x10 runtime block.
// KCD2 (0x118) PACKS the 9 CryStrings first (+0x10..+0x48 and +0x70), keeps the geometry/flag
// scalars in the middle, and GROUPS all runtime-resolved tag blocks into a contiguous tail
// (+0x84..+0x10F = 7 x 0x14). All offsets/sizes ctor+copy-ctor verified; all column names below
// resolved from the combat_zone column binder sub_1800C7520 (data_verticals_2.md).

namespace wh::combatmodule {

class C_CombatZoneData : public I_CombatZoneData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatZoneData;
    ~C_CombatZoneData() override;                              // [0] 0xBE8218
    const CryStringT<char>& GetAttackMnTag() const override;   // [1] 0x1A72EB0
    const CryStringT<char>& GetDefenseMnTag() const override;  // [2] 0x1A73160
    const CryStringT<char>& GetStartMnTag() const override;    // [3] 0x1A749E0
    const CryStringT<char>& GetEndMnTag() const override;      // [4] 0x1A73520

    int32_t          combat_zone_id;              // +0x08
    int32_t          _pad0C;                       // +0x0C
    CryStringT<char> combat_zone_name;            // +0x10
    CryStringT<char> attack_mn_tag;               // +0x18  (interface[1])
    CryStringT<char> defense_mn_tag;              // +0x20  (interface[2])
    CryStringT<char> opp_defense_mn_tag;          // +0x28
    CryStringT<char> start_mn_tag;                // +0x30  (interface[3])
    CryStringT<char> opp_start_mn_tag;            // +0x38
    CryStringT<char> end_mn_tag;                  // +0x40  (interface[4])
    CryStringT<char> finishing_mn_tag;            // +0x48
    // geometry/flags scalar block (names binder-confirmed)
    float            radius_from;                 // +0x50
    float            radius_to;                   // +0x54
    float            angle_from;                  // +0x58
    float            angle_to;                    // +0x5C
    int32_t          cursor_idx;                  // +0x60
    bool             default_zone;                // +0x64
    uint8_t          _pad65[3];                    // +0x65
    int32_t          approx_body_subpart_id;      // +0x68
    int32_t          _pad6C;                       // +0x6C
    CryStringT<char> approx_bone_name;            // +0x70
    int32_t          approx_orig_mat_id;          // +0x78
    int32_t          symetric_combat_zone_id;     // +0x7C  (game's column spelling)
    int32_t          master_strike_combat_zone_id;// +0x80
    // 7 runtime-resolved mn-tag blocks (each 0x14 = {0x10 raw + int32}); filled at tag resolution.
    // Internal per-block layout (Vec4 vs CRC quad + id) UNVERIFIED -- modeled as an opaque block.
    uint8_t          _mnTagRuntimeBlocks[0x8C];   // +0x84 .. +0x10F
    int32_t          _idCopy;                      // +0x110  duplicate of combat_zone_id
    int32_t          _pad114;                      // +0x114
};
static_assert(sizeof(C_CombatZoneData) == 0x118, "C_CombatZoneData must be 0x118");

}  // namespace wh::combatmodule
