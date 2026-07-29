#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_Sharpening -- Lua "Sharpening" table (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ScriptBind_Sharpening@playermodule@wh@@ (TD 0x184E120D8, COL 0x4118918)
// vtable 0x183ACECA0  ctor sub_180EF7400  registration sub_180ECA5D0.
// alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184 @0x180EF7207); owner @+0x60.
// ALL 13 handlers are VIRTUALS on this class (registration stores ICF-folded `call vtbl[slot]`
// thunks shared with C_ScriptBindMinigame; slots resolved from vtable 0x183ACECA0).
//
// -- Lua methods (name(argSig) -> handler; [N] = own-vtable slot) --
//   Start(user,grindstoneId, itemId)  [4]  0x182ECFBE0
//   Stop(user)                        [5]  0x182ED03EC
//   IsActive(user)                    [6]  0x182ECE23C
//   SetGrindstone(user)               [7]  0x182ECF74C
//   DoStepOnGrindstone(user)          [8]  0x182ECD134
//   SetPressure(user,value)           [9]  0x182ECF828
//   SetPosition(user,value)           [10] 0x182ECF7D8
//   SetRotation(user,value)           [11] 0x182ECF878
//   SetOptimalRotation(user)          [12] 0x182ECF798
//   MoveToWorstZone(user)             [13] 0x182ECE7EC
//   GetTotalQuality(user)             [14] 0x182ECDEF0
//   GetWheelSpeed(user)               [15] 0x182ECDFD0
//   SetWheelSpeed(user,value)         [16] 0x182ECF9A4

namespace wh::playermodule {

class C_ScriptBind_Sharpening : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_Sharpening;
    void* m_pOwner;   // +0x60  = C_PlayerModule::m_sub18 (+0x18) bind holder (0xB0, ctor sub_180EF7184); ctor 0x180EF740C stores a2; holder has no RTTI/vtable -> concrete class unnamed
};
static_assert(sizeof(C_ScriptBind_Sharpening) == 0x68, "C_ScriptBind_Sharpening must be 0x68");

}  // namespace wh::playermodule
