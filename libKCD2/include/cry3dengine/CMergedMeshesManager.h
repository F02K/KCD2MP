#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include <unordered_set>
#include "../Offsets/vtables/IMergedMeshesManager.h"
#include "Cry3DEngineBase.h"
#include "CMergedMeshRenderNode.h"
#include "SProjectile.h"
#include "SInstanceSector.h"

// -----------------------------------------------
// CMergedMeshesManager -- merged-vegetation (MMRM) manager singleton,
// KCD2 WHGame.dll 1.5.6 (e4cp). sizeof 0xC208.
// -----------------------------------------------
// RTTI bases: Cry3DEngineBase @+0x8 (EBO, shares with m_Nodes), IMergedMeshesManager @+0x0
// (vtable 0x183C24CB8, 23 slots, no new virtuals -- see Offsets/vtables/IMergedMeshesManager.h).
// sizeof 0xC208: ctor-site alloc `sub_1804F75C0(0xC208)` @0x180a66700 + deleting dtor
// `operator delete(this, 0xC208)` @0x1834db15d. SINGLETON: qword_185496F48 (written in
// C3DEngine::C3DEngine sub_180A66278; readable via I3DEngine slot 209 -> 0x180E3C710).
// Ctor sub_18180D814, dtor sub_1834DA6C8, Init 0x1807959E0 (creates the MERGEDMESH_POOL heap
// of (e_MergedMeshesPool + 0x1000) KiB into qword_18560C618), Shutdown 0x1812B501C,
// Update 0x1804A7E2C, AddInstance (non-virtual!) 0x1834DB738, GetNode 0x1805DE028,
// PreloadMeshes 0x1804D40F8 ("terrain\merged_meshes_sectors\mmrm_used_meshes.lst").
// Spatial hash (proven at 5 sites): cell = floor(fabs(pos.xyz) / 16);
// bucket = m_Nodes[cx & 31][cy & 31][cz & 1]. CompileAreas scans the 3x3x3 neighbourhood.
// Full member/slot evidence: analysis/mesh_engine_re/mergedmesh.md §1.

class CMergedMeshesManager : public Cry3DEngineBase, public Offsets::IMergedMeshesManager {
public:
    enum : uint32_t {
        MMRM_HASH_DIM_XY          = 32,     // x/y hash buckets
        MMRM_HASH_DIM_Z           = 2,      // z hash buckets
        MMRM_CLUSTER_SIZE         = 16,     // metres per cell (the 1/16 in the hash)
        MMRM_MAX_PROJECTILES      = 0x400,  // AddProjectile cap (sub_1834DBBB8)
        MMRM_MAX_SAMPLES_PER_GROUP = 0x400, // AddInstance per-group cap (also 0xFFFF/vertexCount bound)
    };

