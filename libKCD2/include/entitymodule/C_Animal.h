#pragma once
#include <cstdint>
#include "C_Actor.h"

// -----------------------------------------------
// wh::entitymodule::C_Animal : C_Actor  (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x9E8.
// -----------------------------------------------
// RTTI .?AVC_Animal@entitymodule@wh@@  primary vtable 0x183A3A7C0 (280 slots)
// ctor 0x1804CDC0C  dtor sub_180BDFB58.  Own members occupy +0x9C0..+0x9E8 (0x28); base is C_Actor.
// Same 0x28-byte own layout as KCD1 (KCD1 own range 0x930..0x958), shifted +0x90 as C_Actor grew.

struct IPhysicalEntity;   // CryEngine

namespace wh::entitymodule {

class C_Animal : public C_Actor {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Animal;
    int32_t  m_animalId;                               // +0x9C0  init -1 (character/skeleton system id)
    int32_t  m_unk9C4;                                 // +0x9C4  (ctor 0)
    int32_t  m_unk9C8;                                 // +0x9C8  (ctor 0)
    int32_t  m_unk9CC;                                 // +0x9CC  (ctor 0)
    uint32_t m_animalFlags;                            // +0x9D0  init 0x00A00000 (bit meaning undecoded)
    int32_t  m_unk9D4;                                 // +0x9D4  (ctor 0)
    IPhysicalEntity* m_pNeckPhysics;                   // +0x9D8  procedural neck-mass capsule (builder sub_1804CB1E4)  VERIFIED
    int32_t  m_neckPhysAddedParts;                     // +0x9E0  gates neck-physics part re-add (ctor 0)
    bool     m_unk9E4;                                 // +0x9E4  cached phys-param toggle: writer sub_1804CB17C stores 0/1 here and applies it to the actor phys-entity via SetParams (vfunc+32)
    uint8_t  _pad9E5[3];                               // +0x9E5
};
static_assert(sizeof(C_Animal) == 0x9E8, "C_Animal must be 0x9E8");

}  // namespace wh::entitymodule
