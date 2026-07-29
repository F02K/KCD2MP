#pragma once
#include <cstdint>
#include "C_AutoTriggerable.h"
#include "C_Node.h"
#include "C_TypedPortRef.h"

// -----------------------------------------------
// wh::conceptmodule::C_Effect -- ABSTRACT level-to-edge effect node (KCD2 WHGame.dll
// Steam 1.5.6, e4cp).  sizeof 0x88, vtable 0x183A45F30 (46 slots).  Ctor 0x1806B19C0.
// -----------------------------------------------
// NOT the rpgmodule effect system: this is conceptmodule's node-graph effect base
// (C_SharedResource -> C_Node -> C_AutoTriggerable<C_Node> -> here; leaves e.g.
// musicmodule::C_SkaldBoxProbe, rpgmodule::C_BuffEffect, entitymodule::C_ProfileEffect).
// OnExecute [33] is a level-triggered latch on the IsActive bool pin: when the resolved
// trigger source is m_isActive and its value differs from m_lastActiveState, flip the
// latch and emit exactly one OnEffectActivate [43] (rising) / OnEffectDeactivate [44]
// (falling).  Census cross-check: every C_Effect-derived XML tag carries an IsActive
// input pin; non-effect tags never do.  rttr type "wh::conceptmodule::Effect"
// (reg sub_181053AFC), single property "IsActive" (offset 0x40 baked in sub_1810538BC).

namespace wh::conceptmodule {

class C_Effect : public C_AutoTriggerable<C_Node> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_conceptmodule_C_Effect;
    RTTR_ENABLE(C_AutoTriggerable<C_Node>)              // [5..7] trio overrides; type "wh::conceptmodule::Effect"
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x1806B13C4 IsActive edge-detect latch (see above)
    void OnLifecycleEvent(int32_t event) override;      // [34] 0x181E2ED20 base walk + (ev == 8 && m_lastActiveState) -> OnEffectShutdown
    void UnregisterAutoTrigger() override;              // [38] 0x180F6EDA8 unregister, then forced falling edge if latched
    void ForceDeactivate() override;                    // [39] 0x180ACF0AC forced falling edge if latched
    void PropagateToPorts(bool propagate) override;     // [40] 0x1813E804C walks m_ports when true
    virtual void OnEffectActivate() = 0;                // [43] rising edge of IsActive -- the leaf's work
    virtual void OnEffectDeactivate() = 0;              // [44] falling edge of IsActive (also forced by [38]/[39])
    virtual void OnEffectShutdown();                    // [45] base nullsub; lifecycle-event-8 hook while latched [name LIKELY]

    C_TypedPortRef<bool> m_isActive;   // +0x40  rttr "IsActive" input pin
    bool    m_lastActiveState;         // +0x80  edge-detect cache; cleared by [38]/[39]
    uint8_t _pad81[7];                 // +0x81
};
static_assert(sizeof(C_Effect) == 0x88, "conceptmodule::C_Effect must be 0x88 (leaf members start at +0x88)");

}  // namespace wh::conceptmodule
