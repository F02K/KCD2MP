#pragma once
#include <cstdint>
#include "guimodule/C_UIBase.h"

// -----------------------------------------------
// wh::guimodule::C_UIApseStatistics -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28.
// -----------------------------------------------
// RTTI .?AVC_UIApseStatistics@guimodule@wh@@ (TD 0x184C92700). Base C_UIBase only;
// vtable 0x183D2D730 (10 slots; only [0] dtor 0x182AFD7DC differs -- Init is the base
// default). Embedded BY VALUE in C_UIApsePlayer @+0x500 (inline ctor 0x180bc8d8d; size
// pinned by the player panel end 0x528).
// Back-ref holder for the statistics tab; presentation logic lives externally.

namespace wh::guimodule {

class C_UIApse;
class C_UITreeList;

class C_UIApseStatistics : public C_UIBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIApseStatistics;
    C_UITreeList* m_pList;    // +0x10  -> C_UIApsePlayer::m_list ("ApsePlayerList")
    C_UIApse*     m_pApse;    // +0x18  owner hub
    uint16_t      m_word20;   // +0x20  ctor 0 [role UNVERIFIED]
    uint8_t       m_byte22;   // +0x22  ctor 0 [role UNVERIFIED]
    uint8_t       _pad23[5];  // +0x23
};
static_assert(sizeof(C_UIApseStatistics) == 0x28, "C_UIApseStatistics must be 0x28");

}  // namespace wh::guimodule
