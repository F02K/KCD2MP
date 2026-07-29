#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::entitymodule::C_BoneSlotSystem -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x1E0.
// -----------------------------------------------
// Embedded at C_Actor+0x7A0 (boundary-proven: next ctor at +0x980). ctor sub_1809CEA50(this, actor)
// + Reset sub_1809CEB08 + InitDefaults sub_1809CFEA0. Pure POD aggregate -- no vptr, no heap
// members. The prior "10-bucket map" reading was wrong: sub_18050B734 is a generic
// array-construction loop, and +0x1C holds a flat S_BoneSlot[10].
// Semantic names of the state/index arrays are placeholders (no accessor located).

namespace wh::entitymodule {

class C_Actor;

// One bone slot (0x24; built by sub_18101E4AC; also used by SLuaCache_ActorGameParams). The 0x24
// stride forces 4-byte struct alignment, so the 8-byte id at +0x00 is stored as two dwords (a
// natural uint64_t would pad the element to 0x28).
struct S_BoneSlot {
    uint32_t m_idLo;        // +0x00  8-byte id/handle written as one qword-zero (WUID candidate; unverified)
    uint32_t m_idHi;        // +0x04
    bool     m_bOccupied;   // +0x08  init 0
    uint8_t  _pad09[3];     // +0x09
    Vec3     m_vA;          // +0x0C  init (0.5, 0.5, 0.5)
    Vec3     m_vB;          // +0x18  init (0, 0, 0)
};
static_assert(sizeof(S_BoneSlot) == 0x24, "S_BoneSlot must be 0x24");

class C_BoneSlotSystem {
public:
    C_Actor*   m_pOwner;         // +0x000  non-owning back-ptr
    int32_t    m_state[5];       // +0x008  init -1 (grouping into [5] inferred from the fill pattern)
    S_BoneSlot m_slots[10];      // +0x01C  (0x168)
    bool       m_bEnabled;       // +0x184  init 1
    uint8_t    _pad185[3];       // +0x185
    int32_t    m_indicesA[4];    // +0x188  init -1 (semantics unresolved)
    int32_t    m_indicesB[8];    // +0x198  init -1
    int32_t    m_defaultsA[4];   // +0x1B8  init 4  (could equally be one int32[8] with m_defaultsB)
    int32_t    m_defaultsB[4];   // +0x1C8  init 4
    int32_t    m_activeSlot;     // +0x1D8  init -1
    uint8_t    _pad1DC[4];       // +0x1DC
};
static_assert(sizeof(C_BoneSlotSystem) == 0x1E0, "C_BoneSlotSystem must be 0x1E0");

}  // namespace wh::entitymodule
