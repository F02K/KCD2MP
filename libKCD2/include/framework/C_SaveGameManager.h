#pragma once
#include <cstdint>
#include <vector>
#include "I_ReadinessTask.h"
#include "C_ReadinessObserver.h"
#include "C_CrySaveGameHelper.h"
#include "C_CryLoadGameHelper.h"
#include "S_SaveGameTypeSlot.h"
#include "S_SaveBuildStamp.h"
#include "E_SaveGameType.h"

// -----------------------------------------------
// wh::framework::C_SaveGameManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x230.
// -----------------------------------------------
// RTTI .?AVC_SaveGameManager@framework@wh@@ (TD 0x184A68730, COL 0x1841B3230)  vtable 0x183E190E8
// (6 slots: [0]/[1] nullsubs, [2] sub_1825BD8B4, [3] sub_181A76C10, [4] sub_181A76C30,
// [5] deleting dtor -> sub_1825BC4F0)  ctor sub_180793E24.
// NOT a free-standing singleton: allocated (alloc 0x230 @0x180C10E9A) and owned by
// C_PlayerProfileWHManager (ctor sub_180C10E30) at ITS +0x48 (the profile manager's own
// global/root address was not resolved -- honest gap).
//
// === API / HOOK SURFACE ===
//   0x180794928  CreateSaveGame(this, uint8 type, int index, const char* name) -> bool
//                [builds C_SaveGameDescription, serializes world into a 0x8000-grow/40MB-cap
//                 buffer (sub_180794E64), computes slot (sub_1825BD6C0), writes the .whs file
//                 (sub_1825BD378), registers in the slot list (sub_1806E4F0C)]
//   0x180794264  LoadSaveGame(this, reader**, dst**, flags) -> bool
//                [chunk stream parser; chunk ids: 500 container, 501 description
//                 (sub_180C3DB48), 503 CryEngine data, 505 WH data (sub_1807944E4)]
//   0x1825BD8D8  LoadCryEngineData   0x1825BDA54  PostLoadGame
//   0x1806E5274  UpdateSaveGameDescriptions [enumerates "*.whs", rebuilds m_slotsByType]
//   0x180FC32F0  C_PlayerProfileWHManager::commitQueuedSave [queued type/index/name at profile
//                mgr +0xB0/+0xB4/+0xB8; save-in-progress guard byte_18547EB80]
// Globals: qword_18547BD38 = m_pSaveHelper mirror, qword_18547BD40 = m_pLoadHelper mirror.
// CVars: wh_sys_GameSaveName / GameSaveId / GameSaveInstallCompleted / GameSaveForceLoadingError
// / LastLoadedSave.

namespace wh::shared { class I_InputStream; }

namespace wh::framework {

class C_SaveGameManager : public wh::I_ReadinessTask   // +0x00  (0x8; : I_ReadinessDebuggable)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SaveGameManager;
    // The readiness chain is dtor-less (see I_ReadinessTask.h); this class introduces the
    // virtual dtor itself, landing the deleting dtor at slot [5] of its primary vtable.
    virtual ~C_SaveGameManager();          // [5]  deleting dtor sub_1825BC4F0
    // 0x180794928 (see hook surface above); index < 0 resolves the slot by name.
    bool CreateSaveGame(E_SaveGameType::Type type, int32_t index, const char* name);

    S_SaveGameTypeSlot m_slotsByType[5];   // +0x008  (5 x 0x48) per-type save lists; indexed by
                                           //         the active slot selector (this + 0x48*idx)
    C_CrySaveGameHelper* m_pSaveHelper;    // +0x170  (mirrored to qword_18547BD38)
    C_CryLoadGameHelper* m_pLoadHelper;    // +0x178  (mirrored to qword_18547BD40)
    int32_t  m_forceInstallCompleted;      // +0x180  cvar wh_sys_GameSaveInstallCompleted (-1)
    int32_t  m_forceLoadingError;          // +0x184  cvar wh_sys_GameSaveForceLoadingError (-1;
                                           //         2 = force cry-data error, 1 = desc error)
    int32_t  m_activeSlotType;             // +0x188  m_slotsByType selector: 5-bucket save CATEGORY
                                           //         index 0..4 (sub_1806E5274 loop), NOT E_SaveGameType
    int32_t  m_field18C;                   // +0x18C  init 0x7FFFFFFF (INT_MAX sentinel); passed as arg5
                                           //         to the description populate sub_181E21850 (id/sort key)
    uint8_t  m_flag190;                    // +0x190  set 1 while inside LoadCryEngineData
    uint8_t  _pad191[7];                   // +0x191
    wh::C_ReadinessObserver m_afterGameLoadObserver;  // +0x198  (0x70) watches "AfterGameLoad"
                                           //         (built by sub_180B3FF14)
    uint64_t m_field208;                   // +0x208  (ctor 0; role unresolved)
    wh::shared::I_InputStream* m_pendingModuleMsg;  // +0x210  owning; I_InputStream base (+0x08) of a heap
                                           //         C_FragmentedMemoryIOStream (pending checkpoint save buffer, built
                                           //         by sub_180794C08); set in type==6 branch (sub_180794BD0 stores
                                           //         stream+8); deleted via vtable[0] deleting-dtor in dtor sub_1825BC4F0
    std::vector<S_SaveBuildStamp> m_buildStamps;  // +0x218  (0x18) build stamps of the loaded save
};
static_assert(sizeof(C_SaveGameManager) == 0x230, "C_SaveGameManager must be 0x230");
static_assert(offsetof(C_SaveGameManager, m_slotsByType) == 0x08, "slots at 0x08");
static_assert(offsetof(C_SaveGameManager, m_afterGameLoadObserver) == 0x198, "observer at 0x198");
static_assert(offsetof(C_SaveGameManager, m_buildStamps) == 0x218, "build stamps at 0x218");

}  // namespace wh::framework
