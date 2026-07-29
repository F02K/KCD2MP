#pragma once
#include <cstdint>
#include "../framework/C_PointManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DogPointManager : framework::C_PointManager<S_DogPoint>
// -- dog-point registry SINGLETON at qword_185496200 (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x260 (getter sub_18145C770 @0x18145c7c0:
// sub_18549D378(608,...), += 0x260, ctor sub_18145C81C).
// -----------------------------------------------
// RTTI TD rva 0x4F4C528; the real base is C_PointManager<S_DogPoint,...>
// (deriving framework::I_FrameworkLayerListener -- folded into the flat
// template model).  Vtable rva 0x3B8DC50 (6 slots: [0] dtor sub_18062425C,
// [1]sub_180623E70 [2]sub_1832AE908 [3]sub_180B44868 [4]nullsub
// [5]sub_1807D1BE4).  ctor registers console command
// WH_AI_DogPointManager_ReloadPoints (handler sub_1832AED6C) and cvar
// WH_AI_DogPointManager_DrawPoints bound to +0x198.  Container regions typed
// as blobs [U interior].

namespace wh::xgenaimodule {

struct S_DogPoint;   // layout [U] -- template parameter only

class C_DogPointManager : public framework::C_PointManager<S_DogPoint> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DogPointManager;
    ~C_DogPointManager() override;   // [0] sub_18062425C
    void PtVf1() override;           // [1] sub_180623E70 [U role]
    void PtVf2() override;           // [2] sub_1832AE908 [U role]
    void PtVf3() override;           // [3] sub_180B44868 [U role]
    void PtVf4() override;           // [4] nullsub [U role]
    void PtVf5() override;           // [5] sub_1807D1BE4 [U role]

    uint8_t m_cvarDrawPoints;     // +0x198  cvar WH_AI_DogPointManager_DrawPoints target
    uint8_t _pad199[7];           // +0x199
    uint8_t m_hashSet1A0[0x40];   // +0x1A0..+0x1E0  WH hash-set (float 1.0 @+0x1A4; mask 7 @+0x1D0, count 8 @+0x1D8) [U interior]
    uint8_t m_sub1E0[0x40];       // +0x1E0..+0x220  container (sub_1806030E4) [U interior]
    uint8_t m_sub220[0x40];       // +0x220..+0x260  container (sub_1806030E4) [U interior]
};
static_assert(sizeof(C_DogPointManager) == 0x260, "must be 0x260 (alloc 608 at sub_18145C770)");

}  // namespace wh::xgenaimodule
