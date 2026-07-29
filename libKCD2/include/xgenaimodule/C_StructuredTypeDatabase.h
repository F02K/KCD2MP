#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_StructuredTypeDatabase :
// databasemodule::C_ObjectTreeDatabase<TypeDescriptors::S_Type> -- ai_types
// table DB (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL @0x18533B740.  sizeof 0x78
// (no added members; row sizeof 64 proven by slot-4 `>>6`).  ctor sub_180F2BA8C
// (base sub_180F2BB14); tablepath "ai_types".
// -----------------------------------------------
// RTTI TD rva 0x4FFBBA8, COL 0x40CD238.  Vtable rva 0x3AD8810 (9 overrides
// incl. slot [12]).  Row field layout NOT reversed [U].

namespace wh::xgenaimodule {

namespace TypeDescriptors {
struct S_Type {
    uint8_t _raw[64];   // sizeof proven (slot-4 `>>6`); fields [U]
};
static_assert(sizeof(S_Type) == 64);
}  // namespace TypeDescriptors

class C_StructuredTypeDatabase : public databasemodule::C_ObjectTreeDatabase<TypeDescriptors::S_Type> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StructuredTypeDatabase;
    // no added members
};
static_assert(sizeof(C_StructuredTypeDatabase) == 0x78, "C_StructuredTypeDatabase must be 0x78");

}  // namespace wh::xgenaimodule
