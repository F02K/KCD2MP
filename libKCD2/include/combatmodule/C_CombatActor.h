#pragma once
#include <cstdint>
#include "I_CombatActor.h"
#include "E_CounterActionType.h"
#include "../CryEngine/CryCommon/smartptr.h"
#include "../crysystem/EntityEventListenerWithCleanup.h"
#include "../framework/C_Signal.h"
#include "../framework/C_Listeners.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActor -- per-entity combat state object (KCD2 1.5.6, kd7u).  sizeof 0x448.
// -----------------------------------------------
// RTTI primary vtable 0x183E3FAC8 (217 slots)  secondary EntityEventListenerWithCleanup @+0x08 0x183E3FAB0
// ctor sub_1810F01A8   subsystem builder = primary slot 90 sub_180916ECC   dtor sub_1810EF698
// factory sub_1810EFEDC (alloc 0x448 via custom deleter qword_18549D358).
// The player's combat actor is reached via C_Actor::m_pCombatActor (C_Actor+0x278).
//
// Bases: I_CombatActor @+0x00 (provides the interface vtable incl. GetEntity), EntityEventListenerWithCleanup
// @+0x08. Own members from +0x10. The 217-slot primary vtable is not re-enumerated here (only the base
// interface slots are declared). The counter dispatch was located as the non-virtual core sub_181483580
// (see DispatchCounterAction below); the actor-level SetAction entry is the non-virtual sub_18090C850
// (copies the smart_ptr and forwards to the director's sub_1809CC3D4). Their vtable SLOT numbers (if any)
// remain unknown -- do NOT hardcode a slot, find it via a runtime vtable dump.
//
// [FUNDAMENTAL vs KCD1] sizeof 0x780 -> 0x448; every offset moved. MI: I_ItemAttachmentListener base
//   REMOVED, IEntityEventListener folded into EntityEventListenerWithCleanup. C_Signal shrank 0x30 -> 0x10;
//   28 signal heads @+0x38 (KCD1 had ~24). New KCD2 subsystems: GuardVarSystem, SlotPlanner, Weapons,
//   Clinch, Events, OpponentAimingListener, ClenchedHand, Activity.
//
// Signal delegate template args below are KCD1-CORRELATED and TENTATIVE: the descriptor==type identity is
// verified (each empty sentinel has a 1:1 grow helper), but the exact argument lists are inferred by
// order/multiplicity, not emit-site-verified. Heads whose args are genuinely unrecovered use C_Signal<>.

namespace wh::entitymodule { enum class E_HandSlot : int32_t; }

namespace wh::combatmodule {

// --- signal argument types (forward-declared; enum values not recovered) ---
enum class E_CombatActorStateId : int32_t;
namespace E_SignalSource { enum Type : int32_t; }
namespace E_WeaponKind   { enum Type : int32_t; }
class  I_CombatActorAction;
typedef _smart_ptr<I_CombatActorAction> I_CombatActorActionPtr;   // identical typedef in I_CombatActorAction.h
struct S_HitInfo;
struct S_MeleeHitDetails;

// --- subsystem classes (each owned pointer below; RE'd separately) ---
class C_CombatActorDirector;
class C_CombatScene;
struct S_CombatActorState;
class C_CombatActorObject;
class C_CombatActorUpdatedObject;
class C_CombatActorCollisions;
class C_CombatActorAnimCollisions;
class C_CombatActorData;
class C_CombatEffects;
class C_CombatActorRestriction;
class C_CombatEnvironmentManager;
class C_CombatAutomation;
class C_CombatAlignmentManager;
class C_CombatActorVirtualWeapon;
class C_CombatModifierAim;
class C_CombatModifierLookEnemy;
class C_CombatRPG;
class C_CombatHumanPhysics;
class C_CombatActorActionManager;
class C_CombatComboManager;
class C_CombatActorOpponentManager;
class C_CombatActorTimeCop;
class C_CombatActorTimewarp;
class C_CombatActorNextGuardSelector;
class C_CombatActorGuardVarSystem;
class C_CombatActorSlotPlanner;
class C_CombatActorWeapons;
class C_CombatActorClinch;
class C_CombatActorStealth;
class C_CombatActorHorsePullDown;
class C_CombatActorMercyKill;
class C_CombatActorHuntAttack;
class C_CombatActorEvents;
class C_CombatOpponentAimingListener;
class C_CombatActorClenchedHand;
class C_CombatActorActivity;

class C_CombatActor
    : public I_CombatActor                             // +0x00  (217-slot primary vtable; GetEntity=slot7)
    , public ::EntityEventListenerWithCleanup          // +0x08  (2-slot secondary vtable)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActor;

