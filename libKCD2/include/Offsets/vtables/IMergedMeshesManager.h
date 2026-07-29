#pragma once
#include <cstdint>
#include <functional>
#include "IMergeMeshStreamListener.h"
#include "../../cry3dengine/SInstanceSector.h"
#include "../../cry3dengine/SMeshAreaCluster.h"

// -----------------------------------------------
// IMergedMeshesManager -- merged-mesh (vegetation instancing) manager interface,
// KCD2 binary slot order (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct (RTTI .?AUIMergedMeshesManager@@). 23 slots certified from BOTH sides: impl vtable
// 0x183C24CB8 (slot 23 is ASCII data) and the standalone default vtable 0x18402A3B8
// (1 dtor + 22 _purecall) -- CMergedMeshesManager adds NO virtuals of its own.
// LIVE ACCESS: I3DEngine slot 209 (+0x688) -> impl 0x180E3C710 -> singleton global
// qword_185496F48 (readable directly).
//
// Interfuscator-shuffled vs the (older-vintage) SDK header -- this order is authoritative for
// this binary only. Notably absent vs GameSDK: PrepareSegmentData. Slot [7] is an EMPTY STUB
// in retail; its CalculateDensity identity rests on the CRuntimeAreaManager::CreateAreas
// call-site pairing only. Stats slots [10]/[12..15] return distinct counters whose writers
// were not located -- kept as offset-named getters, do not trust stock stat names.
// Evidence: analysis/mesh_engine_re/mergedmesh.md §1.2-1.3.

class CMergedMeshRenderNode;

namespace Offsets {

struct IRenderNode;

struct IMergedMeshesManager {
    virtual ~IMergedMeshesManager() = default;                                 // [0]
    virtual bool CompileSectors() = 0;                                         // [1] fills the member DynArray @+0xC080; KCD2 takes NO out-param (unlike GameSDK) [arity caveat: unused rdx not re-checked]
    virtual SInstanceSector* GetCompiledSector(int idx) = 0;                   // [2] &sectors[idx], stride 0x10
    virtual int GetCompiledSectorCount() = 0;                                  // [3] DynArray count at ptr-4
    virtual void ClearCompiledSectors() = 0;                                   // [4]
    virtual bool CompileAreas(DynArray<SMeshAreaCluster>& clusters, int flags) = 0;  // [5] flags: 2 = CLUSTER_CONVEXHULL_GRAHAMSCAN, 4 = GIFTWRAP (log-string named)
    virtual size_t QueryDensity(const Vec3& pos, void* pSurfaceTypesOut, void* pDensityOut) = 0;  // [6] out-param types unresolved (stock: ISurfaceType*[N] + float[N])
    virtual void CalculateDensity() = 0;                                       // [7] EMPTY STUB in retail (nullsub_1); identity = CreateAreas call-site pairing [UNVERIFIED]
    virtual bool GetUsedMeshes(DynArray<CryStringT<char>>& outMeshNames) = 0;  // [8] CGF filenames from the global SMMRMGeometry list; always returns true
    virtual size_t CurrentSizeInVram() = 0;                                    // [9] = [+0xC0A0] + [+0xC098] (both reset per Update)
    virtual size_t _statC0A8() = 0;                                            // [10] per-frame counter; stock name unassigned (writers not located)
    virtual size_t GeomSizeInMainMem() = 0;                                    // [11] = [+0xC0B0] <- global geometry-cache size qword_18533C220
    virtual size_t _statC0C8() = 0;                                            // [12] persistent accumulator; name unassigned
    virtual size_t _statC0D0() = 0;                                            // [13] persistent accumulator; name unassigned
    virtual size_t _statC0B8() = 0;                                            // [14] per-frame counter; name unassigned
    virtual size_t _statC0C0() = 0;                                            // [15] per-frame counter; name unassigned
    virtual void RegisterStreamListener(IMergeMeshStreamListener* pListener) = 0;    // [16] push_back into +0xC1F0 [listener type per project memory (C_RespawnManager base), not re-confirmed here]
    virtual void UnregisterStreamListener(IMergeMeshStreamListener* pListener) = 0;  // [17] erase-remove on the same vector
    virtual void ForEachNode(std::function<void(CMergedMeshRenderNode*&)> fn) = 0;   // [18] iterates the master node list +0xC008; fn BY VALUE (callee-destroyed)
    virtual bool GetDeferredRegistration() = 0;                                // [19] byte +0xC1E8
    virtual void SetDeferredRegistration(bool bDeferred) = 0;                  // [20] turning OFF drains the deferred map via [21]
    virtual void RegisterEntityDeferred(IRenderNode* pNode) = 0;               // [21] deferred: map insert; immediate: C3DEngine vf+0x178 then vf+0x130(node)
    virtual void ResetAllNodes() = 0;                                          // [22] waits jobs, drops render resources, state -> DIRTY
};
static_assert(sizeof(IMergedMeshesManager) == 0x8, "vptr-only interface");

}  // namespace Offsets
