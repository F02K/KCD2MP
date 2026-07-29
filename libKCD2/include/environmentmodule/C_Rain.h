#pragma once
#include <cstdint>
#include "../Offsets/vtables/IActor.h"   // Offsets::IGameObjectExtension + CGameObjectExtensionHelper

// -----------------------------------------------
// wh::environmentmodule::C_Rain -- rain game-object extension (KCD2 1.5.6, kd7u).
// sizeof 0x180 (alloc 384).
// -----------------------------------------------
// Own vtable 0x18471F090; ctor sub_1839D6990; shared_ptr wrapper sub_1839D7614. CryEngine
// GameObjectExtension (RTTI: CGameObjectExtensionHelper<C_Rain,IGameObjectExtension,0x180>
// -> IGameObjectExtension -> IComponent -> enable_shared_from_this @+0x08). The data-only
// Offsets::IGameObjectExtension base supplies +0x08..+0x3F (weak self-ref, distributer,
// m_pGameObject @+0x28, m_entityId @+0x30, m_pEntity @+0x38); the vptr below lands at +0x00.
// Ctor zeroes that base span, builds the ~0x120 particle-effect holder subobject via
// sub_180B6D7E4(+0x40), then the tail flag/pointers. Extension slot set NOT certified
// (interfuscated SDK side) -- do not call through this vtable.

namespace wh::environmentmodule {

class C_Rain
    : public Offsets::CGameObjectExtensionHelper<C_Rain, Offsets::IGameObjectExtension, 0x180> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Rain;
    virtual ~C_Rain();                // [0]; further slots not certified (see header note)

    uint8_t  m_particleHolder[0x120]; // +0x40..+0x15F sub_180B6D7E4 subobject (particle-effect
                                      //               holder; interior not walked)
    uint8_t  m_flag160;               // +0x160  ctor 0
    uint8_t  _pad161[7];              // +0x161
    std::vector<ITexture*> m_rainTextures;  // +0x168  {first,last,end}; dtor sub_182ADA98C Releases each elem (vtbl+0x10) then frees buffer; filled sub_182ADB5F4 from %ENGINE%/EngineAssets/raintextures.xml
};
static_assert(sizeof(C_Rain) == 0x180, "C_Rain must be 0x180 (alloc 384)");
static_assert(offsetof(C_Rain, m_particleHolder) == 0x40, "particle holder at +0x40");

}  // namespace wh::environmentmodule
