#pragma once
#include <cstdint>
#include "I_ItemRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_ConsumableRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x68.
// -----------------------------------------------
// RTTI .?AVC_ConsumableRuntimeData@entitymodule@wh@@ (TD 0x184C4BE88, COL 0x1841414D0)
// vtable 0x183A390E0  ctor sub_18046F9D8 (delegates straight to abstract base sub_18046FBF0).
// sizeof VERIFIED from the fixed-slot pool sub_18047030C: 106496/1024 = 104 (stride 26 dwords).

namespace wh::entitymodule {

class C_ConsumableRuntimeData : public I_ItemRuntimeData   // +0x00  (0x58)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ConsumableRuntimeData;
    uint32_t m_state58[4];   // +0x58  OWORD zero-init; internal split UNRESOLVED (INFERRED
                             //        {doses/charges, potency, ...}; no reader decompiled)
};
static_assert(sizeof(C_ConsumableRuntimeData) == 0x68, "C_ConsumableRuntimeData must be 0x68");

}  // namespace wh::entitymodule
