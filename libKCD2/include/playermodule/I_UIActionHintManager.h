#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::I_UIActionHintManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface.
// -----------------------------------------------
// RTTI .?AVI_UIActionHintManager@playermodule@wh@@ -- declared in PLAYERMODULE (the
// consumer side of action hints) but implemented only by guimodule's
// C_UIActionHintManager (primary base @+0x00).
//
// Shape evidence: C_UIActionHintManager's primary vtable 0x183C12518 = 6 slots.
// Roles derived from the implementation bodies; coined names flagged.

namespace wh::playermodule {

class I_UIActionHintManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIActionHintManager;
    virtual uint32_t GetInputDeviceClass() const = 0;     // [0] 0x181A727F0  returns the +0x20 device-class value (1=KB/M, 2/16=gamepad flavors; name UNVERIFIED)
    virtual void _vf1() = 0;                              // [1] 0x180F69D48  forwards to the +0x30 cached object vf[+0x10] (UNVERIFIED)
    virtual bool IsGamepadConnected() const = 0;          // [2] 0x180F69CA8  scans pInput devices (type 4, connected) (name UNVERIFIED)
    virtual void _vf3() = 0;                              // [3] 0x180A106CC  action -> hint-text composition (UNVERIFIED)
    virtual void GetKeyLabel(CryStringT<char>& out, const char* keyName) const = 0;  // [4] 0x181519B88  key -> "@ui_control_uc_%d" / "@ui_key_%s" loc label
    virtual ~I_UIActionHintManager();                     // [5] deleting dtor slot (impl 0x182BB8630)
};
static_assert(sizeof(I_UIActionHintManager) == 8);

}  // namespace wh::playermodule
