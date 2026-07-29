#pragma once
#include <vector>
#include "C_Variable.h"

// -----------------------------------------------
// wh::xgenaimodule::C_IndexedArrayTreeVariable : C_Variable -- container variable
// node: index-keyed array of child variables (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x58 (create-site proven: factory sub_1805B34A8 case 1 -> operator
// new(88) -> sub_180B29700).
// -----------------------------------------------
// RTTI TD rva 0x4FFD4A0; COL rva 0x40CCE00; vtable 0x183A79878, 34 slots.
// Implements the child-access interface for real: [8] GetChildByIndex
// sub_1811C8554 (bounds idx<=0xFFF && idx<size, return vec[idx]); [7] = atoi ->
// [8] (sub_1811C852C); [11] GetOrCreateByIndex sub_1811DDE40 (auto-grow, operator
// new(24) per child via sub_1805B8EF8, invalidate callback cache); [10] = atoi ->
// [11].  dtor sub_180B29764; copy-ctor sub_1809C3A9C (deep-copies children via
// operator new(24) + sub_180B2A0AC).  Children are 24-byte heap value-container
// nodes [U element class identity].

namespace wh::xgenaimodule {

class C_IndexedArrayTreeVariable : public C_Variable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_IndexedArrayTreeVariable;
    // concrete: fills every C_Variable pure slot; only [7][8][10][11] bodies verified.
    int   GetId() override;                                     // [1]  [U body]
    void* GetChildByWUID(const void* wuid) override;            // [7]  sub_1811C852C (atoi -> [8])
    void* GetChildByIndex(int idx) override;                    // [8]  sub_1811C8554 (bounds + vec[idx])
    void* GetOrCreateChildByWUID(const void* wuid) override;    // [10] atoi -> [11]
    void* GetOrCreateChildByIndex(int idx) override;            // [11] sub_1811DDE40 (auto-grow)
    void  SetChildByKey(void* key) override;                    // [13] [U body]
    void  RemoveChildByKey(void* key) override;                 // [14] [U body]
    bool  HasChildByWUID(const void* wuid) override;            // [18] [U body]
    bool  HasChildByIndex(int idx) override;                    // [19] [U body]
    void  _vf22() override;                                     // [22] [U body]
    void  _vf24() override;                                     // [24] [U body]
    void  _vf25() override;                                     // [25] [U body]
    void  _vf26() override;                                     // [26] [U body]
    void  _vf27() override;                                     // [27] [U body]
    void  _vf28() override;                                     // [28] [U body]
    void  _vf29() override;                                     // [29] [U body]
    void  _vf31() override;                                     // [31] [U body]
    void  _vf32() override;                                     // [32] [U body]

    std::vector<void*> m_children;   // +0x40  ptrs to 24-byte child value nodes [U pointee type]
};
static_assert(sizeof(C_IndexedArrayTreeVariable) == 0x58, "C_IndexedArrayTreeVariable must be 0x58 (operator new(88))");

}  // namespace wh::xgenaimodule
