#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SpatialQueries -- spatial-query service interface; base
// of the stateless C_SpatialQueries singleton (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_SpatialQueries@xgenaimodule@wh@@.  Shape (5 slots) read
// from the C_SpatialQueries vtable rva 0x3FCC510: [0]sub_1831A27F4
// [1]sub_1808AEDBC [2]sub_1808AF1F8 [3]sub_183420B70 [4]sub_1808AECC8.
// Slots [1]/[2] forward into the global AI/spatial grid manager
// qword_185496248 (+112): [1] query by 4 int params, [2] query by position.
// [0][3][4] NOT decompiled; no dtor claim [U].

namespace wh::xgenaimodule {

class I_SpatialQueries {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SpatialQueries;
    virtual void SqVf0() = 0;   // [0] [U role]
    virtual void SqVf1() = 0;   // [1] grid query by 4 int params [U signature]
    virtual void SqVf2() = 0;   // [2] grid query by position [U signature]
    virtual void SqVf3() = 0;   // [3] [U role]
    virtual void SqVf4() = 0;   // [4] [U role]
};
static_assert(sizeof(I_SpatialQueries) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
