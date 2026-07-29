#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_UsePotAction -- pour the finished potion into the phial (KCD2 1.5.6, kd7u).
// sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_UsePotAction@playermodule@wh@@ (TD 0x184DD5518); vtable 0x183F605C0; ctor
// sub_182E132E8 (finish-phase factory).  The brew-completion verb: plays the pot->flask pour
// (PotionPouring .cax), then the normal completion path grades the brew and grants products/XP.

namespace wh::playermodule {

class C_UsePotAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UsePotAction;

    uint32_t m_evAttachPot;      // +0x68  ~CRC32("attachpot")
    uint32_t m_evDetachPot;      // +0x6C  ~CRC32("detachpot")
    uint32_t m_evDetachFlask;    // +0x70  ~CRC32("detachflask")
    uint32_t m_evPotionPouring;  // +0x74  ~CRC32("potionpouring")
};
static_assert(sizeof(C_UsePotAction) == 0x78, "C_UsePotAction must be 0x78");

}  // namespace wh::playermodule
