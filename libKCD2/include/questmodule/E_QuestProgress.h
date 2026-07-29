#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::questmodule::E_QuestProgress -- quest progress state (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// TD .?AW4E_QuestProgress@questmodule@wh@@. Payload type of the C_Quest +0xB0 progress
// port (C_TypedPortRef<E_QuestProgress>) -- the quest-state cheat lever. Also the domain
// of S_QuestStateChangedData::m_newState. Enumerators CERTIFIED from the RTTR
// enumeration registration sub_18016E4D0 (enumeration_wrapper<...E_QuestProgress,4,0>).

namespace wh::questmodule {

enum class E_QuestProgress : int32_t {
    None   = 0,
    Active = 1,
    Done   = 2,
    Failed = 3,
};

}  // namespace wh::questmodule
