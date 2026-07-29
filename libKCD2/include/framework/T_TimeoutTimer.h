#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::T_TimeoutTimer<TimePolicy> -- small polymorphic timeout timer
// embedded by value in game objects (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x28
// (instantiation proven from the C_PerceptibleVolume embed +0xE8..+0x110, ctor
// sub_180D45C38 @0x180d45cfa..0x180d45d36).
// -----------------------------------------------
// Known instantiation: T_TimeoutTimer<wh::rpgmodule::T_GameWorldTimePolicy>
// (game-world-time policy; drives the perceptible-volume worldTime/timer expiry,
// cf. ItemOwnerFadeConspicuousnessToHours).  Vtable installed @0x180d45cfa;
// slot roles beyond the dtor UNRESOLVED [U].  m_timerId init constant 0x11
// (same ctor-init constant as the KCD1 C_FastTravel timer -- pins dump shifts).

namespace wh::framework {

template <class TimePolicy>
class T_TimeoutTimer {
public:
    virtual ~T_TimeoutTimer();   // [0] slot roles [U]

    uint32_t m_timerId;          // +0x08  init 0x11 (interval-or-id, exact meaning [U])
    uint32_t _pad0C;             // +0x0C
    uint64_t _unk10;             // +0x10  init 0 [U]
    uint64_t _unk18;             // +0x18  init 0 [U]
    uint64_t _unk20;             // +0x20  init 0 [U]
};

}  // namespace wh::framework