    // ---- non-virtual methods (forwarders in src/combatmodule/C_CombatActor.cpp) ----

    // Creates a counter action through the matching factory AND dispatches it (SetAction +
    // opponent-manager SetLockAction internally). Core sub_181483580 (KCD1 sub_1805633D0
    // analogue; KCD1's 3-arg thin wrapper sub_18069536C has no KCD2 equivalent -- the engine
    // uses per-type wrappers instead, see E_CounterActionType.h). pOutAction receives the
    // created action (caller must Release). scopeIndex: animation scope index (0 = primary).
    // useOpponentDefenseZone: SyncBlock path only -- pick the opponent's LIVE defense-zone
    // property (+0x150) instead of the trigger's stored zone (KCD1 a5 semantics, identical).
    // queryId: second field of the perfect-block factory query; the engine's own wrappers pass
    // the resolved id global dword_18532102C for PerfectBlock/SyncPerfectBlock and -1/caller
    // values elsewhere; -1 mirrors the KCD1 thin-wrapper default.
    void DispatchCounterAction(I_CombatActorActionPtr* pOutAction, E_CounterActionType type,
                               uint32_t scopeIndex = 0, bool useOpponentDefenseZone = false,
                               int32_t queryId = -1);

    // Engage `target` as this actor's 1v1 opponent: extracts the target's entity handle
    // (target->GetEntity()+0x38, identical to KCD1) and forwards through the gate helper
    // sub_180B21328 (no-op unless m_isFullyBuilt @+0x27A) into m_pOpponentManager's
    // UpdateOpponent (sub_180B2158C). The hunt-attack Request (sub_18275EB64) calls this both
    // ways to establish the attacker<->victim link, exactly like KCD1. sub_182757B10.
    void SetOpponent(C_CombatActor* target);

    uint8_t  m_updateMode;                             // +0x28   init 4 (update-bucket mode; enum domain UNVERIFIED, no reader traced)
    uint8_t  _pad29[7];                                // +0x29
    uint64_t m_field30;                                // +0x30   dtor sub_1823C8B4C deregisters &this+0x31 from global registry qword_18492D988; init writer & type unrecovered

    // ---- signals (+0x38..+0x1F8, 28 x 0x10; args KCD1-correlated & tentative unless noted) ----
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, E_CombatActorStateId, E_CombatActorStateId> m_onCombatStateChange;       // +0x38
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, E_CombatActorStateId, E_CombatActorStateId> m_onCombatActionStateChange; // +0x48
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, bool> m_onCombatModeChange;                 // +0x58
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, bool> m_onCombatZoneChange;                 // +0x68
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, int>  m_onCombatGuardTypeChange;            // +0x78
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, int>  m_onOpponentGuardType;                // +0x88
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, int>  m_signal98;                           // +0x98   (KCD2-added int signal)
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, int, wh::entitymodule::E_HandSlot> m_onOpponentHandSlot; // +0xA8
    wh::shared::C_Signal<I_CombatActor&, bool> m_onOpponentPBWindow;                                 // +0xB8
    wh::shared::C_Signal<I_CombatActor&, bool> m_signalC8;                                           // +0xC8
    wh::shared::C_Signal<I_CombatActor&, const int&, const I_CombatActorActionPtr&, const I_CombatActorActionPtr&> m_onActionBase; // +0xD8
    wh::shared::C_Signal<I_CombatActor&, const int&, const I_CombatActorActionPtr&, const I_CombatActorActionPtr&> m_signalE8;     // +0xE8
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, const int&, const I_CombatActorActionPtr&, const I_CombatActorActionPtr&> m_signalF8;               // +0xF8
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, const int&, const I_CombatActorActionPtr&, const I_CombatActorActionPtr&> m_onOpponentActionChanged; // +0x108
    wh::shared::C_Signal<> m_onCombatEvent;                                                          // +0x118  (parameterless; high confidence)
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, E_SignalSource::Type, bool> m_signal128;    // +0x128
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, E_SignalSource::Type, bool> m_signal138;    // +0x138
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, E_SignalSource::Type> m_signal148;          // +0x148
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, E_SignalSource::Type, bool> m_signal158;    // +0x158
    wh::shared::C_Signal<I_CombatActor&> m_signal168;                                                // +0x168
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor&, E_SignalSource::Type> m_signal178;          // +0x178
    wh::shared::C_Signal<I_CombatActor&, E_SignalSource::Type, E_WeaponKind::Type> m_onWeaponChange; // +0x188
    wh::shared::C_Signal<> m_signal198;                                                              // +0x198  (M-group; args unrecovered)
    wh::shared::C_Signal<> m_signal1A8;                                                              // +0x1A8  (M-group; args unrecovered)
    wh::shared::C_Signal<> m_signal1B8;                                                              // +0x1B8  (M-group; args unrecovered)
    wh::shared::C_Signal<> m_signal1C8;                                                              // +0x1C8  (M-group; args unrecovered)
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor*, S_HitInfo&, S_MeleeHitDetails&> m_onHit;    // +0x1D8
    wh::shared::C_Signal<I_CombatActor&, I_CombatActor*, S_HitInfo&, S_MeleeHitDetails&> m_signal1E8; // +0x1E8

