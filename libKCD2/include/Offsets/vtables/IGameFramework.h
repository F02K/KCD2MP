#pragma once
#include <cstdint>
#include "IActor.h"          // Offsets::IActor       (GetClientActor return type)
#include "IEntity.h"         // Offsets::IEntity       (GetClientEntity return type)
#include "IActorSystem.h"    // Offsets::IActorSystem  (GetIActorSystem return type)
#include "IItemSystem.h"     // Offsets::IItemSystem   (GetIItemSystem return type)

typedef unsigned int EntityId;
struct INetChannel;   // CryNetwork channel (global ns, as in the SDK); opaque here

// -----------------------------------------------
// IGameFramework - KCD2 binary vtable order  (CryAction CCryAction)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6, image base 0x180000000 (IDA instance "kd7u").
// RTTI:   .?AVCCryAction@@
// vtable: 0x1840472d0  (image-relative 0x40472d0), 143 slots.
// ctor:   0x181865c3a   base dtor: 0x18355fce8   scalar-deleting dtor: slot [5] (0x183560d0c)
// Singleton: the object is a static global at 0x18562EF50 (NOT heap-allocated);
//            IGameFramework* is cached at qword_18549D328 and qword_185497F70.
//
// Verified vs KCD1 (instance "hkmp"): early slots and the subsystem accessors line up
// slot-for-slot with a -1 slide at the top (KCD1 put the dtor at [6]; here it is [5],
// re-aligning by [17]). The subsystem *offsets* drifted +0x08..+0x10 from KCD1.
// Slots [30]+ are partially reshuffled (e.g. GetIGameStatistics moved to [142]);
// names there marked "tentative" are inferred from the accessor's target offset and
// KCD1 semantics -- decompile before relying on them.
//
// Confirmed CCryAction member offsets (from accessor disasm; see crysystem/CCryAction.h):
//   +0x048 ISystem*            +0x510 CLevelSystem        +0x518 CActorSystem
//   +0x520 CItemSystem         +0x530 CActionMapManager   +0x538 CViewSystem
//   +0x558 CGameObjectSystem   +0x560 CUIDraw             +0x578 CMannequinInterface
//   +0x5F8 CGameStatistics     +0x610 CGameSessionHandler +0x490 game GUID string[0x80]

// Action event dispatched to IGameFrameworkListeners via OnActionEvent.
struct SActionEvent {
    int         m_event;        // +0x00  event type (EActionEvent)
    int         m_value;        // +0x04  context-dependent (e.g. ESaveGameReason)
    const char* m_description;  // +0x08  optional string (e.g. checkpoint name)

    // EActionEvent ordinals broadcast via the OnActionEvent slot-120 broadcaster
    // (sub_180668D48). The stock CryAction save/load events (0xA-0xD) are UNCHANGED from
    // KCD1, emitted by CCryAction::SaveGame (sub_183582A68) / LoadGame (sub_18357594C)
    // around (de)serialization. 0xE is a separate Warhorse event, broadcast by
    // C_SaveGameManager::PostLoadGame (sub_1825BDA54) after the whole load pipeline
    // finishes (name INFERRED; sits where stock eAE_inGame would).
    enum : int {
        eAE_PreSave  = 0xA,   // CCryAction::SaveGame, before serialize
        eAE_PostSave = 0xB,   // CCryAction::SaveGame, after serialize (m_description = checkpoint name)
        eAE_PreLoad  = 0xC,   // CCryAction::LoadGame, before deserialize
        eAE_PostLoad = 0xD,   // CCryAction::LoadGame, after deserialize
        eAE_InGame   = 0xE,   // WH C_SaveGameManager::PostLoadGame, load pipeline done (INFERRED)
    };
};

