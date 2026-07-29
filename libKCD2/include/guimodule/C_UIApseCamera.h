#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/C_UILightsManager.h"
#include "framework/I_UICamera.h"
#include "framework/C_Signal.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseCamera -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x1A8.
// -----------------------------------------------
// RTTI .?AVC_UIApseCamera@guimodule@wh@@ (TD 0x184C92F28). ctor sub_18146D1E0.
// Bases: C_UIBase @+0x00 + wh::framework::I_UICamera @+0x10. vtables: primary
// 0x183B8F438 (10 slots; only [4] Update 0x180567A04 overridden) / camera 0x183B8F418
// (3: dtor-thunk 0x18213A198, [1] 0x1814E40A0, [2] 0x1809E5BE8).
// Embedded BY VALUE in C_UIApse @+0x25C0 (size pinned by next member dword @+0x2768).
// [G4a] This is the REAL Apse camera embed -- the old map_vertical.md claim of a
// camera at Apse+0x10C8 mislabeled the C_UIQuestLog embed (see apse.md §1).
//
// Menu-camera driver for the Apse 3D character view; owns the UI lights rig.
// Members +0x28..+0xF8 and the float blocks are UNVERIFIED (sub_18146D7C0 sub-object
// not traced this wave).

namespace wh::guimodule {

class C_UIApse;

class C_UIApseCamera : public C_UIBase, public wh::framework::I_UICamera {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseCamera;
    // I_UICamera impls (subobject vtable 0x183B8F418).
    void _vf1() override;   // [1] 0x1814E40A0  role UNVERIFIED
    void _vf2() override;   // [2] 0x1809E5BE8  role UNVERIFIED

    wh::shared::C_Signal<> m_signal18;  // +0x18  delegate table &unk_185665430 [signature UNVERIFIED]
    uint8_t   m_unk28[0xCC];            // +0x28  POD block; ctor sub_18146D7C0 (0x18146D7CA..0x18146DA52) zero-inits as 17 contiguous 12-byte (Vec3-sized) groups; element type/roles UNVERIFIED, no reader/writer located (hypothesis: Vec3 m_vecs28[17])
    uint8_t   _padF4[4];                // +0xF4  uninitialized by ctor; alignment pad before 8-aligned C_UILightsManager @+0xF8
    C_UILightsManager m_lights;         // +0xF8  (0x40) UI lights rig (ctor sub_18146D41C; matches spine.md)
    float     m_floats138[12];          // +0x138..+0x168  ctor float block [roles UNVERIFIED]
    uint8_t   m_fade168[0x38];          // +0x168..+0x1A0  fade timing (sub_180A70610(this+0x16C, ...)) [layout UNVERIFIED]
    C_UIApse* m_pApse;                  // +0x1A0  owner hub (ctor a2)
};
static_assert(sizeof(C_UIApseCamera) == 0x1A8, "C_UIApseCamera must be 0x1A8");
static_assert(offsetof(C_UIApseCamera, m_lights) == 0xF8, "lights manager at 0xF8");

}  // namespace wh::guimodule
