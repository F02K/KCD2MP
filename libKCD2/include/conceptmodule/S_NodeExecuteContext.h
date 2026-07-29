#pragma once

// -----------------------------------------------
// wh::conceptmodule::S_NodeExecuteContext -- per-trigger execution context
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x10, vtable 0x183A42888 (1 slot).
// -----------------------------------------------
// Passed by const& to C_Node::Execute (slot 13) and OnExecute (slot 33).  Lives at
// offset 0 of the 0x58-byte trigger-subscription record built by the std::bind
// emplace sub_18061D4C8: { S_NodeExecuteContext ctx; std::function<void(
// S_NodeExecuteContext const&)> fn /*+0x10, 0x40B*/; int32 order /*+0x50*/ }.
// The fire pipeline sub_18061D688 gathers subscriptions (sub_18061DD3C), sorts by
// `order` (comparator 0x18061DC68; sourced from the target node's GetExecutionOrder,
// C_Node slot 10 -- so auto-triggerables/effects fire last) and invokes each fn with
// &element, i.e. the record's own leading context.  Polymorphic only for safe
// deletion; no derived context exists (all 4 vtable xrefs construct the base).

namespace wh::conceptmodule {

class I_Port;

struct S_NodeExecuteContext {
    virtual ~S_NodeExecuteContext();   // [0]  0x182650524 -- releases m_source
    _smart_ptr<I_Port> m_source;       // +0x08  the port whose trigger fired
};
static_assert(sizeof(S_NodeExecuteContext) == 0x10, "S_NodeExecuteContext must be 0x10 (subscription-record prefix)");

}  // namespace wh::conceptmodule
