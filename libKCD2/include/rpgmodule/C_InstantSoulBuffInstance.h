#pragma once
#include "C_SoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_InstantSoulBuffInstance : C_SoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_InstantSoulBuffInstance@rpgmodule@wh@@  vtable 0x183C422F8  ctor sub_1819509E0
// = {C_SoulBuffInstance ctor; set vtable} -- adds NO members. Fire-once ("instant") buff leaf:
// applies its definition's effects on Init and immediately expires (behavior via vtable; no
// per-instance state). No KCD1 named analog -- one of KCD2's new structural layers.

namespace wh::rpgmodule {

class C_InstantSoulBuffInstance : public C_SoulBuffInstance   // +0x00  (0xD8; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InstantSoulBuffInstance;
};
static_assert(sizeof(C_InstantSoulBuffInstance) == 0xD8, "C_InstantSoulBuffInstance must be 0xD8");

}  // namespace wh::rpgmodule
