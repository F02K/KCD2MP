#pragma once

#include <cstdint>

// -----------------------------------------------
// IActor - KCD2 binary vtable order (Warhorse merged actor interface)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (KCD2), image base 0x180000000
// RTTI:   .?AVC_Actor@entitymodule@wh@@
// C_Actor primary vtable: 0x183e74198  (275 slots)   <-- this interface
//   secondary vtable {for IGameObjectView}         @ C_Actor +0x40
//   secondary vtable {for IGameObjectProfileManager}@ C_Actor +0x48
// Scalar deleting dtor (slot 0): 0x18285a354 -> operator delete(this, 0x9C0)
//
// Merged (interfuscated) interface: IComponent + IGameObjectExtension + IActor,
// same shape as KCD1 (KCD1 had 242 slots; KCD2 grew to 275). The primary vtable
// carries the IComponent/IGameObjectExtension data at +0x08..+0x3F so that
// sizeof(IActor)==0x40, enabling MI with the two secondary bases at +0x40/+0x48.
//
// VERIFIED accessor slots are decompiled from kd7u; every other slot keeps the
// terse _vfN form (dispatch index is correct, name is not yet recovered). Names
// carried over from the KCD1 slot map are marked "tentative".
//
// Common stubs seen: 0x1803b6e80 = retn 0, 0x18066cd10 = xor eax/retn,
//                    0x180838ae0 = ret false, 0x18041a6a0 = ret true.

struct IEntity;
struct IComponentEventDistributer;   // RTTI .?AUIComponentEventDistributer@@ (global ns, IComponent.h)

