#pragma once

// -----------------------------------------------
// wh::rpgmodule::I_FactionManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (vtable only).
// -----------------------------------------------
// Primary base of C_FactionManager (mdisp 0; the 16-slot primary vtable 0x183B62098 is this
// interface's slot table). Slots NOT individually decompiled -- RTTR exposes
// GetFaction(const S_FactionDefinitionID&) and GetFactionRelation(string, string) through it.
// Only a dtor is declared to carry the vptr; namespace inferred from the implementor, flagged.

namespace wh::rpgmodule {

class I_FactionManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_FactionManager;
    virtual ~I_FactionManager() = default;   // slot position UNVERIFIED
};

}  // namespace wh::rpgmodule
