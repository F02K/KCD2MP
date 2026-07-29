#pragma once
#include <cstdint>
#include <memory>
#include "C_SharedResource.h"
#include "E_PortDirection.h"
#include "E_PortSpecification.h"
#include "S_ResourceRef.h"
#include "rttr/variant.h"

// -----------------------------------------------
// wh::conceptmodule::I_Port -- node-graph port base (KCD2 WHGame.dll Steam 1.5.6,
// e4cp).  sizeof 0x30, vtable 0x183A465B0 (23 slots).
// -----------------------------------------------
// Despite the I_ prefix this carries data.  Two ctors: default 0x1806B28F0, named
// 0x1806B2958(CryStringT const&).  Family split under it: C_PortRef (the node-side
// member handle) vs C_EdgePort -> {C_DataPort, C_TriggerPort} (the wireable graph
// ports), plus C_DebuggerPort.  A port knows its owner node only through the weak
// m_ownerRef handle and its schema through the weak m_definition; the value/trigger
// plumbing is pull-based and synchronous (see C_DataPort / C_ActiveTriggerPort).

namespace wh::conceptmodule {

namespace definition { class I_PortDefinition; }

class I_Port : public C_SharedResource {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_Port;

    virtual ~I_Port();                             // [0]  per-class scalar deleting dtor; slots [1..4] stay nullsub on ports (incl. [4] Activate)
    RTTR_ENABLE(C_SharedResource)                  // [5..7] trio overrides (per-class get_type)
    virtual E_PortDirection GetDirection() const;  // [8]  base 0x18270A554 -> definition->GetDirection() (None if unset); leaves: constant In/Out or runtime +0x30
    virtual CryStringT<char> const& GetName() const;  // [9]  0x181A73320 -> m_name
    virtual bool IsPassThrough() const;            // [10] base true (0x18041A6A0); C_EdgePort: role == InterfaceData; Constant/Asset: false
    virtual bool unk11() const;                    // [11] return false [U]
    virtual bool unk12() const;                    // [12] return false [U]
    virtual bool unk13() const;                    // [13] 0x1806994C0: definition value type == lazily-init global type (qword_1854800A8) [trigger-typed check, U]
    virtual bool unk14() const;                    // [14] return false; keep-filter in ResolvePortPath's interface hop (0x1806988CC) [U]
    virtual void Trigger();                        // [15] base nullsub; C_InputTriggerPort 0x1806920D4 runs the owner; C_ActiveTriggerPort 0x18061D688 fans out
    virtual rttr::variant GetValue();              // [16] base 0x180453D30 invalid variant
    virtual E_PortSpecification GetSpecification() const;  // [17] the port-class discriminator ("role"); base None; leaves per E_PortSpecification.h
    virtual void ForEachConnected(std::function<bool(_smart_ptr<I_Port>&)> visitor, int32_t dir, uint32_t flags);  // [18] dir 1=upstream 2=downstream 3=both; flags bit0 skip pass-through, bit1 recurse; base 0x18061DF80 no links
    virtual void Unbind();                         // [19] 0x1806B3D58: m_ownerRef = {-1, 0}, m_definition.reset()
    virtual void unk20();                          // [20] nullsub [U]
    virtual bool HasOwnValue() const;              // [21] 0x180692744: !pass-through, has definition, non-trigger value checks
    virtual bool IsTriggerSink() const;            // [22] 0x1806921DC: !pass-through, definition direction != Out, trigger-typed

    S_ResourceRef m_ownerRef;                      // +0x10  weak handle to the owning C_Node (resolve 0x180698CF8)
    std::weak_ptr<definition::I_PortDefinition> m_definition;  // +0x18  authoring schema (name, direction, default value, gates)
    CryStringT<char> m_name;                       // +0x28  the XML pin name
};
static_assert(sizeof(I_Port) == 0x30, "I_Port must be 0x30 (derived members start at +0x30)");

}  // namespace wh::conceptmodule
