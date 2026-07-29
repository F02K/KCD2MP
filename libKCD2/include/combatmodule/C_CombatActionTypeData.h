#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatActionTypeData -- KCD2 1.5.6 (kd7u). sizeof 0x10.
// -----------------------------------------------
// combat_action_type row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_AnimationDatabaseStaticIndexed<C_CombatActionTypeData, S_CombatActionTypeTableRow>> =
// C_CombatActionTypeDatabase (vtable 0x183E5A3B8, 23 slots + I_QueryableDatabase MI base @+0xE8,
// vtable 0x183E5A258). ParseRows (vtable[10] = sub_1813E3CA4) reads the 0x10 raw row, keeps the
// int32 id and builds a CryStringT from the patched name pointer; the in-place push writes
// {id @+0, string @+8} and advances the vector end by 16 -> sizeof PROVEN 0x10.
// The name-keyed id-resolver map of E_CombatActionType (C_CombatActionTypeDatabase::Init
// sub_1812B8D48, qword_185320428) consumes this type: see E_CombatActionType.h.
//
// [vs KCD1] Identical to the KCD1 header (KCD1/RE/include/combatmodule/C_CombatActionTypeData.h,
// 0x10, same two fields).

namespace wh::combatmodule {

class C_CombatActionTypeData {
public:
    int32_t          combat_action_type_id;    // +0x00  PK (id domain of E_CombatActionType)
    int32_t          _pad04;                   // +0x04
    CryStringT<char> combat_action_type_name;  // +0x08
};
static_assert(sizeof(C_CombatActionTypeData) == 0x10,
              "C_CombatActionTypeData must be 0x10 (parse loop sub_1813E3CA4 advances 16)");

}  // namespace wh::combatmodule
