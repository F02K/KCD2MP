#pragma once
#include <cstdint>
#include <vector>
#include "I_PositioningManager.h"
#include "../crysystem/IEntitySystemSink.h"

// -----------------------------------------------
// wh::xgenaimodule::C_PositioningManager : I_PositioningManager (@+0x0) +
// IEntitySystemSink (@+0x8, CryEngine) -- NPC positioning manager SINGLETON at
// qword_185493970 (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x190 (getter
// sub_1805FF798 @0x1805ff7ae: sub_1804F75C0(400) -> ctor sub_1805FF7D0).
// -----------------------------------------------
// RTTI TD rva 0x4F4E138.  Vtables: +0x0 rva 0x3FE8858 (15 slots); +0x8 rva
// 0x3FE8810 (8 slots, interfuscator-shuffled -- sink pures left un-overridden,
// ABSTRACT MIRROR per project convention).  ctor registers the sink subobject:
// (*(pEntitySystem+232))(pEntitySystem, this+8, 14); dtor sub_1832B6B10
// unregisters via vtbl+240.  Interior sub-object regions typed as blobs [U].

namespace wh::xgenaimodule {

class C_PositioningManager
    : public I_PositioningManager,
      public IEntitySystemSink {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PositioningManager;
    ~C_PositioningManager() override;   // [0] dtor sub_180B443E0
    void PosVf1() override;  void PosVf2() override;  void PosVf3() override;
    void PosVf4() override;  void PosVf5() override;  void PosVf6() override;
    void PosVf7() override;  void PosVf8() override;  void PosVf9() override;
    void PosVf10() override; void PosVf11() override; void PosVf12() override;
    void PosVf13() override; void PosVf14() override;   // [1..14] impls [U roles]

    std::vector<void*> m_vec10;        // +0x10  zeroed vector [U elem type]
    uint64_t           m_28;           // +0x28  ctor: 0 [U role]
    std::vector<void*> m_areas30;      // +0x30  vector<Area*>; elem = {CryStringT<char> name; std::vector<16B>}; dtor frees (last-first)&~7
    void*              m_listHead48;   // +0x48  head of WH intrusive circular list (16B node {_Next=&head,_Prev=0}); insert sub_18093DE20, dtor sub_1832B69B4
    void**             m_shapeIdxBuckets50; // +0x50  WH open-addr hash index of S_RequestedShapeInfo* (bucket = buckets[i & (cap-1)])
    uint64_t           m_shapeIdxCap58;     // +0x58  bucket count, power-of-2 (mask = cap-1)
    uint64_t           m_shapeIdxHead60;    // +0x60  first slot index (iteration base)
    uint64_t           m_shapeIdxCount68;   // +0x68  live entry count (end = head + count)
    uint8_t            m_sub70[0x78];  // +0x70..+0xE8  sub-object (sub_1805FF8C0) [U interior]
    uint8_t            m_subE8[0x70];  // +0xE8..+0x158  sub-object (sub_1805FFA10) [U interior]
    int32_t            m_158;          // +0x158  ctor: 0 [U role]
    uint8_t            _pad15C[4];     // +0x15C
    uint8_t            m_sub160[0x28]; // +0x160..+0x188  sub-object (sub_1805FFB54) [U interior]
    C_PositioningManager* m_self;      // +0x188  ctor: this
};
static_assert(sizeof(C_PositioningManager) == 0x190, "C_PositioningManager must be 0x190 (alloc 400 at sub_1805FF798)");

}  // namespace wh::xgenaimodule
