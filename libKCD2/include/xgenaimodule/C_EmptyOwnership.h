#pragma once
#include "I_Ownership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_EmptyOwnership : I_Ownership -- the process-wide "no ownership"
// null object (KCD2 WHGame.dll 1.5.6, kd7u).  Modeled vptr-only (0x08); true
// allocation size UNVERIFIED (constructed in place at a fixed global, never heap'd).
// -----------------------------------------------
// RTTI TD rva 0x4F490E8; vtable 0x183FE67A0, 16 slots (all stubs). The spine's
// default GetOwnership (slot 11, sub_18047AFF4) lazily constructs ONE instance at
// off_18492F5F8 (atexit sub_1821C69F0) and returns it for every non-ownable
// linkable -- "has no ownership" is a shared static object, never NULL.
// Cheat note: writing owners through THIS object would affect every non-ownable
// linkable; the real levers are the per-host embedded components.
// Slot impls: [0] sub_1832A871C (ICF-shared with the I_Ownership standalone vtable);
// [1] sub_180838AE0; [2..4] sub_181AA56C0; [5]/[6] nullsub_1; [7] sub_180838AE0;
// [8] sub_18066CD10; [9] sub_181AA56C0; [10..12] nullsub_1; [13] sub_180838AE0;
// [14]/[15] sub_18041A6A0.

namespace wh::xgenaimodule {

class C_EmptyOwnership : public I_Ownership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_EmptyOwnership;
    ~C_EmptyOwnership() override;
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
};
static_assert(sizeof(C_EmptyOwnership) == 0x08, "C_EmptyOwnership modeled vptr-only (in-place static @off_18492F5F8)");

}  // namespace wh::xgenaimodule
