#pragma once
#include <cstdint>
#include "I_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_NoOwnerOwnership : I_Ownership -- the "always reports no owner"
// ownership variant embedded in every C_NPC at +0x4A8 (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x20 (embed span proven by the C_NPC ctor: next member at +0x4C8).
// -----------------------------------------------
// RTTI TD rva 0x4F49268; vtable 0x183A836F0, 16 slots. Constructed inline by
// sub_180BDCB1C: {vptr, 0, 0, 0}. C_NPC's GetOwnership (spine slot 11,
// sub_180708E50) returns this subobject -- NPCs are ownership-capable but always
// report "no owner". Slot impls: [0] sub_1832A87E0; [1] sub_180838AE0;
// [2..4] sub_181AA56C0 (shared); [5]/[6] nullsub_1; [7] sub_18041A6A0;
// [8] sub_181AA56D0; [9] sub_180BD9CE0; [10] sub_1813B33A8; [11] sub_180708E58;
// [12] sub_181377F44; [13] sub_180838AE0; [14]/[15] sub_18041A6A0.

namespace wh::xgenaimodule {

class C_NoOwnerOwnership : public I_Ownership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NoOwnerOwnership;
    ~C_NoOwnerOwnership() override;
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

    std::vector<framework::WUID> m_extraOwners;   // +0x08  {first,last,end}; extra owners only (getters return no-owner sentinel, setters are no-ops)
};
static_assert(sizeof(C_NoOwnerOwnership) == 0x20, "C_NoOwnerOwnership must be 0x20");

}  // namespace wh::xgenaimodule
