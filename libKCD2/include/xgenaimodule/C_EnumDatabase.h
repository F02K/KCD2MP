#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_EnumDatabase :
// databasemodule::C_ObjectTreeDatabase<TypeDescriptors::S_Enum> -- ai_enums
// table DB (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL @0x18533B6C0.  sizeof 0x78
// (no added members; row sizeof 40 proven by slot-4 body).  ctor sub_180F2BA04
// (base sub_180EF5C3C); ns "wh::xgenaimodule::TypeDescriptors", tablepath
// "ai_enums".
// -----------------------------------------------
// RTTI TD rva 0x4FFBD68, COL 0x40CD210.  Vtable rva 0x3AD8720 -- the PRISTINE
// C_ObjectTreeDatabase base shape (22 slots; see G6I dossier for the slot map).
// Row field layout NOT reversed [U] -- modeled as a sized blob.

namespace wh::xgenaimodule {

namespace TypeDescriptors {
struct S_Enum {
    uint8_t _raw[40];   // sizeof proven (slot-4 divisor); fields [U]
};
static_assert(sizeof(S_Enum) == 40);
}  // namespace TypeDescriptors

class C_EnumDatabase : public databasemodule::C_ObjectTreeDatabase<TypeDescriptors::S_Enum> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EnumDatabase;
    // no added members
};
static_assert(sizeof(C_EnumDatabase) == 0x78, "C_EnumDatabase must be 0x78 (pristine tree DB)");

}  // namespace wh::xgenaimodule
