#pragma once
#include <cstdint>
#include "C_SmartEntity.h"
#include "I_Area.h"
#include "C_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartArea : C_SmartEntity + I_Area (@+0x188) -- smart
// area (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x270 (create-site proven:
// sub_180D9B578 @0x180d9b5ff: sub_1804F75C0(624) -> ctor sub_180D9B8E0).
// -----------------------------------------------
// RTTI TD rva 0x4F51E68 (10 bases; I_Area mdisp = 392).  Vtables: primary rva
// 0x3AA3F08 (40 slots); +0x20 I_RWLocked 0x3AA3E00; +0xA8 listener 0x3AA3E20;
// +0xB0 public iface 0x3AA3E40; +0x188 I_Area 0x3AA3E78 (17 slots:
// [0]0x5CD1DC [1]0x47B610 [2]0x32C37E8 [3]0x467FD4 [4]0xB574D0 [5]0x32C3960
// [6]nullsub [7]0x32C2FC0 [8][9]0x1A7FAD0 [10]0x5CB3E8 [11]0x876930
// [12]0x877AA0 [13]0x213D544 [14]0x213D58C [15]0x213D580 [16]0x213D574).
// Primary overrides vs C_SmartEntity: [0]dtor 0xD9B30C, [1]0x14A8040,
// [11]GetOwnership 0x441460, [16]0x11E07E0, [18]0x16A7538, [24]0x1323B68,
// [28]0x17855B8, [29]0xB4448C, [30]0xD9B350, [31]0x1AA5B20, [32]0x19CA0D0,
// [33]0x1AA5B00, [34]0x1AA5AE0, [35]0x11D10A8, [38]0x32C3980, [39]0x1A72470.
// Empty AABB init: min = 3x float(0x58635FA9) ~ +1e15, max = 3x
// float(0xD8635FA9) ~ -1e15 (inside-out box).

namespace wh::xgenaimodule {

class I_Ownership;

class C_SmartArea
    : public C_SmartEntity,
      public I_Area {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartArea;
    ~C_SmartArea() override;                // [0]  deleting dtor 0xD9B30C
    uint32_t GetTypeMask() override;        // [1]  0x14A8040
    I_Ownership* GetOwnership() override;   // [11] 0x441460
    void _vf16() override;                  // [16] 0x11E07E0
    void _vf18() override;                  // [18] 0x16A7538
    void _vf24() override;                  // [24] 0x1323B68
    void _vf28() override;                  // [28] 0x17855B8
    void _vf29() override;                  // [29] 0xB4448C
    void _vf30() override;                  // [30] 0xD9B350
    void _vf31() override;                  // [31] 0x1AA5B20
    void _vf32() override;                  // [32] 0x19CA0D0
    void _vf33() override;                  // [33] 0x1AA5B00
    void _vf34() override;                  // [34] 0x1AA5AE0
    void _vf35() override;                  // [35] 0x11D10A8
    void _vf38() override;                  // [38] 0x32C3980
    void _vf39() override;                  // [39] 0x1A72470
    // I_Area impls (17)
    bool Overlaps(const Vec3& pos) override;  // [0] 0x5CD1DC
    void ArVf1() override;  void ArVf2() override;  void ArVf3() override;
    void ArVf4() override;  void ArVf5() override;  void ArVf6() override;
    void ArVf7() override;  void ArVf8() override;  void ArVf9() override;
    void ArVf10() override; void ArVf11() override; void ArVf12() override;
    void ArVf13() override; void ArVf14() override; void ArVf15() override;
    void ArVf16() override;

    std::vector<Vec3> m_shapePoints; // +0x190  polygon boundary vertices (assign sub_18062E708 frees/memmoves 12*N)
    float       m_1A8;           // +0x1A8  ctor: -1.0f [U role]
    Vec3        m_boundsMin;     // +0x1AC  empty AABB min (~+1e15 each)
    Vec3        m_boundsMax;     // +0x1B8  empty AABB max (~-1e15 each)
    uint8_t     _pad1C4[4];      // +0x1C4
    uint8_t     m_containers1C8[0x60]; // +0x1C8..+0x228  zeroed containers [U types]
    uint8_t     _unk228[8];      // +0x228  [U] 8-byte, ctor-zeroed; no writer found in ctor/init/shape/registration/dtor
    I_Area*     m_areaBackPtr;   // +0x230  = this+0x188 (cached self-ptr to own I_Area subobject; ctor 0x180d9b9e1)
    uint16_t    m_238;           // +0x238  ctor: 1 [U role]
    uint8_t     _pad23A[6];      // +0x23A
    C_Ownership m_ownership;     // +0x240  embedded (0x30)
};
static_assert(sizeof(C_SmartArea) == 0x270, "C_SmartArea must be 0x270 (alloc 624 at sub_180D9B578)");

}  // namespace wh::xgenaimodule
