#pragma once
#include <cstdint>
#include "C_LinkableObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DynamicLinkableObject : C_LinkableObject -- runtime-created
// linkable (dynamic branch of the linkable graph; sibling of the
// C_MessageCapableObject spine branch).  KCD2 WHGame.dll 1.5.6, kd7u.  sizeof NOT
// create-site proven (no own-member init observed; presumed 0x60) -> no size
// assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F5B518; primary vtable 0x183A39688, 20 slots (spine 19 + NEW
// [19]); I_RWLocked subobject vtable @+0x20 = 0x183A39668 (3x nullsub).  Slots
// [2..18] identical to C_LinkableObject's values.  ctor sub_18047BCFC registers in
// the dynamic registry q_1854961A0 (C_DynamicLinkablesManager) instead of
// q_1854961B0.

namespace wh::xgenaimodule {

class C_DynamicLinkableObject : public C_LinkableObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DynamicLinkableObject;
    ~C_DynamicLinkableObject() override;   // [0]  deleting dtor sub_1810AF9D8
    uint32_t GetTypeMask() override;       // [1]  sub_1805D2A10
    virtual void _vf19();                  // [19] NEW sub_180838AE0 (shared stub); Remote* variants override with return-1 sub_18041A6A0 [U role]
};
// sizeof presumed 0x60 (no additional data observed) -- UNPROVEN, no static_assert [U]

}  // namespace wh::xgenaimodule