    // ---- core pointers + subsystem region 1 (+0x1F8..+0x270) ----
    Offsets::IEntity*             m_pEntity;           // +0x1F8  (GetEntity slot7)  VERIFIED
    C_CombatActorDirector*        m_pDirector;         // +0x200  (0xB8)  VERIFIED
    C_CombatScene*                m_pCombatScene;      // +0x208  (ctor arg)  VERIFIED
    S_CombatActorState*           m_pState;            // +0x210  (0xD10)  VERIFIED
    C_CombatActorCollisions*      m_pCollisions;       // +0x218  (0x268)  VERIFIED
    C_CombatActorAnimCollisions*  m_pAnimCollisions;   // +0x220  (0x40)   VERIFIED
    C_CombatActorData*            m_pActorData;        // +0x228  (0x30)   VERIFIED
    C_CombatEffects*              m_pEffects;          // +0x230  (0x30)   VERIFIED
    C_CombatActorRestriction*     m_pRestriction;      // +0x238  (0x50)   VERIFIED
    C_CombatEnvironmentManager*   m_pEnvironmentManager; // +0x240 (0xA0)  VERIFIED
    C_CombatAutomation*           m_pAutomation;       // +0x248  LAZY (null until first use)  VERIFIED
    C_CombatAlignmentManager*     m_pAlignmentManager; // +0x250  (0x78)   VERIFIED
    C_CombatActorVirtualWeapon*   m_pVirtualWeapon;    // +0x258  (0x20)   VERIFIED
    C_CombatModifierAim*          m_pModifierAim;      // +0x260  (0x20)   VERIFIED
    C_CombatModifierLookEnemy*    m_pModifierLookEnemy; // +0x268 (0x20)   VERIFIED
    void*                         m_pActorRef0;        // +0x270  -> 8-byte heap cell holding C_CombatActor* back-ref (builder alloc(8); *cell=this)

    // ---- scalars + listeners (+0x278..+0x386) ----
    uint16_t m_flags278;                               // +0x278  (init 0)
    uint8_t  m_isFullyBuilt;                           // +0x27A  0 -> 1 when the builder finishes
    uint8_t  _pad27B;                                  // +0x27B
    int32_t  m_stateIndex;                             // +0x27C  init -1
    uint8_t  m_isInitialized;                          // +0x280  init 1
    uint8_t  m_flag281;                                // +0x281  (init 0)
    uint8_t  _pad282[2];                               // +0x282
    uint8_t  m_field284[8];                            // +0x284  combat runtime state; state-reset sub_1810EFF60 zeroes it as a qword (with m_stateIndex/m_flags278/timers); non-zero writer & sub-type unrecovered
    uint8_t  m_stateFlags28C;                          // +0x28C  masked &0xFC
    uint8_t  _pad28D[3];                               // +0x28D
    float    m_timerA;                                 // +0x290  init -1.0
    float    m_timerB;                                 // +0x294  init -1.0
    uint8_t  m_meleeRuntimeState[0x1C];                // +0x298  gameplay-set state block
    uint8_t  m_updatePhase;                            // +0x2B4  init 4 (per-frame update phase; enum domain UNVERIFIED)
    uint8_t  _pad2B5[3];                               // +0x2B5
    wh::shared::C_Listeners<C_CombatActorObject, 1>        m_objectListeners;   // +0x2B8  registered subsystem objects
    wh::shared::C_Listeners<C_CombatActorUpdatedObject, 1> m_updateListeners0;  // +0x2E0  per-frame update buckets
    wh::shared::C_Listeners<C_CombatActorUpdatedObject, 1> m_updateListeners1;  // +0x308
    wh::shared::C_Listeners<C_CombatActorUpdatedObject, 1> m_updateListeners2;  // +0x330
    wh::shared::C_Listeners<C_CombatActorUpdatedObject, 1> m_updateListeners3;  // +0x358
    int32_t  m_updateCounter;                          // +0x380  (init 0)
    int16_t  m_index384;                               // +0x384  init -1
    uint8_t  _pad386[2];                               // +0x386

