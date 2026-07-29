#pragma once
#include <cstdint>
#include "C_SoulEffect.h"
#include "../CryEngine/CryCommon/BaseTypes.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_AddBuffEffect -- apply a buff to a soul (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184CE4BB8  vtable 0x183A68CA0.  sizeof 0x40 (ctor sub_1809DC928 write extent;
// in-place ctor (owner, targetWuid, &buffId, &sourceId)).
// Both 16-byte ids modeled as CryGUID [kind INFERRED from KCD1 buff.xml GUIDs].

namespace wh::rpgmodule {

class C_AddBuffEffect : public C_SoulEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AddBuffEffect;
    CryGUID m_buffId;     // +0x18  buff to apply
    CryGUID m_sourceId;   // +0x28  second id (source/init-params candidate, UNVERIFIED)
    void*   m_pUnk38;     // +0x38  ctor 0 [role UNVERIFIED]
};
static_assert(sizeof(C_AddBuffEffect) == 0x40, "C_AddBuffEffect must be 0x40 (ctor write extent)");

}  // namespace wh::rpgmodule
