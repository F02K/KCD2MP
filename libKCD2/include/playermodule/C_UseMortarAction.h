#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"
#include "I_AlchemyMannequinListener.h"

// -----------------------------------------------
// wh::playermodule::C_UseMortarAction -- mill with the pestle (KCD2 1.5.6, kd7u).  sizeof 0x98.
// -----------------------------------------------
// RTTI .?AVC_UseMortarAction@playermodule@wh@@ (TD 0x184DD4B18); primary vtable 0x183F60770; ctor
// sub_181390F40; factory sub_181390E94 (verbs 2/3; gate: mortar bucket[2] non-empty, else the
// pot>=2 path sub_1808D395C).  Mannequin listener (cb sub_1806C38A8) into GameContext[48]+968.
// Mill chain: starter sub_1806C38B8 (find first unmilled record in bucket[2], play mill anim) ->
// anim cb sub_1806C3C50 -> on the mill event calls the WRITER sub_1806C4384 (every bucket[2]
// record: m_flags |= Milled) + model swap sub_1806C4BCC (HerbsMortarMilled / crushed special
// model) + sub_1806C5164 (destroy the pre-mill prop).  MortarUseCount=1 (table Lua) squeezes per
// completed use.  Plays AlchemyPutMortar/AlchemyUse fragments.

namespace wh::playermodule {

class C_UseMortarAction
    : public C_AlchemyActionBase<S_EmptyParams>
    , public I_AlchemyMannequinListener {   // +0x68 listener subobject
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UseMortarAction;

    uint32_t _pad70;      // +0x70
    uint32_t m_evAttach;  // +0x74  ~CRC32("attach")
    uint32_t m_evDetach;  // +0x78  ~CRC32("detach")
    uint32_t m_evUse;     // +0x7C  ~CRC32("use") -> the mill moment
    uint64_t _pad80[3];   // +0x80  listener node/state [contents UNVERIFIED]
};
static_assert(sizeof(C_UseMortarAction) == 0x98, "C_UseMortarAction must be 0x98");

}  // namespace wh::playermodule
