#pragma once
#include <cstdint>
#include "../framework/I_WUIDMappingProvider.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AreaUnionManager : framework::I_WUIDMappingProvider --
// area-union registry SINGLETON at qword_1854960E8 (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0xB0 (getter sub_1811A47C8 @0x1811a481c:
// sub_18549D378(176,...), += 0xB0, ctor sub_1811A4884).  Post-ctor the getter
// registers with a system (sub_180BEB108(sub_18341F52C())).
// -----------------------------------------------
// RTTI TD rva 0x4F47198.  Vtable rva 0x3FE4088 -- 4 slots ([0]0x3281B60
// (dossier labels it the dtor) [1]0x3281B18 [2]0x32805C8 [3]0x154CF4C): one
// MORE than the 3-slot I_WUIDMappingProvider (dtor LAST at [2] per the
// C_InventoryManager-proven model).  DISCREPANCY [U]: modeled as the 3 iface
// slots + one appended own virtual; the slot->body mapping here is NOT
// resolved -- do not call through raw indices.

namespace wh::xgenaimodule {

class C_AreaUnionManager : public framework::I_WUIDMappingProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AreaUnionManager;
    void* GetValueForWuid(const void* wuid) override;          // iface [0] [U body mapping]
    void  GetWuidForKey(void* out, const void* key) override;  // iface [1] [U body mapping]
    ~C_AreaUnionManager() override;                            // iface [2] [U body mapping]
    virtual void _vf3();                                       // appended own virtual [U role/mapping]

    void*    m_defaultRep08;   // +0x08  DynArray<T> head (WHDynStorage single ptr); empty -> &unk_18566BEB0 (game empty-prefix sentinel); elem stride 0x10. T [U]
    uint16_t m_id10;           // +0x10  ctor: 0xFFFF
    uint16_t m_id12;           // +0x12  ctor: 0xFFFF
    uint8_t  _pad14[4];        // +0x14
    uint64_t m_18;             // +0x18  ctor: 0 [U role]
    uint64_t m_20;             // +0x20  ctor: 0 [U role]
    uint8_t  m_byte28;         // +0x28  ctor: 0 [U role]
    uint8_t  _pad29[7];        // +0x29
    uint8_t  m_hashSet30[0x40]; // +0x30..+0x70  WH hash-set (float 1.0 @+0x34; mask 7 @+0x60, count 8 @+0x68) [U interior]
    uint8_t  m_sub70[0x40];    // +0x70..+0xB0  container (sub_1806030E4) [U interior]
};
static_assert(sizeof(C_AreaUnionManager) == 0xB0, "must be 0xB0 (alloc 176 at sub_1811A47C8)");

}  // namespace wh::xgenaimodule
