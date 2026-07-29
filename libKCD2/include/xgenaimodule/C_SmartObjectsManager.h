#pragma once
#include <cstdint>
#include "../crysystem/IEntitySystemSink.h"
#include "I_SmartObjectsManager.h"
#include "I_DebugDraw.h"
#include "../framework/I_WUIDMappingProvider.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartObjectsManager : IEntitySystemSink (@+0x0, CryEngine)
// + I_SmartObjectsManager (@+0x8) + I_DebugDraw (@+0x10) +
// framework::I_WUIDMappingProvider (@+0x18) -- smart-objects manager SINGLETON
// at qword_185493D30 (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x5C0 (create-site
// proven: getter sub_1807D2540 @0x1807D2569: qword_18549D378(1472,&v6),
// _InterlockedAdd(dword_1851DFB18, 0x5C0), ctor sub_180BEB428).
// -----------------------------------------------
// RTTI TD rva 0x4F5B380; vtables 0x3FECE70 (+0x0, 8 slots, interfuscator-
// shuffled CryEngine sink -- do NOT trust raw slot order; IEntitySystemSink
// pures left un-overridden here, ABSTRACT MIRROR per project convention) /
// 0x3FECDE0 (+0x8, 14) / 0x3FECE58 (+0x10, 2) / 0x3FECDC8 (+0x18, 2).
// ctor registers the sink with gEnv->pEntitySystem (vtbl+232 AddSink, flags
// 14, passing THIS -- sink is the primary base) and the two
// wh_ai_SmartObjectDatabase{Load,Save}ValidAnimations cvars.  Owns the
// C_SmartObjectHelpersManager (+0xB8, new 80) and C_GeneratedSmartObjectsManager
// (+0xC0, new 264, ctor sub_180AF2864).  Interior sub-object regions typed as
// blobs [U interior].

namespace wh::xgenaimodule {

class C_SmartObjectHelpersManager;
class C_GeneratedSmartObjectsManager;

class C_SmartObjectsManager
    : public IEntitySystemSink,
      public I_SmartObjectsManager,
      public I_DebugDraw,
      public framework::I_WUIDMappingProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartObjectsManager;
    // I_SmartObjectsManager impls (14) [U roles]
    void SomVf0() override;  void SomVf1() override;  void SomVf2() override;
    void SomVf3() override;  void SomVf4() override;  void SomVf5() override;
    void SomVf6() override;  void SomVf7() override;  void SomVf8() override;
    void SomVf9() override;  void SomVf10() override; void SomVf11() override;
    void SomVf12() override; void SomVf13() override;
    // I_DebugDraw impl
    void DebugDraw() override;   // [1] nullsub
    // framework::I_WUIDMappingProvider impls
    void* GetValueForWuid(const void* wuid) override;          // [0] 0x32EFF54
    void  GetWuidForKey(void* out, const void* key) override;  // [1] 0x32EFF0C

    void*    m_listHeadA;       // +0x20  std::list<T> _Myhead sentinel (self-ref 24B node: {_Next8,_Prev8,val8}); _Mysize @+0x28; standalone (no bucket vec); T (8B value) unknown -- node alloc sub_181AB55C0(sub_180699848(1)=24)
    uint64_t m_listSizeA;       // +0x28  ctor: 0
    std::unordered_map<uint64_t, void*> m_map30;  // +0x30..+0x70  MSVC _Hash (0x40): traits/EBO @+0x30, max_load_factor 1.0f @+0x34, _List _Myhead @+0x38 / _Mysize @+0x40, bucket vec @+0x48/+0x50/+0x58, _Mask 7 @+0x60, _Maxidx 8 @+0x68; node 32B = pair<K8,V8ptr>; find sub_1807E3D68 uses base this+0x30, FNV-1a hash over 8B key (sub_18208BC00). K exact type unknown (8B); V = ptr to polymorphic entry (GetWuidForKey calls value->vfn+24).
    uint8_t  m_sub70[0x40];     // +0x70..+0xB0  sub-object (sub_1806030C0) [U interior]
    uint64_t m_B0;              // +0xB0  ctor: 0
    C_SmartObjectHelpersManager*    m_pHelpers;    // +0xB8  owned (new 80)
    C_GeneratedSmartObjectsManager* m_pGenerated;  // +0xC0  owned (new 264, ctor sub_180AF2864)
    uint8_t  m_subC8[0x178];    // +0xC8..+0x240  big embedded sub-object (sub_181925494) [U interior]
    uint8_t  _unk240[0x20];     // +0x240..+0x260  zeroed (+0x258 byte) [U roles]
    uint8_t  m_sub260[0x348];   // +0x260..+0x5A8  embedded sub-object (sub_180EC4B30) [U interior]
    uint8_t  m_sub5A8[0x18];    // +0x5A8..+0x5C0  tail sub-object (sub_1819B0878) [U interior]
};
static_assert(sizeof(C_SmartObjectsManager) == 0x5C0, "C_SmartObjectsManager must be 0x5C0 (alloc 1472 at sub_1807D2540)");

}  // namespace wh::xgenaimodule
