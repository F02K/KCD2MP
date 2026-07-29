#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_BloodZoneId -- blood/damage body-zone id (KCD2 WHGame.dll 1.5.6, kd7u).  4-byte enum.
// -----------------------------------------------
// Enum name RE-CHOSEN (no RTTI enum or native id-to-name switch found).  This is the final
// body_part.blood_zone_id selected by the damage pipeline, not body_part_id/body_subpart_id and
// not the unrelated reflected E_BodyPart::Type equipment/display category.
//
// [VALUES VERIFIED against KCD2 game data] Values are the complete distinct set in
// Libs/Tables/item/body_part.xml from the installed 1.5.6 Tables.pak.  The six canonical names
// below follow the human rows.  Animal rows reuse 0 for heads, 1 for torsos, 2 for front legs
// (and all horse legs/feet), and 3 for back legs; current animal rows do not use 4 or 5.

namespace wh::rpgmodule {

enum class E_BloodZoneId : int32_t {
    Undefined = -1,
    Head       = 0,
    Torso      = 1,
    LeftArm    = 2,  // Animal reuse: front legs; horse legs/feet.
    RightArm   = 3,  // Animal reuse: back legs.
    LeftLeg    = 4,
    RightLeg   = 5,
};

}  // namespace wh::rpgmodule
