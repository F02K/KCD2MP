#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78, 6 slots (ALLOC-PROVEN).
// -----------------------------------------------
// Root of the ListItem record subtree (flash_leaves.md §3.4): the inventory-list row
// serialization record. Creator sub_1812B3F18 N=136 (ctor 0x1808F67C4). FillUIArgs
// override 0x1805552B4. The +0x18..+0x70 block is IDENTICAL across the subtree.
// Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashListItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1805552B4

    int32_t          m_int18;     // +0x18
    uint8_t          _pad1C[4];   // +0x1C
    CryStringT<char> m_name;      // +0x20
    CryStringT<char> m_icon;      // +0x28
    float            m_f30;       // +0x30
    float            m_f34;       // +0x34
    float            m_f38;       // +0x38
    int32_t          m_int3C;     // +0x3C
    int32_t          m_int40;     // +0x40
    float            m_f44;       // +0x44
    float            m_f48;       // +0x48
    bool             m_bool4C;    // +0x4C
    uint8_t          _pad4D[3];   // +0x4D
    int32_t          m_int50;     // +0x50
    bool             m_bool54;    // +0x54
    bool             m_bool55;    // +0x55
    uint8_t          _pad56[2];   // +0x56
    int32_t          m_int58;     // +0x58
    float            m_f5C;       // +0x5C
    float            m_f60;       // +0x60
    int32_t          m_flags64;   // +0x64  bitset of item categories
    CryStringT<char> m_str68;     // +0x68
    bool             m_bool70;    // +0x70
    uint8_t          _pad71[7];   // +0x71
};
static_assert(sizeof(C_UIFlashListItem) == 0x78, "C_UIFlashListItem must be 0x78 (creator sub_1812B3F18)");
static_assert(offsetof(C_UIFlashListItem, m_flags64) == 0x64, "category bitset at 0x64");

}  // namespace wh::guimodule
