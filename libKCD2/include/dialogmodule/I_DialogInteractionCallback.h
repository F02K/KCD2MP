#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::dialogmodule::I_DialogInteractionCallback -- interaction-response sink
// (KCD2 1.5.6, kd7u).  1 slot, NO virtual dtor.
// -----------------------------------------------
// RTTI .?AVI_DialogInteractionCallback@dialogmodule@wh@@ (TD 0x184B4B8A0, abstract, no COL).
// Implemented by C_DialogInstance @+0x08 (vtable 0x183E63AF0, exactly 1 slot):
// sub_1808D7A94(this, uint id, bool) routes an interaction response -- forwards to the
// inner extension object (full+368) vtbl+48 if live, else stores `id` at full+568 and (if
// bool set) resets the state word full+668 = 256. Name "OnInteractionResponse"
// (OnResponse/OnDecisionSelected) UNVERIFIED. The binary vtable has NO dtor slot.

namespace wh::dialogmodule {

class I_DialogInteractionCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DialogInteractionCallback;
    virtual void OnInteractionResponse(uint32_t id, bool b) = 0;   // [0] sub_1808D7A94 [name UNVERIFIED]
};
static_assert(sizeof(I_DialogInteractionCallback) == 0x08);

}  // namespace wh::dialogmodule
