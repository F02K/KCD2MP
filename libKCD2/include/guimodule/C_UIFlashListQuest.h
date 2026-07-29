#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListQuest -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_180DC8A6C
// N=112 (ctor 0x180DC8278). FillUIArgs override 0x1819C8C80. Names descriptive
// (UNVERIFIED); the +0x48 tail is ctor-only.

namespace wh::guimodule {

class C_UIFlashListQuest : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListQuest;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819C8C80

    int32_t          m_int18;    // +0x18
    int32_t          m_int1C;    // +0x1C
    int32_t          m_int20;    // +0x20
    int32_t          m_int24;    // +0x24
    int32_t          m_int28;    // +0x28
    uint8_t          _pad2C[4];  // +0x2C
    CryStringT<char> m_name;     // +0x30
    bool             m_bool38;   // +0x38
    uint8_t          _pad39[7];  // +0x39
    CryStringT<char> m_str40;    // +0x40
    std::vector<std::shared_ptr<C_UIFlashObject>> m_children;  // +0x48  std::vector<shared_ptr<T>> (empty in ctor; T inferred C_UIFlashObject, see notes)
};
static_assert(sizeof(C_UIFlashListQuest) == 0x60, "C_UIFlashListQuest must be 0x60 (creator sub_180DC8A6C)");

}  // namespace wh::guimodule