    // +0x0008  0xC000-byte spatial hash grid: 2048 vectors, indexed [cx&31][cy&31][cz&1]
    // (iteration strides 0x600/0x30/0x18 in CompileSectors prove the [x][y][z] order;
    // {begin,end,cap} triple proven -> std::vector, NOT DynArray/boost)
    std::vector<CMergedMeshRenderNode*> m_Nodes[MMRM_HASH_DIM_XY][MMRM_HASH_DIM_XY][MMRM_HASH_DIM_Z];
    std::vector<CMergedMeshRenderNode*> m_AllNodes;         // +0xC008  master list; every new node push_back'd here
    std::vector<CMergedMeshRenderNode*> m_NodesListB;       // +0xC020  counted into +0xC0E0 [role guess: visible/streamed]
    std::vector<CMergedMeshRenderNode*> m_ActiveNodes;      // +0xC038  Update's simulate list; erased in place when a node drops resources
    std::vector<CMergedMeshRenderNode*> m_PostRenderNodes;  // +0xC050  per-frame update/deform list, guarded by m_UpdateLock [role guess]
    std::vector<SProjectile>            m_Projectiles;      // +0xC068  bullet disturbances (stride 0x38, cap 1024, spinlock below)
    DynArray<SInstanceSector>           m_InstanceSectors;  // +0xC080  compiled sectors (empty sentinel 0x18560CC38); slots 1..4
    volatile int32_t m_ProjectileLock = 0;                  // +0xC088  spinlock: CAS(0x10000,0) writer / ++ & 0x10000 reader
    uint8_t          _padC08C[4] = {};                      // +0xC08C
    void*            m_sortJobState = nullptr;              // +0xC090  "MMRM_SortActiveInstances" SJobState handle [identity = inference; ctor sub_18078BDEC]
    // ---- stats counters. Only +0xC0B0 has a proven identity; the rest are named by offset
    // because their writers were never located (dossier §6.3) -- do NOT trust stock stat names.
    size_t m_statC098 = 0;                                  // +0xC098  reset each Update; slot 9 returns C0A0+C098 (=> VRAM pair)
    size_t m_statC0A0 = 0;                                  // +0xC0A0  reset each Update; other half of slot 9's sum
    size_t m_statC0A8 = 0;                                  // +0xC0A8  reset each Update; returned by slot 10
    size_t m_GeomSizeInMainMem = 0;                         // +0xC0B0  = qword_18533C220 (geometry-cache size); slot 11 -- the one PROVEN stat
    size_t m_statC0B8 = 0;                                  // +0xC0B8  reset each Update; returned by slot 14
    size_t m_statC0C0 = 0;                                  // +0xC0C0  reset each Update; returned by slot 15
    size_t m_statC0C8 = 0;                                  // +0xC0C8  persistent accumulator; returned by slot 12
    size_t m_statC0D0 = 0;                                  // +0xC0D0  persistent accumulator; returned by slot 13
    size_t m_nTotalNodes = 0;                               // +0xC0D8  = m_AllNodes.size(), refreshed in Update
    size_t m_nNodesListB = 0;                               // +0xC0E0  = m_NodesListB.size()
    size_t m_nStreamingTasksInProgress = 0;                 // +0xC0E8  count of eState_Streaming nodes; compared to e_MergedMeshesMaxStreamingInProgress
    bool   m_PoolOverFlow = false;                          // +0xC0F0  spine/instance pool budget exhausted; consumed+cleared at top of Update [stock name, behaviour match]
    bool   m_bMeshesPreloaded = false;                      // +0xC0F1  set by PreloadMeshes, cleared by Shutdown; gates SyncPreparationStep [stock name: m_MeshListPresent]
    uint8_t _padC0F2[2] = {};                               // +0xC0F2
    uint8_t _unkC0F4[0x60] = {};                            // +0xC0F4  4 x 0x18 POD array (ctor sub_1805F5DA0 then memset 0); NOT the projectile array; unidentified
    uint8_t _unkC154[0x34] = {};                            // +0xC154  ctor-zeroed block (3 OWORDs; 4-byte hole @+0xC164 never written)
    int64_t m_savedCullingLodRatioWH = -1;                  // +0xC188  Shutdown restores e_MergedMeshesCullingLodRatioWH from it (if != -1)
    int64_t m_savedViewDistRatio = -1;                      // +0xC190  Shutdown restores e_MergedMeshesViewDistRatio
    void*   m_UpdateLock = nullptr;                         // +0xC198  SRWLOCK guarding m_PostRenderNodes (InitializeSRWLock @0x18180d96c)
    uint64_t _unkC1A0 = 0;                                  // +0xC1A0
    // +0xC1A8  deferred-registration set, keyed by node pointer (erase-by-node in teardown
    // sub_18043BDC4). MSVC _Hash shape proven (0x40: mlf 1.0f, list, bucket vec, mask 7,
    // maxidx 8); MAY be an unordered_map -- the mapped type, if any, is unknown.
    std::unordered_set<CMergedMeshRenderNode*> m_DeferredNodes;
    bool    m_DeferredRegistration = false;                 // +0xC1E8  slots 19/20; when turned off, drains the set through slot 21
    uint8_t _padC1E9[7] = {};                               // +0xC1E9
    std::vector<Offsets::IMergeMeshStreamListener*> m_StreamListeners;  // +0xC1F0  slot 16 push_back / slot 17 erase-remove [element type = project-memory hypothesis, not RTTI-confirmed]
};
static_assert(sizeof(CMergedMeshesManager) == 0xC208, "alloc @0x180a66700 + operator delete(this,0xC208) @0x1834db15d");
static_assert(offsetof(CMergedMeshesManager, m_Nodes) == 0x08, "hash grid @+0x08 (Cry3DEngineBase EBO)");
static_assert(offsetof(CMergedMeshesManager, m_AllNodes) == 0xC008, "master list @+0xC008");
static_assert(offsetof(CMergedMeshesManager, m_Projectiles) == 0xC068, "projectiles @+0xC068");
static_assert(offsetof(CMergedMeshesManager, m_InstanceSectors) == 0xC080, "compiled sectors @+0xC080");
static_assert(offsetof(CMergedMeshesManager, m_nStreamingTasksInProgress) == 0xC0E8, "streaming count @+0xC0E8");
static_assert(offsetof(CMergedMeshesManager, m_UpdateLock) == 0xC198, "SRWLOCK @+0xC198");
static_assert(offsetof(CMergedMeshesManager, m_DeferredNodes) == 0xC1A8, "deferred set @+0xC1A8");
static_assert(offsetof(CMergedMeshesManager, m_StreamListeners) == 0xC1F0, "listeners @+0xC1F0");
