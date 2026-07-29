#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_Sequence -- dialogue sequence (spoken line + anim/camera
// payload) (KCD2 1.5.6, kd7u).  sizeof 0xE0.
// -----------------------------------------------
// RTTI TD 0x184B67C30; vtable 0x183A9D7D8 (4 slots); ctor sub_180D85CFC; deleting-dtor
// [0] sub_1803C12CC. Carries 5 CryStrings (+0x48/+0x60/+0x68/+0x70/+0x80 -- line id /
// speaker / anim / camera names [attribution UNVERIFIED]) plus several container heads.
// Ctor constants: +0x40 = 1, +0x44 = 1.0f, +0xA0 = 7. Unlabeled regions not walked.

namespace wh::dialogmodule::data {

class C_Sequence {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Sequence;
    virtual ~C_Sequence();       // [0] deleting-dtor sub_1803C12CC
    RTTR_ENABLE()  // [1..3]: get_type 0x181A6CC98, get_derived 0x180F2B20C

    uint8_t  _unk08[0x38];       // +0x08..+0x3F  container heads [not walked]
    int32_t  m_d40;              // +0x40  ctor 1
    float    m_f44;              // +0x44  ctor 1.0f
    CryStringT<char> m_str48;    // +0x48  [role UNVERIFIED]
    uint64_t _q50;               // +0x50  [not walked]
    uint64_t _q58;               // +0x58  [not walked]
    CryStringT<char> m_str60;    // +0x60  [role UNVERIFIED]
    CryStringT<char> m_str68;    // +0x68  [role UNVERIFIED]
    CryStringT<char> m_str70;    // +0x70  [role UNVERIFIED]
    uint64_t _q78;               // +0x78  [not walked]
    CryStringT<char> m_str80;    // +0x80  [role UNVERIFIED]
    uint64_t _q88[3];            // +0x88..+0x9F  [not walked]
    int32_t  m_dA0;              // +0xA0  ctor 7
    uint8_t  _padA4[4];          // +0xA4
    uint8_t  _unkA8[0x38];       // +0xA8..+0xDF  [not walked]
};
static_assert(sizeof(C_Sequence) == 0xE0, "C_Sequence must be 0xE0");

}  // namespace wh::dialogmodule::data
