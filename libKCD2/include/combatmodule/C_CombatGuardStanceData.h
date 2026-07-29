#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::C_CombatGuardStanceData -- KCD2 1.5.6 (kd7u). sizeof 0x48.
// -----------------------------------------------
// combat_guard_stance row -> Data. NON-polymorphic. Stack: C_CombatDatabase<
// C_AnimationDatabaseStaticIndexed<C_CombatGuardStanceData, S_CombatGuardStanceTableRow>> =
// C_CombatGuardStanceDatabase (vtable 0x183BE8A58, 23 slots). ParseRows (vtable[10] =
// sub_1816F12A4) builds {id, 3 CryStrings, zeroed 0x28 runtime tail}; the in-place push
// sub_182799664 copies exactly id@0, strings@8/10/18, oword@20+dword@30, oword@34+dword@44 and
// advances the vector end by 72 -> sizeof PROVEN 0x48. The tag-resolution pass (vtable[16] =
// sub_180F45FD4, stride 72, skips rows with id -1) resolves mn_tag (+0x10) into
// {+0x20..+0x2F raw, +0x30 id} and opp_mn_tag (+0x18) into {+0x34..+0x43 raw, +0x44 id} via
// sub_180F45F64. The 0x10 runtime block's internal layout is UNVERIFIED (modeled opaque, as in
// C_CombatAttackTypeData.h).
//
// [vs KCD1] KCD1 was 0x38 with a single 0x18 zero-init runtime tail
// (KCD1/RE/include/combatmodule/C_CombatGuardStanceData.h); KCD2 grows the tail to 2 x 0x14
// explicit {raw block + resolved id} pairs (one per mn tag) = 0x28 -> 0x48 total. Strings and
// scalar columns unchanged.

namespace wh::combatmodule {

class C_CombatGuardStanceData {
public:
    int32_t          combat_guard_stance_id;    // +0x00  PK (-1 = "none" sentinel row)
    int32_t          _pad04;                    // +0x04
    CryStringT<char> combat_guard_stance_name;  // +0x08
    CryStringT<char> mn_tag;                    // +0x10  Mannequin tag
    CryStringT<char> opp_mn_tag;                // +0x18  opponent-side Mannequin tag
    uint8_t          _mnTagRuntime20[0x10];     // +0x20  resolved-tag raw block (layout UNVERIFIED)
    int32_t          _mnTagResolvedId30;        // +0x30  resolved animation-tag id
    uint8_t          _oppMnTagRuntime34[0x10];  // +0x34  resolved-tag raw block (layout UNVERIFIED)
    int32_t          _oppMnTagResolvedId44;     // +0x44  resolved animation-tag id
};
static_assert(sizeof(C_CombatGuardStanceData) == 0x48,
              "C_CombatGuardStanceData must be 0x48 (push helper sub_182799664 advances 72)");

}  // namespace wh::combatmodule
