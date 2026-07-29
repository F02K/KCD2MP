#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_NPCManager -- NPC manager interface, primary base of
// C_NPCManager (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08.
// -----------------------------------------------
// RTTI TD rva 0x4F6E340. No COL/vtable of its own -- it shares C_NPCManager's
// primary vtable (mdisp 0). The split of the 38 primary slots between this
// interface's contract and C_NPCManager's own additions is UNRESOLVED; this mirror
// models the interface with only the [0] dtor and declares the remaining 37 primary
// slots on C_NPCManager.

namespace wh::xgenaimodule {

class I_NPCManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_NPCManager;
    virtual ~I_NPCManager();   // [0] (slot split beyond this UNRESOLVED)
};
static_assert(sizeof(I_NPCManager) == 0x08, "I_NPCManager is vptr-only");

}  // namespace wh::xgenaimodule
