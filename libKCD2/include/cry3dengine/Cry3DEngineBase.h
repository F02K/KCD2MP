#pragma once

// -----------------------------------------------
// Cry3DEngineBase -- empty common base of the Cry3DEngine classes, KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// RTTI TD `.?AUCry3DEngineBase@@` (0x500F0F8) exists but NO CompleteObjectLocator/vtable does
// -> non-polymorphic. It appears in the base-class arrays of CBrush/CVegetation/
// CDecalRenderNode at mdisp 0x50, CMergedMeshRenderNode at 0x60, CMergedMeshesManager at 0x8,
// CStatObj/COctreeNode at 0x50/0x20 -- always as an EMPTY base (EBO: derived members share the
// recorded offset, proven by the CMergedMeshRenderNode ctor writing a member dword at +0x60,
// 0x1805de384, and CStatObj's refcount at +0x50). In stock CryEngine it carries only statics
// (m_pSystem, m_p3DEngine, ...); those never materialise as instance data here.

struct Cry3DEngineBase
{
};
static_assert(sizeof(Cry3DEngineBase) == 1, "empty base (EBO'd away in every derived class)");
