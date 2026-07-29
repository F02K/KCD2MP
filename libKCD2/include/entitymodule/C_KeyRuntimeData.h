#pragma once
#include <cstdint>
#include "I_ItemRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_KeyRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60.
// -----------------------------------------------
// RTTI .?AVC_KeyRuntimeData@entitymodule@wh@@ (TD 0x184C6F198, COL 0x18432AB98)
// vtable 0x183EC8AF0  ctor sub_182A88C98 (delegates straight to abstract base).
// sizeof VERIFIED from the fixed-slot pool sub_182A8E23C: 98304/1024 = 96 (stride 24 dwords).

namespace wh::entitymodule {

class C_KeyRuntimeData : public I_ItemRuntimeData   // +0x00  (0x58)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_KeyRuntimeData;
    int64_t m_keyRef;   // +0x58  (=0; INFERRED key/lock reference -- no reader decompiled)
};
static_assert(sizeof(C_KeyRuntimeData) == 0x60, "C_KeyRuntimeData must be 0x60");

}  // namespace wh::entitymodule
