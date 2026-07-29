#pragma once
#include <cstdint>
#include "C_DynamicLinkableObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_RemoteDynamicLinkableObject : C_DynamicLinkableObject --
// "remote" dynamic linkable variant (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT
// create-site proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4FF5940; primary vtable 0x183A65410, 20 slots; I_RWLocked
// subobject vtable @+0x20 = 0x183A653F0.  Distinguishing override: [19] =
// sub_18041A6A0 (return 1) vs the dynamic base's shared stub.  [11] GetOwnership
// stays the spine default (C_EmptyOwnership).

namespace wh::xgenaimodule {

class C_RemoteDynamicLinkableObject : public C_DynamicLinkableObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RemoteDynamicLinkableObject;
    ~C_RemoteDynamicLinkableObject() override;  // [0]  deleting dtor sub_18096D6A8
    uint32_t GetTypeMask() override;            // [1]  sub_1813FBA80
    void _vf19() override;                      // [19] sub_18041A6A0 -- return 1 ("is remote"?) [U role]
};
// sizeof not create-site proven -> no static_assert [U]

}  // namespace wh::xgenaimodule
