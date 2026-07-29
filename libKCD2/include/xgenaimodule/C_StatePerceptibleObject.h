#pragma once
#include <cstdint>
#include "C_PerceptibleObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_StatePerceptibleObject : C_PerceptibleObject -- perceivable
// record with per-state tracking (the "perceived states" updated by the perception
// jobs).  KCD2 WHGame.dll 1.5.6, kd7u.  sizeof 0x40 (create-site proven:
// qword_18549D378(64) in the factory sub_1803EB888 if(a3)-branch @0x1803eb8d7).
// -----------------------------------------------
// RTTI TD rva 0x4F4C628; COL 0x1840DFCD0; vtable 0x183A33320, 8 slots (adds
// [6],[7]): [0] sub_1803ECAB4 dtor, [1] sub_18066CD10 (inherited value),
// [2] sub_180ACA504, [3] sub_1804800C8, [4] sub_181A749E0, [5] sub_1803EBD88,
// [6] sub_1832AFDE0, [7] sub_1832AF5EC ([6]/[7] = Serialize write/read by
// analogy with the proven C_PerceptorObject pair [U exact]).
// ctor sub_1803EBC2C (@0x1803ebc60: vtable; +48 = sub_180472128(); +56 = 0);
// dtor sub_1803ECAF8 (cleans +48 via sub_180472A5C/sub_181AB5160, then base).

namespace wh::xgenaimodule {

class C_StatePerceptibleObject : public C_PerceptibleObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StatePerceptibleObject;
    ~C_StatePerceptibleObject() override;   // [0] sub_1803ECAB4
    void _vf2() override;                   // [2] sub_180ACA504 [U role]
    void _vf3() override;                   // [3] sub_1804800C8 [U role]
    void _vf4() override;                   // [4] sub_181A749E0 [U role]
    void _vf5() override;                   // [5] sub_1803EBD88 [U role]
    virtual void _vf6();                    // [6] sub_1832AFDE0 -- Serialize-write (analogy) [U]
    virtual void _vf7();                    // [7] sub_1832AF5EC -- Serialize-read (analogy) [U]

    uint64_t m_stateHandle;   // +0x30  = sub_180472128() (ctor @0x1803ebc70); cleaned by dtor via sub_180472A5C
    uint64_t _unk38;          // +0x38  ctor: 0 [U role]
};
static_assert(sizeof(C_StatePerceptibleObject) == 0x40, "C_StatePerceptibleObject must be 0x40 (alloc 64)");

}  // namespace wh::xgenaimodule
