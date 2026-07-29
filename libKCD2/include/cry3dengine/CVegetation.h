#pragma once
#include <cstddef>
#include <cstdint>
#include "../Offsets/vtables/IVegetation.h"
#include "Cry3DEngineBase.h"

// -----------------------------------------------
// CVegetation : IVegetation, Cry3DEngineBase -- vegetation instance render node,
// KCD2 WHGame.dll 1.5.6 (e4cp). sizeof 0x88.
// -----------------------------------------------
// Vtable 0x183A33D08 (74 slots; COL-boundary certified). Ctor 0x1803FC390, dtor 0x1803FBD60,
// deleting dtor 0x1805BE0F8; live counter dword_185188E18. POOL-allocated (descriptor array
// stride 0x68 @0x18533CE18; freed into one of two pools selected by m_dwRndFlags & 2) -- so
// sizeof 0x88 rests on GetMemoryUsage AddObject(this, 0x88) @0x18353FFA9 (byte-verified) plus
// exact layout closure at +0x87.
//
// The instance is COMPRESSED: no stored matrix (GetMatrix -> tempdata or identity; CalcMatrix
// sub_18065EFA4 rebuilds from the packed bytes), extents/radius packed through
//   encode(x) = (uint8)clamp(x * 255/64 + 1, 0, 255)      // conservative cover
//   decode(b) = LUT[b] = b * 64/255                        // LUT flt_185188F80
// scale = byte/64 (GetScale, CalcMatrix, GetMaxViewDist), angles = byte * 2pi/255
// (CalcMatrix pins 0x7C=Z, 0x85=X, 0x86=Y). Group data comes from the StatInstGroup table
// (CObjManager+0x10, stride 0x1A0) indexed by m_nObjectTypeIndex.
// Chunk loader sub_1803FC6B8 (SVegetationChunk stride 0x3C). Full evidence:
// analysis/mesh_engine_re/cbrush_cvegetation.md §2.

class CDeformableNode;   // 0xB0-byte deform helper [type name = stock hypothesis]

class CVegetation : public Offsets::IVegetation, public Cry3DEngineBase {
public:
    Vec3                      m_vPos;                       // +0x50  world position
    bool                      m_bRegisteredInPODGrid = false;  // +0x5C  set after RegisterBBoxInPODGrid (0x1803FD085); dtor unregisters on it
    uint8_t                   _pad5D[3] = {};               // +0x5D
    Offsets::IPhysicalEntity* m_pPhysEnt = nullptr;         // +0x60  slots 30/31
    CDeformableNode*          m_pDeform = nullptr;          // +0x68
    void*                     m_pInstancingInfo = nullptr;  // +0x70  per-instance data (precise AABB at +0x18); freed by slot 70 [name placeholder, pointee not RE'd]
    int32_t                   m_nObjectTypeIndex = 0;       // +0x78  StatInstGroup index (read as 4-byte int by every consumer)
    uint8_t                   m_ucAngle = 0;                // +0x7C  Z rotation, *2pi/255
    uint8_t                   m_ucScale = 0;                // +0x7D  scale*64
    uint8_t                   m_boxExtendsMax[3] = {};      // +0x7E  +X,+Y,+Z extents (encoded)
    uint8_t                   m_boxExtendsMin[3] = {};      // +0x81  -X,-Y,-Z extents (encoded)
    uint8_t                   m_ucRadius = 0;               // +0x84  max extent-vector length (same encoding)
    uint8_t                   m_ucAngleX = 0;               // +0x85
    uint8_t                   m_ucAngleY = 0;               // +0x86
    uint8_t                   m_bApplyPhys = 0;             // +0x87  set unconditionally early in Physicalize (0x1803FCF63); OffsetPosition uses it as the POD-grid gate. [name GUESS -- NOT reconciled with m_bRegisteredInPODGrid, keep both separate]
};
static_assert(sizeof(CVegetation) == 0x88, "GetMemoryUsage AddObject(this, 0x88) @0x18353FFA9 + layout closure");
static_assert(offsetof(CVegetation, m_vPos) == 0x50, "m_vPos @+0x50");
static_assert(offsetof(CVegetation, m_pPhysEnt) == 0x60, "m_pPhysEnt @+0x60");
static_assert(offsetof(CVegetation, m_pInstancingInfo) == 0x70, "instancing info @+0x70");
static_assert(offsetof(CVegetation, m_nObjectTypeIndex) == 0x78, "group index @+0x78");
static_assert(offsetof(CVegetation, m_ucAngleY) == 0x86, "packed byte block 0x7C..0x87");
