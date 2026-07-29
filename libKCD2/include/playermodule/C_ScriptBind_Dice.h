#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_Dice -- Lua "Dice" table (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ScriptBind_Dice@playermodule@wh@@ (TD 0x184E11FB0, COL 0x41188A0)
// vtable 0x183ACEDA0  ctor sub_180EF7508  registration sub_1817DD16C.
// alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184 @0x180EF7249); owner @+0x60.
//
// -- Lua methods (name(argSig) -> handler) --
//   RollDie(...)                                    0x182ECF2C8  [argSig lives in shared helper
//   ToggleHoldDie(...)                              0x182ED0464   sub_1817DD344; not extracted]
//   HoldDie(userId, dieEntityId, dieNumber, hold)   0x182ECE174
//   OverrideNextThrow(playerIndex, dieValues)       0x182ECEAE0
//   SetAdvantage(playerIndex, advantage)            0x180C820A0
//   SetAIDifficulty(difficulty)                     0x182ECF500
//   SetAIRiskTaking(riskTaking)                     0x182ECF5C8
//   SetScore(player1Score, player2Score)            0x182ECF8C8

namespace wh::playermodule {

class C_ScriptBind_Dice : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_Dice;
    void* m_pOwner;   // +0x60  ctor sub_180EF7508 stores a2 = owning bind-holder (C_PlayerModule::m_sub18,
                      //        0xB0 aggregate at C_PlayerModule+0x18, ctor sub_180EF7184). No RTTI on holder
                      //        so void* is retained -- NOT C_PlayerModule* (that is 0x18 higher).
};
static_assert(sizeof(C_ScriptBind_Dice) == 0x68, "C_ScriptBind_Dice must be 0x68");

}  // namespace wh::playermodule
