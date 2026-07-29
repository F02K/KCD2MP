#pragma once
#include <cstdint>
#include <vector>
#include "../Offsets/vtables/IActor.h"
#include "../framework/C_Signal.h"
#include "../framework/CryDeferrable.h"
#include "../framework/CStateMachine.h"
#include "../CryEngine/CryCommon/CryString.h"   // CryStringT<char>
#include "../CryEngine/CryCommon/Cry_Math.h"
#include "C_PhysicsIgnoreLists.h"
#include "C_BoneSlotSystem.h"
#include "C_IdleState.h"
#include "C_StanceState.h"
#include "C_ActorMoveParams.h"
#include "C_ActorBounds.h"
#include "../animationmodule/C_LookAimIK.h"
#include "../xgenaimodule/I_SystemFromMonsterLODPostponer.h"

// -----------------------------------------------
// wh::entitymodule::C_Actor -- Warhorse base actor (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x9C0.
// -----------------------------------------------
// RTTI .?AVC_Actor@entitymodule@wh@@  primary vtable 0x183E74198 (275 slots)
// ctor 0x1803F0BB8  real dtor 0x1808E120C  scalar-deleting dtor 0x18285A354 (operator delete this,0x9C0)
// GetOrCreateCombatActor (non-virtual) 0x18072DC90 -> returns m_pCombatActor, lazily creating it.
//
// Full MI hierarchy reconstructed and inherited (ClassInformer / 11-entry CHD):
//   CGameObjectExtensionHelper<C_Actor,IActor,64> -> IActor -> IGameObjectExtension
//        (-> IComponent + std::enable_shared_from_this<IComponent>)   @ +0x00  (0x40)
//   IGameObjectView                                                   @ +0x40
//   IGameObjectProfileManager                                         @ +0x48
//   UnsafeOp::CryDeferrable<0> (-> UnsafeOp::ICryDeferrable)  empty    @ +0x50
//   wh::xgenaimodule::I_SystemFromMonsterLODPostponer         empty    @ +0x51
// The two empty trailing bases occupy 1 byte each under MSVC; C_Actor's own members begin at +0x58.
//
// [FUNDAMENTAL vs KCD1] sizeof 0x930 -> 0x9C0; managers relocated (m_pCombatActor 0x1A0 -> 0x278,
//   m_pRagdollManager 0x178 -> 0x190, m_pActionActor 0x1A8 -> 0x280, m_lookAimIK 0x1B0 -> 0x288,
//   m_pActorModel 0x908 -> 0x990, bone system 0x728 -> 0x7A0, m_pPhysicsState 0x4C0 -> 0x238);
//   C_Signal shrank 0x30 -> 0x10 (denser signal block); NEW managers m_pHitDeathReactions @0x240 and
//   m_pConditionController @0x258; m_damageZones (KCD1 inline) became m_pDamageZoneManager @0x1B0.
// [MODERATE vs KCD1] health/maxHealth/gravity -> 0x164/0x168/0x16C; perception -> 0x570/0x574/0x578;
//   view distance -> 0x4D0/0x4D4 (same init values 100/100/9.81, 5deg/30deg/1.0, 11.0).
//
// Pointer slots set OUTSIDE the ctor were traced through their setters: five are RTTI-confirmed
// (ShoutManager/DirtHelper/EquipmentHandler/AIAnimationState/MannequinStateParams), one is
// behavioral (IAnimatedCharacter). Slots still typed void* are honestly unresolved. The IActor
// virtual-getter slot comments (GetCombatActor[193] @0x1A0, GetAnimatedCharacter[221] @0x268)
// carry stale KCD1 offsets.

struct IMovementController;   // CryEngine
struct IAnimatedCharacter;    // CryAction (behavioral identification)

namespace wh::combatmodule { class I_CombatActor; class C_CombatActor; }
namespace wh::rpgmodule    { class C_Soul; }

