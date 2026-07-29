#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEnvironmentDistanceToPlayerBehaviorCondition :
// C_AbstractSmartEnvironemntBehaviorCondition -- distance-to-player SE
// condition (KCD2 WHGame.dll 1.5.6, kd7u).  Size ~0x18 (factory enum 8 ->
// sub_1832D6224, new 24 -- but the enum->class mapping is by ctor-address
// proximity, UNVERIFIED [U]) -- NO static_assert.
// -----------------------------------------------
// Vtable rva 0x3FEBAB8 (13 slots; per-slot bodies not dumped [U]).  Own
// members beyond the 0x10 base NOT itemized [U].

namespace wh::xgenaimodule {

class C_SmartEnvironmentDistanceToPlayerBehaviorCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEnvironmentDistanceToPlayerBehaviorCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]

    float   m_maxDistance;   // +0x10  max distance-to-player; setter 0x1832D7B98 atof->float, eval 0x1832D772C ((max^2)>=dist^2), dbg "DistanceToPlayer Max:%f"
    uint8_t _pad14[4];       // +0x14  tail padding (alloc 0x18; only the +0x10 float is live)
};
// size UNVERIFIED (enum->class mapping by proximity) -- no static_assert.

}  // namespace wh::xgenaimodule
