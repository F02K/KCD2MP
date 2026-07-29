#pragma once
#include <cstdint>
#include "../conceptmodule/C_ModuleBase.h"
#include "../conceptmodule/C_TypedPortRef.h"
#include "../conceptmodule/S_Trigger.h"
#include "../framework/S_ResourceHandle.h"

// -----------------------------------------------
// wh::questmodule::C_ActivityCollection -- activity collection node (KCD2 1.5.6, kd7u).
// modeled sizeof 0x180 [alloc UNPROVEN].
// -----------------------------------------------
// Own vtable 0x183ED41E0; ctor sub_181F378F0 (decompile-verified): C_ModuleBase chain, two
// resource handles, then C_TypedPortRef<uint>@+0xC0, C_TypedPortRef<S_Trigger>@+0x100 and
// @+0x140 (each port's inline I_Port init sub_1806B28F0 + m_flag30 byte at port+0x30 --
// the standard 0x40 port stride).

namespace wh::questmodule {

class C_ActivityCollection : public conceptmodule::C_ModuleBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActivityCollection;
    framework::S_ResourceHandle m_refA0;                                       // +0xA0  (&unk_1855E16D8)
    framework::S_ResourceHandle m_refB0;                                       // +0xB0  (&unk_1855E16C8)
    conceptmodule::C_TypedPortRef<uint32_t> m_portC0;                          // +0xC0  [role UNVERIFIED]
    conceptmodule::C_TypedPortRef<conceptmodule::S_Trigger> m_trigger100;      // +0x100 [role UNVERIFIED]
    conceptmodule::C_TypedPortRef<conceptmodule::S_Trigger> m_trigger140;      // +0x140 [role UNVERIFIED]
};
static_assert(sizeof(C_ActivityCollection) == 0x180, "modeled extent (ctor sub_181F378F0; alloc UNPROVEN)");

}  // namespace wh::questmodule
