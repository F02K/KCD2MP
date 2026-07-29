#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_Decision -- dialogue decision (branch point)
// (KCD2 1.5.6, kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI TD 0x184B67CA0; vtable 0x183A47228 = RTTR trio ONLY, no virtual dtor (see class
// body); ctor sub_1806B393C. A decision groups responses/ports and carries unset-sentinel
// numerics (+0x28 = 0x80000000 "unset int", +0x2C = NaN "unset float"). Container-head
// and COW-array roles UNVERIFIED (responses / ports / included-decision-alias lists).
// C_Dialogue embeds one C_Decision at +0x08.

namespace wh::dialogmodule::data {

class C_Decision {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Decision;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x181A6D2B8, get_derived 0x182832298

    void*    m_pHead08;          // +0x08  DynArray<T> (CryEngine SmallDynStorage, single ptr; empty=&gEmptyHeaderPtr 0x185666F18; blk hdr {cap@-8,cnt@-4}) [elem T UNVERIFIED]
    CryStringT<char> m_arr10;    // +0x10  COW array [role UNVERIFIED]
    CryStringT<char> m_arr18;    // +0x18  COW array [role UNVERIFIED]
    CryStringT<char> m_arr20;    // +0x20  COW array [role UNVERIFIED]
    int32_t  m_d28;              // +0x28  ctor 0x80000000 (unset sentinel)
    float    m_f2C;              // +0x2C  ctor NaN (unset sentinel)
    uint32_t _d30;               // +0x30  [not walked]
    bool     m_hasName;          // +0x34
    uint8_t  _pad35[3];          // +0x35
    uint64_t _q38;               // +0x38  [not walked]
    uint64_t _q40;               // +0x40  [not walked]
    CryStringT<char> m_arr48;    // +0x48  COW array [role UNVERIFIED]
    void*    m_pHead50;          // +0x50  DynArray<T> (CryEngine SmallDynStorage, single ptr; empty=&gEmptyHeaderPtr 0x185666EE8; blk hdr {cap@-8,cnt@-4}) [elem T UNVERIFIED]
    void*    m_pHead58;          // +0x58  DynArray<T> (CryEngine SmallDynStorage, single ptr; empty=&gEmptyHeaderPtr 0x185666EF8; blk hdr {cap@-8,cnt@-4}) [elem T UNVERIFIED]
};
static_assert(sizeof(C_Decision) == 0x60, "C_Decision must be 0x60");

}  // namespace wh::dialogmodule::data
