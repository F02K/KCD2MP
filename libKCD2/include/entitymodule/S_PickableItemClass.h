#pragma once
#include <cstddef>
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"
#include "S_ItemClass.h"

// -----------------------------------------------
// wh::entitymodule::S_PickableItemClass -- world-pickable item classes (KCD2 WHGame.dll 1.5.6).
// sizeof 0x90.  E_ItemType 22 (intermediate); direct base of S_PlayerItemClass + S_NPCToolItemClass.
// -----------------------------------------------
// Vtable 0x183A4EE38, ctor sub_180754C3C.  Field TYPES + defaults are ctor-verified; most
// PURPOSES are not yet pinned (the XML-attr binding is RTTR-reflected and was not traced to
// offset level -- dossier analysis/item_data/s_itemdata_re.md §2/§6).
namespace wh::entitymodule {

class S_PickableItemClass : public S_ItemClass {
public:
    CryStringT<char> m_str38;   // +0x38  \ four class-config strings (model/material/sound
    CryStringT<char> m_str40;   // +0x40  | candidates); purposes UNVERIFIED
    CryStringT<char> m_str48;   // +0x48  |
    CryStringT<char> m_str50;   // +0x50  /
    float    m_unk58;           // +0x58  ctor 0.1f (weight candidate -- "Weight" RTTR string; UNVERIFIED)
    float    m_unk5C;           // +0x5C  ctor 0
    float    m_unk60;           // +0x60  ctor 1.0f
    float    m_unk64;           // +0x64  ctor 1.0f, clamped [0,1]
    int32_t  m_unk68;           // +0x68  ctor 0
    bool     m_unk6C;           // +0x6C  ctor 0
    uint8_t  _pad6D[3];         // +0x6D
    uint64_t m_unk70;           // +0x70  ctor 0 [role UNVERIFIED]
    uint64_t m_unk78;           // +0x78  ctor 0 [role UNVERIFIED]
    uint64_t m_unk80;           // +0x80  ctor 0 [role UNVERIFIED]
    int32_t  m_unk88;           // +0x88  ctor 0
    bool     m_unk8C;           // +0x8C  ctor 0
    uint8_t  _pad8D[3];         // +0x8D
};

static_assert(sizeof(S_PickableItemClass) == 0x90, "S_PickableItemClass must be 0x90");
static_assert(offsetof(S_PickableItemClass, m_unk58) == 0x58, "floats at 0x58");

}  // namespace wh::entitymodule
