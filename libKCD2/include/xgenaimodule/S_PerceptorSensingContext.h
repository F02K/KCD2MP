#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::S_PerceptorSensingContext -- the 0x160-byte perceptor sensing
// aggregate ticked by the perception update (KCD2 WHGame.dll 1.5.6, kd7u).
// [NON-RTTI NAME]: plain aggregate, no vtable/RTTI -- descriptive name.
// sizeof 0x160 (alloc 352 in C_PerceptionManager secondary init sub_180F661D0
// @0x180f6620d; stored at PM+0x1B0).
// -----------------------------------------------
// Built by sub_180F661D0 (@0x180f66235..0x180f6630a).  Ticked by
// sub_18061AFB8(this): update +0x118; sub_18061B074(+0x90); crowd count
// sub_18061B180(this) -> m_crowdCount; recognition loop sub_180584B88(this)
// iterating *(m_manager+0xD8) (the manager's perceptibles list [0]); then
// sub_18061B024(m_manager).  Interior subobjects at +0x08 (sub_180F66440),
// +0x90 (sub_180F6653C), +0x158 (sub_18078BDEC) not resolved [U].

namespace wh::xgenaimodule {

class C_PerceptionManager;

struct S_PerceptorSensingContext {
    C_PerceptionManager* m_manager;   // +0x000  back-ref (@0x180f66235)
    uint8_t  _sub08[0x88];            // +0x008  subobject, init sub_180F66440 [U interior]
    uint8_t  _sub90[0x48];            // +0x090  subobject, init sub_180F6653C(this, mgr) [U interior]
    uint64_t m_perceptibles[6];       // +0x0D8  container init sub_180F66D7C(this+216) [U type; tick reads the MANAGER's list [0], not this -- registration owner unresolved]
    uint64_t m_unk108;                // +0x108  = alloc(1) (@0x180f662af)
    void*    m_gameCtx;               // +0x110  = *(gameCtx+64) (@0x180f662c1)
    int32_t  m_frameStamp;            // +0x118  init -1; updated in tick @0x18061afe2
    uint8_t  _pad11C[0x34];           // +0x11C  [U]
    int32_t  m_crowdCount;            // +0x150  secondary-crowd member count; writer sub_18061B180 (@0x18061aff0); read by recognition FSM sub_182086400 @0x182086474
    uint8_t  _pad154[4];              // +0x154
    uint64_t _tail158;                // +0x158  subobject, init sub_18078BDEC [U]
};
static_assert(sizeof(S_PerceptorSensingContext) == 0x160, "S_PerceptorSensingContext must be 0x160 (alloc 352)");

}  // namespace wh::xgenaimodule
