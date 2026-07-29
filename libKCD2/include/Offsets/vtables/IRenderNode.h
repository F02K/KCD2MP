#pragma once
#include <cstddef>
#include <cstdint>
#include "IShadowCaster.h"

// -----------------------------------------------
// IRenderNode -- Cry3DEngine render-node base, KCD2 binary slot order + data layout
// (WHGame.dll 1.5.6, e4cp).
// -----------------------------------------------
// struct IRenderNode : public IShadowCaster (RTTI, mdisp 0). 73-slot primary vtable CERTIFIED
// from the contiguous .rdata run: default vtable 0x183A33AB8 + 73*8 == COL(CVegetation)
// 0x183A33D00; IShadowCaster owns [0..7] (its standalone vtable 0x183A33A70 is exactly 8 slots),
// IRenderNode adds [8..72]. Implementor vtables audited: CBrush 0x18401FE70 (78), CVegetation
// 0x183A33D08 (74), CDecalRenderNode 0x183A3E810 (74), CMergedMeshRenderNode 0x18402A508 (73).
//
// sizeof == 0x50, three independent proofs: (a) next-base mdisp 0x50 in CBrush/CVegetation/
// CDecalRenderNode + IParticleEmitter (CMultiThreadRefCount vptr stored at +0x50, 0x180422D97);
// (b) ~IRenderNode sized delete `mov edx,50h` @0x18348E808; (c) the inlined ctor's write set
// stops at +0x4F (spine: sub_180422D34, incl. the `strcpy(this+0x4D, "dd")` idiom = 'd'==100 ->
// m_ucViewDistRatio=100, m_ucLodRatio=100, +0x4F=0).
//
// [WH DEVIATION] m_dwRndFlags @+0x28 is a 64-bit field (bit tests at 33/34/39/55/58/59/60, e.g.
// `test [rcx+28h], 0x480000000000100` @0x18040059E) -- stock CE3.8 headers say uint32; porting
// them verbatim mis-sizes the class and corrupts everything past +0x30.
//
// Naming policy: only call-site/ABI-certified slots are named; everything else stays _vfN with
// a behaviour comment. Full per-slot evidence: analysis/mesh_engine_re/irendernode.md §2.
// Same-name overloads are BANNED here (MSVC reverses adjacent overload slots): the bbox pair is
// GetBBoxVirtual()/FillBBox(), the material pair is GetMaterial()/GetMaterialOverride().

namespace Offsets {

struct IStatObj;                // Offsets/vtables/IStatObj.h
struct IEntity;                 // Offsets/vtables/IEntity.h
struct IRenderMesh;             // not yet RE'd
struct IPhysicalEntity;         // not yet RE'd
struct IMaterial;               // not yet RE'd
struct IFoliage;                // vtable 0x18402B5E0 (10 slots), not yet RE'd
struct ICrySizer;               // not yet RE'd
struct IOctreeNode;             // RTTI .?AUIOctreeNode@@; impl COctreeNode (vtables 0x183A389A0/+0x18)
struct SRenderNodeTempData;     // per-node temp data; TYPE NAME = stock hypothesis (matrix @+0x58, IFoliage* @+0xA8)

// m_dwRndFlags bits with PROVEN behaviour (names only where they line up with stock numbering);
// full bit table incl. Render/physics bits: irendernode.md §5, cbrush_cvegetation.md §0.
enum : uint64_t {
    ERF_GOOD_OCCLUDER  = 1ull << 0,   // set when statobj+0x17C has 0x2000 (RegisterEntity 0x180400B67) [stock-name LIKELY]
    ERF_RENDER_ALWAYS  = 1ull << 4,   // always-visible list add/remove (0x180400BD6 / 0x180400303)
    ERF_HIDDEN         = 1ull << 8,   // toggled by _vf68; member of _vf69's visibility mask 0x480000000000100 [stock-name LIKELY]
    ERF_OUTDOORONLY    = 1ull << 11,  // skips VisArea registration (0x180400A73); set by e_MergedMeshesOutdoorOnly path
};

struct IRenderNode : public IShadowCaster {
    // ---- IShadowCaster overrides supplied by IRenderNode in the default vtable ----
    // [5] FillBBox default 0x1834914F0 = `aabb = GetBBoxVirtual()`; [6] default = return nullptr.

