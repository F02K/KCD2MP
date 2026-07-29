#pragma once

// -----------------------------------------------
// wh::conceptmodule::C_NodeManager -- concept-graph node-manager base (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Base of questmodule::C_QuestManager / C_ActivityManager and dialogmodule's
// data::C_DialogueDataManager (RTTI: single-inheritance, shared primary vtable, mdisp 0).
// Modeled as vptr-only: every analyzed derived ctor places its first own field at +0x08
// (all manager ctors are fully inlined, so the base/derived member split past the vptr is
// UNVERIFIED -- if C_NodeManager turns out to own the +0x08 registry handle seen in several
// managers, move that member here).

namespace wh::conceptmodule {

class C_NodeManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NodeManager;
    virtual ~C_NodeManager();   // [0]; slot set not enumerated per-manager
};
static_assert(sizeof(C_NodeManager) == 0x08, "C_NodeManager modeled as vptr-only (see header note)");

}  // namespace wh::conceptmodule
