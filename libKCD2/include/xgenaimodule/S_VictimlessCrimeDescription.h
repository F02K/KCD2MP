#pragma once
#include "I_CrimeDescription.h"

// -----------------------------------------------
// wh::xgenaimodule::S_VictimlessCrimeDescription : I_CrimeDescription -- crime
// without a victim (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT proven -> no size
// assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F44350; vtable 0x183FE4058, 5 slots: [0] sub_180EA1C80,
// [1] = [2] sub_1808D2480 (shared body), [3] sub_18066CD10 (return 0),
// [4] sub_1823CA690.  ctor sub_1832A0084 (doubles as the factory branch for kind
// Victimless).

namespace wh::xgenaimodule {

class S_VictimlessCrimeDescription : public I_CrimeDescription {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_VictimlessCrimeDescription;
    ~S_VictimlessCrimeDescription() override;   // [0] deleting dtor sub_180EA1C80
    void _vf1() override;                       // [1] sub_1808D2480 (same body as [2]) [U role]
    void _vf2() override;                       // [2] sub_1808D2480 [U role]
    void _vf3() override;                       // [3] sub_18066CD10 -- return 0 [U role]
    void _vf4() override;                       // [4] sub_1823CA690 [U role]
};
// payload members unresolved -> no static_assert [U]

}  // namespace wh::xgenaimodule