namespace wh::entitymodule {

class C_Inventory;
class C_RagdollManager;
class C_ActionActor;
class C_HitDeathReactions;
class C_ActorConditionController;
class C_ClothingAttachmentOwnerActor;
class C_ActorModel;
class C_ActorPhysicsState;
class C_ActorHelper;
class C_DamageZoneManager;
class C_ActorShoutManager;         // 56B; registers hit-reaction shout weights ("noHit".."fatalHit")
class C_ActorDirtHelper;           // 0x50; name string "ActorDirtHelper"
class C_ActorEquipmentHandler;     // 24B; self-registers into an owner list
class C_AIAnimationState;          // 48B; AI-only, parses entity "Properties/TagList"
class SMannequinActorStateParams;  // 288B; Mannequin-manager owned (init aborts if null)

class C_Actor
    : public Offsets::CGameObjectExtensionHelper<C_Actor, Offsets::IActor, 64>  // +0x00  (0x40: 275-slot vtable + extension data)
    , public Offsets::IGameObjectView                                          // +0x40
    , public Offsets::IGameObjectProfileManager                                // +0x48
    , public UnsafeOp::CryDeferrable<0>                                         // +0x50  (empty; -> UnsafeOp::ICryDeferrable)
    , public wh::xgenaimodule::I_SystemFromMonsterLODPostponer                  // +0x51  (empty)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Actor;
    // Non-virtual: sub_18072DC90 returns m_pCombatActor, allocating it (C_CombatActor, 0x448) if null.
    wh::combatmodule::C_CombatActor* GetOrCreateCombatActor();
    // The actor's item inventory via the soul chain (typed reimplementation of sub_1808D285C:
    // m_pSoul -> C_Soul::m_inventorySoul (+0x198) -> I_InventorySoul::GetInventory [0]);
    // null if the soul is absent.  Used by the alchemy stocking/reset paths.
    C_Inventory* GetInventory();

    ::CStateMachine<C_Actor> m_stateMachine;           // +0x58   (0x48) transition signal + state ptr + event ring FIFO

    // Signals (each 0x10; all 12 heads verified C_Signal by their init/grow/append helper sets).
    // Emit sites are fully inlined, so arg lists below come from CONNECT-THUNK signatures (a lower
    // bound on arity); arg TYPES beyond arity are inferred from the sharing class, not symbols.
    wh::shared::C_Signal<> m_signalUnkA0;              // +0xA0   T UNRECOVERABLE (sentinel is the process-wide shared empty-COW sentinel)
    wh::shared::C_Signal<wh::combatmodule::I_CombatActor&> m_signalCombatChanged1;  // +0xB0   VERIFIED (same offset/T as KCD1)
    wh::shared::C_Signal<wh::combatmodule::I_CombatActor&> m_signalCombatChanged2;  // +0xC0   VERIFIED (ctor sub_1803F0B9C)
    wh::shared::C_Signal<> m_signalCombatResetD0;      // +0xD0   NO payload (2 independent thunks ignore all args); T shared w/ C_CombatModule+0x10 & combo mgr; name tentative
    wh::shared::C_Signal<> m_signalCombatResetE0;      // +0xE0   pair of +0xD0 (same T)
    wh::shared::C_Signal<> m_signalSoulPropertyF0;     // +0xF0   1 ref arg (thunk sub_1829A9EB8); T shared w/ C_SoulPropertyNotifier; name tentative
    wh::shared::C_Signal<> m_signalSoulProperty100;    // +0x100  pair of +0xF0 (connect via sub_180D5A7C4)
    wh::shared::C_Signal<> m_signalItemChanged110;     // +0x110  >=4 args (subject*, int flags [bit 0x20000000], bool added, bool removed; thunk sub_180879408); KCD1 m_signalItemChanged successor
    wh::shared::C_Signal<> m_signalItemChanged120;     // +0x120  pair of +0x110 (same T)
    wh::shared::C_Signal<> m_signalUnk130;             // +0x130  1 ref arg (thunk sub_18330283C); semantic unresolved
    wh::shared::C_Signal<> m_signalRagdoll140;         // +0x140  1 ref arg; T shared w/ C_RagdollManager+0x38; name tentative
    wh::shared::C_Signal<> m_signalRagdoll150;         // +0x150  pair of +0x140 (connect sub_180B1CAA4, thunk sub_1828F8F3C)

