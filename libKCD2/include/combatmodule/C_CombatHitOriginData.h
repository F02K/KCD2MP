#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatHitOriginData -- KCD2 1.5.6 (kd7u). sizeof 0x30.
// -----------------------------------------------
// combat_hit_origin row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_AnimationDatabaseStaticIndexed<C_CombatHitOriginData, S_CombatHitOriginDBData>> =
// C_CombatHitOriginDatabase (vtable 0x183E59E78, 23 slots). Row->Data ctor sub_181614FE4
// (called from ParseRows vtable[10] = sub_181614EF0) zeroes +0x00..+0x13, writes id@0x14,
// name@0x18, mn_tag@0x20, angle_from@0x28, angle_to@0x2C; the in-place push sub_1827996E0
// advances the vector end by 48 -> sizeof PROVEN 0x30. The tag-resolution pass (vtable[16] =
// sub_181615374, stride 48) resolves mn_tag (+0x20) into {+0x00 raw, +0x10 id} via
// sub_180F45F64 -- this class UNUSUALLY keeps its resolved-tag block at the FRONT, before the
// id. Runtime-block internals UNVERIFIED (modeled opaque).
//
// [vs KCD1] No KCD1 reference header; the KCD1 raw-row schema (combat_hit_origin, rowsize 0x20,
// same 5 columns) matches the KCD2 row -- the Data-side resolved-tag block is the KCD2 addition.

namespace wh::combatmodule {

class C_CombatHitOriginData {
public:
    uint8_t          _mnTagRuntime00[0x10];   // +0x00  resolved-tag raw block (layout UNVERIFIED)
    int32_t          _mnTagResolvedId10;      // +0x10  resolved animation-tag id
    int32_t          combat_hit_origin_id;    // +0x14  PK (-1 = "undefined" sentinel row)
    CryStringT<char> combat_hit_origin_name;  // +0x18
    CryStringT<char> mn_tag;                  // +0x20  Mannequin tag (e.g. "ho_forward")
    float            angle_from;              // +0x28  arc start, degrees
    float            angle_to;                // +0x2C  arc end, degrees
};
static_assert(sizeof(C_CombatHitOriginData) == 0x30,
              "C_CombatHitOriginData must be 0x30 (push helper sub_1827996E0 advances 48)");

}  // namespace wh::combatmodule