    // ---- IRenderNode virtuals, slots [8..72] ----
    virtual bool CanExecuteRenderAsJob() = 0;                       // [8] (cvars+0x404 & BIT(GetRenderNodeType())) && !(flags & 0x10); cvar name unverified
    virtual const char* GetName() = 0;                              // [9] printed as "name:" by RegisterEntity's invalid-bbox log (0x180400C2E)
    virtual const char* GetEntityClassName() = 0;                   // [10] "Brush"/"Vegetation"/"Decal"/"Runtime MergedMesh"
    virtual CryStringT<char> _vf11() = 0;                           // [11] returns an EMPTY CryStringT by sret in all impls (0x1808D2480)
    virtual float _vf12() = 0;                                      // [12] returns 1.0f in all impls
    virtual void _vf13() = 0;                                       // [13] 24-byte body; ICF-folded symbol is an artifact -- unidentified
    virtual IRenderNode* Clone() = 0;                               // [14] CBrush 0x18348FA28 allocs 0x100 + copies; CVeg 0x18353A3CC pool-allocs; default ret0
    virtual const Matrix34& GetMatrix() = 0;                        // [15] CBrush -> &this->m_Matrix (+0x50); CVeg -> tempdata+0x58 or static identity (0x18078C8AC)
    virtual void SetMatrix(const Matrix34& mat) = 0;                // [16] CVeg stores only the translation column into m_vPos
    virtual void GetLocalBounds(AABB& out) = 0;                     // [17] {bbox.min-GetPos, bbox.max-GetPos} (0x183492364); SDK spelling = hypothesis; rax left stale -> treat as void
    virtual Vec3 GetPos(bool bWorldOnly) = 0;                       // [18] BY VALUE (sret); CBrush reads the matrix translation column
    virtual void SetBBox(const AABB& WSBBox) = 0;                   // [19] writes the world-space bbox member the [4]/[5] pair reads back
    virtual void OffsetPosition(const Vec3& delta) = 0;             // [20] shifts pos + bbox (+ phys entity where present)
    virtual bool _vf21() = 0;                                       // [21] returns true in all impls
    virtual IStatObj* GetEntityStatObj(unsigned int nPartId, Matrix34* pMatrix, bool bOnlyVisible) = 0;  // [22] [WH 3-ARG -- stock has 4; matrix written through the 2nd arg (r8), 3-arg call @0x180400B67]
    virtual void* _vf23() = 0;                                      // [23] returns nullptr in all impls
    virtual void SetEntityStatObj(unsigned int nSlot, IStatObj* pStatObj, const Matrix34* pMatrix) = 0;  // [24] CBrush-only override 0x180A75FA0; re-physicalizes on change
    virtual void* _vf25() = 0;                                      // [25] returns nullptr in all impls
    virtual int _vf26() = 0;                                        // [26] returns 1 in all impls (GetSlotCount hypothesis)
    virtual IRenderMesh* GetRenderMesh(int nLod) = 0;               // [27] via m_pStatObj LOD lookup (CBrush 0x183492FE0)
    virtual void SetLodRatio(int nRatio) = 0;                       // [28] clamp [0,255] -> byte +0x4E; NO octree dirty (contrast [59])
    virtual uint8_t _vf29() = 0;                                    // [29] default reads byte +0x4F; CVeg reads group+0x190 (material-layers hypothesis)
    virtual IPhysicalEntity* GetPhysics() = 0;                      // [30] CBrush +0x80 / CVeg +0x60
    virtual void SetPhysics(IPhysicalEntity* pPhys) = 0;            // [31] same members as [30]
    virtual void CheckPhysicalized() = 0;                           // [32] CBrush 0x18348F758: physicalize if not yet and not disabled
    virtual void Physicalize(bool bInstant) = 0;                    // [33] CBrush 0x180530A28 -> sub_180530A3C; CVeg 0x1803FCDFC (0x886 bytes)
    virtual bool PhysicalizeFoliage(bool bPhysicalize, int iSource, int nSlot) = 0;  // [34] builds/destroys the IFoliage in tempdata+0xA8 (CVeg 0x180DDC754)
    virtual IPhysicalEntity* GetBranchPhys(int idx, int nSlot) = 0; // [35] = GetFoliage(nSlot)->vtable+0x40(idx) (0x183491AF4)
    virtual IFoliage* GetFoliage(int nSlot) = 0;                    // [36] CVeg: tempdata+0xA8; CBrush: subobject +0xA0 else this+0xA8
    virtual void SetMaterial(IMaterial* pMat) = 0;                  // [37] CBrush smart-ptr assign +0x88 (0x1806BC2F0); CVeg no-op
    virtual IMaterial* GetMaterial(Vec3* pHitPos) = 0;              // [38] CBrush = m_pMaterial ?: statobj material (FALLBACK chain -- this is what pins 38 vs 39)
    virtual IMaterial* GetMaterialOverride() = 0;                   // [39] CBrush = raw m_pMaterial (+0x88) only
    virtual void SetCollisionClassIndex(uint16_t nIdx) = 0;         // [40] CBrush writes +0x90 -> C3DEngine collision-class table (stride 0xC) -> pe_params 0x1A [name INFERRED from use; no getter in the vtable]
    virtual void SetStatObjGroupIndex(int nVegetationGroupIndex) = 0;  // [41] CVeg writes +0x78 + invalidates tempdata (0x183549570)
    virtual int GetStatObjGroupIndex() = 0;                         // [42] default -1; CVeg reads +0x78
    virtual void SetLayerId(uint16_t nLayerId) = 0;                 // [43] CBrush +0x92 / CDecal +0xC0; notifies C3DEngine on change (sub_180400D2C)
    virtual uint16_t GetLayerId() = 0;                              // [44] reads the same field [43] writes
    virtual float GetMaxViewDist() = 0;                             // [45] cached into +0x48 by RegisterEntity (0x180400B4E)
    virtual void _vf46(uint8_t mode) = 0;                           // [46] tri-state writer of flag bits 59/60 (0x1820D1AA0)
    virtual bool IsAllocatedOutsideOf3DEngineDLL() = 0;             // [47] literally `return GetOwnerEntity() != nullptr` (0x180400290)
    virtual void _vf48() = 0;                                       // [48] unidentified (CBrush 131b / CVeg 181b)
    virtual void Dematerialize() = 0;                               // [49] CBrush releases m_pMaterial (0x183490AB4); stock name corroborated
    virtual void GetMemoryUsage(ICrySizer* pSizer) = 0;             // [50] pushes class name + AddObject(this, sizeof) -- source of the leaf size proofs
    virtual void _vf51() = 0;                                       // [51] never overridden by audited classes
    virtual void _vf52() = 0;                                       // [52] CDecal forwards {float,byte} from an arg struct; unidentified
    virtual void _vf53() = 0;                                       // [53] fills a render-node instance-info block (matrix @+0x58, distance, +0xD8) -- WH instance-streaming shape
    virtual void _vf54() = 0;                                       // [54] never overridden
    virtual bool _vf55() = 0;                                       // [55] default false; CDecal reads its +0xA0 byte 1
    virtual void _vf56() = 0;                                       // [56] CDecal-only override (368b); unidentified
    virtual int _vf57() = 0;                                        // [57] packs flag bits {10,16,23} into 0..7; ==7 gates integrate-into-terrain (GetGIMode hypothesis)
    virtual void _vf58(uint8_t v) = 0;                              // [58] masked insert into flag bits 24..26 (0x181AACF80)
    virtual void SetViewDistRatio(int nRatio) = 0;                  // [59] clamp [0,255] -> byte +0x4D + octree bucket dirty (0x1805DECB0) -- the octree side-effect separates it from [28]
    virtual void _vf60() = 0;                                       // [60] CBrush forwards a 16-byte arg to m_pStatObj; unidentified
    virtual float _vf61() = 0;                                      // [61] default resolves cvar "e_LodFaceAreaTargetSize" (0x1820D1B20); CVeg reads cvars+0x3EC
    virtual void _vf62(uint8_t v) = 0;                              // [62] writes byte +0x0E (one of the few writers of the 0x08..0x0F block)
    virtual void _vf63(const void* pBoxAndFlag) = 0;                // [63] copies a 0x1C {AABB box; bool valid} into CBrush+0xE0 (WH addition; no caller found)
    virtual void _vf64(void* pBoxAndFlagOut) = 0;                   // [64] sret getter of the same 0x1C block; default only zeroes the +0x18 bool
    virtual void* _vf65() = 0;                                      // [65] default nullptr; CMergedMeshRenderNode returns this+0x50 (interface sidecast)
    virtual void SetOwnerEntity(IEntity* pEntity) = 0;              // [66] CDecal stores +0xD0
    virtual IEntity* GetOwnerEntity() = 0;                          // [67] reads the same field; ties [47]
    virtual void _vf68(bool b) = 0;                                 // [68] toggles flag bit 8 then _vf69 (SetHidden if bit8==stock ERF_HIDDEN)
    virtual void _vf69() = 0;                                       // [69] octree visibility refresh: pOcNode->vf1(this, !(flags & 0x480000000000100)) (0x180400584)
    virtual void _vf70() = 0;                                       // [70] CVeg-only: ERF bit-20 (0x100000) toggle, frees +0x70 instancing data
    virtual void _vf71(bool b) = 0;                                 // [71] toggles flag bit 55 then _vf69
    virtual void _vf72(bool b) = 0;                                 // [72] toggles flag bit 58 then _vf69

