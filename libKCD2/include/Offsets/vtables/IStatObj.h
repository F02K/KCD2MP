#pragma once
#include <cstdint>
#include "IMeshObj.h"
#include "IStreamable.h"

// -----------------------------------------------
// IStatObj -- static-geometry (CGF) object interface, KCD2 binary slot order
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct IStatObj : public IMeshObj /* +0x00 */, public IStreamable /* +0x08 */ (RTTI BCA of
// CStatObj: IStatObj contains IMeshObj @0 + IStreamable @+0x08). The primary vtable is the
// 90-slot CStatObj table 0x183A2F888: [0..14] = IMeshObj level (its standalone default vtable
// 0x183A2F808 is exactly 15 slots), [15..89] = the 75 IStatObj-added virtuals below.
// Slot boundary and every entry lookup_funcs-validated; slot 90 is ASCII data ("EntityCl").
//
// Naming: ~60 slots are behaviour-proven from the CStatObj impls (member offsets cited in
// cstatobj.md §2.2); [SDK-GUESS]/[INFERRED] tags mark name-level hypotheses; _vfN = behaviour
// captured but name unresolved. De-overloaded by project rule (GetPhysGeom [12 in IMeshObj]
// vs SetPhysGeom [27]); no adjacent same-name pair remains.

class CIndexedMesh;     // Cry3DEngine indexed mesh, sizeof 0x128 (alloc 0x18351E3A4) -- not yet RE'd
struct SSubObject;      // cry3dengine/SSubObject.h (0xB0 stride)

namespace Offsets {

struct IPhysicalEntity;
struct ICrySizer;

struct IStatObj : public IMeshObj, public IStreamable {
    // ---- IStatObj-added virtuals, primary slots [15..89] ----
    virtual void SetFlags(int nFlags) = 0;                          // [15] +0x178 write + notify (0x18349BE74)
    virtual int GetFlags() = 0;                                     // [16] +0x178
    virtual int GetIDMatBreakable() = 0;                            // [17] +0x180, init -1 [SDK-GUESS: inferred from keep-sys-mesh gating + Clone propagation]
    virtual void SetIDMatBreakable(int nBreakableId) = 0;           // [18] +0x180 write
    virtual bool _vf19() = 0;                                       // [19] returns internal bit 0x20 (+0x17C); bit meaning unknown
    virtual void WaitForStreamCompletion() = 0;                     // [20] waits m_pReadStream if streaming state == 1 (0x18354CDF0) [behaviour-named]
    virtual CIndexedMesh* GetIndexedMesh(bool bCreateIfNone, bool bWaitForStream) = 0;  // [21] 2nd bool triggers [20]; stock has only 1 arg
    virtual CIndexedMesh* CreateIndexedMesh() = 0;                  // [22] 0x128-byte alloc into +0x90
    virtual void _vf23() = 0;                                       // [23] foliage-spine builder, 6 params ("thickness"/"stiffness"/"damping" props) -- WH-extended AnalyzeFoliage shape, arity differs from stock
    virtual void _vf24() = 0;                                       // [24] 7 params; owns the +0x248 member; purpose unresolved
    virtual void* SkinVertices(const void* pVtxStridedByVal, const Matrix34& mtx) = 0;  // [25] 1st arg = 16-byte strided_pointer<Vec3> BY VALUE; return = render mesh per stock [return unverified]
    virtual void CopyFoliageData(IStatObj* pObjDst, bool bMove, void* pSrcFoliage, int* pVtxMap, void* pMovedBoxes, int nMovedBoxes) = 0;  // [26] [SDK-GUESS on param names; bMove + foliage-member usage proven]
    virtual void SetPhysGeom(phys_geometry* pPhysGeom, int nType) = 0;  // [27] +0x128 array write path w/ geom-manager unregister [direction INFERRED -- de-overloaded from IMeshObj::GetPhysGeom]
    virtual void* _vf28() = 0;                                      // [28] returns +0x140 (released via vfunc+0x30 in teardown); unresolved
    virtual float _vf29() = 0;                                      // [29] returns float +0x238 (init -1.0f); meaning unknown
    virtual void _vf30(float f) = 0;                                // [30] sets +0x238
    virtual void SetMaterial(IMaterial* pMaterial) = 0;             // [31] smart-ptr assign +0xE8
    virtual Vec3 GetVegCenter() = 0;                                // [32] +0x118 (written by CalcRadiuses)
    virtual void SetBBoxMin(const Vec3& vBBoxMin) = 0;              // [33] +0x100
    virtual void SetBBoxMax(const Vec3& vBBoxMax) = 0;              // [34] +0x10C
    virtual void Refresh(int nFlags) = 0;                           // [35] flags&8 -> teardown+reload from m_szFileName; failure -> Default.cgf
    virtual IStatObj* GetLodObject(int nLodLevel, bool bReturnNearest) = 0;  // [36] m_pLODs (+0x158), hard bound 6
    virtual void SetLodObject(int nLodLevel, IStatObj* pLod) = 0;   // [37] valid 1..5; lazily allocates the count-prefixed LOD block
    virtual IStatObj* GetLowestLod() = 0;                           // [38] [SDK-GUESS] min-LOD from cvar floor
    virtual int FindNearesLoadedLOD(int nLodIn, bool bSearchUp) = 0;// [39] (stock typo preserved) scans for a LOD with a live render mesh; -1 on miss
    virtual int FindHighestLOD(int nBias) = 0;                      // [40] -1 on miss
    virtual const char* GetFilePath() = 0;                          // [41] m_szFileName data ptr (+0xA0)
    virtual void SetFilePath(const char* szFileName) = 0;           // [42]
    virtual const char* GetGeoName() = 0;                           // [43] +0xA8
    virtual void SetGeoName(const char* szGeoName) = 0;             // [44]
    virtual bool IsSameObject(const char* szFileName, const char* szGeomName) = 0;  // [45] stricmp + slash-normalised path compare
    virtual Vec3 GetHelperPos(const char* szHelperName) = 0;        // [46] translation column of the subobject tm (+0x1C)
    virtual const Matrix34& GetHelperTM(const char* szHelperName) = 0;  // [47] &subobj->tm, else the identity global 0x18493CF70
    virtual bool IsDefaultObject() = 0;                             // [48] internal bit 0x4
    virtual void FreeIndexedMesh() = 0;                             // [49] spin-lock +0x98, delete +0x90
    virtual void GetMemoryUsage(ICrySizer* pSizer) = 0;             // [50] AddObject(this, 0x250) -- the sizeof proof (0x183521E16)
    virtual float GetRadiusVert() = 0;                              // [51] +0xFC
    virtual float GetRadiusHors() = 0;                              // [52] +0xF8
    virtual bool IsPhysicsExist() = 0;                              // [53] phys-geom array non-empty
    virtual IStatObj* _vf54() = 0;                                  // [54] `return this;` (GetIStatObj identity [SDK-GUESS])
    virtual void Invalidate(bool bPhysics) = 0;                     // [55] rebuild render mesh + counters from the indexed mesh
    virtual int GetSubObjectCount() = 0;                            // [56] (end-begin)/0xB0
    virtual void SetSubObjectCount(int nCount) = 0;                 // [57] maintains STATIC_OBJECT_COMPOUND
    virtual SSubObject* GetSubObject(int nIndex) = 0;               // [58] begin + 0xB0*i
    virtual bool IsSubObject() = 0;                                 // [59] internal bit 0x10
    virtual IStatObj* GetCloneSourceObject() = 0;                   // [60] +0x218
    virtual void* _vf61() = 0;                                      // [61] returns +0x220 (ref-counted deform/skin companion); unresolved
    virtual SSubObject* FindSubObject(const char* sNodeName) = 0;   // [62] token-prefix match (len<=0x20, ','/';' delimiters)
    virtual SSubObject* FindSubObject_CGA(const char* sNodeName) = 0;   // [63] exact stricmp
    virtual SSubObject* FindSubObject_StrStr(const char* sNodeName) = 0; // [64] substring match
    virtual bool RemoveSubObject(int nIndex) = 0;                   // [65]
    virtual bool CopySubObject(int nToIndex, IStatObj* pFromObj, int nFromIndex) = 0;  // [66]
    virtual SSubObject& AddSubObject(IStatObj* pStatObj) = 0;       // [67] grows + AddRef, returns the new element
    virtual int PhysicalizeSubobjects(IPhysicalEntity* pent, const Matrix34* pMtx, float mass, float density, int id0,
                                      void* pJointsIdMap, const char* szPropsOverride, int _unk8, int _unk9) = 0;  // [68] [ARITY UNRESOLVED: IDA shows 9 params vs stock 7] CGF-node -> physics-part builder
    virtual int Physicalize(IPhysicalEntity* pent, void* pgp /*pe_geomparams*/, int id, const char* szPropsOverride) = 0;  // [69] matches the stock 4-arg shape
    virtual bool IsDeformable() = 0;                                // [70] cvar gate + internal 0x80000 (self or subobjects)
    virtual bool SaveToCGF(const char* sFilename, void** ppOutChunkFile, bool bHavePhysicsProxy) = 0;  // [71] "From Sandbox" tag
    virtual IStatObj* Clone(bool bCloneGeometry, bool bCloneChildren, bool bMeshesOnly) = 0;  // [72] fresh pool CStatObj, "StatObj_Cloned" mesh tag
    virtual void SetDeformationMorphTarget(IStatObj* pDeformed) = 0;// [73] "StatObj_Deformed"/"StatObj_MorphTarget" tags [return type unverified]
    virtual IStatObj* DeformMorph(const Vec3& pt, float r, float strength, void* pWeights) = 0;  // [74] creates a new CStatObj [return LIKELY]
    virtual void _vf75() = 0;                                       // [75] strips subsets with nPhysicalizeType == -1, then Invalidate(false)
    virtual void _vf76(void* pWriter) = 0;                          // [76] structured-writer dump ("StatObj"/"Flags"/"nvtx"/"ntris"/...)
    virtual const char* GetProperties() = 0;                        // [77] +0xB0 data ptr
    virtual void SetProperties(const char* szProperties) = 0;       // [78] assign + ParseProperties
    virtual bool GetPhysicalProperties(float& mass, float& density) = 0;  // [79] +0x23C/+0x240 ("mass="/"density=" props)
    virtual void* _vf80(float* pOutF) = 0;                          // [80] returns +0x148, *pOutF = +0x150 (init 1.0f); unresolved
    virtual void _vf81() = 0;                                       // [81] deform/skin sibling of [25] (3 params); unresolved
    virtual void _vf82() = 0;                                       // [82] broad reset/rebuild over the WH tail members; unresolved
    virtual void _vf83() = 0;                                       // [83] float aggregation across LODs/subobjects (COMPOUND-gated); unresolved
    virtual void GetStatistics(void* pStats) = 0;                   // [84] SStatistics fill (out struct not RE'd)
    virtual void* _vf85(void* pOut) = 0;                            // [85] copies the container at +0x80 out (sret); unresolved
    virtual void SetStreamingDependencyFilePath(const char* szFileName) = 0;  // [86] +0xB8, with dependency-cycle guard
    virtual float _vf87() = 0;                                      // [87] sqrt of +0xD0 (m_fGeometricMeanFaceArea); unresolved name
    virtual Vec3 _vf88() = 0;                                       // [88] returns the Vec3 at +0xD8 (sret); unresolved
    virtual int _vf89(int nIndex) = 0;                              // [89] finds the subobject named "<subobj[n].name>_Destroyed"; -1 if none
};
static_assert(sizeof(IStatObj) == 0x30, "IMeshObj vptr (8) + IStreamable subobject (0x28)");

}  // namespace Offsets
