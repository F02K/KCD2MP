#pragma once
#include "C_Node.h"
#include "C_TypedPortRef.h"
#include "S_Trigger.h"

// -----------------------------------------------
// wh::conceptmodule::C_If -- the branch node (KCD2 WHGame.dll Steam 1.5.6, e4cp).
// sizeof 0x140, vtable 0x183A458F0 (41 slots).  4,757 corpus uses.
// -----------------------------------------------
// Direct C_Node child (no TypeT).  Ctor 0x1806B1A08, creator 0x1806B09D8 (alloc
// 0x140); rttr type "wh::conceptmodule::If" (reg sub_1816192AC -- pin offsets baked
// there: Exec 0x40, Condition 0x80, True 0xC0, False 0x100).  OnExecute [33]
// 0x18169333C is the whole semantics: read Condition (variant -> bool), fire True
// if set else False (port vslot 15).  Stateless.

namespace wh::conceptmodule {

class C_If : public C_Node {
public:
    RTTR_ENABLE(C_Node)                     // [5..7] trio overrides
    void EnumerateNodeVariants() override;  // [27] 0x18268E70C emits {"If", kind 2, "wh::conceptmodule"}
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x18169333C: Condition ? True : False

    C_TypedPortRef<S_Trigger> m_exec;       // +0x40  "Exec"      In trigger
    C_TypedPortRef<bool>      m_condition;  // +0x80  "Condition" In data
    C_TypedPortRef<S_Trigger> m_true;       // +0xC0  "True"      Out trigger
    C_TypedPortRef<S_Trigger> m_false;      // +0x100 "False"     Out trigger
};
static_assert(sizeof(C_If) == 0x140, "C_If must be 0x140");

}  // namespace wh::conceptmodule
