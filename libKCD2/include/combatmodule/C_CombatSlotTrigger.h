#pragma once
#include <cstdint>
#include "C_CombatTrigger.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatSlotTrigger -- timing-slot trigger (KCD2 1.5.6, kd7u).  sizeof 0x48.
// -----------------------------------------------
// RTTI: .?AVC_CombatSlotTrigger@combatmodule@wh@@ (TD 0x184B3CA60). Primary vtable 0x183A60EF0 (18 slots).
// ctor sub_180914514(this, actor, updateFlags); dtor sub_1809144D0 (releases the ref-counted object at +0x40
// via sub_181AB5160). Direct base C_CombatTrigger (mdisp 0x00). Base of the Combo / PerfectBlock / Riposte
// (and SyncPerfectBlock) leaves.
//
// Adds 6 vtable slots [12..17] over C_CombatTrigger's 12:
//   [12] _vf12 sub_1819DE250 (setup/evaluate)   [13] _vf13 sub_18041A6A0 (returns true)
//   [14] OnActivate (nullsub base)              [15] OnDeactivate (nullsub base)
//   [16] _vf16 (nullsub base)                   [17] _vf17 (nullsub base)
//
// Own data (+0x18..+0x48) initialised by the ctor; field names KCD1-CORRELATED (the KCD2 layout matches the
// KCD1 0x48 SlotTrigger byte-for-byte: the ctor writes qword 0 @+0x18, float 1.0 @+0x20, i64 -100000 @+0x28
// and @+0x30, qword 0 @+0x38, and null @+0x40).
//
// [vs KCD1] Same 0x48 size and field layout; only the vtable addresses moved.

namespace wh::combatmodule {

class C_CombatSlotTrigger : public C_CombatTrigger {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatSlotTrigger;
    virtual void _vf12() {}                 // [12] setup/evaluate (base sub_1819DE250)
    virtual int  _vf13() { return 1; }      // [13] returns true (base sub_18041A6A0)
    virtual void OnActivate() {}            // [14] fires when the slot opens (base nullsub; KCD1-correlated name)
    virtual void OnDeactivate() {}          // [15] fires when the slot closes (base nullsub; KCD1-correlated name)
    virtual void _vf16() {}                 // [16] (base nullsub)
    virtual void _vf17() {}                 // [17] (base nullsub)

    // ---- own data (+0x18..+0x48) ----
    bool     m_setupFlag;      // +0x18  set 0/1 by sub_180909E68 (via _vf12 sub_1819DE250)
    bool     m_activeFlag;     // +0x19  set 0/1 individually (packed pair, not an enum)
    uint8_t  _pad1A;           // +0x1A
    bool     m_isActive;       // +0x1B  slot-open flag (init 0)   [KCD1-correlated]
    float    m_duration;       // +0x1C  timing-window duration (init 0)   [KCD1-correlated]
    float    m_scaleFactor;    // +0x20  blend/scale factor (init 1.0f)   [KCD1-correlated]
    uint32_t _pad24;           // +0x24
    int64_t  m_timerData[3];   // +0x28  timers (init {-100000, -100000, 0}; i64 sentinels)
    void*    m_pOwnedObject;   // +0x40  owned heap object (init 0); dtor 0x18091450D frees it via sub_181AB5160 -> accounted free through global deallocator qword_18549D358 (NOT a refcount release) [pointee type UNVERIFIED]
};
static_assert(sizeof(C_CombatSlotTrigger) == 0x48, "ctor sub_180914514 lays out 0x48");

}  // namespace wh::combatmodule
