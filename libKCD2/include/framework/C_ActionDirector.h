#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include "C_Signal.h"
#include "../CryEngine/CryCommon/smartptr.h"

// -----------------------------------------------
// wh::framework::C_ActionDirector -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x90.
// -----------------------------------------------
// Per-slot stack of active actions. ctor sub_1809CDAD0(this, mode). Root class (RTTI: single base =
// self). Primary base of combatmodule::C_CombatActorDirector (mode 0) and C_ActorDirector (mode 1,
// fixedSlotCount 4). Own vtable 0x183A67DE0 has exactly TWO slots: [0] deleting dtor
// (sub_18257838C -> body sub_180FC1B6C), [1] IsActive (base returns true; combat override checks
// the owning actor's state). There is NO third virtual -- the pointer after slot [1] is the next
// object's RTTI COL.
//
// [MODERATE vs KCD1] Shrank 0xB0 -> 0x90 (the two 0x30 signals became 0x10) and gained a THIRD
// signal (+0x28) and a third vector (m_retiredActions, reserved to 2 in the ctor).
// Signal argument lists below are KCD1-correlated (emit arity matches: notification broadcast
// sub_1809CCF08(this+8, evtType, slot, old, new); slot-change sub_180910F8C(this+24, &slot,
// oldId, newId)) -- arg TYPES tentative.

namespace wh::framework {

class I_Action;

// Notification kind passed to m_onActionNotification (set / removed / replaced). Values unresolved.
enum class E_ActionNotificationType : int32_t;

// Director kind (ctor arg stored @+0x68 by sub_1809CDAD0). KCD2-verified domain:
// C_CombatActorDirector ctor sub_180917E80 passes 0; C_ActorDirector passes 1;
// playermodule::C_AlchemyActionDirector (embedded at C_Alchemy+0x78) passes 3 (minigame mode,
// alchemy RE wave). Mode 2 remains unobserved in kd7u.
enum class E_ActionDirectorMode : int32_t {
    Combat   = 0,
    Actor    = 1,
    Minigame = 3,   // observed: alchemy session director (sub_1816E9570 -> sub_1809CDAD0(this+0x78, 3))
};

class C_ActionDirector {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActionDirector;
    virtual ~C_ActionDirector() = default;   // [0]  sub_18257838C -> sub_180FC1B6C
    virtual bool IsActive() { return true; } // [1]  base sub_18041A6A0 returns true

    wh::shared::C_Signal<E_ActionNotificationType, const int&,
                         const _smart_ptr<I_Action>&, const _smart_ptr<I_Action>&>
             m_onActionNotification;                    // +0x08  per-slot add/remove/replace (sentinel unk_185665D58)
    wh::shared::C_Signal<const int&, int, int>
             m_onSlotChange;                            // +0x18  fired when the slot's action id changes (unk_185665D68)
    wh::shared::C_Signal<> m_signal3;                   // +0x28  NEW in KCD2; role/args unresolved (unk_1855D04E8)
    std::vector<_smart_ptr<I_Action>> m_actions;        // +0x38  current action per slot (resized to slotCount+10)
    std::vector<_smart_ptr<I_Action>> m_actionsPrev;    // +0x50  parallel per-slot vector (lock-step resize; name KCD1-correlated)
    E_ActionDirectorMode m_mode;                        // +0x68  ctor arg (sub_1809CDAD0)
    bool     m_notifying;                               // +0x6C  reentrancy guard around notification emits
    bool     m_flag6D;                                  // +0x6D  gates the previous-action-tracking path (writer not located)
    uint8_t  _pad6E[2];                                 // +0x6E
    std::vector<_smart_ptr<I_Action>> m_retiredActions; // +0x70  deferred-release scratch list (ctor reserves 2)
    int32_t  m_primarySlot;                             // +0x88  current/primary slot index (init 0; KCD1 m_currentSlot init -1)
    int32_t  m_fixedSlotCount;                          // +0x8C  slots below this are "fixed" (C_ActorDirector sets 4)
};
static_assert(sizeof(C_ActionDirector) == 0x90, "C_ActionDirector must be 0x90");
static_assert(offsetof(C_ActionDirector, m_actions) == 0x38, "m_actions offset");
static_assert(offsetof(C_ActionDirector, m_retiredActions) == 0x70, "m_retiredActions offset");

}  // namespace wh::framework
