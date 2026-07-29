#pragma once
#include <cstdint>
#include <unordered_map>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_TreeSignatureDatabase :
// databasemodule::C_ObjectTreeDatabase<BehaviorTree::S_TreeSignature> --
// behavior-tree signature DB (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL
// @0x185336860.  sizeof 0xB8 (base 0x78 + std::unordered_map @+0x78).  ctor
// sub_1811857F8 (base sub_181185900); typename "Signature".
// -----------------------------------------------
// RTTI TD rva 0x4E34018, COL 0x44895E8.  Vtable rva 0x3F9D980 (7 overrides;
// slot [4] NOT overridden -> row sizeof 40 from the inherited 40-byte body).
// Row fields [U]; index map key/value types [U].

namespace wh::xgenaimodule {

namespace BehaviorTree {
struct S_TreeSignature {
    uint8_t _raw[40];   // sizeof proven (inherited 40-byte slot-4 body); fields [U]
};
static_assert(sizeof(S_TreeSignature) == 40);
}  // namespace BehaviorTree

class C_TreeSignatureDatabase : public databasemodule::C_ObjectTreeDatabase<BehaviorTree::S_TreeSignature> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TreeSignatureDatabase;

    std::unordered_map<uint64_t, uint64_t> m_index;   // +0x78  (0x40) key/value types [U]
};
static_assert(sizeof(C_TreeSignatureDatabase) == 0xB8, "C_TreeSignatureDatabase must be 0xB8");

}  // namespace wh::xgenaimodule
