#pragma once
#include <cstdint>
#include <memory>
#include "databasemodule/C_ObjectTreeDatabase.h"
#include "databasemodule/C_ObjectDatabaseKeyIndexed.h"
#include "databasemodule/C_ObjectDatabaseDefaultKeyExtractor.h"
#include "guimodule/I_Cutscene.h"
#include "crysystem/CXConsole.h"   // IConsoleArgumentAutoComplete

// -----------------------------------------------
// wh::guimodule::C_CutsceneDatabase -- the "cutscene" tree DB (KCD2 1.5.6, kd7u).
// sizeof 0x80 (tree extent 0x78 + IConsoleArgumentAutoComplete@+0x78 -- the only MI DB).
// -----------------------------------------------
// RTTI TD RVA 0x4CB0420; primary vtable 0x183B511E8; SECOND subobject @+0x78
// (COL RVA 0x4138FB0, vtable 0x183B501F8) = engine `IConsoleArgumentAutoComplete`
// [G4c Part D: CHD-proven, bcd[6] mdisp 0x78 -- NOT I_CutsceneChangeListener].
// Stack: C_ObjectDatabaseKeyIndexed<C_ObjectTreeDatabase<std::shared_ptr<I_Cutscene>>,
// DefaultKeyExtractor>. GLOBAL STATIC @0x185326630 (ctor sub_1812BBC28, base
// sub_1819BDA38, vtable-install sub_1812BD1A8; secondary vfptr @0x1853266A8;
// ns "wh::guimodule::", name "cutscene", group "ui"). Load [9] 0x181364F20; post-load
// [16] 0x1819ED824; tree vertical (no descriptors/rows; shared tree-parse sub_180C57E60).
//
// The autocompleter serves cutscene names to the console: [1] GetCount 0x182B36C38
// (forwards to the primary table's count getter vf[+0x20]); [2] GetValue 0x181A8CA30
// (indexes the 16-byte shared_ptr element array, returns element->m_name.c_str()).
// Wired to "wh_ui_PlayCutscene" via IConsole::RegisterAutoComplete (@0x1818ab5b5,
// global qword_1853266A8 -- linkage INFERRED, writer untraced).

namespace wh::guimodule {

class C_CutsceneDatabase
    : public wh::databasemodule::C_ObjectDatabaseKeyIndexed<
          wh::databasemodule::C_ObjectTreeDatabase<std::shared_ptr<I_Cutscene>>,
          wh::databasemodule::C_ObjectDatabaseDefaultKeyExtractor<std::shared_ptr<I_Cutscene>>>
    , public IConsoleArgumentAutoComplete   // +0x78
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CutsceneDatabase;
    // IConsoleArgumentAutoComplete (vtable 0x183B501F8).
    int         GetCount() const override;         // [1] 0x182B36C38
    const char* GetValue(int nIndex) const override;   // [2] 0x181A8CA30  returns element I_Cutscene::m_name
};
static_assert(sizeof(C_CutsceneDatabase) == 0x80, "C_CutsceneDatabase must be 0x80 (tree extent + autocomplete base)");

}  // namespace wh::guimodule
