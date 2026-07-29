#pragma once
#include <cstddef>
#include <cstdint>
#include "SMMRMInstance.h"

// -----------------------------------------------
// SMMRMGroupHeader -- per-StatInstGroup header inside a CMergedMeshRenderNode,
// KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Array at node+0xF0, count at node+0xF8, STRIDE 0x80 (iteration `+= 0x80` in AddInstance /
// StreamAsyncOnComplete / InitializeSpines / Render). Type name proven by job RTTI:
// `SMMRMGroupHeader::CullInstances(const CCamera*, Vec3*, Vec3*, float, int)` @0x185019250.
// Only the cited fields are evidence-backed; everything else is an explicit unknown.
// NOTE: the research dossier also attributed a JobState at "+0xD0" to this struct -- that is
// OUTSIDE the proven 0x80 stride and is therefore NOT modelled here (flagged as suspect).
// Sample cap per group: min(0xFFFF / pGeom->vertexCount, 0x400).

struct SMMRMGeometry;   // global geometry-cache record (intrusive list at 0x18533C210); not yet RE'd

struct SMMRMGroupHeader
{
    SMMRMInstance* instances = nullptr;  // +0x00  quantised instance array (stride 0x10)
    void*          _unk08 = nullptr;     // +0x08  cached buffer, released+nulled on AddInstance (sub_180765F4C)
    void*          _unk10 = nullptr;     // +0x10  cached buffer, released+nulled on AddInstance
    SMMRMGeometry* pGeom = nullptr;      // +0x18  from sub_1807682DC(instGroupId)
    uint8_t        _unk20[0x2C] = {};    // +0x20 .. 0x4B  unknown
    int32_t        instGroupId = 0;      // +0x4C  StatInstGroup index (table stride 0x1A0)
    uint32_t       numVertices = 0;      // +0x50  validated against the pvrn chunk header
    uint32_t       numSamples = 0;       // +0x54  live instance count
    uint32_t       capacity = 0;         // +0x58  grown in steps of 0xFF
    uint8_t        _unk5C[8] = {};       // +0x5C .. 0x63  unknown
    float          _unk64 = 0;           // +0x64  = group[0x16C] * group[0x124] * CVars[0x794]
    float          _unk68 = 0;           // +0x68  = CVars[0x7E0] * group[0x11C]
    uint8_t        flags = 0;            // +0x6C  bit0 instances-allocated, bit1 external-buffer, bit2 render-checked
    uint8_t        _unk6D = 0;           // +0x6D  cleared per group in Render (0x1804a07c3)
    uint8_t        _unk6E[0x12] = {};    // +0x6E .. 0x7F  unknown
};
static_assert(sizeof(SMMRMGroupHeader) == 0x80, "stride proven by the += 0x80 iterations");
static_assert(offsetof(SMMRMGroupHeader, instGroupId) == 0x4C, "instGroupId @+0x4C");
static_assert(offsetof(SMMRMGroupHeader, numSamples) == 0x54, "numSamples @+0x54");
