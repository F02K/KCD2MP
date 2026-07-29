#pragma once
#include <cstdint>
#include "../framework/C_BaseModule.h"
#include "../game/S_GameContext.h"

// -----------------------------------------------
// wh::questmodule::C_QuestModule -- quest module spine (KCD2 1.5.6, kd7u).
// sizeof 0x20 (alloc 32).
// -----------------------------------------------
// Own vtable 0x183D189D0 (plain 7-slot module -- no RTTR trio); ctor inlined in the module
// registrar sub_180C9DA44 @0x180C9DF04; stored at S_GameContext +0x118. Slots:
// [1] OnModuleMessage sub_182AF2F30, [2] Init sub_18182B864 (called devirtualized by the
// registrar; builds C_QuestManager into +0x10 and touches the C_ActivityManager global via
// sub_181F435D0), [3] Deinit sub_182AF2EA4, [4] Update sub_180532FF8,
// [5] GetModuleId = 13 (sub_181A7D830), [6] GetModuleName = "QuestModule" (sub_181A71A70).

namespace wh::questmodule {

class C_QuestManager;

class C_QuestModule : public framework::C_BaseModule {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_QuestModule;
    C_QuestManager* m_pQuestManager;   // +0x10  Init alloc 120
    uint64_t        m_unk18;           // +0x18  ctor 0; no writer in Init sub_18182B864 / Deinit sub_182AF2EA4 /
                                       //        OnModuleMessage sub_182AF2F30 (C_ActivityManager is a separate global) [role UNRESOLVED -- likely pad to 0x20]

    [[nodiscard]] static C_QuestModule* GetInstance()
    {
        return game::S_GameContext::GetInstance()->m_pQuestModule;
    }
};
static_assert(sizeof(C_QuestModule) == 0x20, "C_QuestModule must be 0x20 (alloc 32)");

}  // namespace wh::questmodule