namespace Offsets {

struct IGameObject;

// IGameObjectExtension - SDK base of IActor, flattening the extension chain
//   IComponent (empty root; its virtuals are folded onto IActor in this interfuscated build)
//   + std::enable_shared_from_this<IComponent> (the weak_ptr subobject at +0x08)
//   + IGameObjectExtension's own data (+0x18..+0x3F).
// Only the data survives here (virtuals live on IActor). First member sits at +0x08 so the
// derived IActor's vptr occupies +0x00; sizeof 0x38 so IActor rounds to 0x40.
struct IGameObjectExtension {
    uint64_t        m_weakPtr[2];              // +0x08  std::enable_shared_from_this<IComponent> weak_ptr
    IComponentEventDistributer* m_pDistributer;  // +0x18  non-owning back-ptr to CEntitySystem's event-distributer singleton (IComponent::SetDistributer)
    uint32_t        m_componentEntityId;       // +0x20
    uint32_t        m_componentFlags;          // +0x24
    IGameObject*    m_pGameObject;             // +0x28
    uint32_t        m_entityId;                // +0x30  EntityId
    uint32_t        _pad34;                    // +0x34
    IEntity*        m_pEntity;                 // +0x38  VERIFIED (slot 5 returns this)
};
static_assert(sizeof(IGameObjectExtension) == 0x38);

struct IActor : IGameObjectExtension {
    virtual void  Dtor(char flags) = 0;                     // [0]   0x18285a354  scalar deleting dtor, delete 0x9C0
    virtual void  ProcessEvent(void* pEvent) = 0;           // [1]   0x1804a8ea8  SEntityEvent&
    virtual int   GetEventFlags() = 0;                      // [2]   0x1804a97d0
    virtual int   GetEventPriority(int id) = 0;             // [3]   0x18083d2e4
    virtual void  _vf04() = 0;                              // [4]   retn 0
    virtual IEntity* GetEntity() = 0;                       // [5]   return *(this+0x38)          VERIFIED
    virtual void  _vf06() = 0;                              // [6]   retn 0
    virtual bool  Init(IGameObject* pGO) = 0;               // [7]   0x180ae86e0  (1153 bytes)
    virtual void  PostInit(IGameObject* pGO) = 0;           // [8]   0x1808cfe9c
    virtual void  _vf09() = 0;                              // [9]   retn 0
    virtual void  _vf10() = 0;                              // [10]  retn 0
    virtual void  _vf11() = 0;                              // [11]  0x1828604a4  InitClient (tentative)
    virtual void  _vf12() = 0;                              // [12]  0x18285fddc  ReloadExtension (tentative)
    virtual void  _vf13() = 0;                              // [13]  0x18285d928
    virtual void  FullSerialize(void* pSer) = 0;            // [14]  0x1808dfc50  checks byte@0x9B4
    virtual void  _vf15() = 0;                              // [15]  0x18285d264
    virtual bool  _vf16() = 0;                              // [16]  ret false
    virtual bool  _vf17() = 0;                              // [17]  ret true
    virtual int   _vf18() = 0;                              // [18]  ret -1
    virtual void  PostSerialize() = 0;                      // [19]  0x18285feac  (tentative)
    virtual void  _vf20() = 0;                              // [20]  retn 0
    virtual void* _vf21() = 0;                              // [21]  *(rdx)=0; ret rdx
    virtual void  Update(void* ctx, int slot) = 0;          // [22]  0x18064450c  (tentative)
    virtual void  HandleEvent(void* pEvent) = 0;            // [23]  0x180706190  (tentative)
    virtual void  _vf24() = 0;                              // [24]  retn 0
    virtual void  _vf25() = 0;                              // [25]  retn 0
    virtual void* _vf26() = 0;                              // [26]  lea rax, unk_1855968B8
    virtual void  _vf27() = 0;                              // [27]  0x180644b14  SetHealth (tentative)
    virtual void  _vf28() = 0;                              // [28]  retn 0
    virtual void  _vf29() = 0;                              // [29]  0x1815cdc8c
    virtual float GetHealth() = 0;                          // [30]  soul@0x668 else float@0x164 VERIFIED
    virtual void  _vf31() = 0;                              // [31]  0x18285d9c8
    virtual void  SetMaxHealth(float v) = 0;                // [32]  movss [this+0x168],xmm1      VERIFIED
    virtual float GetMaxHealth() = 0;                       // [33]  movss xmm0,[this+0x168]      VERIFIED
    virtual int   GetArmor() = 0;                           // [34]  ret 0
    virtual int   GetMaxArmor() = 0;                        // [35]  ret 0
    virtual bool  IsFallen() = 0;                           // [36]  ret 0 (tentative)
    virtual bool  IsDead() = 0;                             // [37]  vfunc[1] on ragdoll@0x190    VERIFIED
    virtual bool  IsDeadByHealth() = 0;                     // [38]  soul@0x668 else health@0x164 VERIFIED
    virtual void  _vf39() = 0;                              // [39]  xor eax
    virtual bool  _vf40() = 0;                              // [40]  0x18285cf2c  IsFriendlyEntity (tentative)
    virtual bool  _vf41() = 0;                              // [41]  ret false   IsThirdPerson (tentative)
    virtual bool  _vf42() = 0;                              // [42]  0x180830694
    virtual int   _vf43() = 0;                              // [43]  ptr@0x68 -> [+0x50] shr 1
    virtual bool  _vf44() = 0;                              // [44]  0x1804addf0
    virtual void  _vf45() = 0;                              // [45]  retn 0
    virtual void* _vf46() = 0;                              // [46]  calls GetAnimatedCharacter vfunc[0x6E8]
    virtual void  _vf47() = 0;                              // [47]  retn 0
    virtual void* _vf48() = 0;                              // [48]  0x180854cf0  GetCurrentItem (tentative)
    virtual bool  _vf49() = 0;                              // [49]  ret false
    virtual void  _vf50() = 0;                              // [50]  retn 0
    virtual void* _vf51() = 0;                              // [51]  &member@0x2B8
    virtual void  _vf52() = 0;                              // [52]  retn 0
    virtual void  _vf53() = 0;                              // [53]  xor eax
    virtual void  _vf54() = 0;                              // [54]  retn 0
    virtual void  _vf55() = 0;                              // [55]  retn 0
    virtual void  SetViewRotation(const void* q) = 0;       // [56]  obj@0x238 (tentative)
    virtual void  GetViewRotation(void* out) = 0;           // [57]  obj@0x238 +0x24 (tentative)
    virtual void  _vf58() = 0;                              // [58]  obj@0x238
    virtual float GetEyeHeight() = 0;                       // [59]  *(float*)(*(this+0x238)+0x48) VERIFIED
    virtual bool  _vf60() = 0;                              // [60]  ret true  AllowLandingBob (tentative)
    virtual void  _vf61() = 0;                              // [61]  0x18063cfdc
    virtual void  _vf62() = 0;                              // [62]  0x18285d64c  CameraShake (tentative)
    virtual void  _vf63() = 0;                              // [63]  retn 0
    virtual void  _vf64() = 0;                              // [64]  xor eax
    virtual void  _vf65() = 0;                              // [65]  retn 0
    virtual void  _vf66() = 0;                              // [66]  xor eax
    virtual void  _vf67() = 0;                              // [67]  xor eax
    virtual void  _vf68() = 0;                              // [68]  retn 0
    virtual void* GetMovementController() = 0;              // [69]  return *(this+0x180)          VERIFIED
    virtual void* _vf70() = 0;                              // [70]  &member@0x1D0 (lookAimIK)
    virtual void  _vf71() = 0;                              // [71]  ret false
    virtual void  _vf72() = 0;                              // [72]  0x1804a8c8c
    virtual void  _vf73() = 0;                              // [73]  retn 0
    virtual bool  _vf74() = 0;                              // [74]  ret false
    virtual void  _vf75() = 0;                              // [75]  retn 0
    virtual void  _vf76() = 0;                              // [76]  retn 0
    virtual void  _vf77() = 0;                              // [77]  ret false
    virtual bool  IsPlayer() = 0;                           // [78]  gameObject@0x28 channelId    VERIFIED
    virtual bool  IsPlayer2() = 0;                          // [79]  thunk -> [78]                 VERIFIED
    virtual void  _vf80() = 0;                              // [80]  ret false
    virtual void  _vf81() = 0;                              // [81]  retn 0
    virtual void  _vf82() = 0;                              // [82]  retn 0
    virtual const char* GetActorClassName() = 0;            // [83]  lea rax, "C_Actor"            VERIFIED
    virtual int   GetActorClass() = 0;                      // [84]  xor eax (ret 0)
    virtual void  _vf85() = 0;                              // [85]  0x180647e78  ShouldMute (tentative)
    virtual void* _vf86() = 0;                              // [86]  entity@0x38 -> vcall
    virtual void  _vf87() = 0;                              // [87]  __purecall
    virtual void  _vf88() = 0;                              // [88]  retn 0
    virtual void* _vf89() = 0;                              // [89]  vfunc[0x6E0]
    virtual void* GetAnimatedCharacterConst() = 0;          // [90]  *(GetAnimatedCharacter()+0x20) VERIFIED
    virtual void  _vf91() = 0;                              // [91]  retn 0
    virtual void  _vf92() = 0;                              // [92]  retn 0
    virtual void  _vf93() = 0;                              // [93]  retn 0
    virtual void* _vf94() = 0;                              // [94]  vfunc[0x6E0]
    virtual bool  _vf95() = 0;                              // [95]  "Use of IActor resource" - Respawn (tentative)
    virtual void  _vf96() = 0;                              // [96]  "Use of IActor resource" - ResetToSpawn (tentative)
    virtual bool  _vf97() = 0;                              // [97]  ret false  CanBreakGlass (tentative)
    virtual bool  _vf98() = 0;                              // [98]  ret false  MustBreakGlass (tentative)
    virtual void  _vf99()  = 0;                             // [99]  retn 0
    // ---- C_Actor-specific / extended slots (100..274) ----
    virtual void  _vf100() = 0;
    virtual void  _vf101() = 0;
    virtual bool  _vf102() = 0; // [102] dispatch by dl
    virtual void  _vf103() = 0;
    virtual void  _vf104() = 0;
    virtual void  _vf105() = 0;
    virtual bool  _vf106() = 0;
    virtual void  _vf107() = 0;
    virtual bool  _vf108() = 0; // [108] ret true
    virtual void  _vf109() = 0;
    virtual void  _vf110() = 0;
    virtual void  _vf111() = 0;
    virtual void  _vf112() = 0;
    virtual void  _vf113() = 0;
    virtual void  _vf114() = 0;
    virtual void  _vf115() = 0;
    virtual void  _vf116() = 0;                              // [116] jmp vfunc[0x5E0]
    virtual uint8_t _vf117() = 0;                            // [117] return *(uint8*)(this+0x163)
    virtual void  _vf118(uint8_t) = 0;                       // [118] *(this+0x160+idx)=arg (m_specMode[])
    virtual void  _vf119() = 0;
    virtual void  _vf120() = 0;                              // [120] and byte[this+0x9B2],~4  (clear flag)
    virtual void* _vf121() = 0;                              // [121] vfunc[0x6E8]
    virtual void  _vf122() = 0;                              // [122] uses obj@0x990
    virtual void  _vf123() = 0;
    virtual void  _vf124() = 0;
    virtual uint8_t _vf125() = 0;                            // [125] return *(uint8*)(this+0x63D)
    virtual void  _vf126() = 0;
    virtual void  _vf127() = 0;                              // [127] uses ptr@0x688
    virtual void  _vf128() = 0;                              // [128] uses obj@0x990
    virtual bool  _vf129() = 0;
    virtual void  _vf130() = 0;
    virtual void  _vf131() = 0;                              // [131] 983 bytes
    virtual void  _vf132() = 0;
    virtual void* _vf133() = 0;                              // [133] ptr@0x738
    virtual void* _vf134() = 0;                              // [134] ptr@0x738
    virtual void  _vf135(uint8_t) = 0;                       // [135] add [this+0x174], counter
    virtual void  _vf136(uint8_t) = 0;                       // [136] add [this+0x178], counter
    virtual bool  _vf137() = 0;
    virtual int  _vf138() = 0;
    virtual int  _vf139() = 0;
    virtual void  _vf140() = 0;                              // [140] uses ptr@0x260
    virtual void  _vf141() = 0;                              // [141] uses soul@0x668
    virtual void* GetActionActor() = 0;                      // [142] return *(this+0x1A8)          VERIFIED
    virtual void  _vf143() = 0;                              // [143] checks ptr@0x1A8
    virtual int   _vf144() = 0;
    virtual void  _vf145() = 0;
    virtual void  _vf146(int) = 0;                           // [146] *(int*)(this+0x560)=arg (audio id)
    virtual void  _vf147() = 0;
    virtual void  _vf148() = 0;
    virtual void  _vf149() = 0;
    virtual void  _vf150() = 0;
    virtual void  _vf151() = 0;
    virtual void  _vf152() = 0;
    virtual void  _vf153() = 0;                              // [153] or byte[this+0x9B3],1 (set flag)
    virtual void  _vf154() = 0;
    virtual void  _vf155() = 0;
    virtual void  _vf156() = 0;
    virtual bool  _vf157() = 0;
    virtual void  _vf158() = 0;
    virtual void  _vf159() = 0;
    virtual void  _vf160() = 0;
    virtual bool  _vf161() = 0;
    virtual void  _vf162() = 0;
    virtual bool  _vf163() = 0;
    virtual bool  _vf164() = 0;
    virtual bool  _vf165() = 0;
    virtual void  _vf166() = 0;
    virtual void  _vf167() = 0;
    virtual void  _vf168() = 0;
    virtual void  _vf169() = 0;
    virtual bool  _vf170() = 0;
    virtual bool  _vf171() = 0;                              // [171] *(this+0x250); byte@+0x18
    virtual void  _vf172() = 0;                              // [172] uses obj@0x238
    virtual void  _vf173() = 0;                              // [173] jmp vfunc[0x560]
    virtual float _vf174() = 0;                              // [174] movss xmm0, const
    virtual void  _vf175() = 0;                              // [175] uses soul@0x668
    virtual void  _vf176() = 0;
    virtual void  _vf177() = 0;
    virtual void  _vf178() = 0;
    virtual void  _vf179() = 0;
    virtual void  _vf180() = 0;
    virtual void  _vf181() = 0;
    virtual void  _vf182() = 0;
    virtual void  _vf183() = 0;
    virtual void  _vf184() = 0;                              // [184] *(this+0x990)+0x310
    virtual void  _vf185() = 0;
    virtual void  _vf186() = 0;
    virtual void  _vf187() = 0;
    virtual void  _vf188() = 0;
    virtual void  _vf189() = 0;
    virtual void  _vf190() = 0;
    virtual int   _vf191() = 0;
    virtual void  _vf192() = 0;
    virtual void* GetCombatActor() = 0;                      // [193] return *(this+0x1A0)          VERIFIED
    virtual void* _vf194() = 0;                              // [194] return *(this+0x1B0)          VERIFIED
    virtual void  _vf195() = 0;
    virtual void  _vf196() = 0;
    virtual void  _vf197() = 0;
    virtual bool  _vf198() = 0;
    virtual void  _vf199() = 0;
    virtual void  _vf200() = 0;
    virtual void  _vf201() = 0;
    virtual void  _vf202() = 0;                              // [202] lea "Collisions..."
    virtual void  _vf203() = 0;                              // [203] uses obj@0x280
    virtual void  _vf204() = 0;
    virtual void  _vf205() = 0;
    virtual void* _vf206() = 0;                              // [206] &member@0xB0 (signal)
    virtual void* _vf207() = 0;                              // [207] &member@0xC0 (signal)
    virtual void  _vf208() = 0;
    virtual void  _vf209() = 0;                              // [209] clears dword@0x564 + qword@0x568
    virtual void  _vf210() = 0;
    virtual void  _vf211() = 0;
    virtual void  _vf212() = 0;
    virtual void  _vf213() = 0;
    virtual void  _vf214() = 0;
    virtual void  _vf215() = 0;
    virtual void  _vf216() = 0;
    virtual void  _vf217() = 0;
    virtual float _vf218() = 0;                              // [218] movss xmm0,[this+0x4C8]
    virtual float _vf219() = 0;                              // [219] movss xmm0,[this+0x4CC]
    virtual void* _vf220() = 0;                              // [220] return *(this+0x268) (anim char) VERIFIED
    virtual void* GetAnimatedCharacter() = 0;                // [221] return *(this+0x268)          VERIFIED
    virtual void  _vf222() = 0;
    virtual void  _vf223() = 0;
    virtual void  _vf224() = 0;
    virtual void  _vf225() = 0;
    virtual void  _vf226() = 0;
    virtual bool  _vf227() = 0;
    virtual bool  _vf228() = 0;
    virtual bool  _vf229() = 0;
    virtual bool  _vf230() = 0;                              // [230] return *(uint8*)(this+0x5FC) & 1
    virtual void  _vf231() = 0;                              // [231] uses obj@0x990
    virtual bool  _vf232() = 0;
    virtual void  _vf233() = 0;                              // [233] jmp vfunc[0x150]
    virtual void  _vf234() = 0;                              // [234] uses soul@0x668
    virtual void  _vf235() = 0;                              // [235] uses obj@0x990
    virtual int   _vf236() = 0;                              // [236] return 0x65
    virtual void  _vf237() = 0;                              // [237] uses ptr@0x68
    virtual void  _vf238() = 0;
    virtual void  _vf239() = 0;
    virtual void  _vf240() = 0;
    virtual void  _vf241() = 0;
    virtual void  _vf242() = 0;
    virtual int  _vf243() = 0;
    virtual void  _vf244() = 0;
    virtual void  _vf245() = 0;
    virtual void  _vf246() = 0;
    virtual void  _vf247() = 0;
    virtual void  _vf248() = 0;
    virtual void  _vf249() = 0;
    virtual void  _vf250() = 0;
    virtual void  _vf251() = 0;
    virtual void  _vf252() = 0; // [252] jmp vfunc[0x7D8]
    virtual void  _vf253() = 0;
    virtual void  _vf254() = 0;
    virtual void  _vf255() = 0;
    virtual float _vf256() = 0;                              // [256] movss xmm0,[rax+0x34C]
    virtual bool  _vf257() = 0;
    virtual void  _vf258() = 0;
    virtual void  _vf259() = 0; // [259] vfunc[0x6E0]
    virtual void  _vf260() = 0;
    virtual void  _vf261() = 0;
    virtual void  _vf262() = 0;
    virtual void  _vf263() = 0;
    virtual void  _vf264() = 0;
    virtual void  _vf265() = 0;
    virtual void  _vf266() = 0;
    virtual void  _vf267() = 0;
    virtual void  _vf268() = 0;
    virtual void  _vf269() = 0;
    virtual void  _vf270() = 0;
    virtual void  _vf271() = 0;
    virtual void  _vf272() = 0;                              // [272] __purecall
    virtual void  _vf273() = 0;
    virtual void  _vf274() = 0;
};
static_assert(sizeof(IActor) == 0x40);

// Secondary bases (each contributes a single vtable pointer to C_Actor at
// +0x40 / +0x48). Their own virtual sets are opaque / not yet recovered; only
// the vptr matters for the C_Actor layout.
struct IGameObjectView {
    virtual void _vf0() = 0;                                 // secondary vtable {for IGameObjectView}
};
static_assert(sizeof(IGameObjectView) == 0x08);

struct IGameObjectProfileManager {
    virtual void _vf0() = 0;                                 // secondary vtable {for IGameObjectProfileManager}
};
static_assert(sizeof(IGameObjectProfileManager) == 0x08);

// CGameObjectExtensionHelper<T, I, N> - CryEngine game-object-extension helper (CRTP over T).
// Thin: derives the interface I (=IActor) and adds NO data -- it only provides the static N-slot
// RMI dispatch. It is C_Actor's direct primary base; sizeof == sizeof(I) so the layout is IActor's.
template<typename T, typename I, int N>
struct CGameObjectExtensionHelper : I {};

}  // namespace Offsets
