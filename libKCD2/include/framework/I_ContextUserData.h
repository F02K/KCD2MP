#pragma once

// -----------------------------------------------
// wh::framework::I_ContextUserData -- typed user-data tag interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// Single-slot interface: slot [0] is a KIND-id getter, NOT a virtual dtor (verified on
// dialogmodule::data::C_DialogueSequenceHolder, vtable 0x183E6CF20 (1 slot):
// [0] sub_181A72470 = `return 1` -- a type/kind discriminator so context consumers can
// identify the payload). Attached to expression-engine / dialog contexts as opaque
// per-context user data. Do NOT add a virtual dtor -- the binary vtable has exactly one
// slot. Method name coined from behavior [UNVERIFIED].

namespace wh::framework {

class I_ContextUserData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_ContextUserData;
    virtual int GetKind() = 0;   // [0] kind/type discriminator (C_DialogueSequenceHolder: 1)
};
static_assert(sizeof(I_ContextUserData) == 0x08, "I_ContextUserData is a vptr-only interface");

}  // namespace wh::framework
