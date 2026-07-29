#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_BlockUINotificationType -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper.
// -----------------------------------------------
// Nested-Type enum wrapper (rttr name-helper string "NotificationType" @0x183C78E00).
// The UI-notification category selector of C_BlockUINotifications: while triggered, the
// node increments a per-type global refcount dword_1855A7840[type] to suppress that
// notification category. ENUMERATOR NAMES/VALUES NOT RECOVERED; underlying width
// inferred from the dword-indexed refcount table (int-sized index) -- both UNVERIFIED.

namespace wh::guimodule {

struct E_BlockUINotificationType {
    enum Type : int32_t {};   // enumerators UNVERIFIED
};

}  // namespace wh::guimodule
