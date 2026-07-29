#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEnvironmentDistanceToPositioningAreaBehaviorCondition
// : C_AbstractSmartEnvironemntBehaviorCondition -- distance-to-positioning-area
// SE condition (KCD2 WHGame.dll 1.5.6, kd7u).  Size 24-32 (factory branch not
// pinned to this class -- enum->class mapping UNVERIFIED [U]) -- NO
// static_assert.
// -----------------------------------------------
// Vtable rva 0x3FE9740 (13 slots; per-slot bodies not dumped [U]).  Own
// members beyond the 0x10 base NOT itemized [U].

namespace wh::xgenaimodule {

class C_SmartEnvironmentDistanceToPositioningAreaBehaviorCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEnvironmentDistanceToPositioningAreaBehaviorCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]
};
// size UNVERIFIED (24-32, mapping not pinned) -- no static_assert.

}  // namespace wh::xgenaimodule
