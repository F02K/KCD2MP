#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::C_UIFlashDynTextureHelper -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x28.
// -----------------------------------------------
// RTTI .?AVC_UIFlashDynTextureHelper@guimodule@wh@@ (TD 0x184C9ECF0, COL 0x413A5E0,
// no bases).  vtable 0x183A810B8 (1 slot: [0] scalar deleting dtor 0x182B11488).
//
// Embedded BY VALUE in C_UIExtendedInfo (+0xB0) and C_UITreeList (+0xB8); both host
// ctors write only {vfptr, three zeroed qwords}.  Used by the dyn-texture bind slot
// (C_UIExtendedInfo vtable [14] sub_18166C088 -> sub_18073D68C(this, ...) creates the
// element/texture, then rebinds the host's flash element).  OPAQUE SHELL: member roles
// beyond the zero-init were not traced this wave (UNVERIFIED).

namespace wh::guimodule {

class C_UIFlashDynTextureHelper {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UIFlashDynTextureHelper;
    virtual ~C_UIFlashDynTextureHelper();   // [0] 0x182B11488

    uint64_t m_unk08;   // +0x08  ctor 0 [role UNVERIFIED]
    uint64_t m_unk10;   // +0x10  ctor 0 [role UNVERIFIED]
    uint64_t m_unk18;   // +0x18  ctor 0 [role UNVERIFIED]
    int32_t  m_instanceIndex;  // +0x20  flash instance index, written by sub_18073D68C (DWORD, loop index v16)
    uint8_t  _pad24[4];        // +0x24
};
static_assert(sizeof(C_UIFlashDynTextureHelper) == 0x28, "C_UIFlashDynTextureHelper must be 0x28");

}  // namespace wh::guimodule
