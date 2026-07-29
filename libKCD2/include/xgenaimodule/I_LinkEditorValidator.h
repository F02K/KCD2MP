#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_LinkEditorValidator -- link editor validation interface;
// base of the stateless C_LinkEditorValidator singleton (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_LinkEditorValidator@xgenaimodule@wh@@.  Shape (4 slots)
// read from the sole implementor C_LinkEditorValidator (vtable rva 0x475CC68:
// [0] dtor sub_1832F3440, [1]sub_1832F373C [2]sub_1832F36B8 [3]sub_1832F35BC
// -- Validate* roles NOT confirmed [U]).

namespace wh::xgenaimodule {

class I_LinkEditorValidator {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_LinkEditorValidator;
    virtual ~I_LinkEditorValidator();  // [0]
    virtual void LevVf1() = 0;   // [1] [U role]
    virtual void LevVf2() = 0;   // [2] [U role]
    virtual void LevVf3() = 0;   // [3] [U role]
};
static_assert(sizeof(I_LinkEditorValidator) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
