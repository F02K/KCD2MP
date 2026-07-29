#pragma once
#include <cstdint>
#include "conceptmodule/C_Effect.h"
#include "conceptmodule/C_TypedPortRef.h"
#include "guimodule/E_BlockUINotificationType.h"

// -----------------------------------------------
// wh::guimodule::C_BlockUINotifications -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xC8 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184C9E070; COL 0x413AC98; primary vtable 0x183B99D88 (46-slot C_Effect node
// table -- not modeled). Direct C_Effect child. ctor sub_18149C618; as_wh creator
// sub_182B0BB40 (alloc 0xC8, memset @0x182b0bba1).
// Own slots vs C_Effect: [0] dtor 0x1815FCFCC, [5]/[7] trio, [27] 0x182B12F00,
// [43] TRIGGER 0x182B13554, [44] UNTRIGGER 0x182B14070.
// Behavior: TRIGGER bumps the per-type global refcount dword_1855A7840[type] (0->1
// registers a UI-notification block on the framework object sub_1809155C8()+0xD8);
// UNTRIGGER decrements (1->0 unregisters) -- suppresses a UI notification category
// while triggered. Port display-name coined ("NotificationType" helper @0x183C78E00).

namespace wh::guimodule {

class C_BlockUINotifications : public wh::conceptmodule::C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BlockUINotifications;
    wh::conceptmodule::C_TypedPortRef<E_BlockUINotificationType::Type> m_typePort;   // +0x88  (0x40)
};
static_assert(sizeof(C_BlockUINotifications) == 0xC8, "C_BlockUINotifications must be 0xC8 (creator sub_182B0BB40)");
static_assert(offsetof(C_BlockUINotifications, m_typePort) == 0x88, "type port at 0x88");

}  // namespace wh::guimodule
