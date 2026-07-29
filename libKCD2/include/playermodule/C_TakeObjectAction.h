#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_IntParams.h"

// -----------------------------------------------
// wh::playermodule::C_TakeObjectAction -- grab table object into hand (KCD2 1.5.6, kd7u).  sizeof 0x78.
// -----------------------------------------------
// RTTI .?AVC_TakeObjectAction@playermodule@wh@@ (TD 0x184DD4FF0); vtable 0x183A5CCD0; ctor
// sub_1808D3FAC; factory sub_1808D3EF0 (verb 0 -> m_params.m_value=0, verb 5 -> 1; gates:
// verb 0 needs the ingredient vec non-empty, verb 5 needs >=2 pot slots).  Plays the
// AlchemyTake fragment; completes on the "Attach" and "ActionEnd" Mannequin events.

namespace wh::playermodule {

class C_TakeObjectAction : public C_AlchemyActionBase<S_IntParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TakeObjectAction;

    uint32_t m_evAttach;      // +0x68  ~CRC32("attach")    (table dword_1840999F0, lowercased)
    uint32_t m_evActionEnd;   // +0x6C  ~CRC32("actionend")
    uint64_t _pad70;          // +0x70
};
static_assert(sizeof(C_TakeObjectAction) == 0x78, "C_TakeObjectAction must be 0x78");
static_assert(offsetof(C_TakeObjectAction, m_evAttach) == 0x68, "crc fields at 0x68");

}  // namespace wh::playermodule
