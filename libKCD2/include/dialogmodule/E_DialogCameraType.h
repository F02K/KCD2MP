#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::dialogmodule::E_DialogCameraType -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Dialogue camera shot type (C_AnimatedCamera::m_type, ctor arg3; the manager owns
// four cameras = types 0..3). Enumerators certified from the RTTR registration
// sub_1800D3350 (enumeration_wrapper<...E_DialogCameraType,9,1>; count 9 includes
// the BONE_FIRST/BONE_LAST range aliases sharing values 1 and 3).

namespace wh::dialogmodule {

enum class E_DialogCameraType : int32_t {
    BONE_MASTER     = 0,
    BONE_MEDIUM     = 1,  // alias BONE_FIRST
    BONE_CLOSE_SHOT = 2,
    BONE_CLOSEUP    = 3,  // alias BONE_LAST
    CUSTOM          = 4,
    LAST            = 5,
    OBSOLETE        = 6,
};

}  // namespace wh::dialogmodule
