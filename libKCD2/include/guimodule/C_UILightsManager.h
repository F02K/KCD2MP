#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::guimodule::C_UILightsManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x40.
// -----------------------------------------------
// RTTI .?AVC_UILightsManager@guimodule@wh@@ (TD 0x184C9EDD0, COL 0x413A630, no bases).
// vtable 0x183B8F508 (1 slot: deleting dtor sub_182B11524). ctor sub_18146D41C.
// NEW vs KCD1. NOT module-owned: lives EMBEDDED in C_UIApseCamera at +0xF8 (ctor
// sub_18146D1E0 @0x18146d233; the neighbours at +0xF8-0x40.. and +0x138 pin sizeof =
// 0x40) -- scene-light control for the "Apse" 3D character/shop viewport.
//
// Only the ctor-observable layout is certified; member roles are UNVERIFIED (writers
// are Apse-scene setup paths outside the G2 spine scope).

namespace wh::guimodule {

class C_UILightsManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UILightsManager;
    virtual ~C_UILightsManager();      // [0] deleting dtor 0x182B11524 (sole slot)

    std::vector<void*> m_lights;      // +0x08  {first,last,end}; dtor sub_182B10B58 deletes each *elem via vtable[0](e,1) then deallocates -- owned polymorphic light objects (pointee type UNVERIFIED)
    void*             m_20;            // +0x20  ctor 0; NOT freed by dtor sub_182B10B58 (non-owning) [role UNVERIFIED]
    bool              m_28;            // +0x28  ctor 0 (UNVERIFIED)
    uint8_t           _pad29[7];       // +0x29
    CryStringT<char>  m_name;          // +0x30  ctor empty (CryString empty-header idiom sub_1804FD80C()+12)
    bool              m_38;            // +0x38  ctor 0 (UNVERIFIED)
    uint32_t          m_3C;            // +0x3C  ctor 0 (UNVERIFIED)
};
static_assert(sizeof(C_UILightsManager) == 0x40, "C_UILightsManager must be 0x40");

}  // namespace wh::guimodule
