#pragma once
#include <cstdint>
#include <vector>
#include "C_Variable.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SingleVariable : C_Variable -- concrete scalar/typed-value
// variable leaf (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x58 (create-site proven:
// factory sub_1805B34A8 case 0 -> operator new(88) -> ctor sub_1805B80A0).
// -----------------------------------------------
// RTTI TD rva 0x4FFD2C8; COL rva 0x40CCEA0; vtable 0x183A3FE08, 34 slots.  Fills
// the pure typed-value slots; stubs child-access slots [6..11] to sub_181A73520
// (= return this+0x40: its own value container is the only "child"); [13]/[14] ->
// nullsub_1.  ctor sub_1805B80A0(this, owner, a3, name, a5seed): C_Variable ctor,
// value container ctor sub_1805B8EF8(this+0x40, owner), optional seed
// sub_1805B7DA4.  Value = std::vector of 16-byte typed-value entries (dtor masks
// &~0xF, per-element teardown sub_180DCD7E8).

namespace wh::xgenaimodule {

class C_SingleVariable : public C_Variable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SingleVariable;
    // concrete: fills every C_Variable pure slot.  [6..11] -> sub_181A73520
    // (return this+0x40); [13]/[14] -> nullsub_1; typed-value slots own bodies [U addresses].
    int   GetId() override;                                     // [1]
    void* GetChildByWUID(const void* wuid) override;            // [7]  sub_181A73520 stub
    void* GetChildByIndex(int idx) override;                    // [8]  sub_181A73520 stub
    void* GetOrCreateChildByWUID(const void* wuid) override;    // [10] sub_181A73520 stub
    void* GetOrCreateChildByIndex(int idx) override;            // [11] sub_181A73520 stub
    void  SetChildByKey(void* key) override;                    // [13] nullsub_1
    void  RemoveChildByKey(void* key) override;                 // [14] nullsub_1
    bool  HasChildByWUID(const void* wuid) override;            // [18]
    bool  HasChildByIndex(int idx) override;                    // [19]
    void  _vf22() override;                                     // [22]
    void  _vf24() override;                                     // [24]
    void  _vf25() override;                                     // [25]
    void  _vf26() override;                                     // [26]
    void  _vf27() override;                                     // [27]
    void  _vf28() override;                                     // [28]
    void  _vf29() override;                                     // [29]
    void  _vf31() override;                                     // [31]
    void  _vf32() override;                                     // [32]

    // 16-byte typed-value element; interior fields NOT itemized [U]
    struct S_Value {
        uint32_t type;    // +0x00  value-type tag (3=CryStringT<char>, 6=refcounted obj; <=2 = inline POD)
        uint32_t _pad04;  // +0x04  padding (inferred: teardown touches only DWORD@+0 and ptr@+8)
        uint64_t payload; // +0x08  variant by 'type': tag3 CryStringT<char> (decref *p-12), tag6 refcounted obj (Release@vf1), else inline POD
    };

    std::vector<S_Value> m_value;   // +0x40  the value container (sub_1805B8EF8)
};
static_assert(sizeof(C_SingleVariable) == 0x58, "C_SingleVariable must be 0x58 (operator new(88))");

}  // namespace wh::xgenaimodule
