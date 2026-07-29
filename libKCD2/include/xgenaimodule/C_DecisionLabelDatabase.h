#pragma once
#include <cstdint>
#include <vector>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DecisionLabelDatabase :
// databasemodule::C_ObjectTreeDatabase<S_DecisionLabel> -- decision label DB
// (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL @0x18533A1D0.  sizeof 0x90 (base 0x78
// + std::vector @+0x78).  ctor sub_1815411B4 (base sub_1819ABFFC); typename
// "DecisionLabelDatabase".
// -----------------------------------------------
// RTTI TD rva 0x4F600A0, COL 0x40DE268.  Vtable rva 0x3BB09A8.  Row sizeof 16
// (slot-4 `>>4`); row fields [U].

namespace wh::xgenaimodule {

struct S_DecisionLabel {
    uint8_t _raw[16];   // sizeof proven (slot-4 `>>4`); fields [U]
};
static_assert(sizeof(S_DecisionLabel) == 16);

class C_DecisionLabelDatabase : public databasemodule::C_ObjectTreeDatabase<S_DecisionLabel> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DecisionLabelDatabase;

    std::vector<void*> m_vec78;   // +0x78  zeroed at ctor [U elem type]
};
static_assert(sizeof(C_DecisionLabelDatabase) == 0x90, "C_DecisionLabelDatabase must be 0x90");

}  // namespace wh::xgenaimodule
