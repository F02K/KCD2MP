#pragma once
#include <cstdint>
#include "S_CombatSideDBData.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatSideData -- KCD2 1.5.6 (kd7u). sizeof 0x38.
// -----------------------------------------------
// combat_side lookup row -> Data. NON-polymorphic (no own TD). DB stack:
// C_CombatDatabase<C_AnimationDatabaseStaticIndexed<Data, Row>> over
// C_ObjectTableDatabase<Data, Row> (vtable 0x183ABDE70; StaticIndexed layer vtable 0x183E595F8).
// ParseRows sub_181430474; Row->Data converter sub_18143055C copies id + both strings
// (CryString assign sub_1804F3344) + angle_from/angle_to/combat_action_type_id, and ZEROES
// +0x24..+0x37 (OWORD @+0x24 + dword @+0x34) = the mn-tag runtime-resolution block.
// Vector push sub_181430894 advances end by 56 (copies dword, 2 CryStrings, 3 dwords,
// OWORD @+0x24, dword @+0x34) -> sizeof VERIFIED 0x38.
//
// [vs KCD1] KCD1 was 0x30 (ctor sub_180F99784) with a 0x0C zero-init runtime tail; KCD2 grows
// the tail to the standard 0x14 mn-tag runtime block {0x10 raw + int32 resolved id} -> 0x38.
// Field set/order unchanged. The block's internal layout is UNVERIFIED (opaque, filled at
// mannequin-tag resolution time, same shape as C_CombatAttackTypeData's blocks).

namespace wh::combatmodule {

struct C_CombatSideData {
    int32_t          combat_side_id;         // +0x00
    int32_t          _pad04;                 // +0x04
    CryStringT<char> combat_side_name;       // +0x08
    CryStringT<char> mn_tag;                  // +0x10
    float            angle_from;              // +0x18
    float            angle_to;                // +0x1C
    int32_t          combat_action_type_id;   // +0x20
    uint8_t          _mnTagRuntime24[0x10];   // +0x24  runtime-resolved tag block (semantics UNVERIFIED)
    int32_t          _mnTagResolvedId34;      // +0x34  resolved animation-tag id (zero-init)
};
static_assert(sizeof(C_CombatSideData) == 0x38, "C_CombatSideData must be 0x38");

}  // namespace wh::combatmodule
