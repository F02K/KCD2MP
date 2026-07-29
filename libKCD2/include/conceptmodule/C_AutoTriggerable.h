#pragma once
#include <cstdint>
#include <memory>
#include "C_SharedResource.h"

// -----------------------------------------------
// wh::conceptmodule::C_AutoTriggerable<TBase> -- auto-trigger node mixin
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  Adds NO data: sizeof == sizeof(TBase).
// -----------------------------------------------
// Instantiations: <C_Node> (vtable 0x183A46C98, 43 slots, ctor 0x1806B2130 -- base of
// C_Effect) and <C_TemplatedNode> (base of questmodule::C_Objective).  The ctor
// permanently sets C_SharedResource::Flag_AutoTriggerable; runtime enable state is only
// Flag_AutoTriggerEnabled (the auto-trigger port itself lives in the generic m_ports,
// as a member of the C_AutoTriggerPort family).  Slot indices/EAs below are for the
// <C_Node> instantiation; the two added slots land after whatever TBase declared.

namespace wh::conceptmodule {

class I_Port;
namespace definition { class I_PortDefinition; }

template <typename TBase>
class C_AutoTriggerable : public TBase {
public:
    int32_t GetExecutionOrder() const override;          // [10] 0x181A72470 -> 1: sorts after plain nodes in the trigger-fire pipeline
    bool IsAutoTriggerable() const override;             // [24] 0x18041A6A0 -> true
    void SetAutoTriggerEnabled(bool enabled) override;   // [25] 0x1823CC2DC set/clear Flag_AutoTriggerEnabled
    void OnLifecycleEvent(int32_t event) override;       // [34] 0x1806B0C04 walks m_ports (sub_181E2CBB0) for ev in {0,1,3,4,5}
    int32_t MapEventToPhase(int32_t event) override;     // [35] 0x1806B0B80: ev {0,1}->1, 5->2, else 0
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x1804F6120
    void Reset() override;                               // [37] 0x180694E8C (re)registers the auto-trigger (guard Flag_AutoTriggerEnabled; target sub_180694894)
    void UnregisterAutoTrigger() override;               // [38] 0x180F6ED40 (target sub_180F6EE2C)
    virtual _smart_ptr<I_Port> ResolveTriggerSource(_smart_ptr<C_SharedResource> source);  // [41] 0x1806B1478 maps a fired source to one of this node's ports (identity, else name lookup) [name LIKELY]
    virtual void unk42();                                // [42] nullsub everywhere sampled [U]
};

}  // namespace wh::conceptmodule
