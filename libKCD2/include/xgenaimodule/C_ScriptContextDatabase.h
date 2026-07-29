#pragma once
#include <cstdint>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_ScriptContextDatabase :
// databasemodule::C_ObjectTreeDatabase<S_ScriptContextDatabaseNode> -- script
// context DB (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL @0x18533AEF0.  sizeof >=
// 0x78 + container -- exact size UNPROVEN (the +0x78 container built by
// sub_18050B734(this+0x78, 24, 3, libname) has unproven kind/full size) -> NO
// static_assert.  ctor sub_1815412E8 (base sub_180EF5ADC); typename
// "ScriptContext".  Base reg handle @+0x68 built by sub_1803F7538 (0x10
// struct), not the usual sub_1803F7FAC.
// -----------------------------------------------
// RTTI TD rva 0x4FFB7B0, COL 0x40CD198.  Vtable rva 0x3BB0838.  Row sizeof 16
// (slot-4 `>>4`); row fields [U].

namespace wh::xgenaimodule {

struct S_ScriptContextDatabaseNode {
    uint8_t _raw[16];   // sizeof proven (slot-4 `>>4`); fields [U]
};
static_assert(sizeof(S_ScriptContextDatabaseNode) == 16);

class C_ScriptContextDatabase : public databasemodule::C_ObjectTreeDatabase<S_ScriptContextDatabaseNode> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptContextDatabase;

    uint8_t m_container78[8];   // +0x78  container (sub_18050B734, elem 24) -- kind and FULL
                                //   SIZE UNVERIFIED; modeled as an opaque 8-byte stub [U]
};
// total sizeof UNPROVEN -- no static_assert.

}  // namespace wh::xgenaimodule
