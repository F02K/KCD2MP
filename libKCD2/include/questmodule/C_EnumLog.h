#pragma once
#include <cstdint>
#include "C_LogBase.h"

// -----------------------------------------------
// wh::questmodule::C_EnumLog -- enumerated quest log entry (KCD2 1.5.6, kd7u).
// modeled sizeof 0x50 [alloc UNPROVEN].
// -----------------------------------------------
// Own vtable 0x183ED6570; ctor sub_180F6FAA0. C_LogBase + two CryStrings (helper
// sub_1803D1670 = empty-CryString init) around an INT_MIN sentinel dword.

namespace wh::questmodule {

class C_EnumLog : public C_LogBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EnumLog;
    uint8_t  m_flag30;          // +0x30  ctor 0 [role UNVERIFIED]
    uint8_t  _pad31[7];         // +0x31
    CryStringT<char> m_str38;   // +0x38  [role UNVERIFIED]
    uint32_t m_value40;         // +0x40  ctor 0x80000000 (INT_MIN sentinel) [role UNVERIFIED]
    uint32_t _pad44;            // +0x44
    CryStringT<char> m_str48;   // +0x48  [role UNVERIFIED]
};
static_assert(sizeof(C_EnumLog) == 0x50, "modeled extent (ctor field extent; alloc UNPROVEN)");

}  // namespace wh::questmodule
