#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListCodexRecord -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x48 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject CONTAINER record (flash_leaves.md §1/§3.2). Creator sub_1815556BC
// N=88 (ctor 0x181555784). FillUIArgs override 0x1816BFB8C; m_entries serialized by
// the _vf2 [2] override. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashListCodexRecord : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListCodexRecord;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1816BFB8C
    void _vf2(void* pArgs) const override;         // [2] serializes m_entries

    int32_t          m_int18;    // +0x18
    uint8_t          _pad1C[4];  // +0x1C
    CryStringT<char> m_name;     // +0x20
    bool             m_bool28;   // +0x28  inline tag-6 builder sub_181F44490
    uint8_t          _pad29[7];  // +0x29
    std::vector<uint64_t> m_entries;   // +0x30  (0x18) element type UNVERIFIED (flash_leaves.md §5.3)
};
static_assert(sizeof(C_UIFlashListCodexRecord) == 0x48, "C_UIFlashListCodexRecord must be 0x48 (creator sub_1815556BC)");

}  // namespace wh::guimodule
