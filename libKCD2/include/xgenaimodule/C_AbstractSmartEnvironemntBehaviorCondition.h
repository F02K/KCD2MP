#pragma once
#include "I_SmartEnvironemntBehaviorCondition.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AbstractSmartEnvironemntBehaviorCondition :
// I_SmartEnvironemntBehaviorCondition -- shared base of the SE behavior
// conditions (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (vptr +
// framework::WUID @+0x8).  Base ctors sub_180780074 / sub_180780028.
// In-binary "Environemnt" typo preserved.
// -----------------------------------------------
// Vtable rva 0x3A515E0 (13 slots).  Per-slot bodies not diffed against the
// iface [U] -- no overrides declared here (still abstract).  All concrete
// conditions derive directly from this class (RTTI base array [self, Abstract,
// I_...], all mdisp 0).

namespace wh::xgenaimodule {

class C_AbstractSmartEnvironemntBehaviorCondition : public I_SmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AbstractSmartEnvironemntBehaviorCondition;

    framework::WUID m_wuid;   // +0x08
};
static_assert(sizeof(C_AbstractSmartEnvironemntBehaviorCondition) == 0x10, "abstract SE condition base must be 0x10");

}  // namespace wh::xgenaimodule
