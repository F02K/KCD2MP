#pragma once
#include <cstdint>
#include "C_DynamicLinkableObjectWithOwnership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_RemoteDynamicLinkableObjectWithOwnership :
// C_DynamicLinkableObjectWithOwnership -- remote + ownership-backed dynamic
// linkable (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT create-site proven -> no
// size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4FF5990; primary vtable 0x183A655A0, 20 slots; I_RWLocked
// subobject vtable @+0x20 = 0x183A654B8.  [11] GetOwnership = sub_181A7FAD0
// (inherited from the WithOwnership parent); [19] = sub_18041A6A0 (return 1, the
// remote marker shared with C_RemoteDynamicLinkableObject).

namespace wh::xgenaimodule {

class C_RemoteDynamicLinkableObjectWithOwnership : public C_DynamicLinkableObjectWithOwnership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RemoteDynamicLinkableObjectWithOwnership;
    ~C_RemoteDynamicLinkableObjectWithOwnership() override;  // [0]  deleting dtor sub_18096D5A8
    uint32_t GetTypeMask() override;                         // [1]  sub_181440750
    void _vf19() override;                                   // [19] sub_18041A6A0 -- return 1 [U role]
};
// sizeof not create-site proven -> no static_assert [U]

}  // namespace wh::xgenaimodule
