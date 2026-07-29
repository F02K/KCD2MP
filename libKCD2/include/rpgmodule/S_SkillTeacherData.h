#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SkillTeacherData -- payload of C_SkillTeacherCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// sizeof 0x10 (PROVEN: create fn sub_182D1918C tracked-alloc 24; _OWORD @+0x08).  Factory
// caller sub_182D29D74 (site 0x182D29E4F).  16-byte POD; split UNVERIFIED -- raw blob.

namespace wh::rpgmodule {

struct S_SkillTeacherData {
    uint8_t _raw[16];   // +0x00  [U fields]
};
static_assert(sizeof(S_SkillTeacherData) == 0x10, "S_SkillTeacherData must be 0x10");

}  // namespace wh::rpgmodule
