#pragma once
#include <cstdint>
#include "C_DynamicLinkableObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DynamicLinkableObjectWithOwnership : C_DynamicLinkableObject
// -- dynamic linkable with a real (non-empty) ownership provider (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof NOT create-site proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4FF5A20; primary vtable 0x183A654F8, 20 slots; I_RWLocked
// subobject vtable @+0x20 = 0x183A654D8.  Distinguishing override: slot [11]
// GetOwnership = sub_181A7FAD0 (ownership-backed, vs the spine's lazy static
// C_EmptyOwnership default sub_18047AFF4).  [19] stays the shared stub.

namespace wh::xgenaimodule {

class C_DynamicLinkableObjectWithOwnership : public C_DynamicLinkableObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DynamicLinkableObjectWithOwnership;
    ~C_DynamicLinkableObjectWithOwnership() override;  // [0]  deleting dtor sub_18096D6FC
    uint32_t GetTypeMask() override;                   // [1]  sub_1812F7A10
    I_Ownership* GetOwnership() override;              // [11] sub_181A7FAD0 -- ownership-backed [U backing member]
};
// sizeof not create-site proven -> no static_assert [U]

}  // namespace wh::xgenaimodule
