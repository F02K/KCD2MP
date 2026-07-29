#pragma once
#include <cstdint>

struct phys_geometry;   // CryPhysics geometry record (stock global type; not yet RE'd)

// -----------------------------------------------
// SPhysGeomInfo -- CStatObj physics-geometry slot record, KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Element of CStatObj::m_arrPhysGeomInfo (the std::vector-shaped triple at +0x128/+0x130/
// +0x138). Stride 0x10 proven by GetPhysGeom's `count = (end-begin)>>4` and the indexed
// access `*(begin + 0x10*n)` (0x1804AAC8C); the id at +0x08 is what the >=0x1000 reverse
// search compares (0x1804AACD4). Geom ids observed: 0x1000 (default), 0x1001, 0x1002
// (CVegetation::Physicalize scan). Struct name is ours; the stock equivalent is the
// phys_geometry* array with per-slot type ids.

struct SPhysGeomInfo
{
    phys_geometry* pGeom = nullptr;   // +0x00  returned by IStatObj/IMeshObj GetPhysGeom
    int32_t        nType = 0;         // +0x08  geom id (0x1000-based) or slot index
    uint32_t       _pad0C = 0;        // +0x0C
};
static_assert(sizeof(SPhysGeomInfo) == 0x10, "stride proven by (end-begin)>>4");
