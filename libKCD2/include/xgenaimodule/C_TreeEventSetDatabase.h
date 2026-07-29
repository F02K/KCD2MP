#pragma once
#include <cstdint>
#include <unordered_map>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_TreeEventSetDatabase :
// databasemodule::C_ObjectTreeDatabase<BehaviorTree::S_TreeEventSet> --
// behavior-tree event set DB (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL
// @0x1853367A0.  sizeof 0xB8 (base 0x78 + std::unordered_map @+0x78).  ctor
// sub_1811FF5D8 (base sub_1811FF6D4); typename "EventSet".
// -----------------------------------------------
// RTTI TD rva 0x4E34228, COL 0x4489610.  Vtable rva 0x3F9DA38.  Row sizeof 32
// (slot-4 `>>5`); row fields [U].  Index map node payload 16 bytes; key/value
// types NOT decoded [U] -- mirrored as <uint64_t, uint64_t>.

namespace wh::xgenaimodule {

namespace BehaviorTree {
struct S_TreeEventSet {
    uint8_t _raw[32];   // sizeof proven (slot-4 `>>5`); fields [U]
};
static_assert(sizeof(S_TreeEventSet) == 32);
}  // namespace BehaviorTree

class C_TreeEventSetDatabase : public databasemodule::C_ObjectTreeDatabase<BehaviorTree::S_TreeEventSet> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TreeEventSetDatabase;

    std::unordered_map<uint64_t, uint64_t> m_index;   // +0x78  (0x40) key/value types [U]
};
static_assert(sizeof(C_TreeEventSetDatabase) == 0xB8, "C_TreeEventSetDatabase must be 0xB8");

}  // namespace wh::xgenaimodule
