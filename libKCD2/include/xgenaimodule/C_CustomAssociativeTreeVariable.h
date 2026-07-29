#pragma once
#include <cstdint>
#include <vector>
#include "C_Variable.h"

// -----------------------------------------------
// wh::xgenaimodule::C_CustomAssociativeTreeVariable : C_Variable -- container
// variable node over a custom tree definition (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0xB8 (create-site proven: factory sub_1805B34A8 case 3 -> operator
// new(184) -> ctor sub_180CD46B8).
// -----------------------------------------------
// RTTI TD rva 0x4FFBED0; COL rva 0x40CD288; vtable 0x183A91C68, 34 slots
// (heaviest override set; own bodies at 0x18344F../183450.. region).
// ctor sub_180CD46B8(this, owner, customTree, name): C_Variable ctor; +0x40 =
// customTree; zero containers; sub_180CD4C1C(this+0x90); builds two operator
// new(56) nodes in a 2-iteration loop (sub_180CD678C).  dtor sub_180CD4994.

namespace wh::xgenaimodule {

class C_CustomAssociativeTreeVariable : public C_Variable {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CustomAssociativeTreeVariable;
    // concrete: fills every C_Variable pure slot (heaviest override set,
    // 0x18344F../183450.. bodies; per-slot mapping not itemized [U]).
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

    void*                 m_customTree;   // +0x40  custom tree / definition ptr (ctor a3) [U pointee]
    std::vector<void*>    m_nodes;        // +0x48  8-byte elems [U pointee type]
    std::vector<uint32_t> m_indices;      // +0x60  4-byte elems
    uint64_t              m_map[3];       // +0x78  container (dtor sub_180A608D4) [U exact type]
    uint8_t               m_extra[0x28];  // +0x90..+0xB8  embedded (sub_180CD4C1C) [U interior]
};
static_assert(sizeof(C_CustomAssociativeTreeVariable) == 0xB8, "C_CustomAssociativeTreeVariable must be 0xB8 (operator new(184))");

}  // namespace wh::xgenaimodule
