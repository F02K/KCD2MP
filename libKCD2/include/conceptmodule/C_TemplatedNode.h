#pragma once
#include <cstdint>
#include "C_Node.h"
#include "E_TemplateTypename.h"
#include "rttr/type.h"

// -----------------------------------------------
// wh::conceptmodule::C_TemplatedNode -- generic (TypeT) node base
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x48, vtable 0x183E33AD0 (42 slots).
// -----------------------------------------------
// Generics are TYPE ERASURE, not C++ templates: one class per generic tag, holding a
// single rttr::type set by the reflected string property "TypeT" (rttr type
// "wh::conceptmodule::TemplatedNode", reg sub_1800968F0; getter 0x1826B9DD4 returns
// the type's registered name).  ElementAt<int> and ElementAt<Soul*> are the same
// object with a different m_typeT.  Ctor 0x1806B2340 (m_typeT = invalid via
// sub_180C0CDA4).  Generic pins are declared with the untyped C_PortRef;
// concretely-typed pins use C_TypedPortRef<T>.  Base of every TypeT= node family
// (MakeArray/ElementAt*/ForEach/Trace/Assert/StateBase/FunctionBase/Switch,
// C_AutoTriggerable<C_TemplatedNode> -> questmodule::C_Objective).

namespace wh::conceptmodule {

class C_TemplatedNode : public C_Node {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TemplatedNode;
    RTTR_ENABLE(C_Node)                    // [5..7] trio overrides
    virtual void OnTypeTSet(bool valid);   // [41] base nullsub; C_StateVariable 0x1806A75C0 builds the default value for the new type [param LIKELY]

    void SetTypeT(std::string name);       // 0x1806A6FA4 rttr property setter: get_by_name -> m_typeT -> OnTypeTSet(true); bad name -> "unrecognized rttr template TypeT:'%s'"
    rttr::type GetTemplateType(E_TemplateTypename kind);  // 0x1806A8E10: TypeT -> m_typeT; TypeT_0/1 -> template arg 0/1 of m_typeT

    rttr::type m_typeT;   // +0x40  rttr "TypeT"; invalid until the XML attribute is applied
};
static_assert(sizeof(C_TemplatedNode) == 0x48, "C_TemplatedNode must be 0x48 (family ports start at +0x48)");

}  // namespace wh::conceptmodule
