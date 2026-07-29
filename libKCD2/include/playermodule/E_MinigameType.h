#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::playermodule::E_MinigameType -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : int32.
// -----------------------------------------------
// Nested-Type enum wrapper (RTTR enumeration_wrapper<...E_MinigameType::Type,14,0>).
// Enumerators certified from the RTTR registration sub_180227FD0. Message structs
// may store it truncated to a byte (S_GenericMinigameData::m_minigameType).
// INDEPENDENTLY vtable-certified by the session factory sub_1809F0234 (called by
// C_MinigameManager::FindOrCreateSession): each case allocs the class-exact size and installs
// the matching ??_7 (1 C_Sharpening 0x258 / 2 C_Reading 0xE8 / 3 C_Alchemy 0x6D8 /
// 4 C_HerbGathering 0xA0 / 5 C_LockPicking 0x1A8 / 6 C_HoleDigging 0xE8 / 7 C_Dice 0x5B8 /
// 8 C_Pickpocketing 0xF8 / 9 C_StoneThrowing 0xC8 / 10 C_BattleArchery 0x190 /
// 11 C_Distract 0xA8 / 12 C_Blacksmithing 0xF0 / 13 C_ForgeBuilder 0x268; 0/>13 = no session).

namespace wh::playermodule {

struct E_MinigameType {
    enum Type : int32_t {
        Undefined     = 0,
        Sharpening    = 1,
        Reading       = 2,
        Alchemy       = 3,
        HerbGathering = 4,
        LockPicking   = 5,
        HoleDigging   = 6,
        Dice          = 7,
        Pickpocketing = 8,
        StoneThrowing = 9,
        BattleArchery = 10,
        Distract      = 11,
        Blacksmithing = 12,
        ForgeBuilder  = 13,
    };
};

}  // namespace wh::playermodule
