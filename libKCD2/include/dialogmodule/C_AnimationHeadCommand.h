#pragma once
#include "C_AnimationCommand.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_AnimationHeadCommand -- head-animation timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// RTTI TD 0x184B53480; vtable 0x183A93608 (14 slots); ctor sub_180CF69C4 (alloc 24);
// deleting-dtor [0] sub_180EA1C80. Identical layout to C_AnimationCommand -- overrides
// only the handler/serialize slots.

namespace wh::dialogmodule::data {

class C_AnimationHeadCommand : public C_AnimationCommand {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AnimationHeadCommand;
};
static_assert(sizeof(C_AnimationHeadCommand) == 0x18, "C_AnimationHeadCommand adds no data (alloc 24)");

}  // namespace wh::dialogmodule::data
