#pragma once
#include <cstdint>
#include "C_ItemEffect.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"

// -----------------------------------------------
// wh::rpgmodule::C_ItemDestroyedEffect -- report an item's destruction (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// RTTI TD 0x184D4E4A8  vtable 0x183F1ED70.  sizeof 0x38 (PROVEN: factory sub_182CD4140
// operator new(56)).  Ctor sub_182CA7E34(this, owner, targetItemWuid, a4): defaults
// m_itemClassId = xmmword_183F1F0D0 const and m_param30 = -1, then resolves the target item
// via module-registry (+0xE0 module -> +0xC8 -> +0x168 map lookup by m_targetWuid): on hit
// copies the 16 bytes at item-data+0x01 into +0x20, and if item flag 0x1B is set pulls a
// dword via item vfunc[31] into +0x30.

namespace wh::rpgmodule {

class C_ItemDestroyedEffect : public C_ItemEffect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemDestroyedEffect;
    uint64_t m_param18;      // +0x18  ctor arg 4 [role UNVERIFIED -- destroyer WUID candidate]
    CryGUID  m_itemClassId;  // +0x20  from item lookup, default xmmword_183F1F0D0 [guid kind INFERRED]
    int32_t  m_param30;      // +0x30  default -1; item vfunc[31] result +0x08 when flag 0x1B [role UNVERIFIED]
    // +0x34 4-byte tail padding
};
static_assert(sizeof(C_ItemDestroyedEffect) == 0x38,
              "C_ItemDestroyedEffect must be 0x38 (operator new(56))");

}  // namespace wh::rpgmodule
