#pragma once
#include "C_Command.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_CameraCommand -- camera-cut timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// RTTI TD 0x184B533B8; vtable 0x183A93408 (10 slots); ctor sub_180CF6D54 (alloc 24);
// deleting-dtor [0] sub_180EB8034. Ctor sets the base +0x0F flag byte = 6 (camera
// command kind [role UNVERIFIED]).

namespace wh::dialogmodule::data {

class C_CameraCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CameraCommand;
    CryStringT<char> m_cameraName;   // +0x10
};
static_assert(sizeof(C_CameraCommand) == 0x18, "C_CameraCommand must be 0x18 (alloc 24)");

}  // namespace wh::dialogmodule::data
