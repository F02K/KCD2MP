#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartEnvironmentDistanceToEntityBehaviorCondition :
// C_AbstractSmartEnvironemntBehaviorCondition -- distance-to-entity SE
// condition (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x20 (factory sub_18077FD14
// enum 2: operator new(32)).
// -----------------------------------------------
// Vtable rva 0x4768148 -- 14 slots (the only 14-slot member of the family:
// adds one NEW virtual [13]).  +0x10 string built by sub_1804F66CC
// (CryStringT<char> init helper).  Pure-slot impls declared without per-slot
// address evidence [U].

namespace wh::xgenaimodule {

class C_SmartEnvironmentDistanceToEntityBehaviorCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartEnvironmentDistanceToEntityBehaviorCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]
    virtual void _vf13();                           // [13] NEW [U role]

    CryStringT<char> m_entityName;   // +0x10  (sub_1804F66CC) [U role]
    uint64_t         m_18;           // +0x18  ctor: 0 [U role]
};
static_assert(sizeof(C_SmartEnvironmentDistanceToEntityBehaviorCondition) == 0x20, "must be 0x20 (factory new 32)");

}  // namespace wh::xgenaimodule
