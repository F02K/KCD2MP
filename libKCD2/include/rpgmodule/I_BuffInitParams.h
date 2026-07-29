#pragma once

// -----------------------------------------------
// wh::rpgmodule::I_BuffInitParams -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x8 (pure interface).
// -----------------------------------------------
// RTTI .?AVI_BuffInitParams@rpgmodule@wh@@  vtable 0x183A43B60 = [ [0] dtor sub_182878EC8,
// [1..3] _purecall x3 ] -- vtable pointer only, 3 pure virtual methods (semantics not reversed;
// slot names UNKNOWN). Carries parsed per-implementation creation parameters (e.g. item id,
// sleep hours) into the buff-implementation registry's virtual createInstance(def,
// I_BuffInitParams&) dispatch (registry map sub_18063B104, instantiation path sub_18063ACE4).
// KCD1's I_BuffInitParams family carried over 1:1 and expanded -- see C_BuffInitParamsDefault.h
// for the concrete roster.

namespace wh::rpgmodule {

class I_BuffInitParams {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_BuffInitParams;
    virtual ~I_BuffInitParams() = default;   // [0]  (sub_182878EC8 in the interface vtable)
    virtual void vf1() = 0;                  // [1]  _purecall -- signature/semantics UNRESOLVED
    virtual void vf2() = 0;                  // [2]  _purecall -- signature/semantics UNRESOLVED
    virtual void vf3() = 0;                  // [3]  _purecall -- signature/semantics UNRESOLVED
};
static_assert(sizeof(I_BuffInitParams) == 0x8, "I_BuffInitParams must be 0x8");

}  // namespace wh::rpgmodule
