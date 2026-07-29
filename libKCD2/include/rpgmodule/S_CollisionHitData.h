#pragma once
#include <cstdint>
#include "../framework/WUID.h"
#include "../CryEngine/CryCommon/Cry_Math.h"

// -----------------------------------------------
// wh::rpgmodule::S_CollisionHitData -- payload of C_CollisionHitCause (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x68 (PROVEN: ctor sub_1816C7BB4 tracked-alloc 112 = 8+104, bulk copy @0x1816C7C17..C48).
// Create-site sub_1816C79F8 treats the source as two symmetric 32-byte "hit participant" records
// (A @+0x00, B @+0x20) and raises two events with A/B swapped -- both PROVEN. Field breakdown
// below UNVERIFIED this session (applied from an external proposal without independent
// re-derivation); NOTE +0x10 conflicts with an earlier pass's qword-id finding (create-site
// reads a3+16) vs. this proposal's Vec3 velocity claim -- left as proposed, unresolved.

namespace wh::rpgmodule {

struct S_CollisionHitData {
    wh::framework::WUID m_participantAWuid; // +0x00
    uint32_t m_participantAEntityId;        // +0x08
    uint32_t _pad0C;                        // +0x0C
    Vec3 m_participantAVelocity;            // +0x10
    uint32_t _pad1C;                        // +0x1C

    wh::framework::WUID m_participantBWuid; // +0x20
    uint32_t m_participantBEntityId;        // +0x28
    uint32_t _pad2C;                        // +0x2C
    Vec3 m_participantBVelocity;            // +0x30
    uint32_t _pad3C;                        // +0x3C

    uint8_t m_unk40[0x28];                  // +0x40
};
static_assert(sizeof(S_CollisionHitData) == 0x68, "S_CollisionHitData must be 0x68 (alloc 112 minus vptr)");

}  // namespace wh::rpgmodule
