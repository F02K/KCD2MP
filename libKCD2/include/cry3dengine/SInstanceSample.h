#pragma once
#include <cstdint>

// -----------------------------------------------
// SInstanceSample -- CMergedMeshesManager::AddInstance argument, KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Built on the stack by the vegetation-instance creator sub_1835503A4 (the CVegetation-vs-
// merged-mesh fork) and consumed by sub_1834DB738 / sub_1834DB288: pos quantised into the
// 16m sector, quat components *128 -> int8, scale stored to instance +0x0C AND +0x0E
// (the WH scaleOriginal backup). Field offsets from the AddInstance reads
// (+0x00 pos, +0x0C quat, +0x1C group id, +0x20 scale).

struct SInstanceSample
{
    Vec3    pos;                 // +0x00  world position
    Quat    q;                   // +0x0C  built from the three packed Euler bytes * 2pi/255, normalised
    int32_t nInstGroupId = 0;    // +0x1C  StatInstGroup index
    uint8_t scale = 0;           // +0x20  clamped to 255
    uint8_t _pad21[3] = {};      // +0x21
};
static_assert(sizeof(SInstanceSample) == 0x24, "field offsets proven by the AddInstance reads");
