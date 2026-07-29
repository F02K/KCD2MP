#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/C_UITreeList.h"
#include "guimodule/C_UIExtendedInfo.h"
#include "guimodule/C_UIModalDialogInfo.h"
#include "guimodule/C_UIModalDialogQuestion.h"
#include "guimodule/C_UIApseSoulProperties.h"
#include "guimodule/C_UIReputation.h"
#include "guimodule/C_UIApseBuffs.h"
#include "guimodule/C_UIApseStatistics.h"

// -----------------------------------------------
// wh::guimodule::C_UIApsePlayer -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x528.
// -----------------------------------------------
// RTTI .?AVC_UIApsePlayer@guimodule@wh@@ (TD 0x184C926C8). ctor sub_180BC8C58.
// Base C_UIBase only; vtable 0x183D2D788 (10 slots): [1] Init 0x180EC80D8,
// [2] Deinit 0x182AFE5A4, [4] Update 0x180567AFC. NO own virtuals.
// Embedded BY VALUE in C_UIApse @+0xBA0 (size pinned by next embed C_UIQuestLog
// @+0x10C8). DISTINCT from spine.md's C_UIPlayer (0x98, module+0x20) -- different class.
//
// The Apse "player" tab: hosts the shared list/info flash widgets and the four
// sub-panels (soul properties / reputation / buffs / statistics), which hold back-refs
// into m_list/m_info rather than owning widgets of their own.

namespace wh::guimodule {

class C_UIApsePlayer : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApsePlayer;
    bool     m_flag10;                    // +0x10  ctor 1 [role UNVERIFIED]
    uint8_t  _pad11[7];                   // +0x11
    C_UITreeList     m_list;              // +0x18   (0x1E0) flash element "ApsePlayerList" (ctor sub_180BC8484, a3=0)
    C_UIExtendedInfo m_info;              // +0x1F8  (0xD8) flash element "ApsePlayerInfo" (ctor sub_180BC8628)
    C_UIModalDialogInfo m_modalInfo;      // +0x2D0  (0xA8) "ApseModalDialog" (ctor sub_180BC842C)
    C_UIModalDialogQuestion m_question;   // +0x378  (0xE8) "ApseModalDialog" (ctor sub_180BC83D0)
    C_UIApseSoulProperties m_soulProps;   // +0x460  (0x50) back-refs into m_list/m_info
    C_UIReputation m_reputation;          // +0x4B0  (0x20) back-refs into m_list/m_info
    C_UIApseBuffs m_buffs;                // +0x4D0  (0x30) I_UIBuffsListener sink
    C_UIApseStatistics m_statistics;      // +0x500  (0x28) back-refs into m_list/owner
};
static_assert(sizeof(C_UIApsePlayer) == 0x528, "C_UIApsePlayer must be 0x528");
static_assert(offsetof(C_UIApsePlayer, m_list) == 0x18, "tree list at 0x18");
static_assert(offsetof(C_UIApsePlayer, m_question) == 0x378, "question dialog at 0x378");
static_assert(offsetof(C_UIApsePlayer, m_statistics) == 0x500, "statistics panel at 0x500");

}  // namespace wh::guimodule
