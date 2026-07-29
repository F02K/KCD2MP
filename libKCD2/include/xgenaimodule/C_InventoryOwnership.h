#pragma once
#include "I_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_InventoryOwnership : I_Ownership -- the inventory ownership
// variant (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (create-site proven: tracked
// alloc 16, factory sub_180A30BB4).
// -----------------------------------------------
// RTTI TD rva 0x4F490A8; vtable 0x183A6C650, **18 slots** -- the only ownership
// branch that EXTENDS the 16-slot contract (adds [16]/[17]; roles [U]). Unlike the
// embedded variants this one is a HEAP object held BY POINTER at C_AIInventory+0xE0
// and swapped with delete-old semantics (sub_180A309EC @0x180A30AB1); the shop
// variant replaces it when the inventory's shop-data lookup (sub_180A30B08) hits.
// C_AIInventory's GetOwnership (slot 11 impl 0x181A76100 -- IDB FLIRT-misnamed
// "unknown_libname_83", do not trust the name) returns *(this+0xE0).
// Slot impls: [0] sub_180966ADC; [1] sub_180838AE0; [2] sub_18135D180;
// [3]/[4] sub_181AA56C0; [5]/[6] nullsub_1; [7] sub_180838AE0; [8] sub_18066CD10;
// [9] sub_181AA56C0; [10..12] nullsub_1; [13] sub_180838AE0; [14]/[15] sub_18041A6A0;
// [16] sub_180838AE0; [17] nullsub_1.

namespace wh::xgenaimodule {

class C_AIInventory;   // 0xE8 host (G6 wave)

class C_InventoryOwnership : public I_Ownership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InventoryOwnership;
    ~C_InventoryOwnership() override;
    bool _vf1() override;
    const framework::WUID* GetEffectiveOwner() override;   // [2] sub_18135D180
    const framework::WUID* GetBaseOwner() override;
    const framework::WUID* GetOverrideOwner() override;
    void SetBaseOwner(const framework::WUID& owner) override;
    void SetOwner(const framework::WUID& owner) override;
    bool _vf7() override;
    uint64_t GetExtraOwnerCount() override;
    const framework::WUID* GetExtraOwner(uint64_t i) override;
    void* IterateExtraOwners() override;
    bool RemoveExtraOwner(const framework::WUID& owner) override;
    void ClearAllOwners() override;
    bool HasOverrideOwner() override;
    bool SaveOverride(void* writer) override;
    bool LoadOverride(void* reader) override;
    virtual void _vf16();                                  // [16] sub_180838AE0 [U role]
    virtual void _vf17();                                  // [17] nullsub_1 [U role]

    C_AIInventory* m_inventory;   // +0x08  back-ref to the owning inventory [V]
};
static_assert(sizeof(C_InventoryOwnership) == 0x10, "C_InventoryOwnership must be 0x10");

}  // namespace wh::xgenaimodule
