#pragma once

// -----------------------------------------------
// wh::databasemodule::I_DatabaseListener -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (vtable only).
// -----------------------------------------------
// Object-database change listener. Secondary base of wh::rpgmodule::C_SoulList (@+0x10, vtable
// 0x183F44DC8 with a SINGLE virtual, C_SoulList impl 0x18213AA34) and of wh::rpgmodule::
// C_FactionManager (@+0x08, vtable 0x183B62088). Namespace from the C_FactionManager dossier.
// Slot NAME and argument shape are tentative (callback semantics not decompiled).

namespace wh::databasemodule {

class I_DatabaseListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DatabaseListener;
    virtual void OnDatabaseEvent(void* a_event) = 0;   // [0]  (name/args tentative)
};

}  // namespace wh::databasemodule
