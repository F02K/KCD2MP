#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// ===========================================================================
// wh::rpgmodule::S_ReputationChangeRequest -- the reputation-change descriptor
// (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AUS_ReputationChangeRequest@rpgmodule@wh@@ (TD 0x184D4E1F0)  vtable 0x183A52458 (3 slots).
// sizeof 0x28.  KCD2 analog of KCD1's plain 0x20 S_ReputationChangeEntry, now POLYMORPHIC and the
// cause is a NAME STRING instead of a WUID; clamping is new. Embedded in
// C_FactionReputationEffect @+0x20; also built standalone from a reputation_change row
// (ctor sub_180785330(this, row, causeName, weightMult)):
//   scaledChange = weightMult * row+0x10 (change)
//   targetBits   = row+0x14, canChangeHostility = row+0x15, changeId = row+0x00
//   clamp        = row+0x18 when != +/-FLT_MAX
//   then scaledChange *= playerSoul.DerivedStat(70 + (change < 0)) -- the player's reputation
//   gain/loss multiplier derived stats (sub_180785414).
// Copy-ctor sub_182CA8130; default sub_1807853D4. The 3-slot vtable is the RTTR trio, no
// virtual dtor (the old "invalid-WUID out-param pair" reading was the trio;
// C_GenericReputationChangeCause "overriding" [0]/[2] is that class's own trio).

namespace wh::rpgmodule {

struct S_ReputationChangeRequest {
    inline static constexpr auto RTTI = Offsets::RTTI_S_ReputationChangeRequest;
    RTTR_ENABLE()  // [0..2] whole vtable, no dtor: get_type 0x180D67824, get_derived 0x182CB13E0

    float    m_scaledChange;        // +0x08  row change * weight * player gain/loss derived stat
    uint8_t  m_targetBits;          // +0x0C  target layer bitmask (row+0x14)
    uint8_t  m_canChangeHostility;  // +0x0D  (row+0x15)
    uint8_t  _pad0E[2];             // +0x0E
    int32_t  m_changeId;            // +0x10  reputation_change row id
    uint32_t _pad14;                // +0x14
    CryStringT<char> m_causeName;   // +0x18  cause label (KCD1 carried a WUID here)
    uint8_t  m_hasClamp;            // +0x20  set when row+0x18 != +/-FLT_MAX
    uint8_t  _pad21[3];             // +0x21
    float    m_clampValue;          // +0x24  reputation clamp target (row+0x18)
};
static_assert(sizeof(S_ReputationChangeRequest) == 0x28, "S_ReputationChangeRequest must be 0x28");
static_assert(offsetof(S_ReputationChangeRequest, m_causeName) == 0x18, "cause at 0x18");

}  // namespace wh::rpgmodule
