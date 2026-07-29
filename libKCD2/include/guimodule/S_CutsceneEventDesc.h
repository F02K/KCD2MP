#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::guimodule::S_CutsceneEventDesc -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// RTTI TD 0x184CB0788; vtable 0x183BA5900 = RTTR trio ONLY (get_type 0x182B3EC40 /
// get_ptr 0x1805F5DA0 / get_derived_info 0x182B3E730), no virtual dtor. rttr-reflected
// cutscene-event descriptor; embedded BY VALUE in C_CutsceneEvent @+0x08. Layout from
// the copy ctor sub_182B35244. Member roles UNVERIFIED (names descriptive).

namespace wh::guimodule {

class S_CutsceneEventDesc {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_CutsceneEventDesc;
    // RTTR trio [0..2] -- the WHOLE vtable, no virtual dtor: get_type 0x182B3EC40,
    // get_ptr 0x1805F5DA0, get_derived_info 0x182B3E730. Registered BASELESS.
    RTTR_ENABLE()

    int32_t          m_int08;    // +0x08
    int32_t          m_int0C;    // +0x0C
    int32_t          m_int10;    // +0x10
    uint8_t          _pad14[4];  // +0x14
    CryStringT<char> m_str18;    // +0x18
    uint8_t          m_byte20;   // +0x20
    uint8_t          _pad21[3];  // +0x21
    int32_t          m_int24;    // +0x24
    int32_t          m_int28;    // +0x28
    uint8_t          _pad2C[4];  // +0x2C
};
static_assert(sizeof(S_CutsceneEventDesc) == 0x30, "S_CutsceneEventDesc must be 0x30 (copy ctor sub_182B35244)");

}  // namespace wh::guimodule
