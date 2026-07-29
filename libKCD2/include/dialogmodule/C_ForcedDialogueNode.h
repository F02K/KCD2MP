#pragma once
#include <memory>
#include "C_FaderDialogueNode.h"
#include "S_DialogParams.h"
#include "../conceptmodule/C_TypedPortRef.h"
#include "../conceptmodule/S_Trigger.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_ForcedDialogueNode -- force-start dialogue node
// (KCD2 1.5.6, kd7u).  sizeof 0x278.
// -----------------------------------------------
// RTTI TD 0x184B6EFE0; vtable 0x183B42FD8 (45 slots); ctor sub_181259790; deleting-dtor
// [0] sub_181148FAC. The concept-graph node that forces a conversation: adds a trigger
// port and a shared_ptr<S_DialogParams> port (the runtime dialog-parameters payload fed
// to the dialog manager when the node fires).

namespace wh::dialogmodule::data {

class C_ForcedDialogueNode : public C_FaderDialogueNode {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ForcedDialogueNode;
    conceptmodule::C_TypedPortRef<conceptmodule::S_Trigger> m_port1F8;                      // +0x1F8  trigger port
    conceptmodule::C_TypedPortRef<std::shared_ptr<dialogmodule::S_DialogParams>> m_port238; // +0x238  dialog-params port
};
static_assert(sizeof(C_ForcedDialogueNode) == 0x278, "C_ForcedDialogueNode must be 0x278");

}  // namespace wh::dialogmodule::data
