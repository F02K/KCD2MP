#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SEAIBuffTagCondition :
// C_AbstractSmartEnvironemntBehaviorCondition -- AI-buff-tag SE condition
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18 (factory sub_18077FD14 enum 13:
// operator new(24)).
// -----------------------------------------------
// Per-slot bodies not dumped [U]; own member at +0x10 not itemized [U].

namespace wh::xgenaimodule {

class C_SEAIBuffTagCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SEAIBuffTagCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]

    int32_t m_buffTagId;   // +0x10  buff-tag id (name->id via registry; -1 = unknown); writer slot9 sub_18197BB14
    uint8_t _pad14[4];     // +0x14  padding to 0x18 (zero-init at ctor; not copied by clone)
};
static_assert(sizeof(C_SEAIBuffTagCondition) == 0x18, "must be 0x18 (factory new 24, enum 13)");

}  // namespace wh::xgenaimodule
