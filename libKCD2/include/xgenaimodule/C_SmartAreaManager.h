#pragma once
#include <cstdint>
#include "I_SmartAreaManager.h"
#include "../crysystem/IEntitySystemSink.h"
#include "I_DebugDraw.h"
#include "../framework/I_WUIDMappingProvider.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartAreaManager : I_SmartAreaManager (@+0x0, PRIMARY --
// note the base order differs from C_SmartObjectsManager) + IEntitySystemSink
// (@+0x8, CryEngine) + I_DebugDraw (@+0x10) + framework::I_WUIDMappingProvider
// (@+0x18) -- smart-area manager SINGLETON at qword_185493BB0 (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x460 (create-site proven: getter sub_1807D24D8
// @0x1807D2501: qword_18549D378(1120,&v6), += 0x460, ctor sub_180BEB26C).
// dtor sub_1832C1C80 (unregisters the sink via vtbl+240).
// -----------------------------------------------
// RTTI TD rva 0x4F51A08; vtables 0x3FE9900 (+0x0, 19) / 0x3FE99A0 (+0x8, 8,
// interfuscator-shuffled -- sink pures left un-overridden, ABSTRACT MIRROR) /
// 0x3FE98D0 (+0x10, 2) / 0x3FE98E8 (+0x18, 2).  ctor registers the sink
// subobject: (*(pEntitySystem+232))(pEntitySystem, this+8, 20); registers cvar
// wh_ai_LandSmartAreaTemplateName (default "sa_land").  Interior sub-object
// regions typed as blobs [U interior].

namespace wh::xgenaimodule {

class C_SmartAreaManager
    : public I_SmartAreaManager,
      public IEntitySystemSink,
      public I_DebugDraw,
      public framework::I_WUIDMappingProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartAreaManager;
    // I_SmartAreaManager impls (19) [U roles]
    void SamVf0() override;  void SamVf1() override;  void SamVf2() override;
    void SamVf3() override;  void SamVf4() override;  void SamVf5() override;
    void SamVf6() override;  void SamVf7() override;  void SamVf8() override;
    void SamVf9() override;  void SamVf10() override; void SamVf11() override;
    void SamVf12() override; void SamVf13() override; void SamVf14() override;
    void SamVf15() override; void SamVf16() override; void SamVf17() override;
    void SamVf18() override;
    // I_DebugDraw impl
    void DebugDraw() override;   // [1] nullsub
    // framework::I_WUIDMappingProvider impls
    void* GetValueForWuid(const void* wuid) override;          // [0] 0x32C3DAC
    void  GetWuidForKey(void* out, const void* key) override;  // [1] 0x32C3D64

    float    m_scale20;         // +0x20  ctor: 1.0f [U role]
    uint8_t  _pad24[4];         // +0x24
    uint8_t  m_vec28[0x10];     // +0x28..+0x38  container (sub_1803E5334; 0x10 span) [U type]
    uint8_t  m_hash38[0x28];    // +0x38..+0x60  hash-map (mask 7 @+0x50, count 8 @+0x58) [U interior]
    uint8_t  m_sub60[0x40];     // +0x60..+0xA0  sub-object (sub_180BEA890) [U interior]
    uint8_t  m_subA0[0x40];     // +0xA0..+0xE0  sub-object (sub_1806030E4) [U interior]
    uint8_t  m_byteE0;          // +0xE0  ctor: 0 [U role]
    uint8_t  _padE1[7];         // +0xE1
    uint8_t  m_subE8[0x348];    // +0xE8..+0x430  big sub-object (sub_180EC4B30) [U interior]
    uint64_t m_430;             // +0x430  ctor: 0
    void*    m_defaultRep438;   // +0x438  head of a DynArray-family length-prefixed buffer (16-byte elems); dtor 0x1824B8214 frees size=count<<4, count@ptr-8; shared-empty sentinel &unk_185602E98; element type [U]
    int16_t  m_id440;           // +0x440  ctor: -1
    int16_t  m_id442;           // +0x442  ctor: -1
    uint8_t  _pad444[4];        // +0x444
    std::vector<I_SmartAreasManagerListener*> m_listeners;  // +0x448..+0x460  {first@+0x448,last@+0x450,end@+0x458}; AddListener (I_SmartAreaManager slot16 / vtbl+128) push_back 0x180AF27E0; sorted insert/erase 0x1832C42EC; dtor frees 0x1832C1D14
};
static_assert(sizeof(C_SmartAreaManager) == 0x460, "C_SmartAreaManager must be 0x460 (alloc 1120 at sub_1807D24D8)");

}  // namespace wh::xgenaimodule
