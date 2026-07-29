#pragma once
#include "C_Command.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_ScriptCommand -- script-call timeline command
// (KCD2 1.5.6, kd7u).  sizeof 0x18 (alloc 24).
// -----------------------------------------------
// RTTI TD 0x184B533F8; vtable 0x1846FC5D8 (10 slots); ctor sub_18280DCEC (alloc 24);
// deleting-dtor [0] sub_180EB8034. Its Serialize [2] reflects ONLY the script string,
// not the timeline base state.

namespace wh::dialogmodule::data {

class C_ScriptCommand : public C_Command {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptCommand;
    CryStringT<char> m_scriptRef;   // +0x10
};
static_assert(sizeof(C_ScriptCommand) == 0x18, "C_ScriptCommand must be 0x18 (alloc 24)");

}  // namespace wh::dialogmodule::data
