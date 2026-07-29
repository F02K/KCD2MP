#pragma once
#include "C_DialogueNodeEEFunction.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_FunctionExtensionAccepted -- "ExtensionAccepted" dialogue
// expression intrinsic (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD 0x184B67020; vtable 0x183E6CED8 (8 slots). GetName = "ExtensionAccepted".
// Evaluate sub_181E71600: no-arg; queries the active I_DialogueExtension object
// (instance+368 vf[7] GetResult) -> bool. This is how the haggle/barber minigame result
// gates the following dialogue branch (see C_DialogueHaggleExtension +0x2C).

namespace wh::dialogmodule::data {

class C_FunctionExtensionAccepted : public C_DialogueNodeEEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionExtensionAccepted;
};
static_assert(sizeof(C_FunctionExtensionAccepted) == 0x08, "C_FunctionExtensionAccepted adds no data (alloc 8)");

}  // namespace wh::dialogmodule::data
