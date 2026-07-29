#pragma once
#include "../databasemodule/C_ObjectDatabaseIdIndexed.h"
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../REL/Relocation.h"

// -----------------------------------------------
// wh::environmentmodule::C_PickableAreaMaterialDatabase -- "pickable_area_material" table
// database (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183C3B0D8; GLOBAL STATIC object @0x185326130 (RVA 0x5326130); ctor
// sub_181936D4C; table keys "item" / "pickable_area_material". Stack:
// C_ObjectDatabaseIdIndexed<C_ObjectTableDatabase<S_PickableAreaMaterial,
// S_PickableAreaMaterialDBData>, int>. Same modeling caveats as C_PickableAreaDatabase
// (id-type arg dropped; concrete footprint UNVERIFIED -> no sizeof assert; row layouts
// pending the generated-tables pass).

namespace wh::environmentmodule::data {
struct S_PickableAreaMaterial;         // row object  [layout pending generated-tables pass]
struct S_PickableAreaMaterialDBData;   // raw table row [ditto]
}  // namespace wh::environmentmodule::data

namespace wh::environmentmodule {

class C_PickableAreaMaterialDatabase
    : public databasemodule::C_ObjectDatabaseIdIndexed<
          databasemodule::C_ObjectTableDatabase<data::S_PickableAreaMaterial,
                                                data::S_PickableAreaMaterialDBData>> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PickableAreaMaterialDatabase;
    [[nodiscard]] static C_PickableAreaMaterialDatabase* GetInstance()
    {
        return reinterpret_cast<C_PickableAreaMaterialDatabase*>(REL::ID(2327).address());
    }
};

}  // namespace wh::environmentmodule
