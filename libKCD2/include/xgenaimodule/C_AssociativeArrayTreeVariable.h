#pragma once
#include <cstdint>
#include <vector>
#include "C_Variable.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AssociativeArrayTreeVariable : C_Variable -- container
// variable node keyed associatively (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xF0
// (create-site proven: factory sub_1805B34A8 case 2 -> operator new(240); also
// sub_180B298D8 -> operator new(240)).
// -----------------------------------------------
// RTTI TD rva 0x4FFBF20; COL rva 0x40CD260; vtable 0x183A79990, 34 slots.
// ctor sub_180B29B8C(this, owner, name): C_Variable ctor; sub_180B29A98(this+0x40)
// inits the +0x40..+0xB0 block; +0xB0 = sub_180452BA4() (allocated assoc/hash
// object, freed by dtor via sub_181AB5160); zero vector @+0xC0 and container
// @+0xD8; then sub_180B2A76C.  dtor sub_180B29CF8.  The +0x40 block and +0xD8
// container interiors are NOT expanded [U] -- modeled as sized blobs.

namespace wh::xgenaimodule {

class C_AssociativeArrayTreeVariable : public C_Variable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AssociativeArrayTreeVariable;
    // concrete: fills every C_Variable pure slot (per-slot bodies not itemized [U]).
    int   GetId() override;                                     // [1]
    void* GetChildByWUID(const void* wuid) override;            // [7]
    void* GetChildByIndex(int idx) override;                    // [8]
    void* GetOrCreateChildByWUID(const void* wuid) override;    // [10]
    void* GetOrCreateChildByIndex(int idx) override;            // [11]
    void  SetChildByKey(void* key) override;                    // [13]
    void  RemoveChildByKey(void* key) override;                 // [14]
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

    uint8_t            m_assocHeader[0x70];   // +0x40..+0xB0  embedded structure (sub_180B29A98 / dtor sub_180B29E54) [U interior]
    // +0xB0..+0xC0  std::map/std::set (MSVC std::_Tree): {_Myhead sentinel node, _Mysize}
    void*              m_assocTreeHead;       // +0xB0  _Myhead: sentinel node from sub_180452BA4; freed by dtor via sub_181AB5160
    size_t             m_assocTreeSize;       // +0xB8  _Mysize (ctor 0)
    std::vector<void*> m_children;            // +0xC0  8-byte elems (dtor stride 8) [U pointee type]
    uint64_t           m_index[3];            // +0xD8  unordered_map-like container (dtor sub_180A608D4) [U exact type]
};
static_assert(sizeof(C_AssociativeArrayTreeVariable) == 0xF0, "C_AssociativeArrayTreeVariable must be 0xF0 (operator new(240))");

}  // namespace wh::xgenaimodule
