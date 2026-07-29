#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEnvironmentLuaBehaviorCondition :
// C_AbstractSmartEnvironemntBehaviorCondition -- Lua-scripted SE condition
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x30 (factory sub_18077FD14 enum 1:
// operator new(48)).
// -----------------------------------------------
// Vtable rva 0x47681C0 (13 slots; per-slot bodies not dumped [U]).  Pure-slot
// impls declared without per-slot address evidence [U].

namespace wh::xgenaimodule {

class C_SmartEnvironmentLuaBehaviorCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEnvironmentLuaBehaviorCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]

    framework::WUID m_wuid10;   // +0x10  [U role]
    uint64_t        m_18;       // +0x18  ctor: 0 [U role]
    uint64_t        m_20;       // +0x20  ctor: 0 [U role]
    uint64_t        m_28;       // +0x28  ctor: 0 [U role]
};
static_assert(sizeof(C_SmartEnvironmentLuaBehaviorCondition) == 0x30, "must be 0x30 (factory new 48)");

}  // namespace wh::xgenaimodule
