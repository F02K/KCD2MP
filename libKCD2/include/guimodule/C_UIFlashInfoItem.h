#pragma once
#include <cstdint>
#include "guimodule/C_UIFlashObject.h"

// -----------------------------------------------
// wh::guimodule::C_UIFlashInfoItem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50, 8 slots (ALLOC-PROVEN).
// -----------------------------------------------
// Root of the InfoItem record subtree (flash_leaves.md §1.5/§3.3): the item-tooltip
// serialization record. Creator sub_1816F30C0 N=96 (ctor 0x18055E528). FillUIArgs
// override 0x1815795AC. ADDS two own virtuals after the RTTR trio:
//   [6] 0x18055F8F4 writes m_condition (+0x44; -2.0 or the item's value) -- setter
//   [7] 0x180838AE0 `return 0` -- gates the conditional serialization of m_condition
// Roles/names coined, UNVERIFIED. Member names descriptive (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashInfoItem : public C_UIFlashObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashInfoItem;
    void FillUIArgs(void* pArgs) const override;   // [1] 0x1815795AC

    virtual void _vf6();   // [6] 0x18055F8F4  SetConditionValue-ish [role UNVERIFIED]
    virtual bool _vf7();   // [7] 0x180838AE0  HasConditionValue-ish gate [role/signature UNVERIFIED]

    CryStringT<char> m_name;        // +0x18
    CryStringT<char> m_icon;        // +0x20
    CryStringT<char> m_category;    // +0x28
    int32_t          m_amount;      // +0x30
    uint8_t          _pad34[4];     // +0x34
    CryStringT<char> m_str38;       // +0x38
    float            m_weight;      // +0x40
    float            m_condition;   // +0x44  ctor default -1.0f; serialized only when _vf7() passes
    float            m_price;       // +0x48
    bool             m_bool4C;      // +0x4C
    bool             m_bool4D;      // +0x4D
    uint8_t          _pad4E[2];     // +0x4E
};
static_assert(sizeof(C_UIFlashInfoItem) == 0x50, "C_UIFlashInfoItem must be 0x50 (creator sub_1816F30C0)");
static_assert(offsetof(C_UIFlashInfoItem, m_condition) == 0x44, "condition at 0x44 (_vf6/_vf7 target)");

}  // namespace wh::guimodule
