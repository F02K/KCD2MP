#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::entitymodule::C_PhysicsIgnoreLists -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x58.
// -----------------------------------------------
// Embedded at C_Actor+0x1D0 (boundary-proven: next sub-object ctor runs at +0x228). ctor
// sub_1811FF97C(this, ownerActor); dtor sub_1808E1888 frees only the two CryString names.
// The two sub-lists are a generic reusable "named owner-bound handle" (also built by
// C_ActorActionSyncAnimMaster with name "ActorActionSyncAnim") -- NOT a std::vector<EntityId>;
// the +0x08 pointers are lazily-resolved non-owning handles (dtor never frees them).

namespace wh::entitymodule {

class C_Actor;

// Generic named owner-bound handle (0x20; real class name unknown -- descriptive).
struct S_PhysicsIgnoreList {
    C_Actor*         m_pOwner;     // +0x00  back-ptr; element ctor sub_1811FFA0C stores the owner C_Actor (list embedded at C_Actor+0x1D0)
    void*            m_pResolved;  // +0x08  lazy NON-OWNING handle (ctor 0; pointee type unresolved)
    int32_t          m_index;      // +0x10  slot index (0 / 1)
    uint8_t          _pad14[4];    // +0x14
    CryStringT<char> m_name;       // +0x18  "Actor_EntityPhysicsIgnore" / "Actor_AdditionalPhysicsIgnore"
};
static_assert(sizeof(S_PhysicsIgnoreList) == 0x20, "S_PhysicsIgnoreList must be 0x20");

class C_PhysicsIgnoreLists {
public:
    C_Actor* m_pOwner;             // +0x00
    void*    m_pField08;           // +0x08  lazy non-owning handle (role unresolved)
    uint16_t m_field10;            // +0x10  (ctor 0; flags/count -- unresolved)
    uint8_t  m_field12;            // +0x12  (ctor 0)
    uint8_t  _pad13[5];            // +0x13
    S_PhysicsIgnoreList m_list0;   // +0x18  "Actor_EntityPhysicsIgnore",     index 0
    S_PhysicsIgnoreList m_list1;   // +0x38  "Actor_AdditionalPhysicsIgnore", index 1
};
static_assert(sizeof(C_PhysicsIgnoreLists) == 0x58, "C_PhysicsIgnoreLists must be 0x58");

}  // namespace wh::entitymodule
