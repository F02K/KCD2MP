#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::framework -- C_BaseModule / I_ModuleMessageListener  (KCD2 WHGame.dll 1.5.6, kd7u)
// -----------------------------------------------
// Base of every Warhorse game module (C_PlayerModule, C_CombatModule, C_GUIModule, ...).
//
// [FUNDAMENTAL vs KCD1] C_BaseModule was flattened. KCD1 had the ancestry
// C_BaseModule : C_Graph : S_Graph : S_Job (0x88 bytes, four graph-topology
// std::vectors, a stored m_moduleId, and I_ModuleMessageListener at +0x78).
// In KCD2 the graph ancestry is gone: C_BaseModule shares one primary vtable
// with I_ModuleMessageListener (mdisp 0, at +0x00) and carries only m_state.
// Module id is now a compile-time constant returned by GetModuleId, not stored.
//
// C_BaseModule's OWN vtable (0x183A83EA8, guimodule-wave certification) has exactly
// 7 slots -- [0] dtor sub_182627210, [1..3] _purecall, [4] nullsub (Update default),
// [5..6] _purecall -- so anything at slot >= 7 in a concrete module vtable is that
// module's own addition (typically its RTTR_ENABLE() trio; C_GUIModule = [7..9],
// C_PlayerModule = [7] + trio [8..10]). An earlier revision of this header declared
// 11 slots with _vf7.._vf10 here; that was wrong and mis-sized every derived vtable.
//
// Slot roles (certified against C_GUIModule 0x183EEB9C8 and C_PlayerModule 0x183F74448):
//   [0] scalar-deleting dtor          [1] OnModuleMessage (I_ModuleMessageListener)
//   [2] Init(ctx&) -> bool            [3] Deinit  (module registrar sub_180C9DA44 calls
//   [4] Update (nullsub default)          [2] right after each ctor; C_GUIModule [3]
//   [5] GetModuleId (constant)            sub_182B8C864 is a full teardown, so the old
//   [6] GetModuleName                     "PostInit" reading of [3] was wrong)

namespace wh::framework {

// I_ModuleMessageListener -- framework message sink. Shares C_BaseModule's
// primary vtable in KCD2 (single-inheritance, subobject at +0x00).
class I_ModuleMessageListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ModuleMessageListener;
    virtual ~I_ModuleMessageListener() = default;         // [0]
    virtual void OnModuleMessage(void* msg) = 0;          // [1]
};

class C_BaseModule : public I_ModuleMessageListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BaseModule;
    virtual bool        Init(void* ctx) = 0;              // [2]  registrar passes &stack ctx (param type UNVERIFIED)
    virtual void        Deinit() = 0;                     // [3]  teardown (proven on C_GUIModule sub_182B8C864)
    virtual void        Update() = 0;                     // [4]  nullsub default in base vtable
    virtual int         GetModuleId() const = 0;          // [5]  KCD2 returns a constant
    virtual const char* GetModuleName() const = 0;        // [6]

    uint32_t m_state;        // +0x08  module state/flags (ctor inits 0)
    uint32_t _pad0C;         // +0x0C
};
static_assert(sizeof(C_BaseModule) == 0x10, "C_BaseModule must be 0x10 (vtable + m_state)");

}  // namespace wh::framework
