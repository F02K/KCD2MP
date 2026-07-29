#pragma once
#include <cstdint>
#include "../conceptmodule/C_Effect.h"
#include "../conceptmodule/C_TypedPortRef.h"
#include "S_FactionDefinitionID.h"

// -----------------------------------------------
// wh::rpgmodule::C_FactionVisibility : conceptmodule::C_Effect -- concept-graph
// effect that makes a target faction "known/visible" while active (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x108 (ctor sub_182BFC6C0: C_Effect base
// 0x88, faction port @+0x88, bool port @+0xC8, both C_TypedPortRef stride 0x40).
// -----------------------------------------------
// RTTI TD rva 0x4D09E30; COL 0x1843A94C8; vtable 0x4729650 (46 slots; overrides
// C_Effect at {0,5,7,27,43,44}, adds NO new virtuals):
//  [0]  sub_182BFCF84 dtor
//  [5]  sub_181A6D1D8 reflect/init helper [U]
//  [7]  sub_182C0522C copy/clone [U]
//  [27] sub_182BFE9FC GetReflectedType -> "FactionVisibility" ns "wh::rpgmodule"
//  [43] sub_182BFEF4C Activate: faction = resolve(m_factionPort) via manager
//       (sub_180471F74 -> vf+96); faction->m_visibilityRefCount(+0x198) +=
//       (m_signPort ? +1 : -1); logs "Faction '%s' doesn't exists" on miss
//  [44] sub_182BFF248 Deactivate: inverse of [43]
// KEY: C_Faction+0x198 is the faction-VISIBILITY ref-counter (backs the RTTR
// "Visibility" getter/setter sub_182CAE368/sub_182CAE838) -- resolved the
// C_Faction.h "m_runtime198 purpose unknown" field.
// NOTE: embeds 3 ports total (inherited enabled<bool>@+0x40 + the two below);
// the earlier "2x faction + 2x bool" reading counted RTTR property
// registrations, not embedded ports -- the ctor is authoritative.

namespace wh::rpgmodule {

class C_FactionVisibility : public wh::conceptmodule::C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionVisibility;

    wh::conceptmodule::C_TypedPortRef<S_FactionDefinitionID> m_factionPort;  // +0x88  target faction (ctor @0x182bfc6f2)
    wh::conceptmodule::C_TypedPortRef<bool>                  m_signPort;     // +0xC8  increment (true) vs decrement (false) (ctor @0x182bfc70b)
};
static_assert(sizeof(C_FactionVisibility) == 0x108, "C_FactionVisibility must be 0x108 (ctor extent)");

}  // namespace wh::rpgmodule
