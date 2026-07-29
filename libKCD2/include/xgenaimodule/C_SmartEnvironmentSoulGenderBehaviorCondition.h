#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEnvironmentSoulGenderBehaviorCondition :
// C_AbstractSmartEnvironemntBehaviorCondition -- soul-gender SE condition
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18 (factory sub_18077FD14 enum 4:
// operator new(24)).
// -----------------------------------------------
// Vtable rva 0x3D17EB8 (13 slots; per-slot bodies not dumped [U]).  Pure-slot
// impls declared without per-slot address evidence [U].

namespace wh::xgenaimodule {

class C_SmartEnvironmentSoulGenderBehaviorCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEnvironmentSoulGenderBehaviorCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]

    uint8_t m_genderFlag;   // +0x10  gender selector {0..3}: ctor default 1, overwritten by parse slot [9] sub_1819D4BAC (atoi, rejects >3); member names UNVERIFIED
    uint8_t _pad11[7];      // +0x11
};
static_assert(sizeof(C_SmartEnvironmentSoulGenderBehaviorCondition) == 0x18, "must be 0x18 (factory new 24)");

}  // namespace wh::xgenaimodule
