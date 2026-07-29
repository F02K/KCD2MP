#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::NPCState::C_StanceAnimDatabase :
// databasemodule::C_ObjectTreeDatabase<S_StanceAnimData> + unidentified 1-vfunc
// interface (@+0x78) -- stance animation DB (KCD2 WHGame.dll 1.5.6, kd7u).
// GLOBAL @0x18533A610 (2nd vptr @+0x78 = 0x18533A688).  sizeof ~0x88 (ctor
// writes to +0x80; the global slot spans 0xA0 -- trailing padding/extra
// UNVERIFIED) -> NO static_assert.  ctor sub_180F2B504 (base sub_180F2B738);
// ns "wh::xgenaimodule::NPCState::".
// -----------------------------------------------
// RTTI TD rva 0x4FC79C0; primary COL 0x40D1630, vtable rva 0x3AD8140; 2nd base
// COL 0x40D1658, vtable rva 0x3AD7FB8 (1 slot: sub_1814D8EC8).  The 2nd-base
// interface IDENTITY IS UNKNOWN [U] -- modeled as a raw tail vptr, NOT by
// inheritance (pragmatic escape hatch).  Row sizeof 56 (slot-4 `>>3 *inv7`);
// row fields [U].

namespace wh::xgenaimodule::NPCState {

struct S_StanceAnimData {
    uint8_t _raw[56];   // sizeof proven (slot-4 `>>3 *inv7`); fields [U]
};
static_assert(sizeof(S_StanceAnimData) == 56);

class C_StanceAnimDatabase : public databasemodule::C_ObjectTreeDatabase<S_StanceAnimData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StanceAnimDatabase;

    const void* m_iface78Vptr;   // +0x78  vptr of 2nd base wh::xgenaimodule::I_StanceAnimDatabase (RTTI-certified; vtable rva 0x3AD7FB8, 1 slot sub_1814D8EC8) -- SHOULD be a public base; raw-modeled per escape hatch
    uint8_t     m_byte80;        // +0x80  ctor write [U role]
    uint8_t     _pad81[7];       // +0x81
};
// sizeof ~0x88 (trailing extent UNVERIFIED) -- no static_assert.

}  // namespace wh::xgenaimodule::NPCState
