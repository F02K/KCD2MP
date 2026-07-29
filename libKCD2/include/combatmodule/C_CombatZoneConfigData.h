#pragma once
#include <cstdint>
#include "E_CombatZoneId.h"
#include "S_CombatZoneConfigTableRow.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatZoneConfigData -- KCD2 1.5.6 (kd7u). sizeof 0x30.
// -----------------------------------------------
// combat_zone_config row -> Data. POLYMORPHIC ROOT (own TD, RVA 0x4B31EF0; RTTI hierarchy has
// NO base class -- unlike attack-type/zone/weapon-group there is no I_ interface). Vtable
// 0x183BC6568, exactly 3 slots (slot 3 is the next vtable's COL) and NO virtual destructor:
//   [0] sub_181A72970  GetCombatZoneConfigId -> +0x08
//   [1] sub_181A76130  GetCombatZoneId       -> +0x0C
//   [2] sub_181A72C80  GetGuardStanceId      -> +0x10
// (method names INFERRED from the returned fields; the getters are the entire vtable).
// DB stack: C_CombatDatabase<C_AnimationDatabaseStatic<Data, Row>> over
// C_ObjectTableDatabase<Data, Row> (vtable 0x183C3F370). ParseRows sub_1815C6658 converts
// inline: writes the vftable, then the 9 row columns verbatim to +0x08..+0x2B; non-grow path
// advances the vector end by 48 -> sizeof VERIFIED 0x30 (grow path sub_1815C64B4).
//
// [vs KCD1] KCD1 had no C_CombatZoneConfigData class header; KCD2 adds the vtable (+8 shift)
// and the injured_mod column.

namespace wh::combatmodule {

class C_CombatZoneConfigData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatZoneConfigData;
    // NOTE: no virtual dtor in the binary vtable -- do not add one (it would shift the slots).
    virtual int32_t GetCombatZoneConfigId() const;  // [0] 0x181A72970
    virtual E_CombatZoneId GetCombatZoneId() const; // [1] 0x181A76130  (combat_zone FK = E_CombatZoneId domain)
    virtual int32_t GetGuardStanceId() const;       // [2] 0x181A72C80

    int32_t combat_zone_config_id;     // +0x08
    int32_t combat_zone_id;            // +0x0C
    int32_t guard_stance_id;           // +0x10
    int32_t r_weapon_class_id;         // +0x14
    int32_t l_weapon_class_id;         // +0x18
    int32_t r_combat_weapon_group_id;  // +0x1C
    int32_t l_combat_weapon_group_id;  // +0x20
    float   guard_weight;              // +0x24
    float   injured_mod;               // +0x28
    int32_t _pad2C;                    // +0x2C
};
static_assert(sizeof(C_CombatZoneConfigData) == 0x30, "C_CombatZoneConfigData must be 0x30");

}  // namespace wh::combatmodule
