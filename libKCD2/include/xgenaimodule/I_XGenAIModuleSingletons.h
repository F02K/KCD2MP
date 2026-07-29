#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_XGenAIModuleSingletons -- AI singleton-hub getter contract
// (KCD2 WHGame.dll 1.5.6, kd7u).  Pure interface, 67 slots ([0] dtor + 66 getters).
// -----------------------------------------------
// RTTI TD rva 0x4FF3948; interface vtable 0x184012fb8 (all 67 slots _purecall).
// Implemented by C_XGenAIModuleSingletons (vtable 0x183a54af0), reached via
// C_XGenAIModule::m_pSingletons (+0xA8). THE per-frame AI service locator: every getter
// is a lazy module-global singleton accessor (DCL-construct via allocator fn-ptr
// qword_18549D378, plain field-return, or tail-jmp thunk).
//
// The `qword_…` global named per slot below is the directly-patchable singleton pointer
// (the cheat address book). Return types are modeled void* until each manager class is
// ported; Get* names on [V] slots are coined from the resolved singleton's RTTI type
// (names UNVERIFIED, slot->global->type mapping VERIFIED per G1_dossier.md §2.2).
// [U] slots are POD/value or constructed-elsewhere singletons -- types unresolved.

namespace wh::xgenaimodule {

class I_XGenAIModuleSingletons {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_XGenAIModuleSingletons;
    virtual ~I_XGenAIModuleSingletons();                  // [0]  dtor slot (_purecall here; sub_18342FC1C in hub)
    virtual void* GetAIObjectManager() = 0;               // [1]  sub_181A9DC40 -> qword_185481FC8 = C_AIObjectManager (0x110) [V -- G2 correction; FNV-1a-64 object map @+0x60]
    virtual void* GetNPCManager() = 0;                    // [2]  sub_181A9E180 -> qword_185494490 = C_NPCManager [V]
    virtual void* GetStealingHelper() = 0;                // [3]  sub_1808CF910 -> qword_1854960F0 = C_StealingHelper [V] * theft cheats
    virtual void* _vf04() = 0;                            // [4]  sub_1823CD9C4 -> qword_185496138 (16B POD) [U]
    virtual void* GetLinkNetworkQuerySystem() = 0;        // [5]  sub_1831AAC70 -> qword_1855AA178 = C_LinkNetworkQuerySystem [V]
    virtual void* _vf06() = 0;                            // [6]  sub_1804CCF4C -> qword_185496240 [U]
    virtual void* _vf07() = 0;                            // [7]  sub_1831AE49C -> qword_185496240 (same global as [6]) [U]
    virtual void* GetHearingSystem() = 0;                 // [8]  sub_1823CB3E4 -> qword_185493920 = C_HearingSystem [V] * noise/stealth
    virtual void* GetPerceptibleVolumeManager() = 0;      // [9]  sub_1823CD984 -> qword_185496210 = C_PerceptibleVolumeManager [U-likely]
    virtual void* _vf10() = 0;                            // [10] sub_1816B76C0 -> qword_185496248 [U]
    virtual void* GetTriggerAreaManager() = 0;            // [11] sub_1806239B0 -> qword_185496160 = C_TriggerAreaManager [V]
    virtual void* GetSmartEntityResolver() = 0;           // [12] sub_1823CD9EC -> qword_185496120 = C_SmartEntityResolver [V]
    virtual void* _vf13() = 0;                            // [13] sub_180B42940 -> qword_185493BB0 (core dep of [12]/[15]/[43]) [U]
    virtual void* GetAreaUnionManager() = 0;              // [14] sub_1807D2BAC -> qword_1854960E8 = C_AreaUnionManager [V]
    virtual void* GetAreaCallbackDispatch() = 0;          // [15] sub_1823CBD78 -> qword_185496108 = C_AreaCallbackDispatch [V]
    virtual void* _vf16() = 0;                            // [16] sub_181A9DC50 -> qword_185493D30 (also navmesh ctor dep) [U]
    virtual void* GetSmartEntityDatabase() = 0;           // [17] sub_1831ADCC8 -> qword_185496118 = C_SmartEntityDatabase [V]
    virtual void* GetDogPointManager() = 0;               // [18] sub_1806239A8 -> qword_185496200 = C_DogPointManager [V]
    virtual void* _vf19() = 0;                            // [19] sub_180622DC8 -> qword_185496230 [U]
    virtual void* _vf20() = 0;                            // [20] sub_1823CD99C -> qword_185496228 [U]
    virtual void* _vf21() = 0;                            // [21] sub_181AA1AE0 (no global write) [U]
    virtual void* _vf22() = 0;                            // [22] sub_1823D2BD0 -> qword_185496098 (48B POD) [U]
    virtual void* GetLODManager() = 0;                    // [23] sub_1823D3CE8 -> qword_185496178 = C_LODManager [V]
    virtual void* _vf24() = 0;                            // [24] sub_1831AA8F4 -> qword_1855AA158 (link family) [U]
    virtual void* _vf25() = 0;                            // [25] sub_1823CD994 -> qword_185496220 [U]
    virtual void* GetLuaDebugHelper() = 0;                // [26] sub_1831AAEB8 -> qword_1855AC3D8 = C_LuaDebugHelper [V]
    virtual void* GetSituationManager() = 0;              // [27] sub_180F492B0 -> qword_185493960 = C_SituationManager [V]
    virtual void* GetPerceptionManager() = 0;             // [28] sub_1823CB3DC -> qword_185492EA0 = C_PerceptionManager [V] * detection cheats
    virtual void* GetRecognizingThresholdManager() = 0;   // [29] sub_1831ACD98 -> qword_185492EA0 sibling = C_RecognizingThresholdManager [V] * recognition
    virtual void* _vf30() = 0;                            // [30] sub_181A9E170 -> qword_1854961B0 (FormationManager dep) [U]
    virtual void* _vf31() = 0;                            // [31] sub_181AA1AD0 -> qword_1854961A0 [U]
    virtual void* _vf32() = 0;                            // [32] sub_181AA1AC0 -> qword_185493118 [U]
    virtual void* _vf33() = 0;                            // [33] sub_181AA2080 -> qword_185493110 [U]
    virtual void* _vf34() = 0;                            // [34] sub_181AA2090 -> qword_185496678 [U]
    virtual void* _vf35() = 0;                            // [35] sub_181AA20B0 -> qword_185493970 [U]
    virtual void* _vf36() = 0;                            // [36] sub_180A22F40 -> qword_1854965D8 [U]
    virtual void* _vf37() = 0;                            // [37] sub_18047C060 -> qword_185496338 [U]
    virtual void* _vf38() = 0;                            // [38] sub_181AA20A0 -> qword_1854930D0 [U]
    virtual void* _vf39() = 0;                            // [39] sub_18047D100 -> qword_185493D38 [U]
    virtual void* GetIntermissionManager() = 0;           // [40] sub_18062310C -> qword_185492170 = C_IntermissionManager [V]
    virtual void* GetSpatialQueries() = 0;                // [41] sub_1808AF1A0 -> qword_1854927C0 = C_SpatialQueries [V]
    virtual void* GetLinkEditorValidator() = 0;           // [42] sub_1831AAC68 -> qword_1855AA170 = C_LinkEditorValidator [V]
    virtual void* GetSchedulerManager() = 0;              // [43] sub_1809AEC18 -> qword_185491EC8 = scheduler::C_SchedulerManager [V]
    virtual void* _vf44() = 0;                            // [44] sub_180626C54 -> qword_1854960C8 [U]
    virtual void* GetAITransformManager() = 0;            // [45] sub_1803EDDA8 -> qword_1854960C0 = C_AITransformManager [V]
    virtual void* _vf46() = 0;                            // [46] sub_180A30060 -> qword_1854960B8 [U]
    virtual void* _vf47() = 0;                            // [47] sub_18124DD64 -> qword_1854927E0 [U]
    virtual void* GetHelperAnimValidation() = 0;          // [48] sub_18067F3F8 -> qword_1854960E0 = C_HelperAnimValidation [V]
    virtual void* _vf49() = 0;                            // [49] sub_1831AAC24 -> qword_1855AC3E8 [U]
    virtual void* _vf50() = 0;                            // [50] sub_18086D9F0 (no global write) [U]
    virtual void* _vf51() = 0;                            // [51] sub_1814D8EC0 (no global write) [U]
    virtual void* GetFormationManager() = 0;              // [52] sub_1807D3640 -> qword_1854927D8 = movement::C_FormationManager [V]
    virtual void* GetCrimeResolver() = 0;                 // [53] sub_1823CD9E4 -> qword_185496110 = C_CrimeResolver [V] * crime cheat root (embeds C_CrimeLevelAggregator @+0x60)
    virtual void* GetAIKeyAccessManager() = 0;            // [54] sub_18070B31C -> qword_185496140 = C_AIKeyAccessManager [V] * key/access cheats
    virtual void* _vf55() = 0;                            // [55] sub_1823CD96C -> qword_1854927D0 (168B) [U]
    virtual void* GetRandomEventPerception() = 0;         // [56] sub_1831AD154 -> qword_1855AA168 = C_RandomEventPerception [V]
    virtual void* GetAIInventoryManager() = 0;            // [57] sub_1823D2BC8 -> qword_185494428 = C_AIInventoryManager [V]
    virtual void* GetPlayerStateHandler() = 0;            // [58] sub_1823CD9D4 -> qword_1854961D0 = NPCState::C_PlayerStateHandler [V] * player-state cheats
    virtual void* GetSleepwalkingTeleportManager() = 0;   // [59] sub_182069230 -> qword_1855AC3F0 = C_SleepwalkingTeleportManager [V]
    virtual void* _vf60() = 0;                            // [60] sub_1831ADD34 -> qword_1855AA160 (link family) [U]
    virtual void* _vf61() = 0;                            // [61] sub_1823C9E8C -> qword_185492360 (168B) [U]
    virtual void* GetLightSourceManager() = 0;            // [62] sub_1823CF334 -> qword_1854927C8 = C_LightSourceManager [V] * stealth darkness
    virtual void* _vf63() = 0;                            // [63] sub_181AA20C0 (no global write) [U]
    virtual void* _vf64() = 0;                            // [64] sub_1809C1674 -> qword_185492368 [U]
    virtual void* _vf65() = 0;                            // [65] sub_1823CE054 -> qword_185494408 (376B; +0 holds C_XGenAIModule+0x70 handle) [U]
    virtual void* _vf66() = 0;                            // [66] sub_1823CBA20 -> qword_185491010 (376B spatial grid, C_RegularGridCellDummy callbacks) [U]
};
static_assert(sizeof(I_XGenAIModuleSingletons) == 0x08, "I_XGenAIModuleSingletons is vptr-only");

}  // namespace wh::xgenaimodule
