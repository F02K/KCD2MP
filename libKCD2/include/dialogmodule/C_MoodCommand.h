#pragma once
#include "C_Command.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_MoodCommand -- actor-mood timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// RTTI TD 0x184B532B0; vtable 0x183A93588 (10 slots); ctor sub_180CF6A98 (alloc 24);
// deleting-dtor [0] sub_180EB8034. Ctor defaults the mood name to "neutral".

namespace wh::dialogmodule::data {

class C_MoodCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MoodCommand;
    CryStringT<char> m_moodName;   // +0x10  ctor "neutral"
};
static_assert(sizeof(C_MoodCommand) == 0x18, "C_MoodCommand must be 0x18 (alloc 24)");

}  // namespace wh::dialogmodule::data
