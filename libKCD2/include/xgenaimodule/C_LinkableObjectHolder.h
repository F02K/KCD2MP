#pragma once
#include "I_LinkableObjectHolder.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LinkableObjectHolder : I_LinkableObjectHolder -- concrete
// linkable-object holder (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof NOT create-site
// proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4F5CB50; vtable 0x183A651A8, 7 slots: [0] sub_18096D57C,
// [1] sub_181A72900, [2] = [3] sub_180947210 (shared body), [4] sub_18096E2B4,
// [5] sub_18096D63C, [6] nullsub_1.  Members unresolved.

namespace wh::xgenaimodule {

class C_LinkableObjectHolder : public I_LinkableObjectHolder {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LinkableObjectHolder;
    void _vf0() override;   // [0] sub_18096D57C [U role; dtor placement unverified]
    void _vf1() override;   // [1] sub_181A72900 [U role]
    void _vf2() override;   // [2] sub_180947210 (same body as [3]) [U role]
    void _vf3() override;   // [3] sub_180947210 [U role]
    void _vf4() override;   // [4] sub_18096E2B4 [U role]
    void _vf5() override;   // [5] sub_18096D63C [U role]
    void _vf6() override;   // [6] nullsub_1 [U role]
};
// members + sizeof unresolved -> no static_assert [U]

}  // namespace wh::xgenaimodule
