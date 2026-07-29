#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::I_RPGDialog -- RPG dialog interface (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD present (.?AVI_RPGDialog@rpgmodule@wh@@).  The interface itself is NOT RE'd --
// this header exists to scope the nested payload struct S_SkillCheckData, which is the
// template argument of C_Cause<I_RPGDialog::S_SkillCheckData> (base of C_SkillCheckCause;
// confirmed via CHD base[1] TD `.?AV?$C_Cause@US_SkillCheckData@I_RPGDialog@rpgmodule@wh@@`).
// Do NOT instantiate/inherit: the real class is polymorphic; virtuals are unmodeled here [U].

namespace wh::rpgmodule {

class I_RPGDialog {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_RPGDialog;

    // Payload of C_SkillCheckCause.  sizeof 0x18 (PROVEN: create fn sub_182CD62B0
    // tracked-alloc 32, OWORD copy to cause+0x08 + QWORD to cause+0x18).
    struct S_SkillCheckData {
        uint8_t  _raw10[16];   // +0x00  16B POD (dialog/skill ids -- split UNVERIFIED)
        uint64_t m_handle;     // +0x10  handle/result [role UNVERIFIED]
    };
};
static_assert(sizeof(I_RPGDialog::S_SkillCheckData) == 0x18,
              "I_RPGDialog::S_SkillCheckData must be 0x18 (alloc 32 minus vptr)");

}  // namespace wh::rpgmodule
