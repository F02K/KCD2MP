#pragma once
#include <cstdint>
#include "C_WeaponEquipRuntimeData.h"

// -----------------------------------------------
// wh::entitymodule::C_WeaponRuntimeData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_WeaponRuntimeData@entitymodule@wh@@ (TD 0x184C6F078, COL 0x18432AAF8)
// vtable 0x183EC8470  ctor sub_180474450 (delegates to C_WeaponEquipRuntimeData).
// sizeof VERIFIED twice: pool sub_181F1D700 221184/1024 = 216 (stride 54 dwords) AND last member
// qword @+0xD0. The +0xC0..+0xC3 dword (with base pad +0xBC) is NOT written by the ctor --
// initialized later on the InitFromRequest path (string "RequiresRuntimeDataInitFromRequest"
// @0x183B80B28); its type is unknown.

namespace wh::entitymodule {

class C_WeaponRuntimeData : public C_WeaponEquipRuntimeData   // +0x00  (0xC0)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeaponRuntimeData;
    uint8_t  m_byteC0;   // +0xC0  byte-sized (clone copies 1 byte 0x180F19759); late-init on InitFromRequest; role unresolved
    uint8_t  _padC1[3];  // +0xC1
    int32_t  m_fieldC4;  // +0xC4  int32 scalar (ctor 0 @0x18047446E; clone dword-copy 0x180F19765); role unresolved
    void*    m_fieldC8;  // +0xC8  (ctor 0 @0x18047447F; NOT clone-copied -> transient/runtime cache ptr); pointee UNRESOLVED
    void*    m_fieldD0;  // +0xD0  owning ptr (unique_ptr-like: move-assign sub_181F16D70, reset sub_181F1E320 destructs+frees) to 0x38-byte obj (ctor sub_181E3DC50); pointee class UNNAMED
};
static_assert(sizeof(C_WeaponRuntimeData) == 0xD8, "C_WeaponRuntimeData must be 0xD8");

}  // namespace wh::entitymodule
