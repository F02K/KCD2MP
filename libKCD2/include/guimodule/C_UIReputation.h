#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIReputation -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI .?AVC_UIReputation@guimodule@wh@@ (TD 0x184C92658). Base C_UIBase only;
// vtable 0x183D2D680 (10 slots, C_UIBase shape). Embedded BY VALUE in C_UIApsePlayer
// @+0x4B0 (inline ctor 0x180bc8d2d; size pinned by next embed C_UIApseBuffs @+0x4D0).
//
// Pure back-ref holder: points at the player panel's shared list/info flash widgets.
// Its presentation logic lives externally (Init body NOT decompiled this wave).

namespace wh::guimodule {

class C_UITreeList;
class C_UIExtendedInfo;

class C_UIReputation : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIReputation;
    C_UITreeList*     m_pList;   // +0x10  -> C_UIApsePlayer::m_list ("ApsePlayerList")
    C_UIExtendedInfo* m_pInfo;   // +0x18  -> C_UIApsePlayer::m_info ("ApsePlayerInfo")
};
static_assert(sizeof(C_UIReputation) == 0x20, "C_UIReputation must be 0x20");

}  // namespace wh::guimodule