    // ---- data, 0x08..0x4F (ctor spine sub_180422D34; every byte accounted except +0x0F) ----
    uint16_t             m_unk08 = 0;            // +0x08  zero-init word (field boundary proven by the word-sized ctor write)
    uint8_t              m_unk0A = 0;            // +0x0A
    uint8_t              m_unk0B = 0;            // +0x0B  \  zeroed as one dword @0x180422D71
    uint8_t              m_unk0C = 0;            // +0x0C   |
    uint8_t              m_unk0D = 0;            // +0x0D   |
    uint8_t              m_unk0E = 0;            // +0x0E  /  written by _vf62
    uint8_t              m_unk0F;                // +0x0F  NOT initialised by the ctor (padding or derived-class-lazy)
    void*                m_unk10 = nullptr;      // +0x10  no reader found (m_pNext GUESS -- unproven)
    void*                m_unk18 = nullptr;      // +0x18  no reader found (m_pPrev GUESS -- unproven)
    IOctreeNode*         m_pOcNode = nullptr;    // +0x20  octree/VisArea node; nulled by UnRegisterEntity (0x18040043A)
    uint64_t             m_dwRndFlags = 0;       // +0x28  ERF_* bitset -- 64-BIT in KCD2 (widened from stock uint32)
    SRenderNodeTempData* m_pTempData = nullptr;  // +0x30  per-frame temp data (matrix @+0x58, foliage @+0xA8, frame id @+0x0C)
    int32_t              m_unk38 = -1;           // +0x38  cache/handle index; reset to -1 on every render-state change
    uint32_t             m_unk3C = 0;            // +0x3C
    uint32_t             m_unk40 = 0;            // +0x40
    uint32_t             m_unk44 = 0;            // +0x44
    float                m_fWSMaxViewDist = 0;   // +0x48  cached GetMaxViewDist() (RegisterEntity 0x180400B5D)
    uint8_t              m_nInternalFlags = 0;   // +0x4C  bit7 = pending-unregister-queue marker (0x1804002BF)
    uint8_t              m_ucViewDistRatio = 100;// +0x4D  'd'; 255 -> treated as 100.0 (GetMaxViewDist idiom)
    uint8_t              m_ucLodRatio = 100;     // +0x4E  'd'
    uint8_t              m_unk4F = 0;            // +0x4F  read by _vf29; no writer found in the image (m_nMaterialLayers hypothesis, weak)
};
static_assert(sizeof(IRenderNode) == 0x50, "IRenderNode must be 0x50 (sized delete @0x18348E808 + mdisp evidence)");
static_assert(offsetof(IRenderNode, m_pOcNode) == 0x20, "m_pOcNode @+0x20");
static_assert(offsetof(IRenderNode, m_dwRndFlags) == 0x28, "m_dwRndFlags @+0x28 (uint64!)");
static_assert(offsetof(IRenderNode, m_pTempData) == 0x30, "m_pTempData @+0x30");
static_assert(offsetof(IRenderNode, m_fWSMaxViewDist) == 0x48, "m_fWSMaxViewDist @+0x48");
static_assert(offsetof(IRenderNode, m_ucViewDistRatio) == 0x4D, "m_ucViewDistRatio @+0x4D");

}  // namespace Offsets
