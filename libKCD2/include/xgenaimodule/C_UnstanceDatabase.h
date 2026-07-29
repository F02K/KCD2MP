#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::NPCState::C_UnstanceDatabase :
// databasemodule::C_ObjectTreeDatabase<S_UnstanceData> + unidentified 6-vfunc
// interface (@+0x78) -- unstance animation DB (KCD2 WHGame.dll 1.5.6, kd7u).
// GLOBAL @0x18533A6B0 (2nd vptr @+0x78 = 0x18533A728).  sizeof ~0xB8 -> NO
// static_assert (the +0x80..+0xB4 block is zero-init only; container kind
// unproven [U]).  ctor sub_180F2B5B0 (base sub_180F2B67C).
// -----------------------------------------------
// RTTI TD rva 0x4FD4150; primary COL 0x40D02D0, vtable rva 0x3AD8088; 2nd base
// COL 0x40D02F8, vtable rva 0x3AD8050 (6 slots: sub_18086D9F8, sub_18194D860,
// sub_181543CC8, sub_1803A17E0, sub_18086DDC4, sub_1833D7158).  The 2nd-base
// interface IDENTITY IS UNKNOWN [U] -- modeled as a raw tail vptr, NOT by
// inheritance.  Row sizeof 176 (slot-4 `>>4 *inv11`, verified); row fields [U].

namespace wh::xgenaimodule::NPCState {

struct S_UnstanceData {
    uint8_t _raw[176];   // sizeof proven (slot-4 `>>4 *inv11`); fields [U]
};
static_assert(sizeof(S_UnstanceData) == 176);

class C_UnstanceDatabase : public databasemodule::C_ObjectTreeDatabase<S_UnstanceData> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UnstanceDatabase;

    const void* m_iface78Vptr;   // +0x78  vptr of 2nd base wh::xgenaimodule::I_UnstanceDatabase (RTTI-certified; vtable rva 0x3AD8050, 6 slots) -- SHOULD be a public base; raw-modeled per escape hatch
    uint8_t     m_zero80[0x38];  // +0x80..+0xB8  zero-init block (xmm @+0x80/+0x90/+0xA0, dword @+0xB0); container kind unproven [U]
};
// sizeof ~0xB8 (trailing extent UNVERIFIED) -- no static_assert.

}  // namespace wh::xgenaimodule::NPCState
