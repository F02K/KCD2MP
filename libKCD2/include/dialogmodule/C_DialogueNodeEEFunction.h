#pragma once
#include "../framework/I_EEFunction.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_DialogueNodeEEFunction -- dialogue EE-function base
// (KCD2 1.5.6, kd7u).  sizeof 0x08.  Abstract (no COL/own vtable).
// -----------------------------------------------
// RTTI TD 0x184B67100. Base of the 5 named dialogue expression intrinsics
// (C_FunctionPort/Enum/SequenceUsed/ThisSequenceUsed/ExtensionAccepted). Zero data
// members; every concrete node is exactly 8 bytes (factory sub_180E2E8D0 allocs 8) --
// all state is external (EE call args + the running dialog). These gate dialogue
// branching (see framework/I_EEFunction.h for the 8-slot contract).

namespace wh::dialogmodule::data {

class C_DialogueNodeEEFunction : public framework::I_EEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueNodeEEFunction;
};
static_assert(sizeof(C_DialogueNodeEEFunction) == 0x08, "C_DialogueNodeEEFunction adds no data");

}  // namespace wh::dialogmodule::data