    // ---- subsystem region 2 (+0x388..+0x438) ----
    C_CombatRPG*                    m_pCombatRPG;       // +0x388  (0x60)  VERIFIED
    void*                           m_pActorRef1;       // +0x390  -> 8-byte heap cell holding C_CombatActor* back-ref (builder alloc(8); *cell=this)
    C_CombatHumanPhysics*           m_pHumanPhysics;    // +0x398  (0x18)  VERIFIED
    void*                           m_pStateListener;   // +0x3A0  -> heap 0x20 non-poly obj (ctor sub_1809179E4): {C_CombatActor* owner; ListNode* conn; qword; qword}; subscribes cb sub_180C5B160 to m_pState(+0x210) signal @+0x18
    C_CombatActorActionManager*     m_pActionManager;   // +0x3A8  (0x70)  VERIFIED
    C_CombatComboManager*           m_pComboManager;    // +0x3B0  (0xF0)  VERIFIED
    C_CombatActorOpponentManager*   m_pOpponentManager; // +0x3B8  (0x40)  VERIFIED
    C_CombatActorTimeCop*           m_pTimeCop;         // +0x3C0  (0x98)  VERIFIED
    C_CombatActorTimewarp*          m_pTimewarp;        // +0x3C8  (0x48)  VERIFIED
    C_CombatActorNextGuardSelector* m_pNextGuardSelector; // +0x3D0 (0x20) VERIFIED
    C_CombatActorGuardVarSystem*    m_pGuardVarSystem;  // +0x3D8  (0x40)  VERIFIED (NEW in KCD2)
    C_CombatActorSlotPlanner*       m_pSlotPlanner;     // +0x3E0  (0x58)  VERIFIED (NEW in KCD2)
    C_CombatActorWeapons*           m_pWeapons;         // +0x3E8  (0x30)  VERIFIED (NEW in KCD2)
    C_CombatActorClinch*            m_pClinch;          // +0x3F0  (0x28)  VERIFIED (NEW in KCD2)
    C_CombatActorStealth*           m_pStealth;         // +0x3F8  (0xC8)  VERIFIED
    C_CombatActorHorsePullDown*     m_pHorsePullDown;   // +0x400  (0x28)  VERIFIED
    C_CombatActorMercyKill*         m_pMercyKill;       // +0x408  (0x20)  VERIFIED
    C_CombatActorHuntAttack*        m_pHuntAttack;      // +0x410  (0x20)  VERIFIED
    C_CombatActorEvents*            m_pEvents;          // +0x418  (0x78)  VERIFIED (NEW in KCD2)
    C_CombatOpponentAimingListener* m_pOpponentAimingListener; // +0x420 (0x18) VERIFIED (NEW in KCD2)
    C_CombatActorClenchedHand*      m_pClenchedHand;    // +0x428  (0x48)  VERIFIED (NEW in KCD2)
    C_CombatActorActivity*          m_pActivity;        // +0x430  (0x10)  VERIFIED (NEW in KCD2)
    void*                           m_field438;         // +0x438  ctor 0x1810F05CC zero-init; no writer in ctor/builder/dtor/reset; sits directly before m_lock (lock-guarded lazy ptr candidate, unconfirmed)
    void*                           m_lock;             // +0x440  SRWLOCK.Ptr -- ctor 0x1810F05D3 InitializeSRWLock(&this+0x440); field IS the lock (one pointer-sized word), not a pointer to a lock
};
static_assert(sizeof(C_CombatActor) == 0x448, "C_CombatActor must be 0x448");

}  // namespace wh::combatmodule
