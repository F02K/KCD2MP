#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../Offsets/vtables/IRenderNode.h"
#include "../Offsets/vtables/IMergedMeshRenderNode.h"
#include "../Offsets/vtables/IStreamCallback.h"
#include "Cry3DEngineBase.h"
#include "SMMRMGroupHeader.h"

// -----------------------------------------------
// CMergedMeshRenderNode -- merged-vegetation sector node (one per 16m cell),
// KCD2 WHGame.dll 1.5.6 (e4cp). sizeof 0x2E0.
// -----------------------------------------------
// Bases (RTTI + ctor 0x1805DE2D0 vptr stores, re-verified by decompile): IRenderNode @+0x00
// (vtable 0x18402A508, 73 slots -- adds NO primary virtuals), IMergedMeshRenderNode @+0x50
// (0x18402A780, 3 slots), IStreamCallback @+0x58 (0x18402A758, 4 slots), Cry3DEngineBase
// @+0x60 (EBO -- the ctor writes the first member dword AT +0x60, 0x1805de384).
// sizeof 0x2E0: pool alloc @0x1834db8da (MERGEDMESH_POOL) + factory `sub_1804F75C0(0x2E0)`
// @0x180a79603; last ctor write +0x2D8. Ctor tail: SetViewDistRatio(255) via primary slot 59,
// m_fWSMaxViewDist = FLT_MAX, flags |= 0x40000008 (+ ERF_OUTDOORONLY when
// e_MergedMeshesOutdoorOnly). ERType 23 (stub 0x181A7D870 `mov eax,17h`).
// Dtor 0x18043BC68 (deleting 0x18043BC24 -> pool free); teardown sub_18043BDC4 removes the
// node from five manager containers. Ctor/dtor counter dword_185188E6C.
// Sector stream file = "pvrn", chunk magic 0xCAFEBAB6 (StreamAsyncOnComplete 0x180767A48;
// file instances are 12 bytes, in-memory SMMRMInstance is 0x10 -- WH added scaleOriginal).
// Full member/lifecycle evidence: analysis/mesh_engine_re/mergedmesh.md §2.

