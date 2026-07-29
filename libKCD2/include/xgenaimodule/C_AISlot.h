#pragma once
#include <cstdint>
#include "C_DynamicLinkableObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AISlot : C_DynamicLinkableObject -- AI slot object (a
// dynamic linkable with ownership-backed GetOwnership).  KCD2 WHGame.dll 1.5.6,
// kd7u.  sizeof NOT create-site proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4FF59E8; primary vtable 0x183A6B908, 20 slots; I_RWLocked
// subobject vtable @+0x20 = 0x183A6B9B0.  [11] GetOwnership = sub_181A7FAD0
// (ownership-backed); [19] = shared stub sub_180838AE0.

namespace wh::xgenaimodule {

class C_AISlot : public C_DynamicLinkableObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AISlot;
    ~C_AISlot() override;                  // [0]  deleting dtor sub_180A2A028
    uint32_t GetTypeMask() override;       // [1]  sub_1805D2A00
    I_Ownership* GetOwnership() override;  // [11] sub_181A7FAD0 -- ownership-backed [U backing member]
};
// sizeof not create-site proven -> no static_assert [U]

}  // namespace wh::xgenaimodule
