#pragma once
#include <cstdint>

// -----------------------------------------------
// IMeshObj -- mesh-object base interface (head of IStatObj), KCD2 binary slot order
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct (RTTI .?AUIMeshObj@@), primary (mdisp 0) base of IStatObj. Standalone default vtable
// 0x183A2F808 = exactly 15 slots (COL boundary verified): slot 0 = own dtor, slots 5/6 =
// non-pure defaults (GetRadiusSqr/GetRadius -- byte-identical to CStatObj's, i.e. inherited),
// the rest _purecall. Hence CStatObj primary slots [0..14] are IMeshObj-level and [15..89]
// are IStatObj's. Slot names from CStatObj impl behaviour; evidence:
// analysis/mesh_engine_re/cstatobj.md §2.1.

struct SRendParams;
struct SRenderingPassInfo;
struct phys_geometry;   // physics geometry record (stock name; element of the 0x10-stride PhysGeomInfo array)

namespace Offsets {

struct IRenderMesh;
struct IMaterial;

struct IMeshObj {
    virtual ~IMeshObj() = default;                       // [0]
    virtual void AddRef() = 0;                           // [1] interlocked ++ on the owner refcount (CStatObj +0x50)
    virtual void Release() = 0;                          // [2] interlocked --, deferred-delete queue at 0
    virtual int GetRefCount() = 0;                       // [3]
    virtual AABB GetAABB() = 0;                          // [4] BY VALUE (sret); CStatObj reads +0x100..+0x117
    virtual float GetRadiusSqr() = 0;                    // [5] non-pure default: ((max-min)*0.5).len2 via slot 4
    virtual float GetRadius() = 0;                       // [6] non-pure default: sqrtf(...)*0.5
    virtual float GetExtent(int eForm) = 0;              // [7] EGeomForm; lazily builds a 4x0x10 cache (CStatObj +0x230)
    virtual void GetRandomPoints(void* aPointsByVal, void* pRndGen, int eForm) = 0;  // [8] 1st arg = 16-byte Array<PosNorm> BY VALUE (MSVC: pointer to caller temp); PosNorm stride 0x18
    virtual IMaterial* GetMaterial() = 0;                // [9] CStatObj +0xE8
    virtual IRenderMesh* GetRenderMesh() = 0;            // [10] CStatObj +0x58
    virtual void* _vf11() = 0;                           // [11] CStatObj stubs to nullptr -- unidentified query
    virtual phys_geometry* GetPhysGeom(int nType) = 0;   // [12] +0x128 array; nType<0x1000 = index, else reverse id search
    virtual int _vf13(void* pOutArray) = 0;              // [13] fills {+0xD0,+0xD4} per LOD, returns m_nLoadedLodsNum -- name unresolved
    virtual void Render(const SRendParams& rParams, const SRenderingPassInfo& passInfo) = 0;  // [14] early-out on STATIC_OBJECT_HIDDEN
};
static_assert(sizeof(IMeshObj) == 0x8, "vptr-only interface");

}  // namespace Offsets
