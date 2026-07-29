#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::S_ResourceRef -- generation-checked weak handle to a
// C_SharedResource (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x08.
// -----------------------------------------------
// Weak reference into the shared-resource registry (S_GameContext+0x120 -> +0x18 ->
// +0x10): resolve = registry Resolve(id) + require generation match (ABA guard).
// Builder sub_18069885C(dst, &smart_ptr): id = res->m_resourceId, generation =
// registry GetGeneration().  Resolvers sub_180698CF8 / sub_18069912C -> _smart_ptr
// (null on stale/miss).  Used for I_Port::m_ownerRef, port edge links
// (C_InputDataPort::m_incoming, C_OutputTriggerPort::m_outgoing elements),
// C_AutoTriggerPort::m_wrapped and the deserializer's pending-edge records.

namespace wh::conceptmodule {

struct S_ResourceRef {
    uint32_t m_id         = ~0u;   // 0xFFFFFFFF = unset (ctors write -1)
    uint8_t  m_generation = 0;     // compared against registry vcall+0x20
};
static_assert(sizeof(S_ResourceRef) == 0x08, "S_ResourceRef is {id, generation}");

}  // namespace wh::conceptmodule
