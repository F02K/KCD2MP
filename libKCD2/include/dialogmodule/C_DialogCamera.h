#pragma once
#include <cstdint>
#include <vector>
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogCamera -- dialog camera base (framing params)
// (KCD2 1.5.6, kd7u).  sizeof 0xC8.
// -----------------------------------------------
// RTTI TD 0x184B49C98; primary vtable 0x183BFC350 (17 slots); ctor
// sub_1816F3948(this, ownerMgr); deleting-dtor sub_1827F2D54. Builds the dialog shot
// each frame from a bundle of Vec3s + fov/scale params. Size derivation: a deleting-dtor
// variant frees a vector at +0xB0 (a1[22..24]) and C_AnimatedCamera's own fields begin
// at +0xC8 (its ctor zeroes +0xB0/+0xB8/+0xC0 = this base vector) => base extends to
// 0xC8 with the trailing vector. Field NAMES coined (UNVERIFIED); slot roles unwalked
// -- KCD1 parity suggests FillViewParams (the 2.35 letterbox producer) lives on this
// vtable family; CONFIRM the exact KCD2 slot before hooking.

namespace wh::dialogmodule {

class C_DialogCameraManager;

class C_DialogCamera {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogCamera;
    virtual ~C_DialogCamera();   // [0]
    virtual void _vf1();         // [1..16] framing/update slots [roles UNVERIFIED;
    virtual void _vf2();         //         letterbox FillViewParams candidate among them]
    virtual void _vf3();
    virtual void _vf4();
    virtual void _vf5();
    virtual void _vf6();
    virtual void _vf7();
    virtual void _vf8();
    virtual void _vf9();
    virtual void _vf10();
    virtual void _vf11();
    virtual void _vf12();
    virtual void _vf13();
    virtual void _vf14();
    virtual void _vf15();
    virtual void _vf16();

    C_DialogCameraManager* m_pOwner;  // +0x08  ctor sub_1816F3948 arg2 (non-owning back-ptr to owning manager)
    void*    _q10;               // +0x10  ctor 0
    int32_t  _d18;               // +0x18  ctor 0
    float    _f1C;               // +0x1C  ctor 1.0f
    uint32_t _d20[3];            // +0x20..+0x2B  [not walked]
    float    _f2C;               // +0x2C  ctor 1.0f
    Vec3     _v30;               // +0x30  ctor {0,0,0}
    Vec3     _v3C;               // +0x3C  ctor {1,1,1}
    float    m_fov;              // +0x48  ctor 50.0f (C_AnimatedCamera ctor overrides)
    Vec3     _v4C;               // +0x4C  ctor {0,0,0}
    Vec3     _v58;               // +0x58  ctor {1,1,1}
    Vec3     _v64;               // +0x64  ctor {0,0,0}
    Vec3     _v70;               // +0x70  ctor {0,0,0}
    uint32_t _d7C;               // +0x7C  ctor 0 (8-byte zero write @+0x7C, split for alignment)
    uint32_t _d80;               // +0x80  ctor 0
    int32_t  _d84;               // +0x84  ctor 0
    float    _f88;               // +0x88  ctor 1.0f
    uint32_t _d8C[3];            // +0x8C..+0x97  [not walked]
    float    _f98;               // +0x98  ctor 1.0f
    uint32_t _d9C;               // +0x9C  [not walked]
    bool     _bA0;               // +0xA0  ctor 1
    uint8_t  _padA1[3];          // +0xA1
    float    _fA4;               // +0xA4  ctor 5.0f
    float    _fA8;               // +0xA8  ctor ~0.0105f (small angle/damp threshold)
    uint8_t  _padAC[4];          // +0xAC
    std::vector<void*> m_vecB0;  // +0xB0  freed by the deleting-dtor variant [elem type UNVERIFIED]
};
static_assert(sizeof(C_DialogCamera) == 0xC8, "C_DialogCamera must be 0xC8 (derived fields start at +0xC8)");

}  // namespace wh::dialogmodule
