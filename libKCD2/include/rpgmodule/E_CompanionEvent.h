#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_CompanionEvent -- KCD2 WHGame.dll 1.5.6 (kd7u).
// -----------------------------------------------
// Companion (dog/horse) gameplay event kind carried by S_CompanionEventData::m_eventType.
// Enumerators certified from the RTTR registration sub_1801BD9B0
// (enumeration_wrapper<...E_CompanionEvent,10,1>).

namespace wh::rpgmodule {

enum class E_CompanionEvent : int32_t {
    Hit            = 0,
    Kill           = 1,
    AnimalKill     = 2,
    Feed           = 3,
    POIDiscovery   = 4,
    Play           = 5,
    ContextCommand = 6,
    Fetch          = 7,
    Distraction    = 8,
    Praise         = 9,
};

}  // namespace wh::rpgmodule
