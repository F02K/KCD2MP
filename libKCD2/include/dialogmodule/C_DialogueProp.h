#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_DialogueProp -- dialogue prop record (KCD2 1.5.6, kd7u).
// sizeof 0x28.
// -----------------------------------------------
// RTTI TD 0x184B6F228; vtable 0x183B764C8 = RTTR trio ONLY, no virtual dtor (see class
// body); ctor sub_1813C7024. A prop used during a conversation (bound by the C_*PropCommand
// timeline commands). Field roles UNVERIFIED.

namespace wh::dialogmodule::data {

class C_DialogueProp {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueProp;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x180EC3B50, get_derived 0x1828322D8

    CryStringT<char> m_arr08;    // +0x08  COW array [role UNVERIFIED]
    uint8_t  m_b10;              // +0x10  [role UNVERIFIED]
    uint8_t  _pad11[7];          // +0x11
    uint64_t _q18;               // +0x18  [not walked]
    uint64_t _q20;               // +0x20  [not walked]
};
static_assert(sizeof(C_DialogueProp) == 0x28, "C_DialogueProp must be 0x28");

}  // namespace wh::dialogmodule::data
