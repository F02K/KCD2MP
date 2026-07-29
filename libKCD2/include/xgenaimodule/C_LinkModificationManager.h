#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::C_LinkModificationManager -- queued link add/remove arbiter,
// embedded at +0x140 of C_LinkablesManager (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x38 (spans manager +0x140..+0x178; ctor-proven).
// -----------------------------------------------
// RTTI TD rva 0x4FF5AE0; vtable 0x184013E30, 3 slots.  Real base @+0
// framework::C_OverrideConflictManager<S_LinkTagHandle, E_AddRemove::Type>
// (template, not RE'd -- modeled flat here; the ctor writes the base vtable first,
// then the C_LinkModificationManager vtable).
// [2] Apply sub_180A30478(this, edgeTriple, addRemove) operates on the edge triple
// {a2[0], a2[1], a2[2]}: add -> sub_181302A78 / sub_18047CA2C, remove ->
// sub_18070AA3C.

namespace wh::xgenaimodule {

class C_LinkModificationManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LinkModificationManager;
    virtual ~C_LinkModificationManager();          // [0] deleting dtor sub_183442184
    virtual void _vf1();                           // [1] sub_180871EA0 [U role]
    virtual void Apply(void* edgeTriple, int addRemove);  // [2] sub_180A30478 [U exact signature]

    void*    m_pEntries;      // +0x08  base C_OverrideConflictManager<S_LinkTagHandle,E_AddRemove::Type>::m_pEntries: head of a 112-byte std::_Tree sentinel (sub_181AB55C0(112): _Left/_Parent/_Right self-refs @+0/+8/+0x10, WORD@0x18=0x0101 _Color=1/_Isnil=1)
    uint64_t m_count;         // +0x10  base C_OverrideConflictManager::m_count (RB-tree entry count; ctor 0)
    uint64_t m_unk18;         // +0x18  ctor: 0 [U role]
    uint64_t m_unk20;         // +0x20  ctor: 0 [U role]
    uint64_t m_unk28;         // +0x28  ctor: 0 [U role]
    uint8_t  m_byte30;        // +0x30  ctor: 1 [U role]
    uint8_t  _pad31[7];       // +0x31
};
static_assert(sizeof(C_LinkModificationManager) == 0x38, "C_LinkModificationManager must be 0x38 (mgr+0x140..+0x178)");

}  // namespace wh::xgenaimodule
