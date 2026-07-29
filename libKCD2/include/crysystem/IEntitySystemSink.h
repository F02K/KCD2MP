#pragma once

// -----------------------------------------------
// ::IEntitySystemSink -- entity-system spawn/remove/event callback interface (CryEngine stock,
// KCD2 WHGame.dll 1.5.6).  sizeof 0x08 (vtable-only).
// -----------------------------------------------
// Declared here (verbatim 7-slot shape of the stock CryCommon IEntitySystem.h declaration) because
// the stock header cannot compile in this build env (its IComponent.h requires boost) and it would
// also redefine IEntityEventListener against crysystem/EntityEventListenerWithCleanup.h in the same
// TU. Do NOT also include the stock IEntitySystem.h in a TU using this.
//
// NOTE: the interface is <interfuscator:shuffle>d in the shipping binary -- the declaration order
// below is the SDK source order, NOT the runtime vtable order. Do not call through these slots by
// declaration index; the interface is mirrored for layout (one vtable pointer) only.
// Used as a base of wh::combatmodule::C_CombatScene (@+0x10, vtable 0x183A6C7F0).

struct IEntity;
struct SEntitySpawnParams;
struct SEntityEvent;
class ICrySizer;

struct IEntitySystemSink {
    virtual ~IEntitySystemSink() {}
    virtual bool OnBeforeSpawn(SEntitySpawnParams& params) = 0;
    virtual void OnSpawn(IEntity* pEntity, SEntitySpawnParams& params) = 0;
    virtual bool OnRemove(IEntity* pEntity) = 0;
    virtual void OnReused(IEntity* pEntity, SEntitySpawnParams& params) = 0;
    virtual void OnEvent(IEntity* pEntity, SEntityEvent& event) = 0;
    virtual void GetMemoryUsage(ICrySizer* pSizer) const {}
};
static_assert(sizeof(IEntitySystemSink) == 0x08, "IEntitySystemSink is vtable-only");
