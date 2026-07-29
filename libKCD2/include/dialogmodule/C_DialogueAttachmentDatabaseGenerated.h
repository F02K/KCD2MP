#pragma once
#include "S_DialogueAttachment.h"
#include "S_DialogueAttachmentDBData.h"
#include "C_DialogueAttachmentPreprocessor.h"
#include "../animationmodule/C_AnimationDatabaseGenerated.h"
#include "../databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "../databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueAttachmentDatabaseGenerated -- "dialogue_attachment"
// generated animation table (KCD2 1.5.6, kd7u).  sizeof 0xE8 (anim-DB extent).
// -----------------------------------------------
// RTTI TD 0x184B4E9C0; primary vtable 0x183C6E980 (24 slots) + I_AdbChangeListener MI
// vtable 0x183C6E968 (2 slots) @+0x98 (inherited from the C_AnimationDatabaseBase
// chain); registrar/ctor sub_1819A34C0 (base ctor sub_180DF9ECC(this,1)); deleting-dtor
// [0] sub_1828077E8. Stack: C_ObjectDatabaseKeyIndexed<C_AnimationDatabaseGenerated<
// S_DialogueAttachment, S_DialogueAttachmentDBData, C_DialogueAttachmentPreprocessor>,
// DefaultKeyExtractor>; adds NO own data -- configures base m_tableName@+0x50 =
// "dialogue_attachment", m_groupName@+0x70 = "text". Slots: [7] sub_181606698 Register;
// [8] sub_182778B88 Load-hook; [9] sub_180EF3A4C base Load; [10] sub_180C1EF9C
// ParseRows; [16] sub_1819DFB64 RebuildIndices; [22] sub_182807974 OnAdbChanged;
// [23] sub_18066CD10 OnAdbReloaded. Hot-reloaded by wh_dlg_ReloadDB.

namespace wh::dialogmodule {

class C_DialogueAttachmentDatabaseGenerated
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::animationmodule::C_AnimationDatabaseGenerated<
              S_DialogueAttachment, S_DialogueAttachmentDBData, C_DialogueAttachmentPreprocessor>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<S_DialogueAttachment>>
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueAttachmentDatabaseGenerated;
};
static_assert(sizeof(C_DialogueAttachmentDatabaseGenerated) == 0xE8,
              "C_DialogueAttachmentDatabaseGenerated must be 0xE8 (anim-DB extent)");

}  // namespace wh::dialogmodule
