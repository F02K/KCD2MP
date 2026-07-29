#pragma once
#include <cstdint>
#include "../framework/C_BaseModule.h"

// -----------------------------------------------
// wh::xbehaviormodule::C_XBehaviorModule -- module root (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18.
// -----------------------------------------------
// NEW module in KCD2. RTTI .?AVC_XBehaviorModule@xbehaviormodule@wh@@  inline ctor in the module
// bootstrap sub_180C9DA44 (alloc 0x18 == sizeof, registry slot [43] @+0x158); the bootstrap also
// creates the C_XBehaviorModuleSingletons holder stored at +0x10 (holder layout unresolved).

namespace wh::xbehaviormodule {

class C_XBehaviorModuleSingletons;

class C_XBehaviorModule : public wh::framework::C_BaseModule {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_XBehaviorModule;
    C_XBehaviorModuleSingletons* m_pSingletons;   // +0x10
};
static_assert(sizeof(C_XBehaviorModule) == 0x18, "C_XBehaviorModule must be 0x18");

}  // namespace wh::xbehaviormodule