    uint8_t  m_flags160[3];                            // +0x160  flags (ctor sub_182864BC4)
    uint8_t  m_flag163;                                // +0x163  (ctor 0)
    float    m_health;                                 // +0x164  init 100.0
    float    m_maxHealth;                              // +0x168  init 100.0
    float    m_gravity;                                // +0x16C  init 9.81
    uint32_t m_unk170;                                 // +0x170  (ctor 0, gravity qword high dword)
    uint8_t  m_unk174[8];                              // +0x174  qword (ctor 0; unaligned)
    float    m_someScale17C;                           // +0x17C  init 1.0
    IMovementController* m_pMovementController;         // +0x180  IActor GetMovementController[69]  VERIFIED
    void*    m_deferTaskHandle;                        // +0x188  Concurrency::_AutoDeleter<_TaskProcHandle>; CERTIFIED dtor 0x1808E1398 (owns deferred task node)
    C_RagdollManager* m_pRagdollManager;               // +0x190  (0xD0, ctor sub_181366514)  VERIFIED
    C_ActorShoutManager* m_pShoutManager;              // +0x198  OWNED 56B (PostInit; RTTI-verified)
    void*    m_pHelper1A0;                             // +0x1A0  OWNED 16B POD {C_Actor* owner@0, int32@8, bool@0xC}; lazy factory sub_180C03A8C (no vtable -> POD, no RTTI name)
    void*    m_subscription1A8;                        // +0x1A8  OWNED polymorphic T*; dtor sub_18046B5D8 unconditional vtable[0](this,1) (same helper as m_pHitDeathReactions@0x240); T unresolved
    C_DamageZoneManager* m_pDamageZoneManager;         // +0x1B0  (0x90, ctor sub_181366548)  tentative (no RTTI; KCD1 m_damageZones)
    void*    m_pUnk1B8;                                // +0x1B8  OWNED _smart_ptr<T> (intrusive refcount); dtor sub_1808E15E8 (--*(int*)(p+8)==1 -> vtable[0](this,1)); Cry ref-counted T unresolved
    void*    m_pUnk1C0;                                // +0x1C0  OWNED _smart_ptr<T> (intrusive refcount); dtor sub_1808E1730 (--*(int*)(p+8)==1 -> vtable[0](this,1)); Cry ref-counted T unresolved
    void*    m_pUnk1C8;                                // +0x1C8  OWNED _smart_ptr<T> (intrusive refcount); dtor sub_1808E15E8 (same helper/T as +0x1B8); Cry ref-counted T unresolved
    C_PhysicsIgnoreLists m_physicsIgnore;              // +0x1D0  (0x58)
    CryStringT<char> m_str228;                         // +0x228
    CryStringT<char> m_str230;                         // +0x230
    C_ActorPhysicsState* m_pPhysicsState;              // +0x238  (owns 0x98 alloc, ctor sub_181110A8C)  VERIFIED
    C_HitDeathReactions* m_pHitDeathReactions;         // +0x240  (0x290, ctor sub_180515D90)  VERIFIED (NEW in KCD2)
    void*    m_pHelper248;                             // +0x248  OWNED 32B POD {void* owner@0, uint32[3]@8, uint64@0x14}; factory sub_18124D554 (no vtable -> POD, no RTTI name)
    C_ActorHelper* m_pHelper250;                       // +0x250  (0x20 alloc, ctor sub_181332C70)
    C_ActorConditionController* m_pConditionController; // +0x258  (0x80, ctor sub_18136662C)  VERIFIED (NEW in KCD2)
    C_ActorDirtHelper* m_pDirtHelper;                  // +0x260  OWNED 0x50 polymorphic (setter sub_1808DD578; RTTI-verified)
    Offsets::IGameObject* m_pGameObjectOwner;          // +0x268  owning IGameObject (ctor arg a2)  VERIFIED
    void*    m_pUnk270;                                // +0x270  raw ptr (ctor 0; no setter isolable)
    wh::combatmodule::C_CombatActor* m_pCombatActor;   // +0x278  VERIFIED (GetOrCreateCombatActor this[79])  [KCD1 was +0x1A0]
    C_ActionActor* m_pActionActor;                     // +0x280  (0x80, ctor sub_180515D90)  VERIFIED
    wh::animationmodule::C_LookAimIK m_lookAimIK;      // +0x288  (0x188) "Bip01 Head", 6 blend channels
    void*    m_block410[3];                            // +0x410  0x18 zero-init by sub_1809CF028; ABSENT from dtor -> non-owning triple, NOT a std::vector; identity unresolved
    uint8_t  _pad428[8];                               // +0x428
    uint32_t m_unk430;                                 // +0x430  (ctor 0)
    uint8_t  m_stateBlock[0x94];                       // +0x434  zeroed state block (ctor memset 0x94; internals unresolved)
    float    m_unk4C8;                                 // +0x4C8  init 1.0
    uint8_t  _pad4CC[4];                               // +0x4CC
    float    m_viewDist1;                              // +0x4D0  init 11.0
    float    m_viewDist2;                              // +0x4D4  init 11.0
    uint8_t  m_block4D8[0x1C];                          // +0x4D8  zero-init block (mirror target of C_StanceState fields)
    uint32_t m_linkedEntityId;                         // +0x4F4  EntityId (ctor 0)  tentative
    uint8_t  _pad4F8[4];                               // +0x4F8
    uint32_t m_unk4FC;                                 // +0x4FC  (ctor 0)
    float    m_moveSpeedModifier;                      // +0x500  init 1.0
    uint8_t  _pad504[4];                               // +0x504
    C_StanceState m_stanceState;                       // +0x508  (0x4C)
    uint8_t  _pad554[0x10];                            // +0x554
    Vec3     m_lastNavMeshPos;                         // +0x564  zeroed Vec3 (KCD1-correlated name; tentative)
    float    m_perceptionMinAngle;                     // +0x570  init 0.0872665 (~5 deg)
    float    m_perceptionConeHalfAngle;                // +0x574  init 0.5235988 (30 deg)
    float    m_perceptionRangeScale;                   // +0x578  init 1.0
    C_ActorMoveParams m_moveParams;                    // +0x57C  (0x84; m_maxSpeed 80.0 @abs 0x5D8)
    C_ActorBounds m_bounds;                            // +0x600  (0x3C; not a clean AABB -- see its header)
    uint32_t m_unk63C;                                 // +0x63C  (ctor 0)
    uint16_t m_unk640;                                 // +0x640  (ctor 0)
    uint8_t  _pad642[6];                               // +0x642
    wh::shared::C_Signal<> m_observer648;              // +0x648  1 entity-ref arg (thunk sub_1829AA0E8 via connect sub_180AAFDC8)
    wh::shared::C_Signal<> m_observer658;              // +0x658  1 entity-ref arg (same handler as +0x648 but distinct T)
    wh::rpgmodule::C_Soul* m_pSoul;                    // +0x668  the actor's soul: setter sub_18068A324
                                                       //         resolves it from the SOUL REGISTRY
                                                       //         (*(S_GameContext+0x1D0) vf[23], key this+0x38)
                                                       //         [CORRECTED from "IAnimatedCharacter*"]
    CryStringT<char> m_fragmentName;                   // +0x670  "m_FragmentID" (ctor sub_1809CF050)
    C_ActorEquipmentHandler* m_pEquipmentHandler;      // +0x678  list-owned 24B (setter sub_1808D28FC; RTTI-verified)
    C_ClothingAttachmentOwnerActor* m_pClothingOwner;  // +0x680  (0x10, ctor sub_18136657C)  VERIFIED
    void*    m_aiSubMgrCache;                          // +0x688  lazy AI-only cache from a gEnv submanager (pointee unresolved)
    C_IdleState m_idleState;                           // +0x690  (0xA8)
    C_AIAnimationState* m_pAIAnimationState;           // +0x738  OWNED 48B, AI-only (PostInit; RTTI-verified)
    std::vector<void*> m_vec740;                       // +0x740  element type unidentified
    const void* m_pEntityClassParams;                  // +0x758  NON-OWNED per-entity-class registry entry (element type unresolved)
    SMannequinActorStateParams* m_pMannequinStateParams; // +0x760  Mannequin-manager owned (PostInit; RTTI-verified)
    int32_t  m_invalidId;                              // +0x768  init -1
    uint32_t m_unk76C;                                 // +0x76C  (ctor 0)
    uint32_t m_unk770;                                 // +0x770  (ctor 0)
    uint32_t m_unk774;                                 // +0x774  (ctor 0)
    uint64_t m_unk778;                                 // +0x778  (ctor 0)
    uint8_t  m_block780[0x20];                         // +0x780  zero-init block (internals unresolved)
    C_BoneSlotSystem m_boneSlotSystem;                 // +0x7A0  (0x1E0) flat S_BoneSlot[10] -- NOT a hash map
    uint8_t  m_block980[0x10];                         // +0x980  ctor zeroes 12B here (Vec3-shaped) + 4 pad; identity unresolved
    C_ActorModel* m_pActorModel;                       // +0x990  (0x408, ctor sub_1813665D4)  VERIFIED
    uint8_t  m_block998[0x10];                         // +0x998  ctor zeroes 12B (Vec3-shaped) + 4 pad; identity unresolved
    void*    m_pUnk9A8;                                // +0x9A8  raw ptr (ctor 0; pointee polymorphic but base unproven)
    uint16_t m_stateWord9B0;                           // +0x9B0  init 3
    uint8_t  m_flags9B2;                               // +0x9B2  bitflags (ctor (x&1)|0x10)
    uint8_t  m_flags9B3;                               // +0x9B3  bitflags (ctor &= 0xFC)
    uint8_t  m_flag9B4;                                // +0x9B4  (ctor 0; FullSerialize[14] checks it)
    uint8_t  _pad9B5[3];                               // +0x9B5
    void*    m_pDeferredMonster;                       // +0x9B8  I_SystemFromMonsterLODPostponer deferred node; dtor sub_1808E15D8 detaches via sub_1817450BC if non-null (last member)
};
static_assert(sizeof(C_Actor) == 0x9C0, "C_Actor must be 0x9C0");
static_assert(offsetof(C_Actor, m_stateMachine) == 0x58, "m_stateMachine offset");
static_assert(offsetof(C_Actor, m_physicsIgnore) == 0x1D0, "m_physicsIgnore offset");
static_assert(offsetof(C_Actor, m_lookAimIK) == 0x288, "m_lookAimIK offset");
static_assert(offsetof(C_Actor, m_stanceState) == 0x508, "m_stanceState offset");
static_assert(offsetof(C_Actor, m_moveParams) == 0x57C, "m_moveParams offset");
static_assert(offsetof(C_Actor, m_bounds) == 0x600, "m_bounds offset");
static_assert(offsetof(C_Actor, m_idleState) == 0x690, "m_idleState offset");
static_assert(offsetof(C_Actor, m_boneSlotSystem) == 0x7A0, "m_boneSlotSystem offset");

}  // namespace wh::entitymodule
