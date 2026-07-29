#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::E_UIApseState -- KCD2 WHGame.dll 1.5.6 (kd7u).  Enum wrapper, Type : uint8.
// -----------------------------------------------
// Nested-Type enum wrapper. Existence, namespace, width and default are PROVEN
// from RTTI: C_UIApse::m_stateProp (+0x78) is a
// C_ModelProperty<E_UIApseState::Type> (mangled RTTI @0x184c92110, readonly
// variant @0x184c92050, C_StaticDefaultValueTrait default 0).
//
// ENUMERATORS ARE NOT RECOVERABLE from the binary: the enum has no RTTR
// enumeration registration and no reachable ToString (unlike its sibling
// E_UIApseView, sub_180C43980). Do NOT invent members; recover values later by
// tracing the writers of C_UIApse+0x78's value byte.

namespace wh::guimodule {

struct E_UIApseState {
    enum Type : uint8_t {
        // Default = 0 (RTTI default trait). Enumerators unrecovered.
    };
};

}  // namespace wh::guimodule
