#pragma once
#include <cstdint>
#include "C_AutoTriggerable.h"
#include "C_Node.h"

// -----------------------------------------------
// wh::conceptmodule::C_ModuleBase -- concept-graph "module" node base (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Base of the questmodule node family (C_Quest, C_Gameplay, C_ActivityCollection,
// C_ActivityType). Chain: C_ModuleBase -> C_AutoTriggerable<C_Node> -> C_Node ->
// C_SharedResource. Ctor sub_1806B193C: C_AutoTriggerable<C_Node> ctor (sub_1806B2130), own
// vftable, 9 qwords zeroed @+0x40..+0x87, two empty CryStrings @+0x88/+0x90 (helper
// sub_1803D1670 = empty-CryString init: shared header +12), word 0 @+0x98, byte 0 @+0x9B,
// dword 0x7FFFFFFF @+0x9C.  sizeof 0xA0 (PROVEN: C_Gameplay = this + no members, alloc 160;
// C_Quest places its first own member at +0xA0).

namespace wh::conceptmodule {

class C_ModuleBase : public C_AutoTriggerable<C_Node> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ModuleBase;
    uint64_t m_unk40[9];         // +0x40..+0x87  ctor 0 [roles not walked]
    CryStringT<char> m_str88;    // +0x88  empty CryString [role UNVERIFIED]
    CryStringT<char> m_str90;    // +0x90  empty CryString [role UNVERIFIED]
    uint16_t m_word98;           // +0x98  ctor 0 [role UNVERIFIED]
    uint8_t  _pad9A;             // +0x9A  not written by ctor
    uint8_t  m_flag9B;           // +0x9B  ctor 0 [role UNVERIFIED]
    int32_t  m_param9C;          // +0x9C  ctor 0x7FFFFFFF (INT_MAX sentinel) [role UNVERIFIED]
};
static_assert(sizeof(C_ModuleBase) == 0xA0, "C_ModuleBase must be 0xA0 (C_Gameplay alloc / C_Quest first member)");

}  // namespace wh::conceptmodule
