#pragma once
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::dialogmodule::I_UISubtitles -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 6 slots.
// -----------------------------------------------
// RTTI .?AVI_UISubtitles@dialogmodule@wh@@ (TD 0x184CCFC10). Dialog-module face of the
// subtitle HUD. C_UISubtitles implements it @+0x58 (vtable 0x183C33598) and publishes
// the subobject into the dialogmodule object: *(*(ctx+0x150)+0x10)+0x120 = &subs
// (Init sub_180C3C158). Shape: [0] deleting-dtor thunk 0x18213A53C, 2 methods,
// [3..5] RTTR trio (0x18213A6A4/0x18213A5FC/0x18213A5F0).
//   [1] 0x181457948  ShowSubtitle(text, speaker, flag, bForce, bNoCopy): gated on the
//       wh_ui_SubtitlesEnabled CVar (owner+0x70) unless bForce
//   [2] 0x1817990CC  flash "ClearSubtitles"()
// Names from behavior/flash names; [1] tail param roles UNVERIFIED.

namespace wh::dialogmodule {

class I_UISubtitles {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UISubtitles;
    virtual ~I_UISubtitles();                                     // [0]
    virtual void ShowSubtitle(const CryStringT<char>& text, const CryStringT<char>& speaker,
                              bool a4, bool bForce, bool a6) = 0; // [1] 0x181457948
    virtual void ClearSubtitles() = 0;                            // [2] 0x1817990CC flash "ClearSubtitles"
    RTTR_ENABLE()  // [3..5] thunks -0x58; baseless, get_type 0x182B92838
};
static_assert(sizeof(I_UISubtitles) == 8);

}  // namespace wh::dialogmodule
