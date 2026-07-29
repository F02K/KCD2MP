#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_AlchemyEmptyParams.h"

// -----------------------------------------------
// wh::playermodule::C_OpenBookAction -- open/read the recipe book (KCD2 1.5.6, kd7u).  sizeof 0xB8.
// -----------------------------------------------
// RTTI .?AVC_OpenBookAction@playermodule@wh@@ (TD 0x184DD4628); ctor sub_1808D19BC; factory
// sub_1808D1AA0 = VERB 19 (never permission-blocked).  Enters the "alchemy_reading" action-map
// context (read_next/read_prev/read_select/read_toggle/read_back).  Completion hash
// "StartReading" (+0xB0); +0xAC holds a fragment id read from GameContext[32]+248.
//
// AUTOCOOK LINK: the autocook montage's completion callback (installed via sub_180A96308) binds a
// C_OpenBookAction member -- verb 19 is the action that owns/drives the C_AlchemyAnimAction
// montage while the book flow runs the brew.

namespace wh::playermodule {

class C_OpenBookAction : public C_AlchemyActionBase<S_EmptyParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_OpenBookAction;

    uint64_t _pad68[5];        // +0x68  [contents UNVERIFIED]
    uint8_t  m_state90;        // +0x90  ctor = 6 [role UNVERIFIED]
    uint8_t  _pad91[0x1B];     // +0x91
    int32_t  m_fragId;         // +0xAC  fragment id (GameContext[32]+248)
    uint32_t m_evStartReading; // +0xB0  ~CRC32("startreading")
    uint32_t _padB4;           // +0xB4
};
static_assert(sizeof(C_OpenBookAction) == 0xB8, "C_OpenBookAction must be 0xB8");
static_assert(offsetof(C_OpenBookAction, m_fragId) == 0xAC, "frag id at 0xAC");

}  // namespace wh::playermodule
