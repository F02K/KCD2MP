#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListPerk -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1807EA910
// N=56 (ctor 0x1807EA798, not decompiled -- the +0x20 gap is ctor-only). FillUIArgs
// override 0x180556CCC. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashListPerk : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListPerk;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x180556CCC

    CryStringT<char> m_name;     // +0x18
    int32_t          m_int20;    // +0x20  ctor a4 (byte, 0/1) widened to DWORD; FillUIArgs emits as int arg (tag 2)
    int32_t          m_int24;    // +0x24  tag-2 via sub_1803C1F4C
};
static_assert(sizeof(C_UIFlashListPerk) == 0x28, "C_UIFlashListPerk must be 0x28 (creator sub_1807EA910)");

}  // namespace wh::guimodule
