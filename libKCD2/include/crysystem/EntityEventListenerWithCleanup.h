#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// ::EntityEventListenerWithCleanup -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x20.
// -----------------------------------------------
// RTTI .?AUEntityEventListenerWithCleanup@@ (GLOBAL namespace struct) : .?AUIEntityEventListener@@.
// KCD2 wrapper around the engine IEntityEventListener that tracks its own event registrations and
// auto-unsubscribes them on destruction. NOT a bare interface: the vector at +0x08 holds 8-byte
// {uint32 eventId, uint32 param} registration pairs (freed in the base cleanup dtor sub_181E3B500).
// Used as a base of C_CombatActor (@+0x08, vtable 0x183E3FAB0) and C_Item (@+0x08). It REPLACES
// KCD1's separate I_ItemAttachmentListener + IEntityEventListener bases.
//
// IEntityEventListener is declared here (verbatim 2-slot shape of the stock CryCommon
// IEntitySystem.h declaration) because the stock header cannot compile in this build env (its
// IComponent.h requires boost). Do NOT also include the stock IEntitySystem.h in a TU using this.

struct IEntity;
struct SEntityEvent;

struct IEntityEventListener {
    virtual ~IEntityEventListener() {}                                    // [0]
    virtual void OnEntityEvent(IEntity* pEntity, SEntityEvent& event) = 0; // [1]
};

struct EntityEventListenerWithCleanup : public IEntityEventListener {
    std::vector<uint64_t> m_events;   // +0x08  registered {eventId,param} pairs (auto-unsubscribe list)
};
static_assert(sizeof(EntityEventListenerWithCleanup) == 0x20, "EntityEventListenerWithCleanup must be 0x20");
