#pragma once
#include <cstdint>
#include "C_LinkableObject.h"
#include "I_Area.h"
#include "C_Ownership.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::xgenaimodule::C_TriggerArea : C_LinkableObject (primary, carries
// I_RWLocked @+0x20) + I_Area (@+0x60) -- trigger area with an AABB + shape
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x128 (create-site proven:
// sub_180A2C970 @0x180a2ca07: sub_1804F75C0(296) -> ctor sub_180A2C768).
// Area WUID = idx | 0x0F00000000000000.  Owns a heap 72-byte C_CachedPuppet
// sidecar (sub_18047E4C8) in the base region.
// -----------------------------------------------
// RTTI TD rva 0x4F5B348, COL 0x40DE718.  Vtables: primary rva 0x3A6C170
// (19 slots), I_RWLocked rva 0x3A6C0C0 (3 nullsub), I_Area rva 0x3A6C0E0
// (17 slots; [0] Overlaps = sub_1805CD0E4 reads +0x84/+0x90 as AABB min/max
// [V]).  Primary overrides vs C_LinkableObject: [0]dtor sub_18062F1A4,
// [1]GetTypeMask sub_1805D16D0, [11]GetOwnership 0x1A7FB00, [16]0xC9A5E0,
// [18]0x10085D4.  AABB empty-init: min = float(0x58635FA9) ~ +1e15 x3,
// max = float(0xD8635FA9) ~ -1e15 x3.

namespace wh::xgenaimodule {

class C_TriggerArea
    : public C_LinkableObject,
      public I_Area {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TriggerArea;
    ~C_TriggerArea() override;              // [0]  deleting dtor sub_18062F1A4
    uint32_t GetTypeMask() override;        // [1]  sub_1805D16D0
    I_Ownership* GetOwnership() override;   // [11] 0x1A7FB00
    void _vf16() override;                  // [16] 0xC9A5E0
    void _vf18() override;                  // [18] 0x10085D4
    // I_Area impls (17)
    bool Overlaps(const Vec3& pos) override;  // [0] sub_1805CD0E4 (AABB test) [V]
    void ArVf1() override;  void ArVf2() override;  void ArVf3() override;
    void ArVf4() override;  void ArVf5() override;  void ArVf6() override;
    void ArVf7() override;  void ArVf8() override;  void ArVf9() override;
    void ArVf10() override; void ArVf11() override; void ArVf12() override;
    void ArVf13() override; void ArVf14() override; void ArVf15() override;
    void ArVf16() override;

    uint64_t    m_68;             // +0x68  ctor: 0 (I_Area-adjacent data) [U role]
    uint64_t    m_70;             // +0x70  ctor: 0 [U role]
    uint64_t    m_78;             // +0x78  ctor: 0 [U role]
    float       m_centerZOffset;  // +0x80  Z offset added to the polygon centroid by I_Area GetCenter slot[7] sub_1832EE814
    Vec3        m_boundsMin;      // +0x84  empty AABB min (~+1e15 each) [V via Overlaps]
    Vec3        m_boundsMax;      // +0x90  empty AABB max (~-1e15 each) [V]
    uint8_t     _pad9C[4];        // +0x9C
    uint8_t     m_shapeA0[0x48];  // +0xA0..+0xE8  shape/points container (sub_180A2CBE4) [U type]
    I_Area*     m_areaBackPtr;    // +0xE8  = this + 0x60 (self back-ref to the I_Area subobject; ctor 0x180A2C845, non-owning)
    uint16_t    m_F0;             // +0xF0  ctor: 0 [U role]
    bool        m_bTrackAlways;   // +0xF2  entity property "bTrackAlways"
    uint8_t     _padF3[5];        // +0xF3
    C_Ownership m_ownership;      // +0xF8  embedded (0x30)
};
static_assert(sizeof(C_TriggerArea) == 0x128, "C_TriggerArea must be 0x128 (alloc 296 at sub_180A2C970)");

}  // namespace wh::xgenaimodule
