#pragma once

// -----------------------------------------------
// wh::entitymodule::I_ItemCollectionListener -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// Standalone item-collection listener. RTTI CHD 0x1842B1140 contains no base
// classes; vtable 0x183A65020 has five no-op slots and a deleting destructor.

namespace wh::entitymodule {

class I_ItemCollectionListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ItemCollectionListener;
    virtual void ItemCollectionListenerUnk0() {}  // [0] [U signature]
    virtual void ItemCollectionListenerUnk1() {}  // [1] [U signature]
    virtual void ItemCollectionListenerUnk2() {}  // [2] [U signature]
    virtual void ItemCollectionListenerUnk3() {}  // [3] [U signature]
    virtual void ItemCollectionListenerUnk4() {}  // [4] [U signature]
    virtual ~I_ItemCollectionListener() = default; // [5] sub_1829174F8
};
static_assert(sizeof(I_ItemCollectionListener) == 0x08, "I_ItemCollectionListener must be vptr-only");

}  // namespace wh::entitymodule
