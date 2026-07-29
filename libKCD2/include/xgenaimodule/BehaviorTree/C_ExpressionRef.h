#pragma once

// -----------------------------------------------
// wh::xgenaimodule::BehaviorTree::Expressions::C_ExpressionRef -- reference to a
// behavior-tree expression (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10: vptr +
// one payload pointer.  Modeled from its embedded use in C_Variable @+0x30
// (ctor sub_1805B810C stores &C_ExpressionRef::vftable @+0x30 and 0 @+0x38).
// -----------------------------------------------
// RTTI mangled .?AVC_ExpressionRef@Expressions@BehaviorTree@xgenaimodule@wh@@.
// Own vtable slot roles NOT enumerated (out of G4B scope) -- only the dtor slot is
// modeled [U slot count/roles].

namespace wh::xgenaimodule::BehaviorTree::Expressions {

class I_Expression;   // RTTI .?AVI_Expression@Expressions@BehaviorTree@xgenaimodule@wh@@

class C_ExpressionRef {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ExpressionRef;
    virtual ~C_ExpressionRef();   // [0] [U -- vtable not enumerated]

    I_Expression* m_expression;   // +0x08  referenced BT expression (0 at ctor); C_ExpressionRef vtable slots forward to m_expression->vf[n], else log "Expression not initialized."
};
static_assert(sizeof(C_ExpressionRef) == 0x10, "C_ExpressionRef must be 0x10 (vptr + ptr)");

}  // namespace wh::xgenaimodule::BehaviorTree::Expressions
