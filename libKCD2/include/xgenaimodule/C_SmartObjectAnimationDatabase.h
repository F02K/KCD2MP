#pragma once
#include <cstdint>
#include <unordered_map>
#include "../databasemodule/C_ObjectTreeDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartObjectAnimationDatabase :
// databasemodule::C_ObjectTreeDatabase<S_SmartObjectAnimations> -- smart-object
// animation DB (KCD2 WHGame.dll 1.5.6, kd7u).  GLOBAL @0x185338890.  sizeof
// 0xC0 (base 0x78 + qword @+0x78 + std::unordered_map @+0x80).  ctor
// sub_180DED6DC (base sub_180DED7E0); typename "SmartObjectAnimations".
// -----------------------------------------------
// RTTI TD rva 0x4F5A620, COL 0x455A670.  Vtable rva 0x3FEBA00.  Row sizeof 48
// (slot-4 `>>4 *inv3`); row fields [U].  +0x78 qword NOT written by the ctor
// -- member type unknown [U].

namespace wh::xgenaimodule {

struct S_SmartObjectAnimations {
    uint8_t _raw[48];   // sizeof proven (slot-4 `>>4 *inv3`); fields [U]
};
static_assert(sizeof(S_SmartObjectAnimations) == 48);

class C_SmartObjectAnimationDatabase : public databasemodule::C_ObjectTreeDatabase<S_SmartObjectAnimations> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartObjectAnimationDatabase;

    uint64_t m_unk78;   // +0x78  not written by ctor sub_180DED6DC; occupies the same post-base slot as C_SmartEntityDatabase::m_colBrain (a column handle) -- this DB registers no column so it stays 0 [U role]
    std::unordered_map<uint64_t, uint64_t> m_index;   // +0x80  (0x40) key/value types [U]
};
static_assert(sizeof(C_SmartObjectAnimationDatabase) == 0xC0, "C_SmartObjectAnimationDatabase must be 0xC0");

}  // namespace wh::xgenaimodule
