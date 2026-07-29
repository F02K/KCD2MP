#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::I_UICamera -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 3 slots.
// -----------------------------------------------
// RTTI .?AVI_UICamera@framework@wh@@ (TD 0x184C92CF0). UI camera hook implemented by
// C_UIApseCamera at subobject +0x10 (impl vtable 0x183B8F418: [1] 0x1814E40A0,
// [2] 0x1809E5BE8). Standalone (pure) vtable 0x183ED92C8: [0] dtor 0x182AFDAF4,
// [1..2] _purecall. NO rttr trio.
// Method roles/signatures UNVERIFIED (coined _vfNN, void() until traced).

namespace wh::framework {

class I_UICamera {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UICamera;
    virtual ~I_UICamera();      // [0] 0x182AFDAF4 (standalone)
    virtual void _vf1() = 0;    // [1] role UNVERIFIED
    virtual void _vf2() = 0;    // [2] role UNVERIFIED
};
static_assert(sizeof(I_UICamera) == 8);

}  // namespace wh::framework
