#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_ChoiceName -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : uint8.
// -----------------------------------------------
// Nested-Type enum wrapper (rttr property mangling `W4Type@E_ChoiceName@guimodule@wh@@`).
// Identity key of C_UIMenuChoice (byte @element+0x08; C_MenuChoiceDatabase's
// DefaultKeyExtractor keys on it; runtime sorted-vector DB @0x185326760, 0x20 rows,
// lookup sub_181569B00 -- consumed by AddChoiceOption-by-label sub_181F8BA20).
// Values 1..36 observed at builder call sites (usage pattern: 1/2 and 3/4 = On/Off
// pairs, 7..11 = the five quality tiers) but the NAME strings were NOT RECOVERED
// (menu_choices.xml Ids not correlated) -- do not invent values; extend when certified.

namespace wh::guimodule {

struct E_ChoiceName {
    enum Type : uint8_t {};   // enumerators UNVERIFIED
};

}  // namespace wh::guimodule
