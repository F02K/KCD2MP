#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::framework::S_SaveBuildStamp -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x38 (SYNTHETIC name).
// -----------------------------------------------
// Element of C_SaveGameManager::m_buildStamps (+0x218; stride 56 in CreateSaveGame). Records
// which game build touched a save. Builder sub_180795008 fills the CryStrings from the build
// constants/cvars -- observed values: assemblyDateTime "Jun 19 2026 11:21:15", buildMachine
// "RACK-BUILD19", buildLabel "release_1_5_1308617_856", config "MasterMasterSteamPGO", plus
// cvar wh_sys_BuildInfo. Struct name AND field names are INFERRED from those contents (no RTTI
// symbol); +0x00 / +0x30 were not observed being written in the examined paths (UNRESOLVED).

namespace wh::framework {

struct S_SaveBuildStamp {
    uint64_t m_unk00;                     // +0x00  __vftable ptr of polymorphic class (see notes: struct is really `class S_SaveGameDebugInfo` w/ virtual dtor)
    CryStringT<char> m_assemblyDateTime;  // +0x08  e.g. "Jun 19 2026 11:21:15"
    CryStringT<char> m_buildMachine;      // +0x10  e.g. "RACK-BUILD19"
    CryStringT<char> m_buildLabel;        // +0x18  e.g. "release_1_5_1308617_856"
    CryStringT<char> m_config;            // +0x20  e.g. "MasterMasterSteamPGO"
    CryStringT<char> m_buildInfo;         // +0x28  cvar wh_sys_BuildInfo
    CryStringT<char> m_field30;           // +0x30  6th CryString (ctor-init empty; unwritten by builder)
};
static_assert(sizeof(S_SaveBuildStamp) == 0x38, "S_SaveBuildStamp must be 0x38");

}  // namespace wh::framework
