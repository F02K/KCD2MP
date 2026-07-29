#pragma once
#include <cstdint>
#include <unordered_map>
#include "I_RespawnManager.h"
#include "../Offsets/vtables/IMergeMeshStreamListener.h"
#include "../Offsets/vtables/IConsole.h"   // Offsets::ICVar

// -----------------------------------------------
// wh::environmentmodule::C_RespawnManager -- herb/item respawn grid manager
// (KCD2 1.5.6, kd7u).  sizeof 0x1C8 (alloc 456).
// -----------------------------------------------
// Vtables 0x183AB92A8 (+0x00, I_RespawnManager) / 0x183AB9650 (+0x08,
// IMergeMeshStreamListener, RTTI mdisp 0x08); ctor inlined in C_EnvironmentModule::Init
// sub_180E3C398; dtor sub_182ADCCDC. Owned by C_EnvironmentModule @+0x80.
// Cheat surface = the eight cvars below (wh_env_RespawnDistance / RespawnPickDistance /
// RespawnHerbBboxScale / ...); the grid containers are internal bookkeeping.
// The +0xA0 member is signature-matched MSVC std::unordered_* (load-factor 1.0f @+0xA0,
// list @+0xA8, bucket vec init sub_1803B5774(+0xB8), mask 7 @+0xD0, maxidx 8 @+0xD8);
// key/value types UNVERIFIED (could be a set).

namespace wh::environmentmodule {

class C_RespawnManager : public I_RespawnManager, public Offsets::IMergeMeshStreamListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RespawnManager;
    Vec3     m_lastRaycastPos;                // +0x10  last raycast target (writer 0x1809F2DC3; read as 3 floats vs wh_env_RespawnRaycastUpdateDelta @+0x58)
    uint8_t  _pad1C[4];                        // +0x1C  alignment pad (ctor memset covers only +0x10..+0x1C)
    ICVar*         m_cvarRespawnDistance;            // +0x20  wh_env_RespawnDistance
    ICVar*         m_cvarRespawnDebug;               // +0x28  wh_env_RespawnDebug
    ICVar*         m_cvarRespawnDebugIntervalSec;    // +0x30  wh_env_RespawnDebugIntervalSec
    ICVar*         m_cvarRespawnPickDistance;        // +0x38  wh_env_RespawnPickDistance
    ICVar*         m_cvarGridInsertDistanceSquared;  // +0x40  wh_env_GridInsertDistanceSquared
    ICVar*         m_cvarTypicalHerbHeight;          // +0x48  wh_env_RespawnTypicalHerbHeight
    ICVar*         m_cvarRespawnHerbBboxScale;       // +0x50  wh_env_RespawnHerbBboxScale
    ICVar*         m_cvarRaycastUpdateDelta;         // +0x58  wh_env_RespawnRaycastUpdateDelta
    uint8_t  m_container60[0x40];             // +0x60  sub_180E3C63C container [shape not walked]
    std::unordered_map<uint64_t, uint64_t> m_mapA0;   // +0xA0  [element types UNVERIFIED]
    std::vector<std::array<uint8_t, 0x20>> m_vecE0;   // +0xE0  MSVC std::vector{begin@+0xE0,end@+0xE8,cap@+0xF0}; elem stride 0x20 proven; ELEMENT TYPE UNVERIFIED
    uint8_t  m_gridF8[0x40];                  // +0xF8   respawn-grid container 1 (sub_18043C878;
    uint8_t  m_grid138[0x40];                 // +0x138  respawn-grid container 2  0x40 footprint,
    uint8_t  m_grid178[0x40];                 // +0x178  respawn-grid container 3  shape not walked)
    uint8_t  m_flag1B8;                       // +0x1B8  ctor 0
    uint8_t  _pad1B9[3];                      // +0x1B9
    int32_t  m_param1BC;                      // +0x1BC  ctor -1 [role UNVERIFIED]
    uint8_t  m_flag1C0;                       // +0x1C0  ctor 0
    uint8_t  _pad1C1[7];                      // +0x1C1
};
static_assert(sizeof(C_RespawnManager) == 0x1C8, "C_RespawnManager must be 0x1C8 (alloc 456)");
static_assert(offsetof(C_RespawnManager, m_cvarRespawnDistance) == 0x20, "cvar block at +0x20");

}  // namespace wh::environmentmodule
