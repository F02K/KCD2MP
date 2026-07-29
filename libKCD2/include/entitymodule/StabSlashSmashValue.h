#pragma once

// -----------------------------------------------
// wh::entitymodule::StabSlashSmashValue -- RTTR-registered 3-float damage-component triple
// (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTR registration sub_1801BE950 declares the literal type name
// "wh::entitymodule::StabSlashSmashValue" (NOT rpgmodule, NOT S_-prefixed -- corrects an
// external proposal that guessed both wrong) with properties "Stab"/"Slash"/"Smash" at byte
// offsets 0/4/8. Used throughout S_DamageEventData's stab/slash/smash-bitmask-addressed
// float triples (see E_DamageTypeMask.h).

namespace wh::entitymodule {

struct StabSlashSmashValue {
    float stab;   // +0x00
    float slash;  // +0x04
    float smash;  // +0x08
};
static_assert(sizeof(StabSlashSmashValue) == 0x0C, "StabSlashSmashValue must be 0x0C (3 floats)");

}  // namespace wh::entitymodule
