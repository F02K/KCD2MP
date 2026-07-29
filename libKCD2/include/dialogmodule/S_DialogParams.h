#pragma once
#include <cstdint>
#include <vector>
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::S_DialogParams -- runtime dialog-parameters record (KCD2 1.5.6, kd7u).
// sizeof 0xE8 (derived: highest default-ctor init = byte @+0xE0, aligned to 8).
// -----------------------------------------------
// RTTI TD 0x184B12F30; vtable 0x183A64320 = RTTR trio ONLY, no virtual dtor (see class
// body); default ctor sub_18093CAF0; parameterized ctor
// sub_18093BCE8(this, vec<ptr>&, vec<24B>&, uint8 flags). Passed as
// shared_ptr<S_DialogParams> through the C_ForcedDialogueNode typed port. Packed-byte
// constants decoded via int_convert: 0x01000000 = bytes {00 00 00 01},
// 0x01010100 = {00 01 01 01}, 0x0100 = {00 01}. Field roles largely UNVERIFIED.

namespace wh::dialogmodule {

struct S_DialogParams {
    inline static constexpr auto RTTI = Offsets::RTTI_S_DialogParams;
    struct S_Rec24 { uint8_t _unk[24]; };   // 24-byte list-B element [layout UNVERIFIED]
    struct S_Rec16 { uint8_t _unk[16]; };   // 16-byte list-D element [layout UNVERIFIED]

    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x182782F90, get_derived 0x182782D3C

    uint8_t  m_flag8;                       // +0x08  ctor 1
    uint8_t  _pad09[7];                     // +0x09
    CryStringT<char> m_str10;               // +0x10  from empty singleton qword_185578FC0
    std::function<void()> m_callback18;     // +0x18  (0x40) MSVC std::function; SSO buf +0x18..+0x4F, impl ptr @+0x50 (empty-init 0 in ctor). Destroyed via sub_1803E23C0. [signature UNVERIFIED]
    std::vector<void*> m_listA;             // +0x58  8-byte elems (param ctor fills from arg2)
    std::vector<void*> m_listC;             // +0x70  owned vector, 8B elems (verify: move 0x18277650C steals
                                            //        {beg,end,cap}; copy 0x18093CBD4 deep-copies via the same
                                            //        helper sub_1804187BC as m_listA) [elem T UNVERIFIED]
    std::vector<S_Rec24> m_listB;           // +0x88  24-byte elems (param ctor fills from arg3)
    uint32_t m_dA0;                         // +0xA0  ctor 0
    uint32_t m_dA4;                         // +0xA4  ctor 0x01000000 (packed bytes 00 00 00 01)
    uint16_t m_wA8;                         // +0xA8  ctor 0x0100
    uint8_t  m_bAA;                         // +0xAA  ctor 1
    uint8_t  _padAB;                        // +0xAB
    uint32_t m_flags;                       // +0xAC  ctor 0; param ctor = flags arg4
    uint32_t m_dB0;                         // +0xB0  ctor 0x01010100; param ctor sets low byte
    uint32_t m_dB4;                         // +0xB4  ctor 2
    uint8_t  m_bB8;                         // +0xB8  ctor 1
    uint8_t  _padB9[3];                     // +0xB9
    uint32_t m_dBC;                         // +0xBC  ctor 0
    uint32_t m_uniqueId;                    // +0xC0  global counter dword_185483CB0++
    uint16_t m_wC4;                         // +0xC4  ctor 0
    uint8_t  _padC6[2];                     // +0xC6
    std::vector<S_Rec16> m_listD;           // +0xC8  owned vector, 16B elems (verify: copy ctor sizes by
                                            //        (end-beg)>>4 with "vector too long" guard) [elem T UNVERIFIED]
    uint8_t  m_bE0;                         // +0xE0  ctor 0
    uint8_t  _padE1[7];                     // +0xE1
};
static_assert(sizeof(S_DialogParams) == 0xE8, "S_DialogParams must be 0xE8 (last init @+0xE0, align 8)");

}  // namespace wh::dialogmodule
