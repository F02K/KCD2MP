#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBindSkipTime -- Lua "SkipTime" table (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// NEW in KCD2 (no KCD1 counterpart). RTTI .?AVC_ScriptBindSkipTime@playermodule@wh@@
// (TD 0x184E11F28, COL 0x4118968)  vtable 0x183C25CD8  ctor sub_18181A7A0
// registration sub_18181A800 (helper sub_18181A97C holds the name).
// alloc 104 (0x68 == sizeof) -- created by the C_SkipTime global bootstrapper sub_18181A600
// (C_SkipTime static block @0x1854BFF50) and stored at qword_1854C00A8; +0x60 ctor-zeroed.
//
// -- Lua methods (name(argSig) -> handler) --
//   GetSkipTimeMessageUIString(messageType)   0x182ECDEA4

namespace wh::playermodule {

class C_ScriptBindSkipTime : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindSkipTime;
    void* m_pOwner;   // +0x60  ctor sub_18181A7A0 zeroes it; takes no owner arg (standalone C_SkipTime
                      //        bind stored at qword_1854C00A8). Vestigial slot from the shared bind layout.
};
static_assert(sizeof(C_ScriptBindSkipTime) == 0x68, "C_ScriptBindSkipTime must be 0x68");

}  // namespace wh::playermodule
