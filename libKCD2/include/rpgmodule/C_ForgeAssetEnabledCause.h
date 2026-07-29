#pragma once
#include "C_Cause.h"
#include "S_ForgeAssetEnabledData.h"

// -----------------------------------------------
// wh::rpgmodule::C_ForgeAssetEnabledCause -- "ForgeAssetEnabled" event cause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_181FCE630 PLAIN-alloc sub_1804F75C0(24) @0x181FCE654 --
// the wrapper updates dword_1851DFB18 += 24 internally; only cause in the g11b set using
// this allocator form).  RTTI TD rva 0x4D5C150, vtable rva 0x3F27198 (4 slots): [0] dtor
// rva 0x1FD2E20 (releases the two strings), [1] nullsub, [2] Accept rva 0x9DB490 ->
// visitor+0x0, [3] GetTypeName rva 0x1FC0920 -> "ForgeAssetEnabled" (verified).
// Pattern-B ctor; create-site sub_181FD4810.

namespace wh::rpgmodule {

class C_ForgeAssetEnabledCause : public C_Cause<S_ForgeAssetEnabledData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ForgeAssetEnabledCause;
};
static_assert(sizeof(C_ForgeAssetEnabledCause) == 0x18,
              "C_ForgeAssetEnabledCause must be 0x18 (alloc 24 at sub_181FCE630)");

}  // namespace wh::rpgmodule
