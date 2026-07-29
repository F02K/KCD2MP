#pragma once
#include <cstdint>
#include "C_Node.h"
#include "I_Port.h"
#include "rttr/variant.h"

// -----------------------------------------------
// wh::conceptmodule::C_EventNode<TBase> -- event-scoped-data node mixin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  Adds 0x28 over TBase, +1 vtable slot.
// -----------------------------------------------
// Instantiations (RTTI-proven): <C_Node> 0x68 (vt 0x183E29F88; base of C_Select,
// C_GameStart), <C_TemplatedNode> 0x70 (vt 0x183E2B518; base of C_ForEach),
// <C_Function>/<C_MemberFunction> 0x90 (vts 0x183E2ECB8/0x183E2EF78; bases of the
// Event*Function nodes), <C_Effect> 0xB0, <C_AutoTriggerable<C_TemplatedNode>>
// (base of C_StateWatch).  Payload: a {portName -> rttr::variant} hash map + a
// re-entrancy guard.  EmitEvent installs the map, fires the trigger port (port
// vslot 15) SYNCHRONOUSLY, then clears the map -- so downstream nodes can read the
// event's data pins only while the trigger is propagating: the mixin's GetPortValue
// override serves reads from the map and errors with "attempt to read data from
// event port '%s' outside of event trigger execution" (guard == 0) or "data for
// port '%s' was not found in event data" (miss).
// EmitEvent impls (ICF-shared per map offset): <C_Node> 0x181939998,
// <C_Function>/<C_MemberFunction> 0x180618A5C, <C_TemplatedNode> 0x180619E14
// (data dossier "FireEvent"), <C_Effect> 0x18061A490.

namespace wh::conceptmodule {

template <typename TBase>
class C_EventNode : public TBase {
public:
    rttr::variant GetPortValue(_smart_ptr<I_Port> const& port) override;  // [12] event-data map lookup (<C_Node>: 0x18127F9F4)
    virtual void EmitEvent(I_Port* port, void* eventData);  // [+1 slot] install map -> port->Trigger() -> clear map [eventData = the variant map, container type U]

    void*    m_eventDataHead;      // +0x00  hash-map sentinel node (16B alloc, self-linked)
    void*    m_eventDataBuckets;   // +0x08  [container internals LIKELY -- custom hash map]
    uint64_t m_eventDataMask;      // +0x10
    uint64_t m_eventDataSize;      // +0x18
    uint64_t m_activeEventGuard;   // +0x20  0 = outside event-trigger execution
};
static_assert(sizeof(C_EventNode<C_Node>) == 0x68, "C_EventNode adds 0x28 over TBase");

}  // namespace wh::conceptmodule
