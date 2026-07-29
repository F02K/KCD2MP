#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::E_EventState -- C_Event lifecycle stage (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Name RE-CHOSEN (no RTTI/string source). Values verified against C_Event+0x28 writers:
// ctor sub_1809DB5A4 zeroes it, sub_1809DB800 sets 1 after notifying targets/listeners,
// sub_1809DB7A4 sets 2 after invoking every effect's Apply(); GetState (sub_180836120,
// I_Event vtable slot [2]) just returns it.

namespace wh::rpgmodule {

enum class E_EventState : int32_t {
    Constructed     = 0,
    TargetsNotified = 1,
    EffectsApplied  = 2,
};

}  // namespace wh::rpgmodule
