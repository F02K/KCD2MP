#pragma once
#include <cstdint>
#include "C_Animal.h"
#include "S_MountAnimState.h"

// -----------------------------------------------
// wh::entitymodule::C_Horse : C_Animal  (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xA60.
// -----------------------------------------------
// RTTI .?AVC_Horse@entitymodule@wh@@  primary vtable 0x183A6E508 (280 slots)
// factory 0x180A70924  Init sub_180BE0A18 (vtable slot 7)  dtor sub_180BDFA70.
// Own members occupy +0x9E8..+0xA60 (0x78); base is C_Animal.
//
// [FUNDAMENTAL vs KCD1] KCD1 C_Horse inlined all the bridle/rope/rein/rider/anim state up to 0xE38;
// KCD2 is a thin shell -- that state moved into a heap-allocated 0x4C8 controller (m_pHorseData @+0x9E8).
// Only the controller pointer and a POD mount-anim block remain directly on C_Horse.

namespace wh::entitymodule {

// Heap horse-state controller (0x4C8, Init sub_180BE0A18/ctor sub_180BE0B28). Holds the ex-KCD1 inline
// data (anim graph, gait/stamina caches, bridle/rope params). RTTI type name tentative; RE separately.
class S_HorseData;

class C_Horse : public C_Animal {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Horse;
    S_HorseData* m_pHorseData;                         // +0x9E8  (0x4C8 heap controller, OWNED: dtor frees via sub_181AB5160(.,0x4C8))  VERIFIED offset/size
    // POD mount-anim state-event record (round-3 deep map, verify-confirmed byte boundaries).
    // Also constructed standalone on the stack (sub_181E7D6F0) for the CryMannequin state-event
    // pipeline -- see S_MountAnimState.h for the honest inference flags on field semantics.
    S_MountAnimState m_mountAnimState;                 // +0x9F0  (0x70, ctor sub_180A709E8)
};
static_assert(sizeof(C_Horse) == 0xA60, "C_Horse must be 0xA60");

}  // namespace wh::entitymodule
