#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"
#include "guimodule/I_UIBuffsListener.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseBuffs -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// RTTI .?AVC_UIApseBuffs@guimodule@wh@@ (TD 0x184C92690).
// Bases: C_UIBase @+0x00 + I_UIBuffsListener @+0x10. vtables: primary 0x183D18AC8
// (10 slots; only [0] dtor 0x182AFD58C differs -- Init/Deinit/Update are base defaults),
// listener 0x183D18AA0 (4: 0x182AFDB4C / 0x182B00A1C / 0x182B01FC8 / 0x182B01900 --
// the real buff add/remove/update logic lives HERE, not in the C_UIBase slots).
// Embedded BY VALUE in C_UIApsePlayer @+0x4D0 (inline ctor 0x180bc8d50; size pinned by
// next embed C_UIApseStatistics @+0x500).

namespace wh::guimodule {

class C_UITreeList;
class C_UIExtendedInfo;

class C_UIApseBuffs : public C_UIBase, public I_UIBuffsListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseBuffs;
    // I_UIBuffsListener overrides (impl vtable 0x183D18AA0; bodies not traced this wave).
    void OnBuffAdded(void* a2, void* pBuff) override;                      // [0] 0x182AFDB4C
    void OnBuffRemoved(uint64_t buffId) override;                          // [1] 0x182B00A1C
    void OnBuffUpdated(uint64_t buffId, float progress, int a4) override;  // [2] 0x182B01FC8
    void OnBuffIdSwapped(uint64_t newId, uint64_t oldId) override;         // [3] 0x182B01900

    C_UITreeList*     m_pList;   // +0x18  -> C_UIApsePlayer::m_list ("ApsePlayerList")
    C_UIExtendedInfo* m_pInfo;   // +0x20  -> C_UIApsePlayer::m_info ("ApsePlayerInfo")
    uint64_t          m_unk28;   // +0x28  ctor 0 [role UNVERIFIED]
};
static_assert(sizeof(C_UIApseBuffs) == 0x30, "C_UIApseBuffs must be 0x30");
static_assert(offsetof(C_UIApseBuffs, m_pList) == 0x18, "list back-ref at 0x18");

}  // namespace wh::guimodule
