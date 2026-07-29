#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::C_PointManager<T> -- point-registry template, modeled FLAT
// (KCD2 WHGame.dll 1.5.6, kd7u).  Base region 0x198 per the sole enumerated
// instantiation (<S_DogPoint, ...>, base of xgenaimodule::C_DogPointManager --
// derived members start at +0x198).
// -----------------------------------------------
// The real template derives framework::I_FrameworkLayerListener (single
// subobject @+0x0) -- FOLDED into this flat model [U].  Slot count (6) taken
// from the derived C_DogPointManager vtable rva 0x3B8DC50 ([0] dtor,
// [1]sub_180623E70 [2]sub_1832AE908 [3]sub_180B44868 [4]nullsub_1
// [5]sub_1807D1BE4; which slots are the OnLayer* listener callbacks [U]).
// Base init sub_18145C96C; interior fields NOT expanded [U] -- sized blob.
// Template => no RTTI constant.

namespace wh::framework {

template <class T>
class C_PointManager {
public:
    virtual ~C_PointManager();   // [0] deleting dtor (instantiation-specific)
    virtual void PtVf1();        // [1] [U role]
    virtual void PtVf2();        // [2] [U role]
    virtual void PtVf3();        // [3] [U role]
    virtual void PtVf4();        // [4] nullsub in C_DogPointManager [U role]
    virtual void PtVf5();        // [5] [U role]

    uint8_t m_state[0x190];   // +0x08..+0x198  point registry storage [U interior]
};
static_assert(sizeof(C_PointManager<int>) == 0x198,
              "C_PointManager instantiations must be 0x198");

}  // namespace wh::framework
