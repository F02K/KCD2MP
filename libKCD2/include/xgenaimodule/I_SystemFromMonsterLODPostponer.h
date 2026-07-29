#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_SystemFromMonsterLODPostponer -- KCD2 WHGame.dll 1.5.6 (utem).
// Empty tag interface.
// -----------------------------------------------
// RTTI .?AVI_SystemFromMonsterLODPostponer@xgenaimodule@wh@@ (TD 0x184B73C80).  Zero RTTI
// referrers besides C_Actor, which lists it as a non-polymorphic tag base -- no vtable, no
// storage.  Presumably opts the actor out of the "FromMonster" LOD postponing system.

namespace wh::xgenaimodule {

struct I_SystemFromMonsterLODPostponer {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_SystemFromMonsterLODPostponer;
};

}  // namespace wh::xgenaimodule
