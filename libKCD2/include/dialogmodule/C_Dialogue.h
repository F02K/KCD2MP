#pragma once
#include <cstdint>
#include "C_Decision.h"
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_Dialogue -- dialogue definition (KCD2 1.5.6, kd7u).
// sizeof 0xA0.
// -----------------------------------------------
// RTTI TD 0x184B67C68; vtable 0x183A471E0 = RTTR trio ONLY, no virtual dtor (see class
// body); ctor sub_1806B3724. Embeds its root C_Decision by value at +0x08 (ctor/dtor delegation
// confirmed). m_chatTimeLimit defaults from CVar wh_dlg_DefaultChatTimeLimit,
// m_maxDistance from wh_dlg_DefaultMaxDistance. Tail fields +0x70/+0x80..+0x98 not
// walked [roles UNVERIFIED].

namespace wh::dialogmodule::data {

class C_Dialogue {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Dialogue;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x1819DF228, get_derived 0x1828322B8

    C_Decision m_decision;        // +0x08  root decision (embedded by value, 0x60)
    CryStringT<char> m_name;      // +0x68
    uint64_t _q70;                // +0x70  [not walked]
    int32_t  m_chatTimeLimit;     // +0x78  default = CVar wh_dlg_DefaultChatTimeLimit
    float    m_maxDistance;       // +0x7C  default = CVar wh_dlg_DefaultMaxDistance
    uint64_t _q80[4];             // +0x80..+0x9F  [not walked]
};
static_assert(sizeof(C_Dialogue) == 0xA0, "C_Dialogue must be 0xA0");

}  // namespace wh::dialogmodule::data
