#pragma once
#include <cstdint>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_UseHourGlassAction -- turn the sandglass (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_UseHourGlassAction@playermodule@wh@@ (TD 0x184DD45E8); vtable 0x183A5BF40; ctor
// sub_1808D1B4C; factory sub_1808D1C14 (verb 1; also input handler #4 writes pending verb 1).
// One turn = the HourglassTimeout RPG param (10 s; S_Constants base+0x5F4, the DSL sensor's
// divisor -- distinct from the table entity's "HourGlassUnitTime" Lua prop feeding the pot
// ambient) of cook time -- the unit the recipe DSL's Weak/StrongBoilingTime N args are measured in.  Plays the sandglass turn +
// HourGlassAlembic sand .cax.  [Tail fields +0x68.. not individually verified.]

namespace wh::playermodule {

class C_UseHourGlassAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UseHourGlassAction;

    uint64_t _pad68[2];   // +0x68  [contents UNVERIFIED]
};
static_assert(sizeof(C_UseHourGlassAction) == 0x78, "C_UseHourGlassAction must be 0x78");

}  // namespace wh::playermodule
