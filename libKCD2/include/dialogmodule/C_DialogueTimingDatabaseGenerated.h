#pragma once
#include "S_DialogueTiming.h"
#include "S_DialogueTimingDBData.h"
#include "C_DialogueTimingPreprocessor.h"
#include "../animationmodule/C_AnimationDatabaseGenerated.h"
#include "../databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "../databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueTimingDatabaseGenerated -- "dialogue_animation"
// generated animation table (KCD2 1.5.6, kd7u).  sizeof 0xE8 (anim-DB extent).
// -----------------------------------------------
// RTTI TD 0x184B53AB0; primary vtable 0x183C131B8 (24 slots) + I_AdbChangeListener MI
// vtable 0x183C131A0 (2 slots) @+0x98; registrar/ctor sub_181790B8C (base ctor
// sub_180DFA000(this,1)); deleting-dtor [0] sub_18280F834. Stack:
// C_ObjectDatabaseKeyIndexed<C_AnimationDatabaseGenerated<S_DialogueTiming,
// S_DialogueTimingDBData, C_DialogueTimingPreprocessor>, DefaultKeyExtractor>; adds NO
// own data -- configures base m_tableName@+0x50 = "dialogue_animation",
// m_groupName@+0x70 = "skald". Slots: [7] sub_1816066FC Register; [8] sub_182778B88
// Load-hook; [9] sub_180EF3A4C base Load; [10] sub_1809407F0 ParseRows; [16]
// sub_1819DFB24 RebuildIndices; [22] sub_182810050 OnAdbChanged; [23] sub_18066CD10
// OnAdbReloaded. Anim duration scaling via console cmd wh_dlg_SetAnimDurationScales.

namespace wh::dialogmodule {

class C_DialogueTimingDatabaseGenerated
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::animationmodule::C_AnimationDatabaseGenerated<
              S_DialogueTiming, S_DialogueTimingDBData, C_DialogueTimingPreprocessor>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<S_DialogueTiming>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueTimingDatabaseGenerated;
};
static_assert(sizeof(C_DialogueTimingDatabaseGenerated) == 0xE8,
              "C_DialogueTimingDatabaseGenerated must be 0xE8 (anim-DB extent)");

}  // namespace wh::dialogmodule
