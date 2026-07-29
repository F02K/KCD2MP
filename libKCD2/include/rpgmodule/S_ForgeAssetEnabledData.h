#pragma once
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_ForgeAssetEnabledData -- payload of C_ForgeAssetEnabledCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: ctor sub_181FCE630 PLAIN-alloc sub_1804F75C0(24) -- the only g11b
// cause not using the tracked allocator; two CryStringT copies: sub_1804FD208(obj+8,a3)
// @0x181FCE672 + sub_1804FD208(obj+16,a3+8) @0x181FCE67F).

namespace wh::rpgmodule {

struct S_ForgeAssetEnabledData {
    CryStringT<char> m_assetId;   // +0x00  asset/forge id [proposed]
    CryStringT<char> m_name;      // +0x08  2nd id/name [proposed]
};
static_assert(sizeof(S_ForgeAssetEnabledData) == 0x10, "S_ForgeAssetEnabledData must be 0x10");

}  // namespace wh::rpgmodule
