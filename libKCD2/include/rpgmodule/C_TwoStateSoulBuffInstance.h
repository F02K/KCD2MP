#pragma once
#include "C_SoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_TwoStateSoulBuffInstance : C_SoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_TwoStateSoulBuffInstance@rpgmodule@wh@@ (TD 0x184CE4D30)  vtable 0x183EFAB80
// in-place ctor sub_182BC5214 = {C_SoulBuffInstance ctor; set vtable} -- adds NO members.
// [MODERATE vs KCD1] KCD1's two-state leaf stored its on/off state in the instance; the KCD2
// class stores NOTHING -- the active/inactive state is computed via the vtable (leaf overrides),
// so all two-state behavior leaves stay at the base 0xD8 unless they add their own fields
// (see C_TwoStateWithActivationSoulBuffInstance).

namespace wh::rpgmodule {

class C_TwoStateSoulBuffInstance : public C_SoulBuffInstance   // +0x00  (0xD8; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TwoStateSoulBuffInstance;
};
static_assert(sizeof(C_TwoStateSoulBuffInstance) == 0xD8, "C_TwoStateSoulBuffInstance must be 0xD8");

}  // namespace wh::rpgmodule
