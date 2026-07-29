#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashInfoItem.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItemQAMContainer -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60, 9 slots (ALLOC-PROVEN).
// -----------------------------------------------
// InfoItem-subtree record (flash_leaves.md §3.3). Creator sub_182B0C83C N=112 (ctor
// 0x182B06690). FillUIArgs override 0x182B636F4. Overrides [6] 0x182B639FC /
// [7] 0x18041A6A0; ADDS [8] 0x182B63808 (role UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItemQAMContainer : public C_UIFlashInfoItem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItemQAMContainer;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x182B636F4
    void _vf6() override;                          // [6] 0x182B639FC
    bool _vf7() override;                          // [7] 0x18041A6A0
    virtual void _vf8();                           // [8] 0x182B63808  role UNVERIFIED

    float   m_f50;     // +0x50
    float   m_f54;     // +0x54
    int32_t m_int58;   // +0x58
    int32_t m_int5C;   // +0x5C
};
static_assert(sizeof(C_UIFlashInfoItemQAMContainer) == 0x60, "C_UIFlashInfoItemQAMContainer must be 0x60 (creator sub_182B0C83C)");

}  // namespace wh::guimodule
