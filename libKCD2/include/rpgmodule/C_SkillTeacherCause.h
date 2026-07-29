#pragma once
#include "C_Cause.h"
#include "S_SkillTeacherData.h"

// -----------------------------------------------
// wh::rpgmodule::C_SkillTeacherCause -- "SkillTeacher" event cause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x18 (PROVEN: create fn sub_182D1918C tracked-alloc 24).  RTTI TD rva 0x4D715D0,
// vtable rva 0x3F45540 (4 slots): [0] dtor rva 0x14F4A20 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x1549F98 -> visitor+72, [3] GetName rva 0x2CAD8DC -> "SkillTeacher"
// (verified).  Factory caller sub_182D29D74.

namespace wh::rpgmodule {

class C_SkillTeacherCause : public C_Cause<S_SkillTeacherData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SkillTeacherCause;
};
static_assert(sizeof(C_SkillTeacherCause) == 0x18, "C_SkillTeacherCause must be 0x18 (alloc 24 at sub_182D1918C)");

}  // namespace wh::rpgmodule
