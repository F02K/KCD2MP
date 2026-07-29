#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../Offsets/vtables/IStatObj.h"
#include "../Offsets/vtables/IStreamCallback.h"
#include "../Offsets/vtables/IRendermeshOwner.h"
#include "Cry3DEngineBase.h"
#include "SPhysGeomInfo.h"
#include "SSubObject.h"

// -----------------------------------------------
// CStatObj -- static CGF geometry object, KCD2 WHGame.dll 1.5.6 (e4cp). sizeof 0x250.
// -----------------------------------------------
// struct (RTTI is AU -- keep `struct` or the RTTI pipeline's mangled-name match fails).
// Bases (ctor 0x1803C70A4 vptr sequence + adjustor thunks):
//   IStatObj (IMeshObj @+0x00, IStreamable @+0x08) -> vtables 0x183A2F888 (90) / 0x183A2F7C8 (7)
//   IStreamCallback @+0x30 -> 0x183A2F760 (4)    IRendermeshOwner @+0x38 -> 0x183A2F748 (2, WH)
//   stl::intrusive_linked_list_node<CStatObj> @+0x40 (modelled as two members below to avoid
//   pulling the stock StlUtils.h into the umbrella)    Cry3DEngineBase @+0x50 (EBO, 0 bytes)
// sizeof 0x250: GetMemoryUsage AddObject(this, 0x250) @0x183521E16 (byte-verified
// `mov r8d,250h`) + highest init write +0x248 (0x1803C72A3). POOL-allocated from
// CObjManager+0x928 (sub_18052E3B4), NOT operator new.
// Global intrusive list head: qword_185497310. Creation flow (LoadStatObj sub_18052C298,
// async, LOD chain): analysis/mesh_engine_re/cstatobj.md §7; member evidence §6.
//
// The three pointer-triples (+0x128, +0x1E0, +0x200) are {begin,end,cap} vectors; modelled as
// std::vector<T> (layout-identical at _ITERATOR_DEBUG_LEVEL=0; allocator discipline of the
// game side is unverified -- treat as READ-ONLY views, never grow them from plugin code).
// m_pLODs is a WH count-prefixed block (count at ptr-8 == 6), NOT an inline array.

class CIndexedMesh;

