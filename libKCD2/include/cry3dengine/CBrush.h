#pragma once
#include <cstddef>
#include <cstdint>
#include "../Offsets/vtables/IBrush.h"
#include "Cry3DEngineBase.h"

// -----------------------------------------------
// CBrush : IBrush, Cry3DEngineBase -- static brush render node, KCD2 WHGame.dll 1.5.6 (e4cp).
// sizeof 0x100.
// -----------------------------------------------
// Vtable 0x18401FE70 (78 slots, IRenderNode order; COL-boundary certified). Ctor 0x1806BCBBC,
// dtor 0x1805303A4, deleting dtor (slot 0) 0x180530168; live counter dword_185188E14.
// sizeof 0x100 -- FOUR independent proofs: chunk loader `mov ecx,100h` @0x1803FBF8A;
// Clone alloc @0x18348FA59; CreateRenderNode factory arm @0x180A7952F; GetMemoryUsage
// AddObject(this, 0x100) @0x183492A3D.
//
// Creation: level chunk stream (sub_1803FBEEC -> CBrush::Load sub_1808451E0, SBrushChunk
// stride 0x64), bulk loader sub_1803FD684, factory sub_180A79410 (eERType_Brush), Clone.
// Export = sub_183498768 (the only Brush.cpp-string xref). Key overrides: SetMatrix
// 0x180A756EC (48-byte copy + re-physicalize), Render 0x1834974E8, GetBBoxVirtual 0x180431AAC.
// Full slot->impl table: analysis/mesh_engine_re/cbrush_cvegetation.md §1.2.
//
// Smart-pointer notes: m_pMaterial is a _smart_ptr<IMaterial> (AddRef/Release via vtable
// +0x10/+0x18, assign sub_1806BCF48) -- modelled as a raw pointer (layout-identical).
// m_pStatObj is NOT a _smart_ptr: refcounting is open-coded on CStatObj+0x50 with
// non-virtual release sub_18051F994.

struct CStatObj;
class CDeformableNode;   // 0xB0-byte deform helper (alloc @0x18221F009, ctor sub_1834D9DE4) [type name = stock hypothesis]

class CBrush : public Offsets::IBrush, public Cry3DEngineBase {
public:
    // m_brushFlags (+0xB0) bits -- only 0..2 proven (ctor clears exactly these: &= ~7)
    enum : uint32_t {
        eBrushFlag_DrawLast                = 1u << 0,  // serialised (chunk+0x5A bit0), the only cloned bit; set by IBrush::SetDrawLast [bit identity = stock hypothesis]
        eBrushFlag_PhysicalizationDisabled = 1u << 1,  // gates Physicalize/CheckPhysicalized/SetMatrix re-phys (proven)
        eBrushFlag_BBoxInPODGrid           = 1u << 2,  // set on RegisterBBoxInPODGrid (0x180530D79), tested in SetBBox
    };

    Matrix34                  m_Matrix;                    // +0x50  world transform (GetMatrix returns &this->m_Matrix; row-major, 16-byte row stride proven by GetMatrixScale)
    Offsets::IPhysicalEntity* m_pPhysEnt = nullptr;        // +0x80  physics entity (slots 30/31)
    Offsets::IMaterial*       m_pMaterial = nullptr;       // +0x88  _smart_ptr<IMaterial> (see banner)
    uint16_t                  m_collisionClassIdx = 0;     // +0x90  index into C3DEngine's collision-class table (stride 0xC) -> pe_params 0x1A
    uint16_t                  m_nLayerId = 0;              // +0x92  slots 43/44; hidden-layer bitmask key
    uint32_t                  _pad94 = 0;                  // +0x94  never touched
    CStatObj*                 m_pStatObj = nullptr;        // +0x98  geometry (manual refcount @CStatObj+0x50)
    CDeformableNode*          m_pDeform = nullptr;         // +0xA0  deformable helper (0xB0 alloc)
    Offsets::IFoliage*        m_pFoliage = nullptr;        // +0xA8  whole-object foliage (PhysicalizeFoliage fallback)
    uint32_t                  m_brushFlags = 0;            // +0xB0  bits above
    AABB                      m_WSBBox;                    // +0xB4  world-space bbox (slots 4/5/19)
    uint32_t                  m_nLastMovedFrameId = 0;     // +0xCC  = frame id on SetMatrix; Render sets "recently moved" within 3 frames
    uint32_t*                 m_pInstanceData = nullptr;   // +0xD0  COW uint32 array (refcount at data[-4]); moved in by IBrush::_vf76; Render -> SRendParams+0x88 [element semantics unknown]
    int32_t                   m_nInstanceDataCount = 0;    // +0xD8  bit31 = shared-buffer flag
    uint32_t                  _unkDC = 0;                  // +0xDC  never read or written in any examined function
    AABB                      m_unkBoxE0;                  // +0xE0  {AABB, bool} block set/get by IRenderNode slots 63/64 -- WH addition, meaning unknown
    bool                      m_bUnkBoxE0Valid = false;    // +0xF8  ctor zeroes only this byte of the block
    // +0xF9..0xFF tail padding -> 0x100
};
static_assert(sizeof(CBrush) == 0x100, "4 proofs incl. mov ecx,100h @0x1803FBF8A");
static_assert(offsetof(CBrush, m_Matrix) == 0x50, "m_Matrix @+0x50");
static_assert(offsetof(CBrush, m_pPhysEnt) == 0x80, "m_pPhysEnt @+0x80");
static_assert(offsetof(CBrush, m_pMaterial) == 0x88, "m_pMaterial @+0x88");
static_assert(offsetof(CBrush, m_pStatObj) == 0x98, "m_pStatObj @+0x98");
static_assert(offsetof(CBrush, m_WSBBox) == 0xB4, "m_WSBBox @+0xB4");
static_assert(offsetof(CBrush, m_pInstanceData) == 0xD0, "COW array @+0xD0");
static_assert(offsetof(CBrush, m_unkBoxE0) == 0xE0, "slot-63/64 block @+0xE0");
