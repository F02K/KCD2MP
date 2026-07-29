#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::questmodule::S_Tracker -- objective tracker payload (KCD2 1.5.6, kd7u).  sizeof 0x10.
// -----------------------------------------------
// TD 0x184B12158, own vtable 0x183B05E00 ([0..4] real, [5+] nullsub -- slot roles not
// mapped). Polymorphic struct used as the payload type of C_TypedPortRef<S_Tracker>
// (C_Objective @+0x98, C_CarryItemTarget @+0x128). Size VERIFIED from the by-value
// embedding in C_CarryItemTarget (sub_1810D5924: vftable @+0x128, qword @+0x130, next
// member @+0x138).

namespace wh::questmodule {

struct S_Tracker {
    inline static constexpr auto RTTI = Offsets::RTTI_S_Tracker;
    virtual ~S_Tracker();   // [0]; slot roles not mapped

    int32_t m_field08;      // +0x08  ctor sub_181A8C080 stores arg2 here [role UNVERIFIED]
    int32_t m_field0C;      // +0x0C  ctor sub_181A8C080 stores arg3 here [role UNVERIFIED]
};
static_assert(sizeof(S_Tracker) == 0x10, "S_Tracker must be 0x10 (C_CarryItemTarget embedding)");

}  // namespace wh::questmodule
