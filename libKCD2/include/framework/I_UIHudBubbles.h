#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::framework::I_UIHudBubbles -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 9 slots.
// -----------------------------------------------
// RTTI .?AVI_UIHudBubbles@framework@wh@@ (TD 0x184CD0A90). Framework-side face of the
// speech-bubble HUD; C_UIHudBubbles implements it @+0x58 (vtable 0x183C297C8) and
// PUBLISHES the subobject pointer into the framework context: *(*(ctx 0x18549D388
// +0xE0)+0x180) = &bubbles (Init sub_180C3CB74 / cleared by Deinit sub_182B8CA80) --
// that pointer is how dialog code drives bubbles without a guimodule dependency.
// Slot bodies (C_UIHudBubbles impls):
//   [0] deleting-dtor thunk 0x18213A434 (-0x58 -> 0x182B8BD3C)
//   [1] 0x1805E0520  bubble refresh/update sweep (heavy debug-CVar block dword_1855E2B18)
//   [2] 0x1814BE770  allocate a bubble id (reuses the recycled-id list in the OWNER
//       C_UIHudBubbles @owner+0x60 -- the iface subobj sits @owner+0x58 -- else new
//       id = total count+1; hides the flash clip via var-obj vf[+0x108](0))
//   [3] 0x1814BE954  release a bubble id back to the recycle list (+ hide clip)
//   [4] 0x1808C4710  flash "SetBubbleText"(id, text, speaker, ...)
//   [5] 0x1814BEA44  write bubble screen anchor {pos, int, pct*100, visible=1} into the
//       per-bubble record
//   [6..8] RTTR_ENABLE() trio thunks (0x18213A620/0x18213A5FC/0x18213A56C)
// All method names coined from behavior; exact parameter lists UNVERIFIED beyond the
// observed machine signatures noted above.

namespace wh::framework {

class I_UIHudBubbles {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_UIHudBubbles;
    virtual ~I_UIHudBubbles();                                                       // [0]
    virtual void UpdateBubbles() = 0;                                                // [1] 0x1805E0520 (name coined)
    virtual uint32_t AllocBubble() = 0;                                              // [2] 0x1814BE770 (name coined)
    virtual void ReleaseBubble(uint32_t bubbleId) = 0;                               // [3] 0x1814BE954 (name coined)
    virtual void SetBubbleText(int bubbleId, const CryStringT<char>& text,
                               const CryStringT<char>& speaker, bool bLocalize) = 0; // [4] 0x1808C4710 flash "SetBubbleText" (tail args UNVERIFIED)
    virtual void SetBubbleAnchor(const void* pPos, float pct) = 0;                   // [5] 0x1814BEA44 (params UNVERIFIED)
    RTTR_ENABLE()  // [6..8] thunks 0x18213A620/0x18213A5FC/0x18213A56C (-0x58); registered BASELESS, get_type 0x182B927AC
};
static_assert(sizeof(I_UIHudBubbles) == 8);

}  // namespace wh::framework
