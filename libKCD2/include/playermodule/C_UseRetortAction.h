#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"
#include "I_AlchemyMannequinListener.h"

// -----------------------------------------------
// wh::playermodule::C_UseRetortAction -- distill through the retort (KCD2 1.5.6, kd7u).  sizeof 0xC0.
// -----------------------------------------------
// RTTI .?AVC_UseRetortAction@playermodule@wh@@ (TD 0x184DD5448); vtable 0x183F601E0; ctor
// sub_182E13450; factory sub_182E0F240 (state 5, verb 4).  Mannequin listener sub_182E0FF2C
// (cb sub_182E17FD4) into GameContext[48]+984.  Distill machine sub_182E1719C: sets mode 5
// (distilling) + dirty=3, counts bellows pumps vs BellowsUseCountToDistill (sub_182E16834; table
// Lua, CVar-clamped); on reaching the count sub_182E16258 -> WRITER sub_182E2B148 (every retort
// bucket[4] record: m_flags |= Distilled) + bucket move 4->5 (sub_1806C4D5C).  Uses action-map
// context "alchemy_use_bellow_distillation".

namespace wh::playermodule {

class C_UseRetortAction
    : public C_AlchemyActionBase<S_EmptyParams>
    , public I_AlchemyMannequinListener {   // +0x68 listener subobject
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_UseRetortAction;

    uint32_t _pad70;              // +0x70
    uint32_t m_evDetachFlask;     // +0x74  ~CRC32("detachflask")
    uint32_t m_evDetach;          // +0x78  ~CRC32("detach")
    uint32_t m_evAttachRetort;    // +0x7C  ~CRC32("attachretort")
    uint32_t m_evDetachRetort;    // +0x80  ~CRC32("detachretort")
    uint32_t m_evAttach;          // +0x84  ~CRC32("attach")
    uint32_t m_evAttachPot;       // +0x88  ~CRC32("attachpot")
    uint32_t m_evDetachPot;       // +0x8C  ~CRC32("detachpot")
    uint32_t m_evAnimateBellows;  // +0x90  ~CRC32("animatebellows")
    uint32_t _pad94;              // +0x94
    uint64_t _pad98[3];           // +0x98  [contents UNVERIFIED]
    void*    m_pObjB0;            // +0xB0  retort-phase object [role UNVERIFIED]
    uint64_t _padB8;              // +0xB8
};
static_assert(sizeof(C_UseRetortAction) == 0xC0, "C_UseRetortAction must be 0xC0");

}  // namespace wh::playermodule
