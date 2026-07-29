#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::playermodule::C_ScriptBindMinigame -- Lua "Minigame" table (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVC_ScriptBindMinigame@playermodule@wh@@ (TD 0x184E121B8, COL 0x41188F0)
// vtable 0x183ACED40  ctor sub_180EF7484  registration sub_181795808.
// alloc 104 (0x68 == sizeof, bootstrap sub_180EF7184 @0x180EF7228); owner @+0x60.
// The StartX handlers are VIRTUALS on this class (registration stores an ICF-folded
// `call vtbl[slot]` thunk; slot -> real handler resolved from vtable 0x183ACED40).
//
// -- Lua methods (name(argSig) -> handler; [N] = own-vtable slot) --
//   StartHerbGathering(areaId)                 [4]  0x182ED0244
//   StartLockPicking(entityId)                 [5]  0x1819C9A5C
//   StartHoleDigging(entityId)                 [6]  0x182ED02A8
//   StartDice(playerId,tableId,opponentId)     [7]  0x182ECFCF8
//   StartDiceWithScore(targetScore)            [8]  0x182ED01D4
//   StartDiceByName(entityName)                [9]  0x182ED005C
//   WasBookOpened(documentGuidStr)                  0x182ED07EC
//   GetBookStudyProgress(documentGuidStr)           0x182ECD5A4
//   CanStartReadingMinigame(documentEntityId)       0x182ECCA88
//   CanUseMinigame(playerId)                   [10] 0x1809379C8
// -- Lua globals (SetGlobalValue 0x1823DBFA8) --
//   E_MinigameType_Undefined=0, Sharpening=1, Reading=2, Alchemy=3, HerbGathering=4,
//   LockPicking=5, HoleDigging=6, Dice=7, Pickpocketing=8, StoneThrowing=9,
//   BattleArchery=10, Distract=11, Blacksmithing=12, ForgeBuilder=13

namespace wh::playermodule {

class C_ScriptBindMinigame : public CScriptableBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindMinigame;
    void* m_pOwner;   // +0x60  ctor sub_180EF7484 stores a2 = owning bind-holder (C_PlayerModule::m_sub18,
                      //        0xB0 aggregate at C_PlayerModule+0x18, ctor sub_180EF7184). Holder has no
                      //        vtable/RTTI so void* is retained -- NOT C_PlayerModule* (that is 0x18 higher).
};
static_assert(sizeof(C_ScriptBindMinigame) == 0x68, "C_ScriptBindMinigame must be 0x68");

}  // namespace wh::playermodule
