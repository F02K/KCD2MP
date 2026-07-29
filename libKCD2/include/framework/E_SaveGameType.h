#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework::E_SaveGameType -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : int32.
// -----------------------------------------------
// Nested-Type enum wrapper (RTTR enumeration_wrapper<...E_SaveGameType::Type,10,0>).
// Enumerators CERTIFIED from the RTTR registration sub_180060BD0 — this replaces the
// earlier synthetic lowercase names (2 was "save", 6 was mislabeled "moduleMessage",
// 9 was mislabeled "none": it is the Last count sentinel).
// The discriminator is stored (as a byte) at C_SaveGameDescription+0x08, passed to
// C_SaveGameManager::CreateSaveGame (0x180794928), indexes the per-slot
// m_countsBySaveType[10] histogram; per-type filename templates in sub_181E21850.

namespace wh::framework {

struct E_SaveGameType {
    enum Type : int32_t {
        PermanentSave                  = 0,  // "permanent%03d.whs"
        AutoSave                       = 1,  // "autosave%03d.whs"
        ManualSave                     = 2,  // "save%03d.whs"
        QuickSave                      = 3,  // "quicksave%03d.whs"
        Obsolete_NewGame               = 4,
        ExitSave                       = 5,  // "exit.whs"
        LevelSwitchSave                = 6,  // checkpoint via queued module message (manager +0x210 branch)
        Obsolete_LevelSwitchNestedSave = 7,
        CrucialDecisionSave            = 8,  // "crucialdecision%03d.whs"
        Last                           = 9,  // count sentinel
    };
};

}  // namespace wh::framework
