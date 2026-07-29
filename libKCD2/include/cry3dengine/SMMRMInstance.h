#pragma once
#include <cstdint>

// -----------------------------------------------
// SMMRMInstance -- in-memory merged-mesh vegetation instance, KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Stride 0x10 proven by the group allocator `sub_180767994(group, count, 0x10)` and
// AddInstance's `instances + 0x10*i` (0x1834DB288 write set). The on-disk pvrn format stores
// only 12 bytes per instance; the in-memory +0x0E scaleOriginal backup is the WH ADDITION the
// harvest/respawn system restores from (IMergedMeshRenderNode::SetInstancesVisible writes
// scale = bVisible ? scaleOriginal : 0).
// Quantisation (AddInstance): pos = (world - sector.internalAABB.min) * (1/16) * 65535;
// quat components * 128 -> int8; lastLod init 0xFE (-2).

struct SMMRMInstance
{
    uint16_t pos_x = 0;             // +0x00  sector-relative, /65535*16
    uint16_t pos_y = 0;             // +0x02
    uint16_t pos_z = 0;             // +0x04
    int8_t   qx = 0;                // +0x06  quat * 128
    int8_t   qy = 0;                // +0x07
    int8_t   qz = 0;                // +0x08
    int8_t   qw = 0;                // +0x09
    uint8_t  _unk0A[2] = {};        // +0x0A  not written by AddInstance
    uint8_t  scale = 0;             // +0x0C  live scale (zeroed to hide the instance)
    int8_t   lastLod = -2;          // +0x0D
    uint8_t  scaleOriginal = 0;     // +0x0E  WH addition: backup for un-hide
    uint8_t  _pad0F = 0;            // +0x0F
};
static_assert(sizeof(SMMRMInstance) == 0x10, "stride proven at the allocator + AddInstance");
