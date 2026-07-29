#pragma once
#include <cstdint>
#include "E_CompanionEvent.h"

// -----------------------------------------------
// wh::rpgmodule::S_CompanionEventData -- payload of C_CompanionEventCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: ctor sub_182D18E88 tracked-alloc 32; _OWORD@0 + qword@0x10).
// Create-site sub_182D28FCC builds src = { a2 (event type), *(*(a1+24)+48), *functor }.

namespace wh::rpgmodule {

struct S_CompanionEventData {
    E_CompanionEvent m_eventType;   // +0x00  (RTTR reg sub_1801BD9B0; create sub_182D28FCC passes a2)
    uint8_t  _pad04[4];     // +0x04
    uint64_t m_a;           // +0x08  handle [proposed]
    uint64_t m_b;           // +0x10  handle (= *functor result) [proposed]
};
static_assert(sizeof(S_CompanionEventData) == 0x18, "S_CompanionEventData must be 0x18 (24-byte ctor copy)");

}  // namespace wh::rpgmodule
