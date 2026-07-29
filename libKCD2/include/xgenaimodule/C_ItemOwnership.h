#pragma once
#include "I_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_ItemOwnership : I_Ownership -- the item ownership variant,
// embedded at +0x60 of C_AIItem (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x18 (embed
// span proven by the C_AIItem ctor sub_18047BAEC; C_AIItem total 0x78).
// -----------------------------------------------
// RTTI TD rva 0x4F492A8; vtable 0x183FE6718, 16 slots. DIFFERENT SHAPE from
// C_Ownership (G2_supplement §3.10 correction): NO WUID sentinels -- the owner is
// resolved through the item-data row: {vptr, itemDataRow* = sub_181E3E090(module
// +0x168 map, &wuid), 0}. Slot impls: [0] sub_1832A8748; [1] sub_180838AE0;
// [2] sub_1808F5090; [3]/[4] sub_181AA56C0; [5]/[6] nullsub_1; [7] sub_180838AE0;
// [8] sub_18066CD10; [9] sub_181AA56C0; [10..12] nullsub_1; [13] sub_180838AE0;
// [14]/[15] sub_18041A6A0.

namespace wh::xgenaimodule {

class C_ItemOwnership : public I_Ownership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemOwnership;
    ~C_ItemOwnership() override;
    bool _vf1() override;
    const framework::WUID* GetEffectiveOwner() override;
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

    void* m_itemDataRow;   // +0x08  ctor 0x18047BB52: sub_181E3E090(module +0x168 handle-table, itemWUID) -> registry row (slot|gen lookup; row+0x30 == the item WUID, tag 0x02); row struct not RE'd [U pointee]
    void* m_unk10;         // +0x10  ctor: 0 [U role]
};
static_assert(sizeof(C_ItemOwnership) == 0x18, "C_ItemOwnership must be 0x18");

}  // namespace wh::xgenaimodule
