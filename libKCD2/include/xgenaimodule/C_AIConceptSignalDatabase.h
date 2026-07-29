#pragma once
#include <cstdint>
#include <unordered_map>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIConceptSignalDatabase :
// databasemodule::C_ObjectTreeDatabase<S_AIConceptSignal> -- AI concept signal
// DB (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL @0x185338200.  sizeof 0xB8 (base
// 0x78 + std::unordered_map @+0x78).  ctor sub_181540EAC (base sub_1819AC464);
// typename "AIConceptSignalDatabase".
// -----------------------------------------------
// RTTI TD rva 0x4F28DD0, COL 0x40E2CA0.  Vtable rva 0x3D28A30.  Row sizeof 32
// (slot-4 `>>5`); row fields [U]; index map key/value types [U].

namespace wh::xgenaimodule {

struct S_AIConceptSignal {
    uint8_t _raw[32];   // sizeof proven (slot-4 `>>5`); fields [U]
};
static_assert(sizeof(S_AIConceptSignal) == 32);

class C_AIConceptSignalDatabase : public databasemodule::C_ObjectTreeDatabase<S_AIConceptSignal> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIConceptSignalDatabase;

    std::unordered_map<uint64_t, uint64_t> m_index;   // +0x78  (0x40) key/value types [U]
};
static_assert(sizeof(C_AIConceptSignalDatabase) == 0xB8, "C_AIConceptSignalDatabase must be 0xB8");

}  // namespace wh::xgenaimodule
