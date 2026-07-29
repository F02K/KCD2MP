#pragma once
#include <cstdint>
#include "C_ApplyOnLoadPerk.h"

// -----------------------------------------------
// wh::rpgmodule::C_SoulAbilityPerk -- perk granting soul abilities (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_SoulAbilityPerk@rpgmodule@wh@@ (TD 0x184D53D08)  vtable 0x183A38B28, 14 slots.
// sizeof 0x40 (PROVEN: factory sub_18046B920 pooled alloc 64; the owner arg lands in +0x20,
// so for this type C_Perk::m_pOwner IS the C_Soul).  Type id unk_185330AB0 ([10]; AsType
// [11]/[12] also accept the C_Perk base id).
//
// Resolve [9] (0x18046B8CC) = base resolve + equal_range over C_PerkSoulAbilityDatabase's
// sorted guid index by m_id (lower 0x18046A514 / upper 0x18046B834) into the +0x30 slice --
// a VIEW of DB row pointers, never owned (factory inits both ends to the index sentinel
// @db+0xA0).  ApplyEffects [13] (0x1804706D4): per row, sorted-emplace
// {row->soul_ability_id, this} into the owner soul's m_soulAbilities (+0x320; RAW id, no
// validation) and ping the property notifier.  Activate [1] (0x180470750) = ApplyEffects
// then base; Deactivate [2] (0x1804704E4) = base then erase each id (notify id + 0x19E);
// ResolveAlias [8] (0x1804C8DD8) = Resolve, then re-ApplyEffects when m_active (save-load).

namespace wh::rpgmodule {

struct S_PerkSoulAbility;   // perk_soul_ability row (C_PerkSoulAbilityDatabase.h); +0x10 = soul_ability_id

class C_SoulAbilityPerk : public C_ApplyOnLoadPerk {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SoulAbilityPerk;

    void ApplyEffects() override;   // [13] the ability emplacer (0x1804706D4)

    S_PerkSoulAbility** m_rowsBegin;   // +0x30  equal_range slice over the DB's sorted guid
    S_PerkSoulAbility** m_rowsEnd;     // +0x38  index (row POINTERS); empty until Resolve [9]
};
static_assert(sizeof(C_SoulAbilityPerk) == 0x40, "C_SoulAbilityPerk must be 0x40 (pooled alloc 64)");
static_assert(offsetof(C_SoulAbilityPerk, m_rowsBegin) == 0x30, "row slice at 0x30");

}  // namespace wh::rpgmodule
