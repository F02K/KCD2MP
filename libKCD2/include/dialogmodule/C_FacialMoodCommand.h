#pragma once
#include "C_Command.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_FacialMoodCommand -- facial-mood timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// RTTI TD 0x184B53270; vtable 0x183A93488 (10 slots); ctor sub_180CF6C7C (alloc 24);
// deleting-dtor [0] sub_180EB8034. Ctor defaults the mood name to "". Its Serialize
// slot [2] is a nullsub (facial mood is not reflected into the archive).

namespace wh::dialogmodule::data {

class C_FacialMoodCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FacialMoodCommand;
    CryStringT<char> m_moodName;   // +0x10  ctor ""
};
static_assert(sizeof(C_FacialMoodCommand) == 0x18, "C_FacialMoodCommand must be 0x18 (alloc 24)");

}  // namespace wh::dialogmodule::data
