#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::I_FaderListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Interface, 4 slots.
// -----------------------------------------------
// RTTI .?AVI_FaderListener@guimodule@wh@@ (TD 0x184CB0B68). Fade-event sink,
// implemented by C_FaderSuspend (@+0x00, vtable 0x183A85A28: [1] 0x180C08FC4,
// [2] 0x180C08F10, [3] 0x180C08EA4). Callback names/roles UNVERIFIED (coined).

namespace wh::guimodule {

class I_FaderListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_FaderListener;
    virtual ~I_FaderListener();     // [0] 0x1808BB150 (C_FaderSuspend impl)
    virtual void OnFadeIn() = 0;    // [1] name coined [role UNVERIFIED]
    virtual void OnFadeOut() = 0;   // [2] name coined [role UNVERIFIED]
    virtual void OnFade() = 0;      // [3] name coined [role UNVERIFIED]
};
static_assert(sizeof(I_FaderListener) == 8);

}  // namespace wh::guimodule
