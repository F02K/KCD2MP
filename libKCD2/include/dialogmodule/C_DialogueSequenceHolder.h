#pragma once
#include "../framework/I_ContextUserData.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_DialogueSequenceHolder -- per-context sequence user data
// (KCD2 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// RTTI TD 0x184B671E0; vtable 0x183E6CF20 (1 slot: [0] sub_181A72470 = `return 1`, the
// I_ContextUserData KIND id -- NOT a dtor); constructed inline in sub_1803BC3BC.
// Attached to the EE/dialog context so the EE functions (C_FunctionThisSequenceUsed etc.)
// can find the currently-evaluating sequence. Field roles UNVERIFIED.

namespace wh::dialogmodule::data {

class C_DialogueSequenceHolder : public framework::I_ContextUserData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueSequenceHolder;
    int GetKind() override;   // [0] returns 1 (sub_181A72470)

    void* m_pContext;         // +0x08  owning context [role UNVERIFIED]
    void* m_pSequence;        // +0x10  sequence data ptr [role UNVERIFIED]
};
static_assert(sizeof(C_DialogueSequenceHolder) == 0x18, "C_DialogueSequenceHolder must be 0x18");

}  // namespace wh::dialogmodule::data
