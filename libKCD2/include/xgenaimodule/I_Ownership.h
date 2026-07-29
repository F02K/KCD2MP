#pragma once
#include <cstdint>
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::xgenaimodule::I_Ownership -- the 16-slot ownership component contract (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4F49EC0; standalone vtable 0x183A39528 ([0] sub_1832A871C real,
// [1..15] _purecall). Ownership is an EMBEDDED COMPONENT of host objects (never a
// base of spine classes); hosts expose it via spine vtable slot 11 (GetOwnership,
// vf +0x58) -- certified across all 16 audited host vtables (G2_supplement §6.2).
// Non-ownable hosts return the process-wide static C_EmptyOwnership, so callers can
// ALWAYS invoke these methods (never NULL). Slot names below follow the C_Ownership
// role table (G2_supplement §6.1); [1]/[7]/[10] roles UNVERIFIED.
// Cheat lever: SetOwner (slot 6) rewrites the override WUID @+0x10 of the concrete
// component; ClearAllOwners (slot 12) is a one-call total launder.

namespace wh::xgenaimodule {

class I_Ownership {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Ownership;
    virtual ~I_Ownership();                                           // [0]  sub_1832A871C
    virtual bool _vf1() = 0;                                          // [1]  C_Ownership: return 1 [U role]
    virtual const framework::WUID* GetEffectiveOwner() = 0;           // [2]  override if set, else base [V]
    virtual const framework::WUID* GetBaseOwner() = 0;                // [3]  [V]
    virtual const framework::WUID* GetOverrideOwner() = 0;            // [4]  [V]
    virtual void SetBaseOwner(const framework::WUID& owner) = 0;      // [5]  writes +0x08 [V]
    virtual void SetOwner(const framework::WUID& owner) = 0;          // [6]  writes the override +0x10 [V]
    virtual bool _vf7() = 0;                                          // [7]  C_Ownership: return 1 [U role]
    virtual uint64_t GetExtraOwnerCount() = 0;                        // [8]  [V]
    virtual const framework::WUID* GetExtraOwner(uint64_t i) = 0;     // [9]  [V]
    virtual void* IterateExtraOwners() = 0;                           // [10] begin/iterate extra owners [U signature]
    virtual bool RemoveExtraOwner(const framework::WUID& owner) = 0;  // [11] [V]
    virtual void ClearAllOwners() = 0;                                // [12] owner & override -> sentinel, vector cleared [V]
    virtual bool HasOverrideOwner() = 0;                              // [13] +0x10 != qword_185338430 [V]
    virtual bool SaveOverride(void* writer) = 0;                      // [14] [V]
    virtual bool LoadOverride(void* reader) = 0;                      // [15] [V]
};
static_assert(sizeof(I_Ownership) == 0x08, "I_Ownership is vptr-only");

}  // namespace wh::xgenaimodule
