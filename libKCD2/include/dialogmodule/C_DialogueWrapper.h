#pragma once
#include "../conceptmodule/C_ModuleBase.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_DialogueWrapper -- concept-graph dialogue module node
// (KCD2 1.5.6, kd7u).  sizeof 0xA0.
// -----------------------------------------------
// RTTI TD 0x184B6F158; vtable 0x183B44C70 (55 slots); ctor sub_181262C6C; deleting-dtor
// [0] sub_1806B3C18. Chain: C_ModuleBase -> C_AutoTriggerable<C_Node> -> C_Node ->
// C_SharedResource. Adds NO own data: the ctor's writes at +0x40/+0x58/+0x70 (three
// 0x18-wide sub-objects) and the +0x88/+0x90 COW arrays land exactly on
// conceptmodule::C_ModuleBase's m_unk40[9]/m_str88/m_str90 region -- they are the
// (inlined) base-member initializations observed from the derived ctor, not new fields
// (sizeof == sizeof(C_ModuleBase) == 0xA0 confirms).

namespace wh::dialogmodule::data {

class C_DialogueWrapper : public conceptmodule::C_ModuleBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueWrapper;
};
static_assert(sizeof(C_DialogueWrapper) == 0xA0, "C_DialogueWrapper adds no data over C_ModuleBase");

}  // namespace wh::dialogmodule::data
