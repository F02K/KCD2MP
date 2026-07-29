#pragma once
#include "../databasemodule/C_ObjectTreeDatabase.h"
#include "../REL/Relocation.h"

// -----------------------------------------------
// wh::environmentmodule::C_WeatherProfileDatabase -- "WeatherProfiles" tree database
// (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183AB87A0; GLOBAL STATIC object @0x185326240 (RVA 0x5326240); ctor
// sub_180E358EC. Stack: C_ObjectTreeDatabase<S_WeatherProfileData, std::vector>, registered
// as name "WeatherProfiles", namespace "wh::environmentmodule::". Tree-stack extent 0x78;
// concrete footprint UNVERIFIED -> no sizeof assert. Row layout pending the
// generated-tables pass (declared forward-only).

namespace wh::environmentmodule::data {
struct S_WeatherProfileData;   // profile row [layout pending generated-tables pass]
}  // namespace wh::environmentmodule::data

namespace wh::environmentmodule {

class C_WeatherProfileDatabase
    : public databasemodule::C_ObjectTreeDatabase<data::S_WeatherProfileData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WeatherProfileDatabase;
    [[nodiscard]] static C_WeatherProfileDatabase* GetInstance()
    {
        return reinterpret_cast<C_WeatherProfileDatabase*>(REL::ID(2328).address());
    }
};

}  // namespace wh::environmentmodule
