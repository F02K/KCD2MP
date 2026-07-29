#pragma once
#include <cstdint>
#include "../framework/C_LocalizedString.h"
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::data::S_Response -- dialogue response record (KCD2 1.5.6, kd7u).
// sizeof 0x48.
// -----------------------------------------------
// RTTI TD 0x184B67D60; vtable 0x183A980F0 (4 slots); ctor sub_180D2B2E0; deleting-dtor
// [0] sub_180D2B1D8. One spoken/selectable response line of a decision. The 16-byte
// localized-string object at +0x10 is modeled as framework::C_LocalizedString (size
// match + text role; type identity INFERRED, UNVERIFIED). Remaining field roles
// UNVERIFIED.

namespace wh::dialogmodule::data {

struct S_Response {
    inline static constexpr auto RTTI = Offsets::RTTI_S_Response;
    virtual ~S_Response();                 // [0] deleting-dtor sub_180D2B1D8
    RTTR_ENABLE()  // [1..3]: get_type 0x181A6DAB4, get_derived 0x180EFB524

    bool     m_hasText;                    // +0x08
    uint8_t  _pad09[7];                    // +0x09
    framework::C_LocalizedString m_text;   // +0x10  16-byte loc-string [type INFERRED]
    uint64_t m_q20;                        // +0x20  [role UNVERIFIED]
    CryStringT<char> m_ref28;              // +0x28  COW handle [role UNVERIFIED]
    void*    m_pObj30;                     // +0x30  DynArray<T> (WHDynStorage single ptr; empty=&unk_1855D8328; blk hdr {allocFn@-16,cap@-8,cnt@-4}; elem 16B {val, +8=_smart_ptr AddRef}) [elem T UNVERIFIED]
    uint64_t m_q38;                        // +0x38  [role UNVERIFIED]
    uint64_t m_q40;                        // +0x40  [role UNVERIFIED]
};
static_assert(sizeof(S_Response) == 0x48, "S_Response must be 0x48");

}  // namespace wh::dialogmodule::data
