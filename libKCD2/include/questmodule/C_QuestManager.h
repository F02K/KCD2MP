#pragma once
#include <cstdint>
#include "../conceptmodule/C_NodeManager.h"
#include "../framework/S_ResourceHandle.h"

// -----------------------------------------------
// wh::questmodule::C_QuestManager -- quest node registry (KCD2 1.5.6, kd7u).
// sizeof 0x78 (alloc 120).
// -----------------------------------------------
// Own vtable 0x183C27A90 (12 slots); ctor inlined in C_QuestModule::Init sub_18182B864
// (memset 0x78, vftable, 4x resource handles, +0x48/+0x50/+0x58 = 0; +0x60..+0x77 stays
// memset-only). Owned by C_QuestModule @+0x10; Init calls its vfunc[9] (+0x48).
// Debug cvars registered around it: wh_quest_ListQuests, wh_quest_ListQuestsPage,
// wh_quest_DebugQuestMarkers ("Select quest by ...").

namespace wh::questmodule {

class C_QuestManager : public conceptmodule::C_NodeManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_QuestManager;
    framework::S_ResourceHandle m_ref08;   // +0x08  (&unk_1856691D8) [target UNVERIFIED]
    framework::S_ResourceHandle m_ref18;   // +0x18  (&unk_1856691D8)
    framework::S_ResourceHandle m_ref28;   // +0x28  (&unk_1856691D8)
    framework::S_ResourceHandle m_ref38;   // +0x38  (&unk_1856691D8)
    std::vector<uint64_t> m_nodeEntries;   // +0x48  begin/last/end; push_back sub_181F42F20; 8-byte elem {uint32 nodeId@node+8; uint8 kind} [elem placeholder uint64_t]
    ICVar* m_cvarDebugQuestMarkers;        // +0x60  wh_quest_DebugQuestMarkers (RegisterString, init slot9 sub_18182B950)
    ICVar* m_cvarListQuests;               // +0x68  wh_quest_ListQuests (RegisterInt)
    ICVar* m_cvarListQuestsPage;           // +0x70  wh_quest_ListQuestsPage (RegisterInt)
};
static_assert(sizeof(C_QuestManager) == 0x78, "C_QuestManager must be 0x78 (alloc 120)");

}  // namespace wh::questmodule
