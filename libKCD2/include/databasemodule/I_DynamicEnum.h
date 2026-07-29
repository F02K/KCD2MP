#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::databasemodule::I_DynamicEnum -- table-backed enum view (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVI_DynamicEnum@databasemodule@wh@@ (TD 0x184AEF338).  5 slots.
// Secondary base @+0xB8 of the C_ObjectDatabaseIdIndexed databases (C_SoulArchetypeDatabase
// vtable 0x183F1A7E8, C_SocialClassDatabase 0x183C397F0): exposes an id/name enum over the rows.
// All impls use displacement-adjusted `this` (db = this - 0xB8) and the row array @db+0x30 with
// the instantiation's row stride (S_SoulArchetype rows are 0x58: value @row+0x08, name @row+0x10).

namespace wh::databasemodule {

class I_DynamicEnum {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DynamicEnum;
    virtual uint64_t GetCount() = 0;                      // [0] 0x00  forwards to db base slot [4] (0x180BE29A0)
    virtual uint32_t GetValue(uint64_t index) = 0;        // [1] 0x08  row[index]+0x08 (0x181A92A90)
    virtual const char* GetName(uint64_t index) = 0;      // [2] 0x10  row[index]+0x10 (0x181A92AC0)
    virtual uint32_t GetValueByName(const char* n) = 0;   // [3] 0x18  strcmp scan; -1 if absent (0x1812C3CAC)
    virtual const char* GetNameByValue(int64_t v) = 0;    // [4] 0x20  index lookup sub_1805CE084 -> row+0x10 (0x182C8A6B0)
};
static_assert(sizeof(I_DynamicEnum) == 0x08, "pure interface: vtable pointer only");

}  // namespace wh::databasemodule
