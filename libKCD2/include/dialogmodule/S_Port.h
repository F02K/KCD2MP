#pragma once
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::S_Port -- dialogue port reference record (KCD2 1.5.6, kd7u).
// sizeof 0x10.
// -----------------------------------------------
// RTTI TD 0x184B67D28; vtable 0x183AB1E68 (3 slots -- all RTTR trio, no dtor slot);
// ctor sub_180DD7A24. Names a concept-graph port the dialogue graph binds to (resolved by
// the "Port" EE function C_FunctionPort at branch-evaluation time).

namespace wh::dialogmodule::data {

struct S_Port {
    inline static constexpr auto RTTI = Offsets::RTTI_S_Port;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x181A6DAD0, get_derived 0x1828322F8

    CryStringT<char> m_portRef;   // +0x08  port name/id
};
static_assert(sizeof(S_Port) == 0x10, "S_Port must be 0x10");

}  // namespace wh::dialogmodule::data
