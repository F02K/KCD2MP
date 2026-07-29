#pragma once
#include <cstdint>
#include "C_LinkableObject.h"
#include "I_Area.h"
#include "C_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AreaUnion : C_LinkableObject (primary, carries
// I_RWLocked @+0x20) + I_Area (@+0x60) -- union of sub-areas (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x108 (create-site proven: sub_1811A4DF4 @0x1811a4e29:
// sub_1804F75C0(264) -> ctor sub_1811A4EA0).  Area WUID = idx |
// 0x1400000000000000.  Same base region as C_TriggerArea; NO AABB block, no
// bTrackAlways (32 bytes smaller).
// -----------------------------------------------
// RTTI TD rva 0x4F47130, COL 0x40E03D8.  Vtables: primary rva 0x3B27AD0
// (19 slots), I_RWLocked rva 0x3B27A20 (3 nullsub), I_Area rva 0x3B27A40
// (17 slots; [0] = sub_1805CCF2C).  Primary overrides vs C_LinkableObject:
// [0]dtor sub_1811A4CC4, [11]GetOwnership 0x1AA49D0 (IDA-mislabeled thunk),
// [16]0x1694CE8, [18]0x158F9C8; [1] GetTypeMask INHERITED (0x5D29F0 =
// C_LinkableObject's own body).

namespace wh::xgenaimodule {

class C_AreaUnion
    : public C_LinkableObject,
      public I_Area {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AreaUnion;
    ~C_AreaUnion() override;                // [0]  deleting dtor sub_1811A4CC4
    I_Ownership* GetOwnership() override;   // [11] 0x1AA49D0
    void _vf16() override;                  // [16] 0x1694CE8
    void _vf18() override;                  // [18] 0x158F9C8
    // I_Area impls (17)
    bool Overlaps(const Vec3& pos) override;  // [0] sub_1805CCF2C
    void ArVf1() override;  void ArVf2() override;  void ArVf3() override;
    void ArVf4() override;  void ArVf5() override;  void ArVf6() override;
    void ArVf7() override;  void ArVf8() override;  void ArVf9() override;
    void ArVf10() override; void ArVf11() override; void ArVf12() override;
    void ArVf13() override; void ArVf14() override; void ArVf15() override;
    void ArVf16() override;

    uint64_t    m_68;             // +0x68  ctor: 0 [U role]
    uint64_t    m_70;             // +0x70  ctor: 0 [U role]
    uint64_t    m_78;             // +0x78  ctor: 0 [U role]
    uint8_t     m_members80[0x48]; // +0x80..+0xC8  sub-area container (sub_180A2CBE4) [U type]
    I_Area*     m_areaBackPtr;    // +0xC8  = &this->I_Area subobject (this+0x60); ctor stores a1+96
    uint16_t    m_D0;             // +0xD0  ctor: 0 [U role]
    uint8_t     _padD2[6];        // +0xD2
    C_Ownership m_ownership;      // +0xD8  embedded (0x30)
};
static_assert(sizeof(C_AreaUnion) == 0x108, "C_AreaUnion must be 0x108 (alloc 264 at sub_1811A4DF4)");

}  // namespace wh::xgenaimodule