struct CStatObj : public Offsets::IStatObj,
                  public Offsets::IStreamCallback,
                  public Offsets::IRendermeshOwner,
                  public Cry3DEngineBase {
    // m_nFlags (+0x178) -- public STATIC_OBJECT_* bits observed (CE3.8 numbering intact)
    enum : uint32_t {
        eStaticObjectFlag_Hidden          = 1u << 0,   // Render early-out
        eStaticObjectFlag_Clone           = 1u << 1,   // SkinVertices forces a Clone when clear
        eStaticObjectFlag_Compound        = 1u << 5,   // maintained by SetSubObjectCount
        eStaticObjectFlag_NoPlayerCollide = 1u << 10,  // "no_player_collide" property
        eStaticObjectFlag_Entity          = 1u << 20,  // "entity" property (WH/late-CE)
        eStaticObjectFlag_Pickable        = 1u << 21,  // "pickable" (WH)
        eStaticObjectFlag_NoAutoHidepoints= 1u << 22,  // "no_auto_hidepoints" (WH)
        eStaticObjectFlag_Dynamic         = 1u << 23,  // "dynamic" -> dynamic render mesh
    };

    CStatObj*                  m_pNext = nullptr;              // +0x40  intrusive list (global head qword_185497310)
    CStatObj*                  m_pPrev = nullptr;              // +0x48
    volatile int               m_nRefCount = 0;                // +0x50  interlocked (AddRef 0x18052C21C / Release 0x18051F9A9)
    uint32_t                   m_nLastDrawFrameId = 0;         // +0x54
    Offsets::IRenderMesh*      m_pRenderMesh = nullptr;        // +0x58  _smart_ptr; committed from +0x70 under the SRWLOCK
    void*                      m_lockRenderMesh = nullptr;     // +0x60  SRWLOCK (InitializeSRWLock @0x1803C713C)
    void*                      m_unk68 = nullptr;              // +0x68  ctor-zeroed only
    Offsets::IRenderMesh*      m_pPendingRenderMesh = nullptr; // +0x70  async build target (swap 0x18353E68D)
    Offsets::IRenderMesh*      m_pStreamedRenderMesh = nullptr;// +0x78  freed when internal 0x400
    uint8_t                    m_unk80[0x10] = {};             // +0x80  16-byte container (dtor sub_1803D5078; copied out by _vf85)
    CIndexedMesh*              m_pIndexedMesh = nullptr;       // +0x90  sizeof(CIndexedMesh) == 0x128
    volatile int               m_lockIndexedMesh = 0;          // +0x98  spin lock (0x10000 writer token)
    uint32_t                   _pad9C = 0;                     // +0x9C
    CryStringT<char>           m_szFileName;                   // +0xA0
    CryStringT<char>           m_szGeomName;                   // +0xA8
    CryStringT<char>           m_szProperties;                 // +0xB0
    CryStringT<char>           m_szStreamingDependencyFilePath;// +0xB8
    int32_t                    m_nLoadedTrisCount = 0;         // +0xC0
    int32_t                    m_nLoadedVertexCount = 0;       // +0xC4
    int32_t                    m_nRenderTrisCount = 0;         // +0xC8  visible-subset tris
    int32_t                    m_nRenderMatIds = 0;            // +0xCC
    float                      m_fGeometricMeanFaceArea = 0;   // +0xD0
    int32_t                    m_unkD4 = 0;                    // +0xD4  load-time tris snapshot [role UNVERIFIED]
    Vec3                       m_unkD8;                        // +0xD8  read out by _vf88; meaning unknown
    uint32_t                   _padE4 = 0;                     // +0xE4
    Offsets::IMaterial*        m_pMaterial = nullptr;          // +0xE8  _smart_ptr; default from Get3DEngine material manager
    void*                      m_unkF0 = nullptr;              // +0xF0  ctor-zeroed only
    float                      m_fRadiusHors = 0;              // +0xF8  sqrt(maxAbsX^2 + maxAbsY^2)
    float                      m_fRadiusVert = 0;              // +0xFC  max.z * 0.5
    AABB                       m_AABB;                         // +0x100 SetBBoxMin/+0x10C SetBBoxMax
    Vec3                       m_vVegCenter;                   // +0x118 CalcRadiuses output
    uint32_t                   _pad124 = 0;                    // +0x124
    std::vector<SPhysGeomInfo> m_arrPhysGeomInfo;              // +0x128 element 0x10 (GetPhysGeom `(end-begin)>>4`)
    void*                      m_unk140 = nullptr;             // +0x140 released via vfunc+0x30 in teardown; unresolved
    void*                      m_unk148 = nullptr;             // +0x148 returned by _vf80; unresolved
    float                      m_unk150 = 1.0f;                // +0x150 _vf80 out-float
    bool                       m_bCanUnload = false;           // +0x154 IsUnloadable
    uint8_t                    _pad155[3] = {};                // +0x155
    CStatObj**                 m_pLODs = nullptr;              // +0x158 WH count-prefixed block: base=alloc(0x38), *(uint64*)base=6, m_pLODs=base+8 -- count at ptr-8, MAX 6
    CStatObj*                  m_pParentObject = nullptr;      // +0x160 weak (LOD child -> parent)
    uint8_t                    m_unk168 = 0;                   // +0x168 LOD bias/base [UNVERIFIED]
    uint8_t                    m_nMinUsableLod0 = 0;           // +0x169 [SDK-GUESS]
    uint8_t                    m_nMaxUsableLod = 0;            // +0x16A
    uint8_t                    m_nLoadedLodsNum = 1;           // +0x16B
    uint32_t                   _pad16C = 0;                    // +0x16C
    CryStringT<char>           m_cgfNodeName;                  // +0x170 CGF-node/LOD matching name
    int32_t                    m_nFlags = 0;                   // +0x178 STATIC_OBJECT_* (enum above)
    uint32_t                   m_nInternalFlags = 0;           // +0x17C WH internal bitset (21 bits mapped in cstatobj.md §8.2)
    int32_t                    m_idmatBreakable = -1;          // +0x180 [SDK-GUESS name]
    int32_t                    m_unk184 = 0;                   // +0x184 async render-mesh size/handle (IRendermeshOwner slot 1)
    int32_t                    m_unk188 = 0;                   // +0x188 cached render-mesh memory usage
    int32_t                    m_arrStreamableCacheSize[2] = {-1, -1};  // +0x18C cached streamable sizes (slot picked by internal 0x4000)
    uint32_t                   _pad194 = 0;                    // +0x194
    void*                      m_pReadStream = nullptr;        // +0x198 IReadStreamPtr (StartStreaming stores, StreamOnComplete clears)
    void*                      m_unk1A0 = nullptr;             // +0x1A0 ref-counted; unresolved
    int32_t                    m_nModificationId = 0;          // +0x1A8 ++ in Refresh
    uint32_t                   _pad1AC = 0;                    // +0x1AC
    uint16_t*                  m_unk1B0 = nullptr;             // +0x1B0 deformable index remap ("Deformable StatObj Mesh")
    void*                      m_unk1B8 = nullptr;             // +0x1B8 cloth tangents (internal 0x20000) OR skin data (0x40000)
    void*                      m_unk1C0 = nullptr;             // +0x1C0 deform working buffer
    void*                      m_pSpines = nullptr;            // +0x1C8 foliage SSpine array, element 0x40
    int32_t                    m_nSpines = 0;                  // +0x1D0
    uint32_t                   _pad1D4 = 0;                    // +0x1D4
    void*                      m_unk1D8 = nullptr;             // +0x1D8 foliage aux buffer
    std::vector<uint16_t>      m_unk1E0;                       // +0x1E0 2-byte-element triple (vertex indices)
    void*                      m_unk1F8 = nullptr;             // +0x1F8 async mesh source smart ptr (IRendermeshOwner slot 0; pointee NOT CMesh)
    std::vector<SSubObject>    m_subObjects;                   // +0x200 element 0xB0
    CStatObj*                  m_pClonedSourceObject = nullptr;// +0x218
    CStatObj*                  m_unk220 = nullptr;             // +0x220 owning ref; deform/skin companion [UNVERIFIED]
    int32_t                    m_nSubObjectMeshCount = 0;      // +0x228
    uint32_t                   m_unk22C = 0;                   // +0x22C ctor-zeroed only
    void*                      m_unk230 = nullptr;             // +0x230 lazy per-EGeomForm extent cache (0x48 = 8 hdr + 4 x 0x10)
    float                      m_unk238 = -1.0f;               // +0x238 get/set via slots 29/30; meaning unknown
    float                      m_fMass = -1.0f;                // +0x23C "mass=" property
    float                      m_fDensity = -1.0f;             // +0x240 "density=" property
    uint32_t                   _pad244 = 0;                    // +0x244
    void*                      m_unk248 = nullptr;             // +0x248 owned by _vf24; teardown-freed via sub_183513854
};
static_assert(sizeof(CStatObj) == 0x250, "AddObject(this, 0x250) @0x183521E16 + init bound +0x248");
static_assert(offsetof(CStatObj, m_pNext) == 0x40, "intrusive node @+0x40");
static_assert(offsetof(CStatObj, m_nRefCount) == 0x50, "refcount @+0x50 (Cry3DEngineBase is EBO'd)");
static_assert(offsetof(CStatObj, m_pRenderMesh) == 0x58, "render mesh @+0x58");
static_assert(offsetof(CStatObj, m_szFileName) == 0xA0, "m_szFileName @+0xA0");
static_assert(offsetof(CStatObj, m_pMaterial) == 0xE8, "m_pMaterial @+0xE8");
static_assert(offsetof(CStatObj, m_AABB) == 0x100, "m_AABB @+0x100");
static_assert(offsetof(CStatObj, m_arrPhysGeomInfo) == 0x128, "phys-geom vector @+0x128");
static_assert(offsetof(CStatObj, m_pLODs) == 0x158, "m_pLODs @+0x158");
static_assert(offsetof(CStatObj, m_nFlags) == 0x178, "m_nFlags @+0x178");
static_assert(offsetof(CStatObj, m_subObjects) == 0x200, "m_subObjects @+0x200");
static_assert(offsetof(CStatObj, m_unk248) == 0x248, "last member @+0x248");
