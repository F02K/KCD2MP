#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEnvironmentExpressionBehaviorCondition :
// C_AbstractSmartEnvironemntBehaviorCondition -- expression-driven SE
// condition (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x28 (factory sub_18077FD14
// enum 0: operator new(40), ctor sub_1832D624C).
// -----------------------------------------------
// Vtable rva 0x3A51650 (13 slots; per-slot bodies not dumped [U]).  The
// concrete class implements the 11 pure iface slots -- declared without
// per-slot address evidence [U].

namespace wh::xgenaimodule {

class C_SmartEnvironmentExpressionBehaviorCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEnvironmentExpressionBehaviorCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]

    framework::WUID m_exprWuid;   // +0x10  (expression id) [U role]
    uint64_t        m_18;         // +0x18  ctor: 0 [U role]
    void*           m_20;         // +0x20  ctor a2 = descriptor field +0x20; stored raw (no refcount/copy helper, unlike m_exprWuid which uses sub_1804FD208) -> plain context ptr [U pointee]
};
static_assert(sizeof(C_SmartEnvironmentExpressionBehaviorCondition) == 0x28, "must be 0x28 (factory new 40)");

}  // namespace wh::xgenaimodule
