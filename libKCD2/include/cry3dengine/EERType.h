#pragma once

// -----------------------------------------------
// EERType -- Cry3DEngine render-node type ids, KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Values PROVEN from the per-class GetRenderNodeType() stubs (IRenderNode vtable slot [7],
// `mov eax, imm; ret`) and independently corroborated by slot [8]'s per-type cvar bitmask
// (BIT(ERType) against C3DEngineCVars+0x404):
//   CBrush                0x181A72470 -> 1     CVegetation      0x181A72480 -> 2
//   CDecalRenderNode      0x181A74280 -> 7     CMergedMeshRenderNode 0x181A7D870 -> 23 (0x17)
//   COwnedBrush (WH)      factory arm 0x180A795AF, dispatch value 0x1A -> 26
// The stock CE3.8 EERType numbering holds for 1/2/7/23; every other stock name is a
// HYPOTHESIS and is deliberately not declared here. Additional values observed at call
// sites but with unidentified classes: 3, 6 (forced always-visible on unregister,
// 0x180400303), 17 (integrate-into-terrain gate, 0x1804009DD), and the
// C3DEngine::CreateRenderNode factory (sub_180A79410) arms 6/7/9/0xA/0xB/0xC/0xF/0x11/
// 0x15/0x16/0x17/0x18/0x1A -- see analysis/mesh_engine_re/cbrush_cvegetation.md §4.2.

enum EERType
{
    eERType_Brush       = 1,
    eERType_Vegetation  = 2,
    eERType_Decal       = 7,
    eERType_MergedMesh  = 23,   // 0x17 -- NOT 22; certified by the slot-7 stub at 0x181A7D870
    eERType_OwnedBrush  = 26,   // 0x1A -- WH-added CBrush subclass
};