namespace Offsets {

// Lean binary vtable base. Accessor slots carry the CCryAction member offset they
// touch; non-accessor slots keep their KCD1 name where the slide is confirmed,
// otherwise stay terse (_vfN) with the function byte-size / offset as a hint.
struct IGameFramework {
    virtual void _vf0() = 0;                                          // [0]   0x00  forwards through GameObjectSystem@+0x558
    virtual void _vf1() = 0;                                          // [1]   0x08  uses +0x520 (ItemSystem)
    virtual void _vf2() = 0;                                          // [2]   0x10  uses +0x518 (ActorSystem); jmp
    virtual void _vf3() = 0;                                          // [3]   0x18  uses +0x5B8
    virtual void _vf4() = 0;                                          // [4]   0x20  uses +0x5B8
    virtual void Dtor(bool freeMem) = 0;                             // [5]   0x28  scalar-deleting dtor -> base dtor 0x18355FCE8; delete N=0x7C0
    virtual bool Init(void* startupParams) = 0;                      // [6]   0x30  2601 bytes; returns bool ("Failed to initialize CryENGINE!")
    virtual void _vf7() = 0;                                         // [7]   0x38  309 bytes  (KCD1 PostInit)
    virtual void _vf8() = 0;                                         // [8]   0x40  1190 bytes (KCD1 CompleteInit)
    virtual void _vf9() = 0;                                         // [9]   0x48  1481 bytes (KCD1 Shutdown)
    virtual void _vf10() = 0;                                        // [10]  0x50  911 bytes  (KCD1 PreUpdate)
    virtual void _vf11() = 0;                                        // [11]  0x58  929 bytes  (KCD1 PostUpdate)
    virtual void _vf12() = 0;                                        // [12]  0x60  163 bytes  (KCD1 Reset)
    virtual void _vf13() = 0;                                        // [13]  0x68  618 bytes  (KCD1 PauseGame)
    virtual bool _vf14(uint16_t idx) const = 0;                     // [14]  0x70  cmp dword[this+idx*4+8],0; setnle (KCD1 IsGamePaused-adjacent)
    virtual void _vf15() = 0;                                        // [15]  0x78  49 bytes
    virtual void* _vf16() const = 0;                                // [16]  0x80  reads game-context ptr@+0x88
    virtual bool IsLoadingSaveGame() const = 0;                     // [17]  0x88  return *(bool*)(this+0x44)                   VERIFIED
    virtual void SetLoadingSaveGame(bool bLoading) = 0;             // [18]  0x90  *(bool*)(this+0x44)=arg                      VERIFIED
    virtual void* GetISystem() = 0;                                 // [19]  0x98  return *(this+0x48)                          VERIFIED
    virtual void* _vf20() = 0;                                      // [20]  0xA0  return *(this+0x5E0)
    virtual void* GetIUIDraw() = 0;                                 // [21]  0xA8  return *(this+0x560) (CUIDraw)               VERIFIED
    virtual void* GetIMannequinInterface() = 0;                    // [22]  0xB0  return *(this+0x578) (CMannequinInterface)   VERIFIED
    virtual void* GetIGameObjectSystem() = 0;                      // [23]  0xB8  return *(this+0x558) (CGameObjectSystem)     VERIFIED
    virtual void* GetILevelSystem() = 0;                           // [24]  0xC0  return *(this+0x510) (CLevelSystem)          VERIFIED
    virtual IActorSystem* GetIActorSystem() = 0;                   // [25]  0xC8  return *(this+0x518) (CActorSystem)          VERIFIED
    virtual IItemSystem* GetIItemSystem() = 0;                     // [26]  0xD0  return CItemSystem inner ptr (+0x520+8)      VERIFIED
    virtual void* GetITimeDemoRecorder() = 0;                      // [27]  0xD8  return global qword_1856652A0+8              VERIFIED
    virtual void* GetIActionMapManager() = 0;                      // [28]  0xE0  return *(this+0x530) (CActionMapManager)     VERIFIED
    virtual void* GetIViewSystem() = 0;                            // [29]  0xE8  return *(this+0x538) (CViewSystem)           VERIFIED
    virtual void* _vf30() = 0;                                     // [30]  0xF0  return *(this+0x540)
    virtual void* GetIVehicleSystem() = 0;                         // [31]  0xF8  return *(this+0x548)                         tentative
    virtual void* GetIGameRulesSystem() = 0;                       // [32]  0x100 return *(this+0x550)                         tentative
    virtual void* _vf33() = 0;                                     // [33]  0x108 return *(this+0x5A8)
    virtual void* GetIGameTokenSystem() = 0;                       // [34]  0x110 return *(this+0x5B0)                         tentative
    virtual void* GetIEffectSystem() = 0;                          // [35]  0x118 return *(this+0x580)                         tentative
    virtual void* GetIMaterialEffects() = 0;                       // [36]  0x120 return *(this+0x598)                         tentative
    virtual void* _vf37() = 0;                                     // [37]  0x128 return *(this+0x590)
    virtual void* _vf38() = 0;                                     // [38]  0x130 return *(this+0x5A0)
    virtual void* _vf39() = 0;                                     // [39]  0x138 101 bytes, TLS-guarded (lazy getter)
    virtual void* _vf40() = 0;                                     // [40]  0x140 return *(this+0x600)
    virtual void* _vf41() = 0;                                     // [41]  0x148 return *(this+0x5D0)
    virtual void* _vf42() = 0;                                     // [42]  0x150 return *(this+0x608)
    virtual void _vf43() = 0;                                      // [43]  0x158 jmp sub_1835717E8 (thunk)
    virtual void* _vf44() = 0;                                     // [44]  0x160 return *(this+0x5D8)
    virtual void* _vf45() = 0;                                     // [45]  0x168 return *(this+0x5E8)
    virtual void* _vf46() = 0;                                     // [46]  0x170 return *(this+0x5F0)
    virtual void _vf47() = 0;                                      // [47]  0x178 uses session handler@+0x610
    virtual void* _vf48() = 0;                                     // [48]  0x180 return *(this+0x528)
    virtual void _vf49() = 0;                                      // [49]  0x188 160 bytes
    virtual void _vf50() = 0;                                      // [50]  0x190 179 bytes
    virtual void _vf51() = 0;                                      // [51]  0x198 uses game-context@+0x88
    virtual void _vf52() = 0;                                      // [52]  0x1A0 324 bytes
    virtual void _vf53() = 0;                                      // [53]  0x1A8 uses +0x88
    virtual void _vf54() = 0;                                      // [54]  0x1B0 uses +0x88
    virtual void SetGameSessionHandler(void* pHandler) = 0;        // [55]  0x1B8 *(this+0x610)=arg (returns old)              VERIFIED
    virtual void _vf56() = 0;                                      // [56]  0x1C0 uses +0x88
    virtual void _vf57() = 0;                                      // [57]  0x1C8 uses +0x88
    virtual void _vf58() = 0;                                      // [58]  0x1D0
    virtual void _vf59() = 0;                                      // [59]  0x1D8 146 bytes
    virtual void _vf60() = 0;                                      // [60]  0x1E0 uses +0x88
    virtual void _vf61() = 0;                                      // [61]  0x1E8 uses +0x88
    virtual void _vf62() = 0;                                      // [62]  0x1F0 140 bytes
    virtual void _vf63() = 0;                                      // [63]  0x1F8 jmp sub_183584E00
    virtual IActor*  GetClientActor() const = 0;                   // [64]  0x200 0x1806458C4 -> helper 0x1806458E0: m_pGame(+0x88) -> clientNub(+0x48) -> channel(+0x10) -> playerId(+0x20) -> IActorSystem::GetActor (slot 3), cached at game+0xA3C/+0xA40; editor branch via qword_18492D890
    virtual EntityId GetClientActorId() const = 0;                 // [65]  0x208 0x1807EF07C: GetClientActor()->m_entityId (IGameObjectExtension +0x30)
    virtual IEntity* GetClientEntity() const = 0;                  // [66]  0x210 0x1835711A4: pEntitySystem(qword_18492D8A0)->GetEntity( (*this)[67]() )
    virtual EntityId GetClientEntityId() const = 0;                // [67]  0x218 0x1809F3CD0: m_pGame -> clientNub -> channel -> raw id @+0x20
    virtual INetChannel* GetClientChannel() const = 0;             // [68]  0x220 0x18357117C: same chain -> INetChannel* @channel+0x08
    virtual void DelegateAuthority(EntityId entityId, uint16_t channelId) = 0; // [69] 0x228 0x183565EE0: channelId ? (*this)[73](channelId) : 0, then gamecontext obj(+0x58)->vfunc[24](entityId, chan)
    virtual void GetServerTime(int64_t& serverTimeOut) = 0;        // [70]  0x230 0x1813125F8: CTimeValue sret (ref param = the hidden return ptr); server: timer qword_18492D880 slot 6; client: channel remote time + offset @+0x70
    virtual uint16_t GetGameChannelId(INetChannel* pNetChannel) = 0; // [71] 0x238 0x183571568: server nub via (*this)[141], reverse lookup sub_180B85DA8
    virtual bool IsChannelOnHold(uint16_t channelId) = 0;          // [72]  0x240 0x183574240: server nub channel (sub_180B876D8) -> hold byte @+0x3B
    virtual INetChannel* GetNetChannel(uint16_t channelId) = 0;    // [73]  0x248 0x1835724EC: gated on byte_18492DA38 (server-side flag, role unverified); server nub channel -> INetChannel* @+0x08
    virtual void _vf74() = 0;                                      // [74]  0x250 uses cs:qword_18492D8A0
    virtual void _vf75() = 0;                                      // [75]  0x258 88 bytes
    virtual void _vf76() = 0;                                      // [76]  0x260 165 bytes
    virtual void _vf77() = 0;                                      // [77]  0x268 50 bytes
    virtual void* _vf78() = 0;                                     // [78]  0x270 return *(this+0x630)
    virtual void* _vf79() = 0;                                     // [79]  0x278 return *(this+0x638)
    virtual void _vf80() = 0;                                      // [80]  0x280 883 bytes
    virtual void _vf81() = 0;                                      // [81]  0x288 1111 bytes
    virtual void _vf82() = 0;                                      // [82]  0x290 *(byte*)(this+0x742)=1; uses +0x750
    virtual void _vf83() = 0;                                      // [83]  0x298 311 bytes
    virtual bool _vf84() const = 0;                               // [84]  0x2A0 return *(bool*)(this+0x741)
    virtual void _vf85() = 0;                                      // [85]  0x2A8 calls sub_18066B990
    virtual void _vf86() = 0;                                      // [86]  0x2B0 calls sub_18066B990
    virtual void _vf87() = 0;                                      // [87]  0x2B8 uses +0x630
    virtual void _vf88() = 0;                                      // [88]  0x2C0 uses +0x630
    virtual void AllowSave(bool bAllow) = 0;                      // [89]  0x2C8 *(bool*)(this+0x78C)=arg                       VERIFIED
    virtual void AllowLoad(bool bAllow) = 0;                      // [90]  0x2D0 *(bool*)(this+0x78D)=arg                       VERIFIED
    virtual void _vf91() = 0;                                      // [91]  0x2D8 140 bytes
    virtual void _vf92() = 0;                                      // [92]  0x2E0 69 bytes
    virtual void _vf93() = 0;                                      // [93]  0x2E8 206 bytes
    virtual void _vf94() = 0;                                      // [94]  0x2F0 46 bytes
    virtual void _vf95() = 0;                                      // [95]  0x2F8 105 bytes
    virtual void _vf96() = 0;                                      // [96]  0x300 136 bytes
    virtual void* _vf97() = 0;                                     // [97]  0x308 return *(this+0x6A8)
    virtual void _vf98(void* p) = 0;                              // [98]  0x310 *(this+0x760)=arg
    virtual void _vf99() = 0;                                      // [99]  0x318 *(this+0x760)=0
    virtual void _vf100() = 0;                                     // [100] 0x320 414 bytes
    virtual void _vf101() = 0;                                     // [101] 0x328 uses ptr@+0x758
    virtual void _vf102() = 0;                                     // [102] 0x330 uses +0x88
    virtual void _vf103() = 0;                                     // [103] 0x338 uses +0x88
    virtual void _vf104() = 0;                                     // [104] 0x340 lea +0x490; copies 0x80 bytes into GUID buffer
    virtual const char* GetGameGUID() = 0;                        // [105] 0x348 return (char*)(this+0x490)                     VERIFIED
    virtual void _vf106() = 0;                                     // [106] 0x350 calls sub_18066B990
    virtual void _vf107() = 0;                                     // [107] 0x358 1072 bytes
    virtual void _vf108(uint8_t v) = 0;                           // [108] 0x360 *(dword*)(this+0x788)=v
    virtual void _vf109() = 0;                                     // [109] 0x368 retn 0 (stub)
    virtual void _vf110() = 0;                                     // [110] 0x370 95 bytes
    virtual void _vf111() = 0;                                     // [111] 0x378 162 bytes
    virtual bool _vf112() const = 0;                              // [112] 0x380 return *(dword*)(this+0x788)!=0
    virtual void _vf113() = 0;                                     // [113] 0x388 calls sub_18066B990
    virtual void _vf114() = 0;                                     // [114] 0x390 mov rcx,[this+0x790]; jmp
    virtual void* _vf115() const = 0;                             // [115] 0x398 return **(this+0x790)
    virtual void _vf116() = 0;                                     // [116] 0x3A0 uses ptr@+0x790
    virtual void _vf117() = 0;                                     // [117] 0x3A8 42 bytes
    virtual void _vf118() = 0;                                     // [118] 0x3B0 121 bytes
    virtual void _vf119() = 0;                                     // [119] 0x3B8 199 bytes
    virtual void OnActionEvent(SActionEvent* a_event) = 0;         // [120] 0x3C0 sub_180668D48
                                                                   //       broadcaster: loops listener
                                                                   //       list @+0x758, calls each +0x28  VERIFIED
    virtual void _vf121() = 0;                                     // [121] 0x3C8 uses ItemSystem@+0x520
    virtual void _vf122() = 0;                                     // [122] 0x3D0 uses +0x88
    virtual void _vf123() = 0;                                     // [123] 0x3D8 100 bytes  sub_183576D00 (game-context forwarder; NOT OnActionEvent -- that is slot 120)
    virtual void _vf124() = 0;                                     // [124] 0x3E0 41 bytes
    virtual void _vf125() = 0;                                     // [125] 0x3E8 71 bytes
    virtual void _vf126() = 0;                                     // [126] 0x3F0 105 bytes
    virtual void _vf127() = 0;                                     // [127] 0x3F8 51 bytes
    virtual uint32_t GetPreUpdateTicks() = 0;                     // [128] 0x400 return+reset *(dword*)(this+0x7B4)             tentative
    virtual float GetLoadSaveDelay() const = 0;                   // [129] 0x408 return (float)*(double*)(this+0x7A0)          tentative
    virtual void _vf130() = 0;                                     // [130] 0x410 223 bytes
    virtual void _vf131() = 0;                                     // [131] 0x418 cmp cs:byte_18492DA39
    virtual void* _vf132() = 0;                                    // [132] 0x420 return *(this+0x620)
    virtual void _vf133() = 0;                                     // [133] 0x428 uses +0x630
    virtual void _vf134() = 0;                                     // [134] 0x430 jmp sub_18093F3DC
    virtual void _vf135() = 0;                                     // [135] 0x438 retn 0 (stub)
    virtual void _vf136() = 0;                                     // [136] 0x440 lea r9,[this+0x6F0] (container@+0x6F0)
    virtual void _vf137() = 0;                                     // [137] 0x448 reads [this+0x6F8] (container end)
    virtual void _vf138() = 0;                                     // [138] 0x450 175 bytes
    virtual void* _vf139() = 0;                                    // [139] 0x458 return *(this+0x588)
    virtual void _vf140() = 0;                                     // [140] 0x460 62 bytes
    virtual void _vf141() = 0;                                     // [141] 0x468 uses +0x88
    virtual void* GetIGameStatistics() = 0;                       // [142] 0x470 return *(this+0x5F8) (CGameStatistics)        VERIFIED
};

}  // namespace Offsets
