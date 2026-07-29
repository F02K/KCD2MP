#pragma once
#include <cstdint>
#include "I_Port.h"

// -----------------------------------------------
// wh::conceptmodule::C_PortRef -- lazily-resolving node-side port handle
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x40, vtable 0x183A46670 (23 slots).
// -----------------------------------------------
// Ctor 0x1806B2890.  NOT a C_EdgePort: role stays None and direction stays
// definition-driven, so a port ref can never be an edge endpoint.  It is the type of
// the C_TypedPortRef<T> members embedded in node classes (0x40 stride): on first use
// Resolve() looks up the node's REAL port object (C_InputDataPort/
// C_OutputTriggerPort/...) by the definition's name and caches a smart ptr to it;
// Trigger/GetValue forward there.  An unwired ref falls back to the definition's
// default value.  C_Node::Activate is what binds these members to their node
// (rttr property sweep, body 0x1804F4BB0).

namespace wh::conceptmodule {

class C_PortRef : public I_Port {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PortRef;

    ~C_PortRef() override;                            // [0]  0x18266F018 releases m_cachedPort
    RTTR_ENABLE(I_Port)                               // [5..7] trio overrides
    CryStringT<char> const& GetName() const override; // [9]  0x18061A324: resolved ? resolved->GetName() : m_name
    void Trigger() override;                          // [15] 0x18061CF64: Resolve(), forward to the real port
    rttr::variant GetValue() override;                // [16] 0x1806B1020: Resolve(), forward; unwired -> definition GetDefaultValue()

    _smart_ptr<I_Port> Resolve();                     // 0x1806B1118 (lookup 0x181E38DF0: m_ownerRef -> node, definition name -> FindPortByName); caches

    bool m_resolved;                 // +0x30  lazy-cache flag
    uint8_t _pad31[7];               // +0x31
    _smart_ptr<I_Port> m_cachedPort; // +0x38  the resolved real port
};
static_assert(sizeof(C_PortRef) == 0x40, "C_PortRef must be 0x40 (host embed stride)");

}  // namespace wh::conceptmodule
