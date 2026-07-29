#pragma once
#include "C_Command.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_HideActorCommand -- hide-actor timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x10 (alloc 16).
// -----------------------------------------------
// RTTI TD 0x184B528F8; vtable 0x183A93830 (10 slots); ctor sub_180CF6734 (alloc 16);
// deleting-dtor [0] sub_180EB7FC8. No own fields -- Serialize writes the constant
// 0 (= hidden).

namespace wh::dialogmodule::data {

class C_HideActorCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_HideActorCommand;
};
static_assert(sizeof(C_HideActorCommand) == 0x10, "C_HideActorCommand adds no data (alloc 16)");

}  // namespace wh::dialogmodule::data
