#pragma once
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::S_IncludedDecisionAlias -- decision-include alias record
// (KCD2 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// RTTI TD 0x184B67CE0; vtable 0x183AC7B28 (4 slots); ctor sub_180ECF744; deleting-dtor
// [0] sub_180DD7C30; RTTR trio [1..3]. Aliases a decision pulled into a dialogue
// via include.

namespace wh::dialogmodule::data {

struct S_IncludedDecisionAlias {
    inline static constexpr auto RTTI = Offsets::RTTI_S_IncludedDecisionAlias;
    virtual ~S_IncludedDecisionAlias();   // [0] deleting-dtor sub_180DD7C30
    RTTR_ENABLE()  // [1..3]: get_type 0x181A6DB68, get_derived 0x181431944 (ex-"Clone" guess)

    CryStringT<char> m_alias;             // +0x08
};
static_assert(sizeof(S_IncludedDecisionAlias) == 0x10, "S_IncludedDecisionAlias must be 0x10");

}  // namespace wh::dialogmodule::data
