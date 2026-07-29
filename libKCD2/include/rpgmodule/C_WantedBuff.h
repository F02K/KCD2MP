#pragma once
#include "C_TwoStateSoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_WantedBuff : C_TwoStateSoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_WantedBuff@rpgmodule@wh@@  vtable 0x183F0DBB8  make sub_182BFD9AC (alloc 216 = 0xD8,
// memset 0xD8, base ctor sub_18046C4DC, set vtable) -- adds NO members (VERIFY-confirmed).
// The crime-system "wanted" status buff (KCD1 analog present unchanged). Per-type default-def
// sentinel qword_18532C378 (RUNTIME-populated; all-zero on disk); on match the default id is
// CONSTRUCTED by sub_18046C474 (generated 7-byte value | tag byte 0x01).
// Chain: C_WantedBuff : C_TwoStateSoulBuffInstance : C_SoulBuffInstance : C_BuffInstance<C_Soul>
// : C_BuffInstanceBase (all mdisp 0).

namespace wh::rpgmodule {

class C_WantedBuff : public C_TwoStateSoulBuffInstance   // +0x00  (0xD8; no own members)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_WantedBuff;
};
static_assert(sizeof(C_WantedBuff) == 0xD8, "C_WantedBuff must be 0xD8");

}  // namespace wh::rpgmodule
