#pragma once
#include "../databasemodule/C_ObjectDatabaseIdIndexed.h"
#include "../databasemodule/C_ObjectTableDatabase.h"
#include "../REL/Relocation.h"

// -----------------------------------------------
// wh::environmentmodule::C_PickableAreaDatabase -- "pickable_area" table database
// (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Own vtable 0x183ECF3D8; GLOBAL STATIC object @0x185326030 (RVA 0x5326030); ctor
// sub_1811106B8. Stack: C_ObjectDatabaseIdIndexed<C_ObjectTableDatabase<C_PickableAreaData,
// S_PickableAreaRow>, uint8> (the id-type template arg is dropped in the ported
// C_ObjectDatabaseIdIndexed, matching the other DB ports). Template-stack extent 0xB8;
// the concrete global's full footprint (possible trailing I_DynamicEnum base like other
// id-indexed DBs) is UNVERIFIED, so no sizeof assert here.
// Row types are declared forward-only: their layouts come with the generated-tables pass.

namespace wh::environmentmodule::data {
class C_PickableAreaData;    // row object  [layout pending generated-tables pass]
struct S_PickableAreaRow;    // raw table row [ditto]
}  // namespace wh::environmentmodule::data

namespace wh::environmentmodule {

class C_PickableAreaDatabase
    : public databasemodule::C_ObjectDatabaseIdIndexed<
          databasemodule::C_ObjectTableDatabase<data::C_PickableAreaData, data::S_PickableAreaRow>> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PickableAreaDatabase;
    // The global static instance (ctor stamps the vtable directly at this address).
    [[nodiscard]] static C_PickableAreaDatabase* GetInstance()
    {
        return reinterpret_cast<C_PickableAreaDatabase*>(REL::ID(2326).address());
    }
};

}  // namespace wh::environmentmodule
