#pragma once
#include "../framework/I_EEFunction.h"

// -----------------------------------------------
// wh::environmentmodule::C_FunctionIsRaining -- "IsRaining" expression function
// (KCD2 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// Own vtable 0x183C732B0; wh::framework::I_EEFunction implementation with no data. Created
// and registered by C_EnvironmentModule::Init via sub_1819B78A4 (alloc 8) into the
// expression-function manager (module+0x60 -> +0x430); not stored as a module field.

namespace wh::environmentmodule {

class C_FunctionIsRaining : public framework::I_EEFunction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FunctionIsRaining;
};
static_assert(sizeof(C_FunctionIsRaining) == 0x08, "C_FunctionIsRaining adds no data (alloc 8)");

}  // namespace wh::environmentmodule
