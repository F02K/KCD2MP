#pragma once
#include <cstdint>
#include "C_DialogCamera.h"
#include "E_DialogCameraType.h"

// -----------------------------------------------
// wh::dialogmodule::C_AnimatedCamera -- concrete dialog shot camera (KCD2 1.5.6, kd7u).
// sizeof 0xD0 (alloc 208, x4 in the camera manager ctor).
// -----------------------------------------------
// RTTI TD 0x184B49CD0; primary vtable 0x183BFC2C0 (17 slots -- same shape as the base,
// overrides only); ctor sub_1816F38F4(this, ownerMgr, int type): C_DialogCamera ctor
// first, then overrides base m_fov = ~0.598f and sets m_type = arg3 (shot type 0..3).
// C_DialogCameraManager owns one per shot type (+0x18/+0x20/+0x28/+0x38).
// KCD1 parity: this class produced the 2.35 cinemascope letterbox via FillViewParams
// (KCD1 vtable slot 2) -- in KCD2 the equivalent lives on vtable 0x183BFC2C0; CONFIRM
// the slot before hooking (see memory reference_dialogue_letterbox).

namespace wh::dialogmodule {

class C_AnimatedCamera : public C_DialogCamera {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AnimatedCamera;
    E_DialogCameraType m_type;  // +0xC8  shot type (ctor arg3; the manager's 4 cameras use BONE_MASTER..BONE_CLOSEUP)
    uint8_t _padCC[4];   // +0xCC
};
static_assert(sizeof(C_AnimatedCamera) == 0xD0, "C_AnimatedCamera must be 0xD0 (alloc 208)");

}  // namespace wh::dialogmodule
