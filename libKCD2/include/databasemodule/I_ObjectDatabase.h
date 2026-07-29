#pragma once

// -----------------------------------------------
// wh::databasemodule::I_ObjectDatabase -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (sizeof 0x08).
// -----------------------------------------------
// RTTI .?AVI_ObjectDatabase@databasemodule@wh@@ (TD 0x184AF0828)  own vtable 0x183E3D938, 3 slots.
//
// [FUNDAMENTAL vs KCD1] KCD1's I_ObjectDatabase was a 20-slot interface. KCD2 slims it to THREE
// slots -- everything else moved down into C_ObjectDatabaseBase (21 slots). Slot meanings read
// from the base/concrete overrides (C_ReputationChangeDatabase).

namespace wh::databasemodule {

class I_ObjectDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ObjectDatabase;
    virtual ~I_ObjectDatabase() = 0;            // [0] 0x00  scalar-deleting
    virtual int GetVersion() = 0;               // [1] 0x08  base impl returns 1 (0x181A72470)
    // [2] 0x10  concrete impl returns the table name (std::string @db+0x50, SSO-aware)
    virtual const char* GetTableName() = 0;     //           (0x180EF35F8)
};
static_assert(sizeof(I_ObjectDatabase) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::databasemodule
