#pragma once
#include <cstdint>
#include <vector>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashMapQuestMarker -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58 (ALLOC-PROVEN).
// -----------------------------------------------
// C_UIFlashObject serialization record (flash_leaves.md §1). Creator sub_1814A8108
// N=104 (ctor 0x1814A81D0). FillUIArgs override 0x1819BF100 (serializes the point list
// inline: count then x/y pairs, loop stride 8 = two floats). Names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashMapQuestMarker : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashMapQuestMarker;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1819BF100

    CryStringT<char> m_name;     // +0x18
    CryStringT<char> m_str20;    // +0x20
    int32_t          m_int28;    // +0x28
    int32_t          m_int2C;    // +0x2C
    int32_t          m_int30;    // +0x30
    float            m_x;        // +0x34
    float            m_y;        // +0x38
    uint8_t          _pad3C[4];  // +0x3C
    std::vector<Vec2> m_points;  // +0x40  (0x18) area outline points, stride 8 = {float,float}
};
static_assert(sizeof(C_UIFlashMapQuestMarker) == 0x58, "C_UIFlashMapQuestMarker must be 0x58 (creator sub_1814A8108)");
static_assert(offsetof(C_UIFlashMapQuestMarker, m_points) == 0x40, "point list at 0x40");

}  // namespace wh::guimodule
