#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBind_ShootingContest -- Lua "ShootingContest" table (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// NEW in KCD2 (no KCD1 counterpart). RTTI .?AVC_ScriptBind_ShootingContest@playermodule@wh@@
// (TD 0x184E12030, COL 0x4118940)  vtable 0x183ACEDC8  ctor sub_180EF758C
// registration sub_181810E00.  alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184
// @0x180EF72EC); +0x60 ctor-zeroed (no owner passed).
//
// -- Lua methods (name(argSig) -> handler) --
//   ShowShootingContestTable(playerName, opponentName1, opponentName1, opponentName2,
//       playerArrows, opponentArrows1, opponentArrows2, isTargetVisible)     0x182ECFAE8
//       [duplicated "opponentName1" is verbatim from the binary argSig string]
//   HideShootingContestTable()                                               0x182ECE150
//   UpdateShootingContestTable(playerArrows, opponentArrows1, opponentArrows2,
//       playerPoints, opponentPoints1, opponentPoints2)                      0x182ED078C
//   ShowShootingHit(xPos, yPos)                                              0x182ECFBB8
//   ShowHorseShootingContestTable(playerName, opponentName, playerScore,
//       opponentScore, trackName)                                            0x182ECFA2C
//   HideHorseShootingContestTable()                                          0x182ECE0D0
//   UpdateHorseShootingContestTable(playerScore)                             0x182ED0764

namespace wh::playermodule {

class C_ScriptBind_ShootingContest : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBind_ShootingContest;
    void* m_pOwner;   // +0x60  ctor sub_180EF758C @0x180EF75B0 zeros it (single-arg ctor, no owner passed); shared-layout slot, always null for this bind
};
static_assert(sizeof(C_ScriptBind_ShootingContest) == 0x68, "C_ScriptBind_ShootingContest must be 0x68");

}  // namespace wh::playermodule
