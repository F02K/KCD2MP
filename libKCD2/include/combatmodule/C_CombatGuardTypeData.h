#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatGuardTypeData -- KCD2 1.5.6 (kd7u). sizeof 0x58.
// -----------------------------------------------
// combat_guard_type row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_AnimationDatabaseStaticIndexed<C_CombatGuardTypeData, S_CombatGuardTypeTableRow>> =
// C_CombatGuardTypeDatabase (vtable 0x183D28458, 23 slots; MI subobject @+0xE8, vtable
// 0x183B501A8). Row->Data ctor sub_1814CC548 (called from ParseRows vtable[10] = sub_1814CC470)
// writes id@0, name@8, mn_tag@0x10, zeroes +0x18..+0x2B, end_mn_tag@0x30, zeroes +0x38..+0x4B,
// then the 5 flag bytes @+0x4C..+0x50. The in-place push sub_1814CC61C copies the same fields and
// advances the vector end by 88 -> sizeof PROVEN 0x58. The tag-resolution pass (vtable[16] =
// sub_180F45EB0, stride 88, skips id -1) resolves mn_tag (+0x10) into {+0x18 raw, +0x28 id} and
// end_mn_tag (+0x30) into {+0x38 raw, +0x48 id} via sub_180F45F64. Runtime-block internals
// UNVERIFIED (modeled opaque).
//
// [vs KCD1] KCD1 was 0x40 (KCD1/RE/include/combatmodule/C_CombatGuardTypeData.h): same
// interleaved string+runtime shape but with a 0x10/0x0C runtime pair and ONLY move_restriction.
// KCD2 widens each runtime area to {0x10 raw + int32 resolved id} and appends 4 new bool columns
// (sync_guard, has_auto_transition, is_asymmetric, hidden_combat_ui) -> 0x58.

namespace wh::combatmodule {

class C_CombatGuardTypeData {
public:
    int32_t          combat_guard_type_id;    // +0x00  PK (-1 = "none" sentinel row)
    int32_t          _pad04;                  // +0x04
    CryStringT<char> combat_guard_type_name;  // +0x08
    CryStringT<char> mn_tag;                  // +0x10  guard-enter Mannequin tag
    uint8_t          _mnTagRuntime18[0x10];   // +0x18  resolved-tag raw block (layout UNVERIFIED)
    int32_t          _mnTagResolvedId28;      // +0x28  resolved animation-tag id
    int32_t          _pad2C;                  // +0x2C
    CryStringT<char> end_mn_tag;              // +0x30  guard-exit Mannequin tag
    uint8_t          _endMnTagRuntime38[0x10];// +0x38  resolved-tag raw block (layout UNVERIFIED)
    int32_t          _endMnTagResolvedId48;   // +0x48  resolved animation-tag id
    bool             move_restriction;        // +0x4C
    bool             sync_guard;              // +0x4D  (table default false)
    bool             has_auto_transition;     // +0x4E  (table default TRUE)
    bool             is_asymmetric;           // +0x4F  (table default false)
    bool             hidden_combat_ui;        // +0x50  (table default false)
    uint8_t          _pad51[7];               // +0x51
};
static_assert(sizeof(C_CombatGuardTypeData) == 0x58,
              "C_CombatGuardTypeData must be 0x58 (push helper sub_1814CC61C advances 88)");

}  // namespace wh::combatmodule
