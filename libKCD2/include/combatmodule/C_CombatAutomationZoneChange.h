#pragma once
#include <cstdint>
#include <cstring>
#include <unordered_set>
#include "C_CombatAutomationAction.h"
#include "E_CombatAutomationZoneRequestedChange.h"
#include "E_CombatAutomationZoneState.h"
#include "E_CombatZoneId.h"
#include "../framework/InplaceVector.h"
#include "../framework/HashPrimitives.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationZoneChange : C_CombatAutomationAction -- KCD2 1.5.6 (kd7u).  sizeof 0x1F0.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x2D8 (ctor sub_1810EEC08; vtable 0x183B09E10; debug dump
// sub_182768CB8 "ZONES: ..."). The +0x90 block is a wh::shared::InplaceVector<T,8> (identified by
// its exact layout: 8 inline 20-byte records, in-use flag, self ptr, allocator ptr, spill
// std::vector -- helpers sub_1810EE424/sub_1810EE50C use the /5 magic = 20-byte stride).

namespace wh::combatmodule {

// Inline zone-listener record (20 bytes, alignment 4; field split undecomposed).
struct S_ZoneChangeRecord {
    uint32_t _raw[5];
};
static_assert(sizeof(S_ZoneChangeRecord) == 0x14, "S_ZoneChangeRecord must be 0x14");

// 24-byte trivially-destructible element of m_zoneMap (node 40B). Key/value split unresolved --
// could equally be an unordered_map<8B,16B>; modeled as a set element for layout.
struct S_ZoneMapEntry {
    uint8_t _raw[24];
    bool operator==(const S_ZoneMapEntry& o) const { return std::memcmp(_raw, o._raw, sizeof(_raw)) == 0; }
};

class C_CombatAutomationZoneChange : public C_CombatAutomationAction {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationZoneChange;
    const char* GetName() const override { return "AutomationZoneChange"; }   // [3] 0x181A7E260
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::ZoneChange; }                          // [10] 0x181A72CF0

    bool       m_active;              // +0x030  "ZONES: Active"
    uint8_t    _pad31[7];             // +0x031
    uint64_t   m_id0;                 // +0x038  LCG-generated unique id (low 32 bits meaningful)
    uint64_t   m_id1;                 // +0x040  (rand+1)
    uint64_t   m_id2;                 // +0x048  (rand+3)
    uint64_t   m_field50;             // +0x050  (ctor 0; no reader observed)
    CTimeValue m_timer58;             // +0x058  init -100000 (role unresolved)
    CTimeValue m_zoneChangeTime;      // +0x060  "Change in %.2fs"
    CTimeValue m_timer68;             // +0x068  init -100000
    CTimeValue m_timer70;             // +0x070  init -100000
    CTimeValue m_timer78;             // +0x078  init -100000
    CTimeValue m_timer80;             // +0x080  init -100000
    CTimeValue m_timer88;             // +0x088  init -100000
    wh::shared::InplaceVector<S_ZoneChangeRecord, 8> m_records;   // +0x090  (0xD0) inline 8 + heap spill
    std::unordered_set<S_ZoneMapEntry, wh::shared::S_DefaultHash<S_ZoneMapEntry>> m_zoneMap;  // +0x160  (0x40)
    float      m_cfg1A0;              // +0x1A0  init 3.0
    float      m_cfg1A4;              // +0x1A4  init 7.0
    float      m_cfg1A8;              // +0x1A8  init 1.0
    float      m_cfg1AC;              // +0x1AC  init 2.0
    float      m_cfg1B0;              // +0x1B0  init 0.0
    float      m_cfg1B4;              // +0x1B4  init 0.5
    float      m_cfg1B8;              // +0x1B8  init 0.0
    float      m_cfg1BC;              // +0x1BC  init 1.0
    E_CombatAutomationZoneRequestedChange m_requestedChange;  // +0x1C0  "ReqChange" (ToString sub_181E3F1B0)
    E_CombatAutomationZoneState m_state;  // +0x1C1  "State" (inline decode @0x182768D98)
    uint8_t    _pad1C2[2];            // +0x1C2
    int32_t    m_changeCount;         // +0x1C4  "Changes: %d"
    int32_t    m_zoneId1C8;           // +0x1C8  init -1; likely E_CombatZoneId slot but role UNVERIFIED (not in the debug dump)
    E_CombatZoneId m_targetAtkZone;   // +0x1CC  init Undefined(-1); "TargetAtkZone" dump @0x182768E71 [domain medium: writers untraced]
    CTimeValue m_sequenceChangeTime;  // +0x1D0  "Sequence change"
    CTimeValue m_nextSequenceTime;    // +0x1D8  "Next sequence in"
    uint64_t   m_id3;                 // +0x1E0  (rand+4)
    uint8_t    m_flags1E8;            // +0x1E8  bitfield (ctor: low2 = 0, bit2 = 1)
    uint8_t    _pad1E9[7];            // +0x1E9
};
static_assert(sizeof(C_CombatAutomationZoneChange) == 0x1F0, "C_CombatAutomationZoneChange must be 0x1F0");

}  // namespace wh::combatmodule
