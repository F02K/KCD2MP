#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DogPointDatabase :
// databasemodule::C_ObjectTreeDatabase<S_DogPointLayer> -- dog point layer DB
// (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL @0x1853384D0.  sizeof 0x78 (no added
// members).  ctor sub_181540FBC (base sub_1819ABE84); typename "DogPoints".
// -----------------------------------------------
// RTTI TD rva 0x4F4C960, COL 0x40DFD70.  Vtable rva 0x3D28750 (7 overrides;
// slot [4] NOT overridden -> row sizeof 40 from the inherited body).  Row
// fields [U].

namespace wh::xgenaimodule {

struct S_DogPointLayer {
    inline static constexpr auto RTTI = Offsets::RTTI_S_DogPointLayer;
    uint8_t _raw[40];   // sizeof proven (inherited 40-byte slot-4 body); fields [U]
};
static_assert(sizeof(S_DogPointLayer) == 40);

class C_DogPointDatabase : public databasemodule::C_ObjectTreeDatabase<S_DogPointLayer> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DogPointDatabase;
    // no added members
};
static_assert(sizeof(C_DogPointDatabase) == 0x78, "C_DogPointDatabase must be 0x78");

}  // namespace wh::xgenaimodule
