#pragma once
#include "I_XGenAIModuleSingletons.h"

// -----------------------------------------------
// wh::xgenaimodule::C_XGenAIModuleSingletons : I_XGenAIModuleSingletons -- the concrete
// AI singleton hub (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vptr only, stateless).
// -----------------------------------------------
// RTTI TD rva 0x4FF35C8; vtable 0x183a54af0 (67 slots: [0] dtor sub_18342FC1C +
// 66 getters -- slot map documented on the interface). ctor sub_1807D3078 writes ONLY
// the vtable. Built by C_XGenAIModule::Init (alloc 8) and stored at module +0xA8 --
// state lives in the per-slot module globals, not in the hub object.
// Overrides intentionally NOT redeclared (class stays abstract in this mirror; it is
// only ever reached by pointer).

namespace wh::xgenaimodule {

class C_XGenAIModuleSingletons : public I_XGenAIModuleSingletons {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_XGenAIModuleSingletons;
};
static_assert(sizeof(C_XGenAIModuleSingletons) == 0x08, "C_XGenAIModuleSingletons is vptr-only");

}  // namespace wh::xgenaimodule
