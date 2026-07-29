#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "../questmodule/E_QuestProgress.h"

// -----------------------------------------------
// wh::rpgmodule::S_QuestStateChangedData -- payload of C_QuestStateChangedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_180DC911C tracked-alloc 32; qword @+0, CryStringT copy
// sub_1804FD208(v5+16,a2+8), dword @+16).  Caller sub_180DC9060 @0x180DC908C.  Ctor
// returns the raw cause; caller posts separately.  Own dtor 0x181407D98 releases the
// string.

namespace wh::rpgmodule {

struct S_QuestStateChangedData {
    uint64_t         m_quest;      // +0x00  quest handle/WUID [proposed]
    CryStringT<char> m_questId;    // +0x08  [proposed]
    wh::questmodule::E_QuestProgress m_newState;   // +0x10  (RTTR reg sub_18016E4D0; value comes off the quest progress port, writer sub_180DC9060)
    uint8_t          _pad14[4];    // +0x14
};
static_assert(sizeof(S_QuestStateChangedData) == 0x18, "S_QuestStateChangedData must be 0x18");

}  // namespace wh::rpgmodule
