#pragma once
#include <cstdint>
#include <vector>
#include "I_DebugDraw.h"
#include "../framework/I_ReadinessTask.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LODManager : I_DebugDraw (@+0x0, primary) +
// wh::I_ReadinessTask (@+0x8) -- AI LOD manager SINGLETON at qword_185496178
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x118 (getter sub_180B5136C
// @0x180b513bc: sub_18549D378(280,...), dword_1851DFB18 += 0x118, ctor
// sub_180B51408).  dtor sub_183320450.
// -----------------------------------------------
// RTTI TD rva 0x4F6E308.  Vtables: +0x0 rva 0x3FF1F38 ([0] dtor sub_183320D14,
// [1] nullsub = DebugDraw); +0x8 I_ReadinessTask rva 0x3FF1F50 (5 slots:
// [0][1] nullsub (I_ReadinessDebuggable defaults), [2]sub_18209C850
// [3]sub_18209C620 [4]sub_18209C630 -- roles [U]).  ctor registers a per-frame
// callback functor {sub_180D9C3A4, this} via sub_180B51B70.  WH-hash /
// container regions typed as blobs [U interior].

namespace wh::xgenaimodule {

class C_LODAgent;   // per-agent LOD record (see C_LODAgent.h)

class C_LODManager
    : public I_DebugDraw,
      public wh::I_ReadinessTask {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LODManager;
    ~C_LODManager() override;    // [0] deleting dtor sub_183320D14
    void DebugDraw() override;   // [1] nullsub
    void TaskVf2() override;     // readiness [2] sub_18209C850 [U role]
    void TaskVf3() override;     // readiness [3] sub_18209C620 [U role]
    void TaskVf4() override;     // readiness [4] sub_18209C630 [U role]

    uint8_t  m_flags10;        // +0x10  ctor: 0 [U role]
    uint8_t  _pad11[3];        // +0x11
    float    m_scale14;        // +0x14  ctor: 1.0f [U role]
    uint8_t  m_hashSet18[0x38]; // +0x18..+0x50  WH hash-set (node sub_181AB55C0(32); mask 7 @+0x40, count 8 @+0x48) [U interior]
    std::vector<C_LODAgent*> m_agents50; // +0x50  {first,last,end}; dtor free 0x18332057E, erase memmove 0x1805D2370, elem stride 8 = C_LODAgent*
    uint8_t  m_map68[0x40];    // +0x68..+0xA8  WH map/list (sub_18043C878) [U interior]
    uint64_t m_A8;             // +0xA8  ctor: 0 [U role]
    uint8_t  m_byteB0;         // +0xB0  ctor: 0 [U role]
    uint8_t  _padB1[7];        // +0xB1
    uint8_t  m_mapB8[0x40];    // +0xB8..+0xF8  WH map/list (sub_18043C878) [U interior]
    uint64_t m_F8;             // +0xF8  ctor: 0 [U role]
    int32_t  m_100;            // +0x100  ctor: 0 [U role]
    uint8_t  m_byte104;        // +0x104  ctor: 0 [U role]
    uint8_t  _pad105[3];       // +0x105
    float    m_pos108[2];      // +0x108  ctor: {-1000.0f, -1000.0f} [U role]
    float    m_110;            // +0x110  ctor: -1000.0f [U role]
    uint8_t  _pad114[4];       // +0x114
};
static_assert(sizeof(C_LODManager) == 0x118, "C_LODManager must be 0x118 (alloc 280 at sub_180B5136C)");

}  // namespace wh::xgenaimodule
