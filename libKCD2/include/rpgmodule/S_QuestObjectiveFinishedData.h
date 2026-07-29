#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::rpgmodule::S_QuestObjectiveFinishedData -- payload of C_QuestObjectiveFinishedCause
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182CDEA48 tracked-alloc 32; qword v5[1]=*a2 + two
// CryStringT copies sub_1804FD208(v5+2,a2+1) / (v5+3,a2+2)).  Caller sub_182CFA994
// @0x182CFA9C0.  Ctor returns the raw cause; caller posts separately.  Own dtor
// 0x182CED6D0 releases the two strings.

namespace wh::rpgmodule {

struct S_QuestObjectiveFinishedData {
    uint64_t         m_quest;         // +0x00  quest handle/WUID [proposed]
    CryStringT<char> m_questId;       // +0x08  [proposed]
    CryStringT<char> m_objectiveId;   // +0x10  [proposed]
};
static_assert(sizeof(S_QuestObjectiveFinishedData) == 0x18, "S_QuestObjectiveFinishedData must be 0x18");

}  // namespace wh::rpgmodule
