#pragma once
#include "C_Cause.h"
#include "I_RPGDialog.h"

// -----------------------------------------------
// wh::rpgmodule::C_SkillCheckCause -- "SkillCheck" event cause (KCD2 WHGame.dll 1.5.6,
// kd7u).  SPECIAL: payload is the NESTED struct I_RPGDialog::S_SkillCheckData (not a
// scope-level S_SkillCheckData), so the base is C_Cause<I_RPGDialog::S_SkillCheckData>
// (confirmed via CHD base[1] TD `.?AV?$C_Cause@US_SkillCheckData@I_RPGDialog@...`).
// -----------------------------------------------
// sizeof 0x20 (PROVEN: create fn sub_182CD62B0 tracked-alloc 32).  RTTI TD rva 0x4D5C6D8,
// vtable rva 0x3F27748 (4 slots): [0] dtor rva 0x15A3D58 (ICF trivial), [1] nullsub,
// [2] Accept rva 0x2C8A49C -> visitor+216, [3] GetName rva 0x2C8AD28 -> "SkillCheck"
// (verified).  Factory caller sub_182CFBA04.

namespace wh::rpgmodule {

class C_SkillCheckCause : public C_Cause<I_RPGDialog::S_SkillCheckData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SkillCheckCause;
};
static_assert(sizeof(C_SkillCheckCause) == 0x20, "C_SkillCheckCause must be 0x20 (alloc 32 at sub_182CD62B0)");

}  // namespace wh::rpgmodule
