#pragma once
#include <cstdint>
#include "I_DialogCameraManager.h"
#include "C_AnimatedCamera.h"
#include "../crysystem/IEntitySystemSink.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogCameraManager -- dialog camera orchestrator
// (KCD2 1.5.6, kd7u).  sizeof 0xD0 (alloc 208).
// -----------------------------------------------
// RTTI TD 0x184B4B960. Bases: I_DialogCameraManager (mdisp 0, primary vtable 0x183E63CA8,
// 7 slots) + ::IEntitySystemSink (mdisp 0x8, vtable 0x183E63CE8, 8 slots). ctor
// sub_1816F365C; deleting-dtor sub_1827FBDF0. Created in the manager CVar-reg fn
// sub_180E3DE3C and stored at C_DialogManager+0x188. Owns 4 C_AnimatedCamera (one per
// shot type; alloc 208 each) and registers its sink subobject (this+0x8) with the
// CryEntitySystem via qword_18492D8A0->vf[29](this+8, 4). Field roles beyond the camera
// slots UNVERIFIED (framing/selection state). CVar wh_dlg_NoCameras disables the whole
// system (KCD1: removed letterbox bars but killed the dialog cam).

namespace wh::dialogmodule {

class C_DialogCameraManager : public I_DialogCameraManager, public ::IEntitySystemSink {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogCameraManager;
    // +0x00 I_DialogCameraManager vptr / +0x08 IEntitySystemSink vptr
    uint64_t _o10;                 // +0x10  ctor 0 [role UNVERIFIED]
    C_AnimatedCamera* m_pCam1;     // +0x18  shot type 1
    C_AnimatedCamera* m_pCam2;     // +0x20  shot type 2
    C_AnimatedCamera* m_pCam3;     // +0x28  shot type 3
    uint64_t _q30;                 // +0x30  ctor 0
    C_AnimatedCamera* m_pCam0;     // +0x38  shot type 0
    void*    _q40;                 // +0x40  ctor 0
    uint64_t _q48;                 // +0x48  [not walked]
    int32_t  _d50;                 // +0x50  ctor 0
    float    _f54;                 // +0x54  ctor 1.0f
    Vec3     _v58;                 // +0x58  ctor {0,0,0}
    float    _f64;                 // +0x64  ctor 65.0f
    bool     _b68;                 // +0x68  ctor 0
    uint8_t  _pad69[3];            // +0x69
    uint32_t _d6C;                 // +0x6C  ctor 0 (8-byte zero write @+0x6C, split for alignment)
    uint32_t _d70;                 // +0x70  ctor 0
    float    _f74;                 // +0x74  ctor 1.0f
    int32_t  _d78;                 // +0x78  ctor 2
    uint8_t  _pad7C[4];            // +0x7C
    void*    _q80;                 // +0x80  ctor 0
    int32_t  _d88;                 // +0x88  ctor 5
    int16_t  _w8C;                 // +0x8C  ctor 256
    uint8_t  _pad8E[2];            // +0x8E
    uint64_t _q90;                 // +0x90  ctor 0
    uint64_t _q98;                 // +0x98  ctor 0
    uint64_t m_listA0[3];          // +0xA0..+0xB7  intrusive list head (alloc-56 self-ref
                                   //         sentinel; active-camera list) [type UNVERIFIED]
    int32_t  _dB8;                 // +0xB8  ctor 0
    uint8_t  _padBC[4];            // +0xBC
    uint8_t  _bC0;                 // +0xC0  [not walked]
    uint8_t  _bC1;                 // +0xC1  ctor 1 (16-bit write {1,0} @+0xC1)
    uint8_t  _bC2;                 // +0xC2  ctor 0
    uint8_t  _bC3;                 // +0xC3  ctor 0
    uint8_t  _padC4[4];            // +0xC4
    int64_t  m_qC8;                // +0xC8  ctor -100000 [sentinel; role UNVERIFIED]
};
static_assert(sizeof(C_DialogCameraManager) == 0xD0, "C_DialogCameraManager must be 0xD0 (alloc 208)");
static_assert(offsetof(C_DialogCameraManager, m_pCam1) == 0x18);
static_assert(offsetof(C_DialogCameraManager, m_pCam0) == 0x38);
static_assert(offsetof(C_DialogCameraManager, m_qC8) == 0xC8);

}  // namespace wh::dialogmodule
