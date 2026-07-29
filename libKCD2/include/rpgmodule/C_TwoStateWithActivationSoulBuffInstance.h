#pragma once
#include <cstdint>
#include "C_TwoStateSoulBuffInstance.h"

// -----------------------------------------------
// wh::rpgmodule::C_TwoStateWithActivationSoulBuffInstance -- KCD2 1.5.6 (kd7u).  sizeof 0xE0.
// -----------------------------------------------
// RTTI .?AVC_TwoStateWithActivationSoulBuffInstance@rpgmodule@wh@@  vtable 0x183BDDAB8
// in-place ctor sub_181641DE8 = {C_TwoStateSoulBuffInstance ctor; set vtable; byte@0xD8 = 0}.
// The two-state variant that DOES store its activation latch in the instance. sizeof 0xE0 is
// structurally forced (base 0xD8 + bool, 8-aligned); no direct operator-new alloc site was
// located for this layer (constructed in place / by derived makes) -- flagged, not fabricated.

namespace wh::rpgmodule {

class C_TwoStateWithActivationSoulBuffInstance : public C_TwoStateSoulBuffInstance   // +0x00  (0xD8)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_TwoStateWithActivationSoulBuffInstance;
    bool    m_activated;   // +0xD8  activation latch (name INFERRED; ctor inits false)
    uint8_t _padD9[7];     // +0xD9
};
static_assert(sizeof(C_TwoStateWithActivationSoulBuffInstance) == 0xE0,
              "C_TwoStateWithActivationSoulBuffInstance must be 0xE0");
static_assert(offsetof(C_TwoStateWithActivationSoulBuffInstance, m_activated) == 0xD8, "latch at 0xD8");

}  // namespace wh::rpgmodule
