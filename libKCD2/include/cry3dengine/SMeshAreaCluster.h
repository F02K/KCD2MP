#pragma once

// -----------------------------------------------
// SMeshAreaCluster -- merged-mesh vegetation cluster (CompileAreas output),
// KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Element of the DynArray filled by IMergedMeshesManager::CompileAreas (slot [5],
// 0x1834DF83C). Stride 0x20 proven by the consumer CRuntimeAreaManager::CreateAreas
// (0x183706BF0): `cluster = base + 0x20*i`; it reads extents floats at +0x00/+0x0C.. and the
// boundary-point DynArray at +0x18 (count at ptr-4; 8-byte x/y pairs -> Vec2). Flag value 2
// passed by CreateAreas = CLUSTER_CONVEXHULL_GRAHAMSCAN (named in the "no clustering
// algorithm selected" log string at the CompileAreas site).

struct SMeshAreaCluster
{
    AABB           extents;    // +0x00  cluster bounds
    DynArray<Vec2> boundary;   // +0x18  convex-hull boundary points (x,y)
};
static_assert(sizeof(SMeshAreaCluster) == 0x20, "stride proven by CreateAreas (base + 0x20*i)");
