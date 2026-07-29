#pragma once
#include "I_LinkEditorValidator.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LinkEditorValidator : I_LinkEditorValidator -- stateless
// link-editor validation SINGLETON at qword_1855AA170 (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08 (getter sub_1831A810C @0x1831a8156:
// qword_18549D378(8,...), += 8; ctor inlined -- writes only the vptr).
// -----------------------------------------------
// RTTI TD rva 0x4EC0650.  Vtable rva 0x475CC68 (4 slots: [0] dtor
// sub_1832F3440, [1]sub_1832F373C [2]sub_1832F36B8 [3]sub_1832F35BC).

namespace wh::xgenaimodule {

class C_LinkEditorValidator : public I_LinkEditorValidator {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LinkEditorValidator;
    ~C_LinkEditorValidator() override;   // [0] sub_1832F3440
    void LevVf1() override;   // [1] sub_1832F373C [U role]
    void LevVf2() override;   // [2] sub_1832F36B8 [U role]
    void LevVf3() override;   // [3] sub_1832F35BC [U role]
};
static_assert(sizeof(C_LinkEditorValidator) == 0x08, "stateless: vptr only (alloc 8)");

}  // namespace wh::xgenaimodule
