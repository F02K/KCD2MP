#pragma once
#include <cstdint>
#include "conceptmodule/C_Effect.h"
#include "conceptmodule/C_StateProxy.h"
#include "conceptmodule/C_TypedPortRef.h"
#include "framework/C_LocalizedString.h"

// -----------------------------------------------
// wh::guimodule::C_InfiniteHoldInteractionTrigger -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x120 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184C9E1B0; COL 0x413A9A0; primary vtable 0x183B06328 (46-slot
// C_StateProxy<C_Effect> node table -- not modeled). Base: C_StateProxy<C_Effect>
// (0x90). ctor sub_1810D5C00; as_wh creator sub_182B0BC6C (alloc 0x120).
// Own slots vs C_Effect: [0] dtor 0x1815517F0, [5]/[7] trio, [12] port reader
// 0x1816A4050, [27] 0x181396BC4, [28] 0x1811DBDA4 (C_StateProxy extra), [43] TRIGGER
// 0x182B135D4, [44] UNTRIGGER 0x182B140E8.
// Behavior: TRIGGER sets the global "infinite hold interaction active" latch
// byte_1856692DA and shows the hold prompt with m_textPort text (sub_182B12404);
// UNTRIGGER clears the latch and tears the prompt down (sub_182B16038) if m_handle118
// != -1. Port display-names coined.

namespace wh::guimodule {

class C_InfiniteHoldInteractionTrigger
    : public wh::conceptmodule::C_StateProxy<wh::conceptmodule::C_Effect> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InfiniteHoldInteractionTrigger;
    wh::conceptmodule::C_TypedPortRef<bool> m_enabledPort;                              // +0x90  (0x40) [name coined]
    wh::conceptmodule::C_TypedPortRef<wh::framework::C_LocalizedString> m_textPort;     // +0xD0  prompt text [name coined]
    uint8_t  m_active;      // +0x110  trigger =1 / untrigger =0
    uint8_t  _pad111[7];    // +0x111
    uint64_t m_handle118;   // +0x118  ctor -1; untrigger tests ==-1 [prompt handle; name coined]
};
static_assert(sizeof(C_InfiniteHoldInteractionTrigger) == 0x120, "C_InfiniteHoldInteractionTrigger must be 0x120 (creator sub_182B0BC6C)");
static_assert(offsetof(C_InfiniteHoldInteractionTrigger, m_enabledPort) == 0x90, "bool port at 0x90");

}  // namespace wh::guimodule
