#pragma once
#include <cstdint>
#include <cstddef>
#include "S_SocialClass.h"
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../databasemodule/C_ObjectDatabaseIdIndexed.h"
#include "../databasemodule/I_DynamicEnum.h"

// -----------------------------------------------
// wh::rpgmodule::C_SocialClassDatabase -- the social_class table
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xC0.
// -----------------------------------------------
// RTTI .?AVC_SocialClassDatabase@rpgmodule@wh@@ (TD 0x184D413E8)  vtables: primary 0x183C398D8,
// I_DynamicEnum @+0xB8 0x183C397F0 (enum view over the 0x48 S_SocialClass rows; value @row+0x00,
// name @row+0x08 -- non-polymorphic rows).
// Stack: C_ObjectDatabaseIdIndexed<C_ObjectTableDatabase<S_SocialClass, S_SocialClassDBData>>
// + I_DynamicEnum declared HERE (ctor sub_18192F920 writes the +0xB8 vfptr; no concrete tail).
// GLOBAL STATIC instance @0x185330030 (table "social_class", group "rpg").

namespace wh::rpgmodule {

// Per-row DB payload type of the table template (second arg). Opaque -- not RE'd.
struct S_SocialClassDBData;

class C_SocialClassDatabase
    : public wh::databasemodule::C_ObjectDatabaseIdIndexed<
          wh::databasemodule::C_ObjectTableDatabase<S_SocialClass, S_SocialClassDBData>>
    , public wh::databasemodule::I_DynamicEnum   // +0xB8
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SocialClassDatabase;
    // Returns the global static instance @RVA 0x5330030. Impl in src/databasemodule/databasemodule.cpp.
    static C_SocialClassDatabase* GetInstance();
};
static_assert(sizeof(C_SocialClassDatabase) == 0xC0,
              "C_SocialClassDatabase must be 0xC0 (ctor sub_18192F920 write extent)");

}  // namespace wh::rpgmodule
