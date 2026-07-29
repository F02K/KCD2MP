#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::E_CombatTag -- KCD2 WHGame.dll 1.5.6 (kd7u). 4-byte enum.
// -----------------------------------------------
// combat_tag id. Values recovered from the name->id resolver sub_1815E22D8 (strcmp ladder over
// the combat_tag_name column, called from the combat_tag ParseRows sub_1815E21F4) and are
// 1:1 with the row ids in Libs/Tables/combat/combat_tag.xml (NONE=0 ..
// ATTACK_TYPE_DONT_PROJECT_WEAPON=6; LAST=7 is binary-only, unknown names fall back to 0).
// The enum's ORIGINAL NAME is not in the binary -- "E_CombatTag" is INFERRED from the
// combat_tag table / C_CombatTagData naming.

namespace wh::combatmodule {

enum class E_CombatTag : int32_t {
    None                       = 0,  // "NONE"
    ZoneLeftSide               = 1,  // "ZONE_LEFTSIDE"
    ZoneRightSide              = 2,  // "ZONE_RIGHTSIDE"
    ZoneCenter                 = 3,  // "ZONE_CENTER"
    AttackTypeSwitchZone       = 4,  // "ATTACK_TYPE_SWITCH_ZONE"
    AttackTypeDontProjectBones = 5,  // "ATTACK_TYPE_DONT_PROJECT_BONES"
    AttackTypeDontProjectWeapon= 6,  // "ATTACK_TYPE_DONT_PROJECT_WEAPON"
    Last                       = 7,  // "LAST" (not present in combat_tag.xml)
};

}  // namespace wh::combatmodule
