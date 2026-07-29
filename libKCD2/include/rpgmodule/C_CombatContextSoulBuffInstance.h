#pragma once
#include "C_ConstantContextCheckingSoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_CombatContextSoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_CombatContextSoulBuffInstance@rpgmodule@wh@@  vtable 0x183EF9530
// make sub_182BC5DF0 (alloc 0xD8) -- adds NO members. Constant buff that is only active while
// the owning soul is in a combat context (context test via the abstract context-checking layer's
// vtable; the context/tag itself comes from the buff definition, not instance state).
// Chain: C_CombatContextSoulBuffInstance : C_ConstantContextCheckingSoulBuffInstance :
// C_ConstantSoulBuffInstance : C_SoulBuffInstance : ... (all mdisp 0). New in KCD2.

namespace wh::rpgmodule {

class C_CombatContextSoulBuffInstance : public C_ConstantContextCheckingSoulBuffInstance   // (0xD8)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatContextSoulBuffInstance;
};
static_assert(sizeof(C_CombatContextSoulBuffInstance) == 0xD8,
              "C_CombatContextSoulBuffInstance must be 0xD8");

}  // namespace wh::rpgmodule
