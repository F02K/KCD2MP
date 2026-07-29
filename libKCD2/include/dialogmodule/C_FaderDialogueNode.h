#pragma once
#include "C_DialogueNode.h"
#include "../conceptmodule/C_TypedPortRef.h"
#include "../conceptmodule/S_Trigger.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_FaderDialogueNode -- dialogue node with fader trigger ports
// (KCD2 1.5.6, kd7u).  sizeof 0x1F8.
// -----------------------------------------------
// RTTI TD 0x184B6F198; vtable 0x183BD35C8 (45 slots); ctor sub_1806B33D4 (per-port init
// sub_1806B2890); deleting-dtor [0] sub_181149020. Adds 4 embedded typed port refs
// (0x40 stride each): 3x S_Trigger + 1x bool. Which port carries the bool payload is
// UNVERIFIED (modeled as the last); port roles (fade-in/fade-out/done + enable?)
// UNVERIFIED.

namespace wh::dialogmodule::data {

class C_FaderDialogueNode : public C_DialogueNode {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FaderDialogueNode;
    conceptmodule::C_TypedPortRef<conceptmodule::S_Trigger> m_portF8;    // +0x0F8  trigger port
    conceptmodule::C_TypedPortRef<conceptmodule::S_Trigger> m_port138;   // +0x138  trigger port
    conceptmodule::C_TypedPortRef<conceptmodule::S_Trigger> m_port178;   // +0x178  trigger port
    conceptmodule::C_TypedPortRef<bool> m_port1B8;                       // +0x1B8  bool port [payload slot UNVERIFIED]
};
static_assert(sizeof(C_FaderDialogueNode) == 0x1F8, "C_FaderDialogueNode must be 0x1F8");

}  // namespace wh::dialogmodule::data
