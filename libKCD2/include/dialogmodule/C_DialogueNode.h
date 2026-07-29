#pragma once
#include <cstdint>
#include "C_Dialogue.h"
#include "../conceptmodule/C_Node.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_DialogueNode -- concept-graph dialogue node
// (KCD2 1.5.6, kd7u).  sizeof 0xF8.
// -----------------------------------------------
// RTTI TD 0x184B67BF0; vtable 0x183A47248 (45 slots -- the conceptmodule::C_Node family
// vtable; overrides [27] 0x1814126D8 / [28] 0x1806AF400 / [31] 0x18282C7C8, adds NO new
// slots); ctor sub_1806B36D0; deleting-dtor [0] sub_181149100. Chain:
// C_DialogueNode -> C_Node -> C_SharedResource (base extent 0x40). Embeds its
// C_Dialogue by value at +0x40 (ctor/dtor delegation confirmed). Tail COW arrays
// [roles UNVERIFIED].

namespace wh::dialogmodule::data {

class C_DialogueNode : public conceptmodule::C_Node {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueNode;
    C_Dialogue m_dialogue;      // +0x40  embedded by value (0xA0)
    CryStringT<char> m_arrE0;   // +0xE0  COW array [role UNVERIFIED]
    CryStringT<char> m_arrE8;   // +0xE8  COW array [role UNVERIFIED]
    int32_t  m_dF0;             // +0xF0  [role UNVERIFIED]
    uint8_t  _padF4[4];         // +0xF4
};
static_assert(sizeof(C_DialogueNode) == 0xF8, "C_DialogueNode must be 0xF8");

}  // namespace wh::dialogmodule::data
