#pragma once
#include <cstdint>
#include "../conceptmodule/C_ModuleBase.h"
#include "../conceptmodule/C_TypedPortRef.h"

// -----------------------------------------------
// wh::questmodule::C_ActivityType -- activity type node (KCD2 1.5.6, kd7u).
// modeled sizeof 0x1A8 [alloc UNPROVEN].
// -----------------------------------------------
// Own vtable 0x183ED43A0; ctor sub_181F379B0 (decompile-verified): C_ModuleBase chain +
// four typed ports at the standard 0x40 stride + one trailing qword.

namespace wh::questmodule {

class C_ActivityType : public conceptmodule::C_ModuleBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActivityType;
    conceptmodule::C_TypedPortRef<bool>     m_portA0;    // +0xA0   [role UNVERIFIED]
    conceptmodule::C_TypedPortRef<uint32_t> m_portE0;    // +0xE0   [role UNVERIFIED]
    conceptmodule::C_TypedPortRef<float>    m_port120;   // +0x120  [role UNVERIFIED]
    conceptmodule::C_TypedPortRef<uint32_t> m_port160;   // +0x160  [role UNVERIFIED]
    uint64_t m_unk1A0;                                   // +0x1A0  ctor sub_181F379B0 zeroes; no writer found (vtable 0x183ED43A0 ref'd only by ctor) [role UNRESOLVED]
};
static_assert(sizeof(C_ActivityType) == 0x1A8, "modeled extent (ctor sub_181F379B0; alloc UNPROVEN)");

}  // namespace wh::questmodule
