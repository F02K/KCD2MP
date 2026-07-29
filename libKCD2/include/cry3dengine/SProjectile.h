#pragma once
#include <cstdint>

// -----------------------------------------------
// SProjectile -- merged-mesh projectile/bullet disturbance record,
// KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Element of CMergedMeshesManager's vector at +0xC068 -- stride 0x38 proven by the
// /0x38 size computations (0x6DB6DB6DB6DB6DB7 magic-multiply at 0x180e8bfa7/0x1834dbc1b);
// cap 1024 in AddProjectile (sub_1834DBBB8, spinlock +0xC088). Fed by the physics
// EventPhysCollision handler (sub_1834EBE8C: entity type 5, e_MergedMeshesBulletSpeedFactor/
// Lifetime cvars, size clamp [1.0, 8.5]). Only the three leading Vec3s are field-proven;
// the tail is an explicit unknown.

struct SProjectile
{
    // three Vec3s written on insert (pos/dir/aux -- roles unresolved)
    Vec3    _vec00;              // +0x00
    Vec3    _vec0C;              // +0x0C
    Vec3    _vec18;              // +0x18
    uint8_t _unk24[0x14] = {};   // +0x24 .. 0x37  unknown (lifetime/size/entity ref live here)
};
static_assert(sizeof(SProjectile) == 0x38, "stride proven by the /0x38 size computations");
