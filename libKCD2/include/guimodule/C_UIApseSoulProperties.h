#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseSoulProperties -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x50.
// -----------------------------------------------
// RTTI .?AVC_UIApseSoulProperties@guimodule@wh@@ (TD 0x184C92618). Base C_UIBase only;
// vtable 0x183D18040 (10 slots; ONLY [0] dtor 0x182AFD7A8 differs from C_UIBase --
// Init/Deinit/Update are all base defaults). Embedded BY VALUE in C_UIApsePlayer @+0x460
// (inline ctor 0x180bc8ce7; size pinned by next embed C_UIReputation @+0x4B0).
//
// Back-ref holder for the soul-properties tab: the presentation logic lives in external
// model callbacks, not here.

namespace wh::guimodule {

class C_UIApse;
class C_UITreeList;
class C_UIExtendedInfo;

class C_UIApseSoulProperties : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseSoulProperties;
    std::vector<void*> m_entries;  // +0x10  {first,last,end}; elem -> heap 16B {owner,observable} record (ctor sub_1808EE69C, dtor sub_1816A2B80)
    C_UITreeList*     m_pList;   // +0x28  -> C_UIApsePlayer::m_list ("ApsePlayerList")
    C_UIExtendedInfo* m_pInfo;   // +0x30  -> C_UIApsePlayer::m_info ("ApsePlayerInfo")
    C_UIModalDialogInfo*     m_pModalInfo;     // +0x38  -> C_UIApsePlayer+0x2D0 ("ApseModalDialog", ctor sub_180BC842C)
    C_UIModalDialogQuestion* m_pModalQuestion; // +0x40  -> C_UIApsePlayer+0x378 (ctor sub_180BC83D0)
    C_UIApse*         m_pApse;   // +0x48  owner hub
};
static_assert(sizeof(C_UIApseSoulProperties) == 0x50, "C_UIApseSoulProperties must be 0x50");
static_assert(offsetof(C_UIApseSoulProperties, m_pList) == 0x28, "list back-ref at 0x28");

}  // namespace wh::guimodule
