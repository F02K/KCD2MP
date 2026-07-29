#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_UICharacterState -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : uint8.
// -----------------------------------------------
// Nested-Type enum wrapper. Existence, namespace, width and default are PROVEN
// from RTTI: C_UIApse::m_charStateProp (+0xA0) is a
// C_ModelProperty<E_UICharacterState::Type> (mangled RTTI @0x184c92300,
// readonly variant @0x184c92240, C_StaticDefaultValueTrait default 0).
//
// ENUMERATORS ARE NOT RECOVERABLE from the binary (no RTTR enumeration
// registration, no reachable ToString). Do NOT invent members; recover values
// later by tracing the writers of C_UIApse+0xA0's value byte.

namespace wh::guimodule {

struct E_UICharacterState {
    enum Type : uint8_t {
        // Default = 0 (RTTI default trait). Enumerators unrecovered.
    };
};

}  // namespace wh::guimodule
