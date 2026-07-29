#pragma once
#include <cstdint>
#include "E_CombatInputClass.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatInputClassData -- KCD2 1.5.6 (kd7u). sizeof 0x30.
// -----------------------------------------------
// combat_input_class row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_AnimationDatabaseStaticIndexed<C_CombatInputClassData, S_CombatInputClassTableRow>> =
// C_CombatInputClassDatabase (vtable 0x183BB5EC8, 23 slots; MI subobject @+0xE8, vtable
// 0x183BB5DE0). Row->Data ctor sub_18163644C (called from ParseRows vtable[10] = sub_181636380)
// writes NAME first (@0 from row+8), id@8 (from row+0), mn_tag@0x10, zeroes +0x18..+0x2B; the
// in-place push sub_181636320 copies the same fields and advances the vector end by 48 -> sizeof
// PROVEN 0x30. The tag-resolution pass (vtable[16] = sub_180F45490, stride 48, skips id -1 at
// +0x08) resolves mn_tag (+0x10) into {+0x18 raw, +0x28 id} via sub_180F45F64. Runtime-block
// internals UNVERIFIED (modeled opaque). E_CombatInputClass name->id resolution registered by
// C_CombatInputClassDatabase::Init sub_18155CF10 (see E_CombatInputClass.h).
//
// [vs KCD1] KCD1 was 0x18 {id@0, name@8, mn_tag@0x10} (KCD1/RE/include/combatmodule/
// C_CombatInputClassData.h). KCD2 REORDERS name to +0x00 and id to +0x08 and appends the
// {0x10 raw + int32 resolved id} tag block -> 0x30.

namespace wh::combatmodule {

class C_CombatInputClassData {
public:
    CryStringT<char>   combat_input_class_name;  // +0x00
    E_CombatInputClass combat_input_class_id;    // +0x08  PK (-1 = "none" sentinel row)
    int32_t            _pad0C;                   // +0x0C
    CryStringT<char>   mn_tag;                   // +0x10  Mannequin tag
    uint8_t            _mnTagRuntime18[0x10];    // +0x18  resolved-tag raw block (layout UNVERIFIED)
    int32_t            _mnTagResolvedId28;       // +0x28  resolved animation-tag id
    int32_t            _pad2C;                   // +0x2C
};
static_assert(sizeof(C_CombatInputClassData) == 0x30,
              "C_CombatInputClassData must be 0x30 (push helper sub_181636320 advances 48)");

}  // namespace wh::combatmodule
