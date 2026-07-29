#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::S_CreditsRole -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20, POLYMORPHIC.
// -----------------------------------------------
// Parsed "credit_role" Data element -- the ONLY polymorphic table Data in the guimodule
// DB set: vftable 0x183B2F220 is a SINGLE-slot vtable holding just the virtual deleting
// dtor sub_1811D8B0C (no base class observed; why Role alone carries a vtable is not
// established -- G4d §UNVERIFIED 5). Layout from ParseRows sub_1811D89E0 (writes vftable
// @+0x00 [evidence 0x1811D8A5E], int @+0x08, CryStringT @+0x10, int @+0x18, int @+0x1C;
// in-place ctor sub_1811D8B4C advances +0x20).

namespace wh::guimodule {

class S_CreditsRole {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_CreditsRole;
    virtual ~S_CreditsRole();       // [0] 0x1811D8B0C -- the vtable's only slot

    int32_t          credit_role_id;     // +0x08
    uint8_t          _pad0C[4];          // +0x0C
    CryStringT<char> credit_role_name;   // +0x10
    int32_t          order_by;           // +0x18
    int32_t          credit_layout_id;   // +0x1C
};
static_assert(sizeof(S_CreditsRole) == 0x20, "S_CreditsRole must be 0x20 (ParseRows sub_1811D89E0)");

}  // namespace wh::guimodule
