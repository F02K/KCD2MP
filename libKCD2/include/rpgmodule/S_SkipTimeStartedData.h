#pragma once

// -----------------------------------------------
// wh::rpgmodule::S_SkipTimeStartedData -- payload of C_SkipTimeStartedCause (KCD2
// WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Create fn sub_181950EB4 tracked-allocs 16 but writes ONLY the vtable -- the 8-byte
// payload region [obj+0x08..+0x10] is left uninitialized (factory caller sub_181950D2C
// does not fill it either).  Modeled EMPTY [UNVERIFIED: a genuinely empty struct vs an
// 8-byte member filled elsewhere produce the same allocation size].

namespace wh::rpgmodule {

struct S_SkipTimeStartedData {
};

}  // namespace wh::rpgmodule
