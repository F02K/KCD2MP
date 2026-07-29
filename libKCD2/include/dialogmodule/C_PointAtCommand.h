#pragma once
#include "C_Command.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_PointAtCommand -- point-at timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// RTTI TD 0x184B53438; vtable 0x1846FC630 (10 slots); ctor sub_18280DC44 (alloc 24);
// deleting-dtor [0] sub_180EB8034.

namespace wh::dialogmodule::data {

class C_PointAtCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PointAtCommand;
    CryStringT<char> m_targetName;   // +0x10
};
static_assert(sizeof(C_PointAtCommand) == 0x18, "C_PointAtCommand must be 0x18 (alloc 24)");

}  // namespace wh::dialogmodule::data
