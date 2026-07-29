#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SocialClass -- social_class table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AUS_SocialClass@rpgmodule@wh@@ (via the C_SocialClassDatabase template stack).
// Row stride 0x48 (PROVEN: C_SocialClassDatabase's I_DynamicEnum impls index with *72).
// NON-polymorphic (value @row+0x00, name @row+0x08 -- unlike the vfptr-carrying S_SoulArchetype).
// Only the enum-view fields are typed; the rest is unread in the decompiled surface.

namespace wh::rpgmodule {

struct S_SocialClass {
    int32_t  m_id;          // +0x00  I_DynamicEnum::GetValue reads row+0x00 (0x180C78580)
    uint32_t _pad04;        // +0x04
    const char* m_name;     // +0x08  I_DynamicEnum::GetName reads row+0x08 (0x180C78560)
    uint8_t  _unk10[0x38];  // +0x10  no reader decompiled [UNVERIFIED]
};
static_assert(sizeof(S_SocialClass) == 0x48, "S_SocialClass must be 0x48 (I_DynamicEnum stride 72)");

}  // namespace wh::rpgmodule
