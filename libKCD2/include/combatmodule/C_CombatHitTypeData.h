#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatHitTypeData -- KCD2 1.5.6 (kd7u). sizeof 0x30.
// -----------------------------------------------
// combat_hit_type row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_AnimationDatabaseStaticIndexed<C_CombatHitTypeData, S_CombatHitTypeDBData>> =
// C_CombatHitTypeDatabase (vtable 0x183BDEAA0, 23 slots). Row->Data ctor sub_1816331B8 (called
// from ParseRows vtable[10] = sub_181647D70) writes id@0, name@8, mn_tag@0x10; the push
// sub_181647E3C copies those plus the +0x18 oword / +0x28 dword tail and advances the vector end
// by 48 -> sizeof PROVEN 0x30. NOTE: the row-ctor does NOT zero the +0x18..+0x2B tail (it is
// copied indeterminate from the stack temp) -- it only becomes meaningful after the
// tag-resolution pass (vtable[16] = sub_181647E98, stride 48) resolves mn_tag (+0x10) into
// {+0x18 raw, +0x28 id} via sub_180F45F64. Runtime-block internals UNVERIFIED (modeled opaque).
//
// [vs KCD1] No KCD1 reference header; the KCD1 raw-row schema (combat_hit_type, rowsize 0x18,
// same 3 columns) matches the KCD2 row -- the Data-side resolved-tag block is the KCD2 addition.

namespace wh::combatmodule {

class C_CombatHitTypeData {
public:
    int32_t          combat_hit_type_id;    // +0x00  PK (1 = minor, 2 = major)
    int32_t          _pad04;                // +0x04
    CryStringT<char> combat_hit_type_name;  // +0x08
    CryStringT<char> mn_tag;                // +0x10  Mannequin tag (e.g. "hitTypeLow")
    uint8_t          _mnTagRuntime18[0x10]; // +0x18  resolved-tag raw block (layout UNVERIFIED)
    int32_t          _mnTagResolvedId28;    // +0x28  resolved animation-tag id
    int32_t          _pad2C;                // +0x2C
};
static_assert(sizeof(C_CombatHitTypeData) == 0x30,
              "C_CombatHitTypeData must be 0x30 (push helper sub_181647E3C advances 48)");

}  // namespace wh::combatmodule
