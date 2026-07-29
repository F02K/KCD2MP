#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::I_CutsceneChangeListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface, 2 slots.
// -----------------------------------------------
// RTTI .?AVI_CutsceneChangeListener@guimodule@wh@@ (TD 0x184C9EAD8). Cutscene lifecycle
// sink: C_CutscenePlayer notifies its C_Listeners<I_CutsceneChangeListener,16>
// (@player+0x78); implemented by C_CutsceneHandler (@+0x40, vtable 0x183B1C1B0:
// [1] OnCutsceneChanged 0x182B12028 fires the node's trigger output ports per
// changeType). changeType enum values (started/stopped/skipped/...) UNVERIFIED.

namespace wh::guimodule {

class I_Cutscene;

class I_CutsceneChangeListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CutsceneChangeListener;
    virtual ~I_CutsceneChangeListener();                                       // [0]
    virtual void OnCutsceneChanged(I_Cutscene* pCutscene, int changeType) = 0; // [1] name coined; param types INFERRED
};
static_assert(sizeof(I_CutsceneChangeListener) == 8);

}  // namespace wh::guimodule
