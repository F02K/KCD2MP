#pragma once
#include <cstdint>
#include <vector>
#include "I_VariableIndex.h"

// -----------------------------------------------
// wh::xgenaimodule::C_VariableIndex : I_VariableIndex -- concrete variable
// symbol-table: sorted uint16-hash -> variable map + parent chain (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x28 (proven via C_LocalVariableIndex operator new(40) in
// sub_1805B66E8 -- identical layout; also embedded 0x28 in C_SUBBBase @+0x98 and
// C_AIBrain @+0xC8).
// -----------------------------------------------
// RTTI TD rva 0x4EFBED0; COL rva 0x40CCF40; vtable 0x183A3FB58, 21 slots --
// implements all 20 I_VariableIndex slots and ADDS slot [20] = GetParent duplicate
// (sub_180947210, same body as [9]).  ctor sub_1805B71CC: m_parent =
// qword_185496678 (the C_GlobalVariableIndex singleton), empty map.  dtor
// sub_1805B7170 (frees the map, rebases vptr to I_VariableIndex).

namespace wh::xgenaimodule {

class C_VariableIndex : public I_VariableIndex {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_VariableIndex;

    // sorted-by-keyHash map entry (16B), recovered from the binary-search bodies
    // sub_1805B889C / sub_1809FDC74 / sub_183460C88.  Name proposed [U].
    struct S_Entry {
        uint16_t keyHash;   // +0x00  sort key (sub_1805B8998 name hash)
        uint8_t  flags;     // +0x02  read by sub_1809FDC74 [U role]
        uint8_t  _pad[5];   // +0x03  [U possibly secondary key]
        C_Variable* variable;  // +0x08  factory sub_1805B34A8 result (C_SingleVariable/IndexedArray/...); binsearched by [1] sub_1805C6DFC
    };

    void* GetVarByKeyHash(uint16_t hash, bool recurseToParent) override;  // [1]  sub_1805C6DFC
    void* GetVarByName(const char* name, bool recurse) override;          // [2]  sub_1805C71F0
    void  Insert(void* entry) override;                                   // [3]  sub_1805B889C
    void* CreateVariable(void* desc) override;                            // [4]  sub_1805B33EC
    void  RemoveVariable(const char* name) override;                      // [5]  sub_1809FDC74
    void  CollectAll(void* outVec, bool recurse) override;                // [8]  sub_183460C88
    void  ForEachChild() override;                                        // [10] sub_18041B184
    void  _vf11() override;                                               // [11] [U role]
    void  Serialize(void* stream) override;                               // [12] [U body]
    void  Deserialize(void* stream) override;                             // [13] [U body]
    void  NetSerialize(void* stream) override;                            // [14] stream tag 6401 [U body]
    void  ParseFromString(const char* str) override;                      // [15] [U body]
    void  GetIdentity(void* outIdentity) override;                        // [16] [U body]
    const char* GetVariableName(uint64_t handle) override;                // [18] sub_183460BB0 (map search)
    virtual I_VariableIndex* GetParent2();   // [20] NEW slot, sub_180947210 (= [9] body) [U why duplicated]

    std::vector<S_Entry> m_map;   // +0x10  sorted by keyHash; binsearched by [1]
};
static_assert(sizeof(C_VariableIndex) == 0x28, "C_VariableIndex must be 0x28 (operator new(40))");
static_assert(sizeof(C_VariableIndex::S_Entry) == 0x10, "index entry is 16 bytes");

}  // namespace wh::xgenaimodule
