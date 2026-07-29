#pragma once
#include <cstdint>
#include "E_PortDirection.h"
#include "rttr/type.h"
#include "rttr/variant.h"

// -----------------------------------------------
// wh::conceptmodule::definition::I_PortDefinition -- the authoring-side port schema
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  ABSTRACT.  [sizeof/vtable EA UNVERIFIED]
// -----------------------------------------------
// Every live I_Port holds a std::weak_ptr to its definition (+0x18); C_NodeDefinition
// owns std::vector<std::shared_ptr<I_PortDefinition>> (wrapper vt 0x183AC31B8) -- the
// reflected port list the deserializer walks.  Impls (all created via make_shared):
// C_CompileTimePortDefinition (_Ref_count_obj2 vt 0x183A8E058), C_RunTimePortDefinition
// (0x183A3D1F0), C_InterfacePortDefinition (vt 0x183A45790).  rttr metadata surface:
// Name, Visibility, Direction, ConnectionType, Type, Template, IsHidden, IsDeprecated,
// IsVariadic, IsSequential, IsOptional, IsInternal, IsAutoTriggerable,
// IsTriggerableWhenHibernated, LinkedTriggerPorts, ValueOriginConstraint, DefaultValue.
// Slot map below covers only the runtime-consumed subset (offsets seen at call sites);
// total slot count and the tail are UNVERIFIED.

namespace wh::conceptmodule::definition {

class I_PortDefinition {
public:
    virtual ~I_PortDefinition();                       // [0]
    virtual CryStringT<char> GetName();                // [1]  0x08  consumed by C_PortRef lazy resolve (0x181E38E2F)
    virtual int32_t unk02();                           // [2]  0x10  enum; != 1 tested in I_Port::HasOwnValue [ValueOriginConstraint candidate, U]
    virtual E_PortDirection GetDirection();            // [3]  0x18  1=In 2=Out 3=Interface (GetOrCreatePort switch 0x1806996C0)
    virtual int32_t unk04();                           // [4]  0x20  enum; == 1 filter in the auto-trigger wrap (0x181E27A92) [U]
    virtual void unk05();                              // [5]  0x28  [U]
    virtual rttr::type GetValueType();                 // [6]  0x30  the port's value type (0x1806927BB, 0x1806994DF)
    virtual void unk07();                              // [7]  0x38  [U]
    virtual rttr::variant GetDefaultValue();           // [8]  0x40  fallback for unwired C_PortRef::GetValue (0x1806B1082)
    virtual void unk09();                              // [9]  0x48  [U]
    virtual void unk10();                              // [10] 0x50  [U]
    virtual void unk11();                              // [11] 0x58  [U]
    virtual void unk12();                              // [12] 0x60  [U]
    virtual void unk13();                              // [13] 0x68  [U]
    virtual void unk14();                              // [14] 0x70  [U]
    virtual bool IsAutoTriggerable();                  // [15] 0x78  gate in MaybeMakeAutoTrigger 0x181E27B08 [name LIKELY]
    virtual bool IsTriggerableWhenHibernated();        // [16] 0x80  gate in C_Node::Execute 0x180692060 [name LIKELY]
    virtual bool IsInterface();                        // [17] 0x88  selects Interface vs plain Constant/Asset in CreatePorts 0x1806A0532 [name LIKELY]
    // further slots exist [UNVERIFIED]
};

}  // namespace wh::conceptmodule::definition
