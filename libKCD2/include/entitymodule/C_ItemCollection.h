#pragma once
#include <cstdint>
#include "I_ItemCollectionListener.h"
#include "../framework/WUID.h"
#include "../framework/C_Listeners.h"

// -----------------------------------------------
// wh::entitymodule::C_ItemCollection -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// Secondary base of C_Inventory (@+0xE8, own sub-vtable 0x183A2D698). Holds the collection-listener
// registry. base-ctor sub_1803A4CA0. Has its own vtable (0x183A2D698) at +0x00; the keyed registry
// member at +0x08 carries its own (C_DependentListeners) vtable -- two distinct vtables.

namespace wh::entitymodule {

class C_ItemCollection {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemCollection;
    virtual ~C_ItemCollection() = default;      // +0x00  own sub-vtable 0x183A2D698
    // Keyed collection-listener registry (0x68): inner vtable @+0x08, delegate vector, WUID guards.
    wh::shared::C_DependentListeners<I_ItemCollectionListener, wh::framework::WUID, 2> m_collectionListeners;  // +0x08
};
static_assert(sizeof(C_ItemCollection) == 0x70, "C_ItemCollection must be 0x70");

}  // namespace wh::entitymodule
