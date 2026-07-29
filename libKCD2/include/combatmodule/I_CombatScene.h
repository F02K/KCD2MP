#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::combatmodule::I_CombatScene -- interface for the combat scene
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable only).
// -----------------------------------------------
// RTTI .?AVI_CombatScene@combatmodule@wh@@   abstract vtable 0x183E5DD58
//   [0] deleting dtor (sub_1827C6170)   [1] purecall   [2] purecall   [3] = TD name bytes (boundary).
// So the interface is a virtual dtor + 2 pure methods.
//
// [vs KCD1] widened by one: KCD1 I_CombatScene had a single method (GetActorsByEntityId). KCD2 adds
// a second (the scene-wide actor-relation refresh below). Slots resolved from the C_CombatScene
// primary vtable 0x183A6C7D0.

namespace wh::combatmodule {

class C_CombatActor;

class I_CombatScene {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatScene;
    virtual ~I_CombatScene() = default;                                          // [0]

    // Collect every scene actor whose entity id matches. Iterates m_actors, calls each actor's
    // entity-id query (I_CombatActor vtable +0x48) and pushes matches into outActors (pre-reserved
    // to 3). Impl sub_180C5962C.
    virtual void GetActorsByEntityId(uint32_t entityId,
                                     std::vector<C_CombatActor*>& outActors) = 0; // [1]

    // Rebuild the scene-wide actor-relation state: gathers the live actors, builds an entity-id
    // index and refreshes each actor's per-frame relation fields (writes actor+0x384). Impl
    // sub_1809D45A4. [name inferred from behavior]
    virtual void RefreshActorRelations() = 0;                                    // [2]
};

}  // namespace wh::combatmodule
