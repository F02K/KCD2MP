#pragma once
#include "I_SpatialQueries.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SpatialQueries : I_SpatialQueries -- stateless spatial
// query service SINGLETON at qword_1854927C0 (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08 (getter sub_180FF4EE4: operator new(8), DCLP via _Mtx
// unk_18491DFE0).  Only a vptr.
// -----------------------------------------------
// RTTI TD rva 0x4EC2AD0, COL 0x44FAEE0.  Vtable rva 0x3FCC510 (5 slots:
// [0]sub_1831A27F4 [1]sub_1808AEDBC [2]sub_1808AF1F8 [3]sub_183420B70
// [4]sub_1808AECC8; [1]/[2] forward into the grid manager qword_185496248+112).

namespace wh::xgenaimodule {

class C_SpatialQueries : public I_SpatialQueries {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SpatialQueries;
    void SqVf0() override;   // [0] sub_1831A27F4
    void SqVf1() override;   // [1] sub_1808AEDBC (grid query by 4 int params)
    void SqVf2() override;   // [2] sub_1808AF1F8 (grid query by position)
    void SqVf3() override;   // [3] sub_183420B70
    void SqVf4() override;   // [4] sub_1808AECC8
};
static_assert(sizeof(C_SpatialQueries) == 0x08, "stateless: vptr only (new 8)");

}  // namespace wh::xgenaimodule
