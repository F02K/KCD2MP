#pragma once
#include <cstdint>
#include "C_CombatWeaponGroupData.h"
#include "S_CombatNativeGuardZoneTableRow.h"

namespace wh::entitymodule { struct S_WeaponClass; }  // weapon_class row -- not yet RE'd in KCD2

// -----------------------------------------------
// wh::combatmodule::C_CombatNativeGuardZoneData -- KCD2 1.5.6 (kd7u). sizeof 0x38.
// -----------------------------------------------
// combat_native_guard_zone row -> Data (FK-resolver layer). NON-polymorphic POD (no own TD).
// DB stack: C_CombatDatabase<C_AnimationDatabaseStatic<Data, Row>> over
// C_ObjectTableDatabase<Data, Row> (vtable 0x183C3F5A8); concrete
// C_CombatNativeGuardZoneDatabase vtable 0x183C3F720 (TD RVA 0x4B32890).
// ParseRows sub_1816F0454; Row->Data converter sub_1816F0504 copies the six int columns
// verbatim to +0x00..+0x17, then EAGERLY resolves four FK pointers (disasm-verified arg wiring):
//   +0x18/+0x20 = sub_1816F059C(r/l_weapon_class_id): direct-index lookup in the weapon-class
//                 DB global qword_185325670 (miss -> shared static sentinel unk_1855952F0,
//                 default-constructed by sub_180C5F414 which writes the
//                 wh::entitymodule::S_WeaponClass vftable -> pointee type PROVEN);
//   +0x28/+0x30 = sub_180933FD0(&qword_185321520, &r/l_combat_weapon_group_id): map lookup
//                 (db+0xD8) in the combat-weapon-group DB (miss -> sentinel unk_1854821B8
//                 built by sub_1815B18FC = C_CombatWeaponGroupData default ctor).
// Vector push helper sub_18257BA04 advances end by 56 -> sizeof VERIFIED 0x38.
//
// [vs KCD1] No KCD1 C_CombatNativeGuardZoneData header existed; the KCD1 auto-generated row
// showed the same column set (see S_CombatNativeGuardZoneTableRow.h).

namespace wh::combatmodule {

struct C_CombatNativeGuardZoneData {
    int32_t combat_guard_stance_id;                 // +0x00
    int32_t combat_zone_id;                         // +0x04
    int32_t r_weapon_class_id;                      // +0x08
    int32_t l_weapon_class_id;                      // +0x0C
    int32_t r_combat_weapon_group_id;               // +0x10
    int32_t l_combat_weapon_group_id;               // +0x14
    entitymodule::S_WeaponClass* pRWeaponClass;     // +0x18  resolved from r_weapon_class_id
    entitymodule::S_WeaponClass* pLWeaponClass;     // +0x20  resolved from l_weapon_class_id
    C_CombatWeaponGroupData* pRCombatWeaponGroup;   // +0x28  resolved from r_combat_weapon_group_id
    C_CombatWeaponGroupData* pLCombatWeaponGroup;   // +0x30  resolved from l_combat_weapon_group_id
};
static_assert(sizeof(C_CombatNativeGuardZoneData) == 0x38,
              "C_CombatNativeGuardZoneData must be 0x38");

}  // namespace wh::combatmodule
