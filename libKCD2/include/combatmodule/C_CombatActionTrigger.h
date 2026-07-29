#pragma once
#include <cstdint>
#include "C_CombatTrigger.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActionTrigger -- action-bound trigger (KCD2 1.5.6, kd7u).  sizeof 0x30.
// -----------------------------------------------
// RTTI: .?AVC_CombatActionTrigger@combatmodule@wh@@ (TD 0x184B3C930). Primary vtable 0x183AED0D0 (12 slots).
// ctor sub_180FC14C4 (alloc 0x30 == PROVEN sizeof); dtor sub_180FC1454 (unregisters two actor listeners).
// Direct base C_CombatTrigger (mdisp 0x00): this is a SIBLING of C_CombatSlotTrigger, NOT a parent of the
// Slot/Combo/PerfectBlock/Riposte branch (decoded from every leaf's RTTI BaseClassArray).
//
// Concrete: it implements the base's pure [11] (sub_181A74D00) and overrides Reset[1], GetSubsystemId[2],
// GetName[3], the debug getters [4]/[5], and update-phase [6] (sub_181418588); it adds NO new vtable slots
// (12 total, same count as C_CombatTrigger). The ctor registers itself into two of the owning actor's
// listener lists (actor vtable +872 / +856) and requests update phase 1 (updateFlags=1 to the UpdatedObject
// ctor). Own data starts at +0x18.
//
// This class had no dedicated KCD1 reference header; its shape is fully KCD2-derived here.

namespace wh::combatmodule {

class C_CombatActionTrigger : public C_CombatTrigger {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActionTrigger;
    // ---- own data (+0x18..+0x30; set by ctor sub_180FC14C4) ----
    int64_t  m_time18;     // +0x18  init -100000 (i64 timestamp/"never" sentinel; semantics UNVERIFIED)
    void*    m_pMatchedRule; // +0x20  non-owning ptr to matched 20-byte rule record inside a global vector (begin/end @ xmmword_185320320); set in update sub_181418588 @0x182360515 = sub_1814185EC(this,actor); rule.float@+0xC arms m_time18, rule.dword@+0x8 builds a spawn request, reset to 0 (NOT released) @0x18236059B
    bool     m_flag28;     // +0x28  init 0
    uint8_t  _pad29[7];    // +0x29
    // int _vf11() const override;  // [11] sub_181A74D00 (implements C_CombatTrigger's pure slot)
};
static_assert(sizeof(C_CombatActionTrigger) == 0x30, "ctor sub_180FC14C4 allocates 0x30");

}  // namespace wh::combatmodule
