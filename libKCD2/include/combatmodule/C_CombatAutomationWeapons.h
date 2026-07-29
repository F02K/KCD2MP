#pragma once
#include <cstdint>
#include "C_CombatAutomationAction.h"
#include "E_CombatAutomationWeaponRequest.h"
#include "../entitymodule/I_EquipmentManagerListener.h"
#include "../game/I_EntitySideEffectCallback.h"
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatAutomationWeapons -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x90.
// -----------------------------------------------
// Embedded by value at C_CombatAutomation+0x560 (ctor sub_1810EF1B4; primary vtable 0x183B0A2A0;
// debug dump sub_182768BE8 "WEAPONS: ..."). MULTIPLE INHERITANCE (RTTI-confirmed):
// C_CombatAutomationAction (primary) + I_EquipmentManagerListener (mdisp 0x30; its
// OnEquipmentChanged handler sub_1808D4B64 replans weapons and resets m_equipReplanTimer) +
// wh::game::I_EntitySideEffectCallback (mdisp 0x38; sets the bool inside m_stateFlags80 on match).

namespace wh::combatmodule {

class C_CombatAutomationWeapons
    : public C_CombatAutomationAction                  // +0x00  (0x30)
    , public wh::entitymodule::I_EquipmentManagerListener  // +0x30  (vtable 0x183B0A278)
    , public wh::game::I_EntitySideEffectCallback      // +0x38  (vtable 0x183B0A260)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatAutomationWeapons;
    const char* GetName() const override { return "AutomationWeapons"; }   // [3] 0x181A7E250
    E_CombatAutomationActionKind GetActionKind() const override { return E_CombatAutomationActionKind::Weapons; }                      // [10] 0x181A7D850
    void OnEntitySideEffectAdded(void* sideEffect) override {}             // [0]+[1] ICF-folded onto sub_182767740: sets the bool in m_stateFlags80 on match
    void OnEntitySideEffectRemoved(void* sideEffect) override {}           // (same folded handler)

    wh::shared::C_Signal<> m_signalA;      // +0x40  (sentinel unk_1855D4D60; args unrecovered)
    wh::shared::C_Signal<> m_signalB;      // +0x50  (sentinel unk_185665AC0; args unrecovered)
    CTimeValue m_updateTimer;              // +0x60  "Update %.2fs"
    CTimeValue m_equipReplanTimer;         // +0x68  reset to -100000 by the equip callback
    float      m_cfg70;                    // +0x70  init 5.0
    E_CombatAutomationWeaponRequest m_requestedWeapon;  // +0x74  "Req" model (ToString sub_1827690D4)
    uint8_t    _pad75[3];                  // +0x75
    CTimeValue m_overrideReplaceTimer;     // +0x78  "OverRepl %.2fs"
    int32_t    m_stateFlags80;             // +0x80  state/flags; the side-effect callback sets the byte at +0x82
    uint8_t    _pad84[4];                  // +0x84
    uint64_t   m_field88;                  // +0x88  (ctor 0; no reader observed)
};
static_assert(sizeof(C_CombatAutomationWeapons) == 0x90, "C_CombatAutomationWeapons must be 0x90");

}  // namespace wh::combatmodule
