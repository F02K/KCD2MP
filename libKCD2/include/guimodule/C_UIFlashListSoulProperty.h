#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashListSoulProperty -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x60 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_18055A268
// N=112 (ctor 0x18055ADB0). FillUIArgs override 0x180556EF4. Names descriptive
// (UNVERIFIED); the +0x44 tail is ctor-only.

namespace wh::guimodule {

class C_UIFlashListSoulProperty : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashListSoulProperty;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x180556EF4

    CryStringT<char> m_name;        // +0x18
    CryStringT<char> m_str20;       // +0x20
    int32_t          m_int28;       // +0x28
    int32_t          m_int2C;       // +0x2C
    int32_t          m_int30;       // +0x30
    int32_t          m_int34;       // +0x34
    int32_t          m_int38;       // +0x38
    float            m_val3C;       // +0x3C
    int32_t          m_int40;       // +0x40
    uint8_t          _pad44[4];     // +0x44  alignment padding (ctor never writes; 8-aligns the vector)
    std::vector<std::shared_ptr<C_UIFlashObject>> m_children;  // +0x48  dtor 0x1814BD15C {first,last,end}; pointee INFERRED
};
static_assert(sizeof(C_UIFlashListSoulProperty) == 0x60, "C_UIFlashListSoulProperty must be 0x60 (creator sub_18055A268)");

}  // namespace wh::guimodule
