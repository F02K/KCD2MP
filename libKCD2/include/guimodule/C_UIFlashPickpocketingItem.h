#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashPickpocketingItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_182BB5078
// N=96 (ctor 0x182B8A9FC). FillUIArgs override 0x182B8D3F4. Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashPickpocketingItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashPickpocketingItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B8D3F4

    CryStringT<char> m_name;     // +0x18
    float            m_f20;      // +0x20
    int32_t          m_int24;    // +0x24
    bool             m_bool28;   // +0x28
    uint8_t          _pad29[3];  // +0x29
    float            m_f2C;      // +0x2C
    int32_t          m_int30;    // +0x30
    float            m_f34;      // +0x34
    CryStringT<char> m_icon;     // +0x38
    float            m_f40;      // +0x40
    int32_t          m_int44;    // +0x44
    float            m_f48;      // +0x48
    uint8_t          _pad4C[4];  // +0x4C
};
static_assert(sizeof(C_UIFlashPickpocketingItem) == 0x50, "C_UIFlashPickpocketingItem must be 0x50 (creator sub_182BB5078)");

}  // namespace wh::guimodule