class CMergedMeshRenderNode : public Offsets::IRenderNode,
                              public Offsets::IMergedMeshRenderNode,
                              public Offsets::IStreamCallback,
                              public Cry3DEngineBase {
public:
    // +0x6C lifecycle (all transitions behaviour-anchored; value 3's name is positional inference)
    enum E_State : int32_t {
        eState_Initial    = 0,   // ctor
        eState_Dirty      = 1,   // Setup / AddInstance / RemoveRenderResources; Render+Update react by calling Prepare
        eState_Preparing  = 2,   // Prepare (sub_180492DF0) stage 1
        eState_Prepared   = 3,   // streamable; StreamIn (sub_180B968D4) requires it, StreamOut returns to it [name = positional inference, no string]
        eState_Streaming  = 4,   // set by StreamIn; both stream callbacks demand it; tallied into mgr+0xC0E8
        eState_StreamedIn = 5,   // renderable; all 3 IMergedMeshRenderNode slots require it
        eState_Error      = 6,   // bad chunk magic / state-check failure; Render logs "MergedMeshRendernode is in an Error state"
    };

    // +0xE0 node flags -- only these bits have observed meaning (ctor mask 0xFFFFFC40)
    enum : uint32_t {
        eNodeFlag_CreatedByManager = 0x1,          // set on manager-created nodes (0x1834db9b8 / 0x1805de27c)
        eNodeFlag_Unk02            = 0x2,          // cleared when render resources are dropped (0x1804a8207)
        eNodeFlag_Unk40            = 0x40,         // set unconditionally by the ctor
        eNodeFlag_Active           = 0x100,        // "activated"; toggling it fires primary vtable slot 68
        eNodeFlag_BlockSpineInit   = 0x200,        // blocks the InitializeSpines job dispatch (0x1804a83cc)
        eNodeFlag_UnkSignBit       = 0x80000000u,  // checked before deactivation (0x1821f34d9); meaning unknown
    };

    enum : uint32_t { ePVRNChunkMagic = 0xCAFEBAB6 };  // sector stream ("pvrn") chunk magic

    // element of the +0x2B8 vector -- only the 0x10 stride is proven (dtor frees (cap-begin)&~0xF)
    struct SUnkElem10 { uint8_t _raw[0x10]; };

    int32_t             m_lastDrawFrameId = 0;      // +0x60  (shares +0x60 with the empty base) Update compares to passInfo+0xC to activate/deactivate
    int32_t             _unk64 = 0;                 // +0x64  Render takes &this->_unk64 as a cursor (0x1804a0833)
    int32_t             _unk68 = 0;                 // +0x68
    E_State             m_State = eState_Initial;   // +0x6C  lifecycle state machine (§2.4)
    uint64_t            m_nInstances = 0;           // +0x70  total instances in this node (++ per AddInstance; Update costs 0x10 * count)
    int64_t             _unk78 = -1;                // +0x78
    int64_t             _unk80 = -1;                // +0x80
    int32_t             _unk88 = 0;                 // +0x88
    AABB                m_internalAABB;             // +0x8C  sector bounds (16m cell); AddInstance quantises samples against .min
    AABB                m_visibleAABB;              // +0xA4  fminf/fmaxf-grown over every instance's geometry AABB
    Vec3                m_pos;                      // +0xBC  sector centre = (min+max)*0.5; the manager hash + RemoveFromHash key
    Vec3                m_initPos;                  // +0xC8  second copy of the same centre (Setup writes both)
    uint32_t            _unkD4 = 0;                 // +0xD4  read in Render (0x1804a0da4); int-or-float shape unknown
    int32_t             m_nLodA = -1;               // +0xD8  LOD index; reset to -1 by Prepare
    int32_t             m_nLodB = -1;               // +0xDC  LOD index; reset to -1 by Prepare
    uint32_t            m_nodeFlags = eNodeFlag_Unk40;  // +0xE0  bits above; ctor does [+0xE0] = ([+0xE0] & 0xFFFFFC40) | 0x40
    int32_t             m_renderMode = 2;           // +0xE4  Render switches on it: 0 / 1 / else
    int32_t             _unkE8 = 0;                 // +0xE8
    bool                _unkEC = false;             // +0xEC
    uint8_t             _padED[3] = {};             // +0xED
    SMMRMGroupHeader*   m_groups = nullptr;         // +0xF0  per-group header array, stride 0x80; grown by sub_1834D9C08
    uint32_t            m_nGroups = 0;              // +0xF8
    uint32_t            _unkFC = 0;                 // +0xFC  (covered by the ctor's qword store over 0xF8)
    uint8_t             _unk100[0x60] = {};         // +0x100 memset(this+0x100, 0, 0xC0) block @0x1805de444...
    float               m_unk160 = 1.0f;            // +0x160 ...with this float explicitly set to 1.0f after the memset
    uint8_t             _unk164[0x52] = {};         // +0x164
    bool                m_unk1B6 = true;            // +0x1B6 ...and this bool explicitly set after the memset
    uint8_t             _pad1B7[9] = {};            // +0x1B7 remainder of the memset block, up to +0x1C0
    void*               m_unkWind1C0 = nullptr;     // +0x1C0 wind/deform sampling target: sub_180A3E634(this+0x1C0, this+0x8C) per frame
    uint8_t             _unk1C8[0x80] = {};         // +0x1C8 memset(this+0x1C8, 0, 0x80) @0x1805de4f6
    void*               _unk248 = nullptr;          // +0x248
    void*               m_pColliders = nullptr;     // +0x250 lazy 0x300-byte buffer, filled via pPhysicalWorld GetEntitiesInBox (sub_1808A8FE0)
    int32_t             m_nColliders = 0;           // +0x258
    uint8_t             _pad25C[4] = {};            // +0x25C
    void*               m_pProjectiles = nullptr;   // +0x260 lazy 12 x 0x28-byte projectile snapshots (sub_180E8BF2C) -- NOT the manager's 0x38 SProjectile; element shape not RE'd
    int32_t             m_nProjectiles = 0;         // +0x268
    uint8_t             _pad26C[4] = {};            // +0x26C
    void*               m_geomJobState = nullptr;   // +0x270 JobManager::SJobState handle #1 (geometry/cull; Render's sync object) [SJobState identity = inference; ctor sub_18078BDEC, release sub_1804A7710, pointee+0x20 = priority]
    void*               m_mergeJobState = nullptr;  // +0x278 SJobState handle #2 (merge)
    void*               m_spineJobState = nullptr;  // +0x280 SJobState handle #3 (InitializeSpines, priority 2)
    std::vector<void*>  m_spineBufA;                // +0x288 spine/deform buffer; {begin,end,cap} triple proven, ELEMENT TYPE UNKNOWN (elem dtor sub_180492FAC) -- void* is a layout placeholder
    std::vector<void*>  m_spineBufB;                // +0x2A0 same shape; both cleared by Prepare and RemoveRenderResources
    std::vector<SUnkElem10> m_unkVec2B8;            // +0x2B8 16-byte elements proven; truncated by Prepare; purpose unknown
    void*               m_pReadStream = nullptr;    // +0x2D0 pending IReadStreamPtr (ref-counted; released via pointee vfunc +0x80/+0x30 then swap-null sub_180396184) [IReadStream identity = inference]
    void*               _unk2D8 = nullptr;          // +0x2D8 ref-counted ptr (dtor releases via sub_1804F6588)
};
static_assert(sizeof(CMergedMeshRenderNode) == 0x2E0, "two alloc sites: pool @0x1834db8da, factory @0x180a79603");
static_assert(offsetof(CMergedMeshRenderNode, m_lastDrawFrameId) == 0x60, "members start AT +0x60 (Cry3DEngineBase EBO)");
static_assert(offsetof(CMergedMeshRenderNode, m_State) == 0x6C, "state @+0x6C");
static_assert(offsetof(CMergedMeshRenderNode, m_internalAABB) == 0x8C, "sector AABB @+0x8C");
static_assert(offsetof(CMergedMeshRenderNode, m_visibleAABB) == 0xA4, "visible AABB @+0xA4");
static_assert(offsetof(CMergedMeshRenderNode, m_pos) == 0xBC, "hash key pos @+0xBC");
static_assert(offsetof(CMergedMeshRenderNode, m_groups) == 0xF0, "group array @+0xF0");
static_assert(offsetof(CMergedMeshRenderNode, m_nGroups) == 0xF8, "group count @+0xF8");
static_assert(offsetof(CMergedMeshRenderNode, m_spineJobState) == 0x280, "job state #3 @+0x280");
static_assert(offsetof(CMergedMeshRenderNode, m_pReadStream) == 0x2D0, "stream ptr @+0x2D0");
