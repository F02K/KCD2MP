#pragma once
#include <cstdint>

typedef unsigned int EntityId;
typedef uint64_t EntityGUID;   // CryEngine SDK CryCommon/IEntity.h: typedef uint64 EntityGUID

// -----------------------------------------------
// IEntity - KCD2 binary vtable order  (CryEntitySystem CEntity)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (release_1_5 / assembly 15693), md5 170a55fe1ef804b4a9ac6fbf9f6843e5
// RTTI:   .?AVCEntity@@   TypeDescriptor 0x185042118
// vtable: 0x184072c78 (image-relative 0x4072c78), 158 slots, COL 0x184626b88
// Concrete data layout: see crysystem/CEntity.h (class CEntity, sizeof 0xF8).
//
// Confidence: offsets + accessor identities marked VERIFIED are read straight from the
// disasm/ctor. Names on non-accessor slots ("tentative") are inferred from KCD1 slot
// position and want a decompile pass before relying on them.

struct IEntityProxy;

namespace Offsets {

struct IEntity {
    virtual void _vf0() = 0;                                          // [0]   0x00  scalar-deleting dtor (0x1808e2e00)
    virtual EntityId GetId() const = 0;                              // [1]   0x08  return *(uint32*)(this+0x0C)                 VERIFIED
    virtual EntityGUID GetGuid() const = 0;                          // [2]   0x10  return *(uint64*)(this+0x10)                 VERIFIED
    virtual void* GetClass() const = 0;                              // [3]   0x18  return *(this+0x20) (IEntityClass*)          VERIFIED
    virtual void* GetArchetype() const = 0;                          // [4]   0x20  return *(this+0x28)                          VERIFIED
    virtual void SetFlags(uint32_t flags) = 0;                       // [5]   0x28  *(this+0x18)=flags, notify proxies           VERIFIED
    virtual uint32_t GetFlags() const = 0;                           // [6]   0x30  return *(uint32*)(this+0x18)                 VERIFIED
    virtual void AddFlags(uint32_t flags) = 0;                       // [7]   0x38  or [this+0x18],flags; -> SetFlags            VERIFIED
    virtual void ClearFlags(uint32_t flags) = 0;                     // [8]   0x40  and-not [this+0x18]; -> SetFlags             VERIFIED
    virtual bool CheckFlags(uint32_t mask) const = 0;                // [9]   0x48  (mask & *(this+0x18))==mask                  VERIFIED
    virtual void SetFlagsExtended(uint32_t flags) = 0;               // [10]  0x50  *(this+0x1C)=flags                           VERIFIED
    virtual uint32_t GetFlagsExtended() const = 0;                   // [11]  0x58  return *(uint32*)(this+0x1C)                 VERIFIED
    virtual bool CheckFlagsExtended(uint32_t mask) const = 0;        // [12]  0x60  (mask & *(this+0x1C))==mask                  VERIFIED
    virtual bool _vf13() const = 0;                                  // [13]  0x68  return *(uint8*)(this+0xF6)                  byte getter @0xF6
    virtual void _vf14(bool) = 0;                                    // [14]  0x70  reads word@0xF2 (arg-dependent)              @0xF2
    virtual bool _vf15() const = 0;                                  // [15]  0x78  return *(uint16*)(this+0xF2)!=0
    virtual uint32_t _vf16() const = 0;                              // [16]  0x80  (*(uint32*)(this+0x08) >> 5) & 1             bitfield@+0x08
    virtual void SetName(const char* name) = 0;                      // [17]  0x88  stashes arg (const char*); rewrites m_name   tentative (pairs with [18])
    virtual const char* GetName() const = 0;                         // [18]  0x90  return *(char**)(this+0xE0)                  VERIFIED name@0xE0
    virtual void _vf19() = 0;                                        // [19]  0x98  216 bytes (text description / debug name?)
    virtual void _vf20() = 0;                                        // [20]  0xA0  216 bytes
    virtual bool _vf21() const = 0;                                  // [21]  0xA8  (*(uint32*)(this+0x08) >> 0x17) & 1          bitfield@+0x08
    virtual void _vf22() = 0;                                        // [22]  0xB0  469 bytes
    virtual void _vf23() = 0;                                        // [23]  0xB8
    virtual void _vf24() = 0;                                        // [24]  0xC0  543 bytes
    virtual void _vf25() = 0;                                        // [25]  0xC8  reads ptr@0x98
    virtual void _vf26() = 0;                                        // [26]  0xD0  reads ptr@0x98
    virtual void _vf27() = 0;                                        // [27]  0xD8  reads ptr@0x98
    virtual void _vf28() = 0;                                        // [28]  0xE0  reads ptr@0x98
    virtual void _vf29() = 0;                                        // [29]  0xE8  reads ptr@0x98
    virtual void GetWorldTM(Matrix34& out) const = 0;                // [30]  0xF0  618 bytes (fills Matrix34)                   tentative
    virtual void SetWorldTM(const Matrix34& tm, int flags) = 0;      // [31]  0xF8  204 bytes                                    tentative
    virtual Matrix34* GetWorldTMPtr() = 0;                           // [32]  0x100 lea rax,[this+0x58]                          VERIFIED worldTM@0x58
    virtual void _vf33() = 0;                                        // [33]  0x108
    virtual void GetWorldBounds(AABB& bbox) const = 0;               // [34]  0x110  VERIFIED via CFlowNode_EntityGetBounds::ProcessEvent (sub_18361B6E0, "World" branch, offset 272)
    virtual void GetLocalBounds(AABB& bbox) const = 0;               // [35]  0x118  VERIFIED via same node's "Local" branch, offset 280
    virtual void _vf36() = 0;                                        // [36]  0x120  197 bytes
    virtual void _vf37() = 0;                                        // [37]  0x128
    virtual void _vf38() = 0;                                        // [38]  0x130
    virtual void SetLocalPos(const Vec3& pos, int flags) = 0;        // [39]  0x138  lea r11,[this+0x30]; writes localPos         tentative
    virtual Vec3* GetLocalPosPtr() = 0;                             // [40]  0x140 lea rax,[this+0x30]                          VERIFIED localPos@0x30
    virtual void SetLocalRot(const Quat& rot, int flags) = 0;        // [41]  0x148  writes localRot@0x3C                         tentative
    virtual Quat* GetLocalRotPtr() = 0;                             // [42]  0x150 lea rax,[this+0x3C]                          VERIFIED localRot@0x3C
    virtual void SetLocalScale(const Vec3& scale, int flags) = 0;    // [43]  0x158  writes localScale@0x4C                       tentative
    virtual Vec3* GetLocalScalePtr() = 0;                           // [44]  0x160 lea rax,[this+0x4C]                          VERIFIED localScale@0x4C
    virtual void _vf45() = 0;                                        // [45]  0x168  292 bytes (SetPosRotScale?)
    virtual void GetWorldPos(Vec3& out) const = 0;                   // [46]  0x170 reads 0x64/0x74/0x84 into out                VERIFIED worldPos@0x64
    virtual void _vf47() = 0;                                        // [47]  0x178  GetWorldAngles? (reads float consts)
    virtual void _vf48() = 0;                                        // [48]  0x180  GetWorldRotation? (reads float consts)
    virtual void GetForwardDir(Vec3& out) const = 0;                 // [49]  0x188  test bit0x400000@0x08 then reads fwd@0x88     tentative
    virtual void _vf50() = 0;                                        // [50]  0x190
    virtual void _vf51() = 0;                                        // [51]  0x198  reads dword@0x08 (mask 0x3C000)
    virtual void _vf52() = 0;                                        // [52]  0x1A0
    virtual bool _vf53() const = 0;                                  // [53]  0x1A8  return *(uint8*)(this+0x08) & 1              bitfield@+0x08 bit0
    virtual uint32_t _vf54() const = 0;                              // [54]  0x1B0  (dword@0x08 >> 0x15) & 1
    virtual void _vf55() = 0;                                        // [55]  0x1B8
    virtual void _vf56() = 0;                                        // [56]  0x1C0
    virtual void _vf57() = 0;                                        // [57]  0x1C8  4644 bytes (Serialize?)
    virtual void _vf58() = 0;                                        // [58]  0x1D0
    virtual void _vf59() = 0;                                        // [59]  0x1D8
    virtual void _vf60() = 0;                                        // [60]  0x1E0  1147 bytes
    virtual void _vf61() = 0;                                        // [61]  0x1E8
    virtual void _vf62() = 0;                                        // [62]  0x1F0  reads dword@0x0C (id) + arg (SetTimer/KillTimer?)
    virtual void _vf63() = 0;                                        // [63]  0x1F8
    virtual uint32_t _vf64() const = 0;                              // [64]  0x200  (dword@0x08 >> 4) & 1
    virtual void _vf65() = 0;                                        // [65]  0x208
    virtual uint32_t _vf66() const = 0;                              // [66]  0x210  (dword@0x08 >> 0x12) & 1
    virtual IEntityProxy* GetProxy(int proxyType) const = 0;         // [67]  0x218  60 bytes (KCD1 GetProxy slot)                tentative
    virtual bool HasAI() const = 0;                                  // [68]  0x220  return *(uint32*)(this+0x94)!=0              VERIFIED aiObjectID@0x94
    virtual uint32_t GetAIObjectID() const = 0;                      // [69]  0x228  return *(uint32*)(this+0x94)                VERIFIED
    virtual void SetAIObjectID(uint32_t id) = 0;                     // [70]  0x230  *(uint32*)(this+0x94)=id                    VERIFIED
    virtual void _vf71() = 0;                                        // [71]  0x238
    virtual void _vf72() = 0;                                        // [72]  0x240  writes bitfield@0x08 (mask 0x3C000)
    virtual uint32_t _vf73() const = 0;                              // [73]  0x248  (dword@0x08 >> 0x0E) & 0xF
    virtual void _vf74() = 0;                                        // [74]  0x250  reads ptr@0xA8 (event/link list)
    virtual void _vf75() = 0;                                        // [75]  0x258
    virtual void _vf76() = 0;                                        // [76]  0x260  1016 bytes
    virtual void _vf77() = 0;                                        // [77]  0x268
    virtual void _vf78() = 0;                                        // [78]  0x270
    virtual void _vf79() = 0;                                        // [79]  0x278
    virtual void _vf80() = 0;                                        // [80]  0x280
    virtual void _vf81() = 0;                                        // [81]  0x288
    virtual void _vf82() = 0;                                        // [82]  0x290
    virtual void _vf83() = 0;                                        // [83]  0x298
    virtual void _vf84() = 0;                                        // [84]  0x2A0
    virtual void* _vf85() const = 0;                                 // [85]  0x2A8  return *(void**)(this+0xA0)                 ptr@0xA0
    virtual void _vf86() = 0;                                        // [86]  0x2B0
    virtual void _vf87() = 0;                                        // [87]  0x2B8
    virtual void _vf88() = 0;                                        // [88]  0x2C0
    virtual void _vf89() = 0;                                        // [89]  0x2C8
    virtual void _vf90() = 0;                                        // [90]  0x2D0
    virtual void _vf91() = 0;                                        // [91]  0x2D8
    virtual void _vf92() = 0;                                        // [92]  0x2E0
    virtual void _vf93() = 0;                                        // [93]  0x2E8
    virtual void _vf94() = 0;                                        // [94]  0x2F0
    virtual void _vf95() = 0;                                        // [95]  0x2F8
    virtual void _vf96() = 0;                                        // [96]  0x300
    virtual void _vf97() = 0;                                        // [97]  0x308
    virtual void _vf98() = 0;                                        // [98]  0x310
    virtual void _vf99() = 0;                                        // [99]  0x318
    virtual bool _vf100() = 0;                                       // [100] 0x320  return false (stub)
    virtual void _vf101() = 0;                                       // [101] 0x328
    virtual void _vf102() = 0;                                       // [102] 0x330
    virtual void _vf103() = 0;                                       // [103] 0x338
    virtual void _vf104() = 0;                                       // [104] 0x340
    virtual void _vf105() = 0;                                       // [105] 0x348
    virtual void _vf106() = 0;                                       // [106] 0x350
    virtual void _vf107() = 0;                                       // [107] 0x358
    virtual void _vf108() = 0;                                       // [108] 0x360
    virtual void _vf109() = 0;                                       // [109] 0x368
    virtual void _vf110() = 0;                                       // [110] 0x370
    virtual void _vf111() = 0;                                       // [111] 0x378
    virtual void _vf112() = 0;                                       // [112] 0x380
    virtual void _vf113() = 0;                                       // [113] 0x388  jmp sub_180AB45D4 (thunk)
    virtual void _vf114() = 0;                                       // [114] 0x390  1443 bytes
    virtual void _vf115() = 0;                                       // [115] 0x398
    virtual void _vf116() = 0;                                       // [116] 0x3A0
    virtual void* _vf117() const = 0;                                // [117] 0x3A8  return *(void**)(this+0xD0)                 ptr@0xD0
    virtual void _vf118() = 0;                                       // [118] 0x3B0
    virtual void _vf119() = 0;                                       // [119] 0x3B8
    virtual void _vf120() = 0;                                       // [120] 0x3C0
    virtual void _vf121() = 0;                                       // [121] 0x3C8
    virtual void _vf122() = 0;                                       // [122] 0x3D0
    virtual void _vf123() = 0;                                       // [123] 0x3D8
    virtual void _vf124() = 0;                                       // [124] 0x3E0
    virtual void _vf125() = 0;                                       // [125] 0x3E8
    virtual uint32_t _vf126() const = 0;                             // [126] 0x3F0  (dword@0x08 >> 3) & 1
    virtual void _vf127() = 0;                                       // [127] 0x3F8
    virtual void _vf128() = 0;                                       // [128] 0x400
    virtual void IncKeepAliveCounter() = 0;                          // [129] 0x408  inc word[this+0xF0]                         VERIFIED keepAlive@0xF0
    virtual void DecKeepAliveCounter() = 0;                          // [130] 0x410  add word[this+0xF0],0xFFFF (-1)             VERIFIED
    virtual void ResetKeepAliveCounter() = 0;                        // [131] 0x418  *(uint16*)(this+0xF0)=0                     VERIFIED
    virtual bool IsKeptAlive() const = 0;                            // [132] 0x420  return *(uint16*)(this+0xF0)!=0            VERIFIED
    virtual void _vf133() = 0;                                       // [133] 0x428  return (stub)
    virtual void _vf134() = 0;                                       // [134] 0x430
    virtual void _vf135(uint16_t) = 0;                               // [135] 0x438  *(uint16*)(this+0xF4)=arg                   word@0xF4
    virtual uint16_t _vf136() const = 0;                             // [136] 0x440  return *(uint16*)(this+0xF4)
    virtual bool _vf137() const = 0;                                 // [137] 0x448  test dword[this+0x18],0x8000 (NO_SAVE)
    virtual void SetExportedToGame(bool) = 0;                        // [138] 0x450  lea "bExportedToGame"; -> flag setter       string-tagged
    virtual void SetExportedToTest(bool) = 0;                        // [139] 0x458  lea "bExportedToTest"; -> flag setter       string-tagged
    virtual void _vf140() = 0;                                       // [140] 0x460  reads dword@0x08 (mask 0x3C01)
    virtual uint32_t _vf141() const = 0;                             // [141] 0x468  (dword@0x1C >> 0x12) & 1                    flagsExtended bit
    virtual void _vf142() = 0;                                       // [142] 0x470  btr bit0x12 of dword@0x1C                    flagsExtended bit
    virtual void _vf143() = 0;                                       // [143] 0x478
    virtual void _vf144() = 0;                                       // [144] 0x480
    virtual void _vf145() = 0;                                       // [145] 0x488
    virtual void _vf146() = 0;                                       // [146] 0x490
    virtual void _vf147() = 0;                                       // [147] 0x498
    virtual void _vf148() = 0;                                       // [148] 0x4A0
    virtual void _vf149() = 0;                                       // [149] 0x4A8
    virtual void _vf150() = 0;                                       // [150] 0x4B0
    virtual void* _vf151() = 0;                                      // [151] 0x4B8  return this (mov rax,rcx)
    virtual void _vf152() = 0;                                       // [152] 0x4C0
    virtual void _vf153() = 0;                                       // [153] 0x4C8  btr dword[this+0x08],0x13 (clear bitfield)
    virtual void _vf154() = 0;                                       // [154] 0x4D0
    virtual void _vf155() = 0;                                       // [155] 0x4D8
    virtual uint32_t _vf156(uint32_t) = 0;                           // [156] 0x4E0  return arg (identity)
    virtual void _vf157() = 0;                                       // [157] 0x4E8  reads ptr@0xD0
};

}  // namespace Offsets
