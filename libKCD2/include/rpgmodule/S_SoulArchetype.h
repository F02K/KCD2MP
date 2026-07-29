#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::rpgmodule::S_SoulArchetype -- soul_archetype table row (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AUS_SoulArchetype@rpgmodule@wh@@ (TD 0x184D415C8; polymorphic -- own vtable 0x183B52798).
// Row type of C_SoulArchetypeDatabase (C_ObjectDatabaseIdIndexed stack with an I_DynamicEnum
// view). Row stride 0x58 (PROVEN: the I_DynamicEnum impls index the row array with *88).
// Only the enum-view fields are typed (value @+0x08, name @+0x10); the rest is unread in the
// decompiled surface. The vfptr slot count is NOT enumerated.

namespace wh::rpgmodule {

struct S_SoulArchetype {
    inline static constexpr auto RTTI = Offsets::RTTI_S_SoulArchetype;
    virtual ~S_SoulArchetype() = 0;   // +0x00 vfptr (slot set NOT enumerated; abstract in this mirror)

    int32_t  m_id;          // +0x08  I_DynamicEnum::GetValue reads row+0x08
    uint32_t _pad0C;        // +0x0C
    CryStringT<char> m_name;  // +0x10  RTTR member_object_ptr CryStringT<char> (GetName returns its char*)
    int32_t  m_field18;     // +0x18  ctor default -1 (index/id sentinel; loader copies DBData+0x10)
    int32_t  m_field1C;     // +0x1C  ctor default 4 (small int/enum)
    float    m_field20;     // +0x20  ctor default 1.0f (SSE-init lane0 @xmmword_18409EE30)
    float    m_field24;     // +0x24  ctor default 0.0f (SSE block +0x20..+0x2F) [float inferred]
    float    m_field28;     // +0x28  ctor default 0.0f (SSE block) [float inferred]
    float    m_field2C;     // +0x2C  ctor default 0.0f (SSE block) [float inferred]
    int32_t  m_field30;     // +0x30  ctor default 0 [int/float ambiguous]
    float    m_field34;     // +0x34  ctor default 100.0f
    float    m_field38;     // +0x38  ctor default 100.0f
    uint8_t  m_field3C;     // +0x3C  1-byte field (bool candidate; loader copies DBData+0x34)
    uint8_t  _pad3D[3];     // +0x3D  align pad
    float    m_field40;     // +0x40  ctor fmaxf(0,x) clamp>=0; loader parses via sub_1823D9D34
    int32_t  m_field44;     // +0x44  ctor default 0 [int/float ambiguous]
    int32_t  m_field48;     // +0x48  ctor default 0 [int/float ambiguous]
    int32_t  m_field4C;     // +0x4C  ctor default 0 [int/float ambiguous]
    int32_t  m_field50;     // +0x50  ctor default 0 [int/float ambiguous]
    uint8_t  _pad54[4];     // +0x54  tail pad to 0x58 (not copied by copy-ctor)
};
static_assert(sizeof(S_SoulArchetype) == 0x58, "S_SoulArchetype must be 0x58 (I_DynamicEnum stride 88)");

}  // namespace wh::rpgmodule
