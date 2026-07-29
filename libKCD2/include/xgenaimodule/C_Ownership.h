#pragma once
#include <vector>
#include "I_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_Ownership : I_Ownership -- the generic embedded ownership
// component (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x30.
// -----------------------------------------------
// RTTI TD rva 0x4F49B28; vtable 0x183A6BB50, 16 slots. NO standalone ctor exists --
// always constructed INLINE by 8 host ctors (G2_dossier §8.2 embed offsets:
// C_DynamicLinkableObjectWithOwnership/C_AISlot/C_AIInventoryOwner +0x60,
// C_SmartObjectDefault/C_NavigationSmartObject +0x1A0, C_TriggerArea +0xF8,
// C_AreaUnion +0xD8, C_SmartArea +0x240); dtor sub_180A2A158. Both WUIDs init to the
// no-owner sentinel qword_185338430 (RUNTIME-initialized global -- read at runtime,
// not a constant). Slot impls: [0] sub_1832A8814; [1] sub_18041A6A0; [2] sub_18047BA04;
// [3] sub_181A71D30; [4] sub_181A72EC0; [5] sub_181AA5700; [6] sub_181AA5710;
// [7] sub_18041A6A0; [8] sub_181A74AB0; [9] sub_18047C1A0; [10] sub_18047B058;
// [11] sub_1813B33B4 (linear search + memmove-erase sub_180708E64); [12] sub_180FFDCE0;
// [13] sub_1832A97A4; [14] sub_1832A9284; [15] sub_1832A9080.
// NPC-spawn owner cleanup sub_180BDADE4 drives slots 3/4/6 exactly as declared here.

namespace wh::xgenaimodule {

class C_Ownership : public I_Ownership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Ownership;
    ~C_Ownership() override;
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

    framework::WUID              m_owner;          // +0x08  base owner; init = sentinel qword_185338430
    framework::WUID              m_ownerOverride;  // +0x10  effective/override owner; init = sentinel
    std::vector<framework::WUID> m_extraOwners;    // +0x18  extra owners
};
static_assert(sizeof(std::vector<wh::framework::WUID>) == 0x18, "std::vector must be 0x18");
static_assert(sizeof(C_Ownership) == 0x30, "C_Ownership must be 0x30");

}  // namespace wh::xgenaimodule
