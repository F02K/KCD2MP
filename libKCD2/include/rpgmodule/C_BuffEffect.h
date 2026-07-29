#pragma once
#include <cstdint>
#include <vector>
#include "../conceptmodule/C_SoulEffect.h"
#include "../conceptmodule/C_TypedPortRef.h"
#include "../conceptmodule/C_TypedArrayPortRef.h"

// -----------------------------------------------
// wh::rpgmodule::C_BuffEffect -- concept-graph node applying a buff to souls
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D09D58  vtable 0x183F0C730.  sizeof 0x148 (PROVEN: rttr constructor_wrapper
// factory sub_181183938 operator new(328) + memset 0).  Chain: conceptmodule::C_SoulEffect ->
// C_Effect -> C_AutoTriggerable<C_Node> -> C_Node -> C_SharedResource (NOT the rpgmodule
// C_Effect system).  Ctor sub_1811839F0: souls array-port @+0x88, buff-id port @+0xC8,
// float 1.0 @+0x108, then an unordered_map-shaped hash @+0x110 (sentinel list node, 16-entry
// bucket vector fill sub_181E11900, mask 7, maxidx 8).  rttr-exposed ports: "souls" vector
// port and a S_BuffDefinitionId port (property_wrapper TDs).

namespace wh::rpgmodule {

class I_Soul;
struct S_BuffDefinitionId;   // not yet RE'd (port payload type only)

class C_BuffEffect : public conceptmodule::C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BuffEffect;
    conceptmodule::C_TypedArrayPortRef<std::vector<I_Soul*>> m_soulsPort;   // +0x88
    conceptmodule::C_TypedPortRef<S_BuffDefinitionId> m_buffIdPort;         // +0xC8
    // +0x108  std::unordered_map (MSVC _Hash, sizeof 0x40) -- SUBSUMES the former m_param108
    //         (which is the hash's max_load_factor 1.0f, not a gameplay float). Layout: list
    //         sentinel @+0x110, size @+0x118, bucket vector @+0x120..0x130, mask 7 @+0x138,
    //         maxidx 8 @+0x140.  Node: value @node+0x18 = polymorphic pointer (its dtor calls
    //         virtuals), key @node+0x10 = 8-byte trivially-destructible [K,V exact types UNVERIFIED].
    std::unordered_map<uint64_t, void*> m_map;   // +0x108  (name inferred)
};
static_assert(sizeof(C_BuffEffect) == 0x148, "C_BuffEffect must be 0x148 (operator new(328))");

}  // namespace wh::rpgmodule
