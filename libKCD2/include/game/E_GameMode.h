#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::game::E_GameMode -- KCD2 WHGame.dll 1.5.6 (kd7u).  Underlying int32.
// -----------------------------------------------
// The playthrough difficulty/ruleset mode. Queried at runtime through the framework:
//   framework (S_GameContext::m_pFramework, +0x18; also the global qword_18492D890)
//     -> vf[0x118]  GetGameModeObject()
//     -> vf[0x10]   GetId()   == E_GameMode::Type
// A ready-made getter exists at 0x181F435E0 and is published to reflection/script under
// the literal name "wh::rpgmodule::GetGameMode" with the description
// "Returns the current game mode" (property name "GameMode"), registrar sub_18016D870.
//
// PROVENANCE (values PROVEN, not inferred): the RTTR enumeration registration
// sub_1803884D0 builds the enumerator table from literal name/value pairs --
//   "none"(len 4) -> 0, "normal"(len 6) -> 1, "hardcore"(len 8) -> 2
// -- and registers it as rttr::detail::enumeration_wrapper<wh::game::E_GameMode::Type,3,0>
// under the RTTR type name "E_GameMode" (strings 0x18408F0C8 / 0x18408F0D8).
//
// NOTE: the Tables.pak `game_mode_id` data column happens to agree numerically
// (2 = hardcore), but THIS enum is the authority for any comparison in the binary.
// DO NOT confuse with the perk-table column "exclude_in_game_mode" (registered at row
// +0x70 by sub_1801AEB30 alongside metaperk_id/skill_selector) -- unrelated to this enum.
//
// FINDING ALL HARDCORE GATES: byte-scan .text for `FF 90 18 01 00 00`
// (call qword ptr [rax+118h]) = 528 sites, then decode forward for `cmp eax, <imm>`.
// Exactly 12 compare against Hardcore. The map/compass suppression family is:
//   sub_181F4A2A0  fast-travel POI points   (see C_UIFlashMapPoiMarker / I_POI [19])
//   sub_180C4D10C  blocks mark types 9 (Dog) / 44 (Nest)
//   sub_180C4CB40 + C_UICompass::SendAddCompassMarker_180C4D2E8   compass marks
//   sub_180DC9764  sends "SetPlayer" WITHOUT position/heading (no player dot on map)
//   sub_180DCB1A0  suppresses "SetCheckpoint" (sends "ResetCheckpoint" instead)
//   sub_181F98670  selects the alternate field +0x80 over +0x7C

namespace wh::game {

struct E_GameMode {
    enum Type : int32_t {
        None     = 0,
        Normal   = 1,
        Hardcore = 2,
    };
};

}  // namespace wh::game
