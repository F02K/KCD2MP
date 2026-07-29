#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_PeriodicTimerData -- payload of C_PeriodicTimerCause (KCD2 WHGame.dll
// 1.5.6, kd7u).
// -----------------------------------------------
// Factory sub_18150A524 tracked-allocs 16 but writes ONLY the vtable -- the 8-byte payload
// region is left uninitialized, and caller sub_1804C089C routes the tick vector elsewhere.
// Modeled EMPTY [UNVERIFIED: an 8-byte never-initialized POD would produce the same
// allocation size].

namespace wh::rpgmodule {

struct S_PeriodicTimerData {
};

}  // namespace wh::rpgmodule
