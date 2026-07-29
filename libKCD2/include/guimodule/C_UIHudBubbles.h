#pragma once
#include <cstdint>
#include <list>
#include "guimodule/C_UIFlashBase.h"
#include "framework/I_UIHudBubbles.h"

// -----------------------------------------------
// wh::guimodule::C_UIHudBubbles -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x88.
// -----------------------------------------------
// RTTI .?AVC_UIHudBubbles@guimodule@wh@@ (TD 0x184CCFD60).
// Bases: C_UIFlashBase @+0x00 (COL 0x4135CE8), IUIElementEventListener @+0x10
// (COL 0x4135D10), wh::framework::I_UIHudBubbles @+0x58 (COL 0x4135D38).
// vtables: primary 0x183C29818 (14; GetElementName [11] 0x181A8E8F0 = "hud"),
// listener 0x183C29780 (8), bubbles 0x183C297C8 (9). ctor sub_181843CA0; rttr
// as_std_shared_ptr creator sub_181843C14 (alloc 0x88).
//
// NPC speech-bubble HUD. Init [1] sub_180C3CB74: base Init, PUBLISHES the
// I_UIHudBubbles subobject into the framework context (*(ctx+0xE0)+0x180 = this+0x58,
// cleared by Deinit [2] sub_182B8CA80) and caches the wh_ui_BubblesEnabled ICVar.
// OnModuleMessage [3] sub_18085DD40: msg 36 -> I_UIHudBubbles vf[1] (bubble refresh).
// Bubble ids are recycled through the two std::lists; the flash side is driven via
// per-bubble IFlashVariableObject handles (lookup sub_1814BEAA4) and the
// "SetBubbleText" call.

struct ICVar;

namespace wh::guimodule {

class C_UIHudBubbles : public C_UIFlashBase, public wh::framework::I_UIHudBubbles {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIHudBubbles;
    // C_UIBase overrides.
    void Init(C_GUIModule* pModule) override;   // [1] 0x180C3CB74
    void Deinit() override;                     // [2] 0x182B8CA80
    void OnModuleMessage(void* msg) override;   // [3] 0x18085DD40  msg 36 -> UpdateBubbles
    // I_UIHudBubbles overrides (vtable 0x183C297C8).
    void UpdateBubbles() override;                                               // [B1] 0x1805E0520
    uint32_t AllocBubble() override;                                             // [B2] 0x1814BE770
    void ReleaseBubble(uint32_t bubbleId) override;                              // [B3] 0x1814BE954
    void SetBubbleText(int bubbleId, const CryStringT<char>& text,
                       const CryStringT<char>& speaker, bool bLocalize) override;  // [B4] 0x1808C4710
    void SetBubbleAnchor(const void* pPos, float pct) override;                  // [B5] 0x1814BEA44

    std::list<uint32_t> m_recycledBubbles;   // +0x60  hidden/free bubble ids (AllocBubble pops @0x1814be797, ReleaseBubble pushes @0x1814be9bf)
    std::list<uint32_t> m_bubbles;           // +0x70  all created bubble ids; size+1 = next new id (@0x1814be850)
    ICVar* m_pBubblesEnabledCVar;            // +0x80  wh_ui_BubblesEnabled (Init @0x180c3cbbc)
};
static_assert(sizeof(C_UIHudBubbles) == 0x88, "C_UIHudBubbles must be 0x88");
static_assert(offsetof(C_UIHudBubbles, m_recycledBubbles) == 0x60, "bubble lists at 0x60");

}  // namespace wh::guimodule
