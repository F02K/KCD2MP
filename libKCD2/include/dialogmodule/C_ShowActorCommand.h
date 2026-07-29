#pragma once
#include "C_Command.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_ShowActorCommand -- show-actor timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x10 (alloc 16).
// -----------------------------------------------
// RTTI TD 0x184B52820; vtable 0x183A938B0 (10 slots); ctor sub_180CF6680 (alloc 16);
// deleting-dtor [0] sub_180515DE0. No own fields -- Serialize writes the constant
// 1 (= visible). (Research-brief ctor labels for Show/Hide were flipped; corrected:
// sub_180CF6680 constructs THIS class.)

namespace wh::dialogmodule::data {

class C_ShowActorCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ShowActorCommand;
};
static_assert(sizeof(C_ShowActorCommand) == 0x10, "C_ShowActorCommand adds no data (alloc 16)");

}  // namespace wh::dialogmodule::data
