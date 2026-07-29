#pragma once
#include <cstdint>
#include "C_AbstractSmartEnvironemntBehaviorCondition.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SEEmptyElementCondition :
// C_AbstractSmartEnvironemntBehaviorCondition -- empty-element SE condition
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18 (factory sub_18077FD14 enum 12:
// operator new(24); ctor writes +0x10 byte = 6).
// -----------------------------------------------
// Per-slot bodies not dumped [U]; pure-slot impls declared without per-slot
// address evidence [U].

namespace wh::xgenaimodule {

class C_SEEmptyElementCondition : public C_AbstractSmartEnvironemntBehaviorCondition {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SEEmptyElementCondition;
    void _vf1() override;  void _vf2() override;  void _vf3() override;
    void _vf4() override;  void _vf5() override;  void _vf6() override;
    void _vf7() override;  void _vf8() override;  void _vf9() override;
    void _vf10() override; void _vf11() override;   // [1..11] impls [U roles]

    uint8_t m_kind;      // +0x10  constant 6 (factory-set; parse sub_181151A90 only reads !=6) — a fixed tag, not a live enum field
    uint8_t _pad11[7];   // +0x11
};
static_assert(sizeof(C_SEEmptyElementCondition) == 0x18, "must be 0x18 (factory new 24, enum 12)");

}  // namespace wh::xgenaimodule
