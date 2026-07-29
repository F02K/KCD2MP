#pragma once
#include <cstdint>
#include "C_SoulEffect.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_AddItemEffect -- add item(s) to a soul's inventory (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E7D8  vtable 0x183F1EE10.  sizeof 0x60 (PROVEN: factory sub_180B45964
// operator new(96)).  Ctor sub_180B462D4(this, owner, targetWuid, &itemClassGuid, a5, a6, a7,
// &guid2, floatBits); factory defaults: a7=0, guid2=xmmword_183A79D88 const, float=-1.0f
// (bits 0xBF800000 via int_convert).

namespace wh::rpgmodule {

class C_AddItemEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AddItemEffect;
    CryGUID  m_itemClassId;   // +0x18  16-byte ctor arg 4 [guid kind INFERRED from width]
    int32_t  m_param28;       // +0x28  ctor arg 5 [role UNVERIFIED -- amount candidate]
    int32_t  m_param2C;       // +0x2C  ctor arg 6 [role UNVERIFIED]
    int32_t  m_param30;       // +0x30  ctor arg 7, factory default 0 [role UNVERIFIED]
    float    m_param34;       // +0x34  ctor arg 9, factory default -1.0f [role UNVERIFIED -- health/quality candidate]
    CryGUID  m_guid38;        // +0x38  16-byte ctor arg 8, factory default = xmmword_183A79D88 [role UNVERIFIED]
    std::vector<uint64_t> m_createdItemIds;   // +0x48  ctor zeroes {first,last,cap}; Apply(0x181666970) push_back's *(spawnedItem+0x48) once per created item; element = 8-byte handle (wh::framework::WUID candidate, INFERRED). Name inferred.
};
static_assert(sizeof(C_AddItemEffect) == 0x60, "C_AddItemEffect must be 0x60 (operator new(96))");

}  // namespace wh::rpgmodule
