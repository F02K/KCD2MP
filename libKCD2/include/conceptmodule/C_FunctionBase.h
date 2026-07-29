#pragma once
#include <cstdint>
#include "C_TemplatedNode.h"
#include "rttr/method.h"

// -----------------------------------------------
// wh::conceptmodule::C_FunctionBase -- reflected-call node base
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x68, vtable 0x183E2E8A8 (42 slots).
// -----------------------------------------------
// Ctor 0x1806B2208; rttr type "wh::conceptmodule::FunctionBase" (reg inside
// sub_180086B60).  The Skald->C++ bridge: a node that calls an rttr-reflected
// method.  MethodName (and DeclaringType for member variants) arrive as XML
// Constants; resolution is BY NAME, LAZY, CACHED once per node (GetMethod
// 0x18069168C -> ResolveMethod 0x181E2E800: DeclaringType valid -> type.get_method,
// else the global method registry -- this IS the Function-vs-MemberFunction split).
// GetPortDefinitions [28] 0x1804F5504 synthesizes the pin set from the method
// signature: one In data pin PER PARAMETER (named by parameter name; defaults
// honored), a return-value Out pin when non-void (name via metadata 3, default
// "<method>::"-joined), else Exec In-trigger + OnExec Out-trigger; metadata keys
// steer naming/hiding/struct-parameter mode (keys 0..6, 0xA, 0xB [meanings LIKELY]).
// InvokeMethod 0x180691400 marshals arguments parameter-name -> same-named pin
// (0x1806919D0; struct-parameter path 0x1806AA4BC) and stashes the executing node in
// TLS (TEB.TLS[TlsIndex] + 0x1E0) so reflected game functions can reach the graph.
// OnExecute [33] 0x18061AD78: invoke (discard result), then fire "OnExec".

namespace wh::conceptmodule {

class C_FunctionBase : public C_TemplatedNode {
public:
    RTTR_ENABLE(C_TemplatedNode)   // [5..7] trio overrides
    bool Load(XmlNodeRef node) override;  // [11] 0x1826B9E10 validates the resolved method: "method:'%s' does not exist"
    void GetPortDefinitions(std::function<void(std::shared_ptr<definition::I_PortDefinition> const&)> sink) override;  // [28] 0x1804F5504 pins from the method signature (see above)
    void OnExecute(S_NodeExecuteContext const& ctx) override;  // [33] 0x18061AD78 invoke + fire OnExec

    rttr::method GetMethod();          // 0x18069168C lazy cache (m_methodResolved/m_method)
    rttr::variant InvokeMethod(void* instanceDesc);  // 0x180691400 sret; TLS node scope; metadata[1] -> struct path [instanceDesc shape LIKELY]

    CryStringT<char> m_methodName;     // +0x48  rttr "MethodName" (from the XML Constant)
    rttr::type   m_declaringType;      // +0x50  rttr "DeclaringType"; invalid on free-function nodes
    bool         m_methodResolved;     // +0x58  lazy-cache flag
    uint8_t      _pad59[7];            // +0x59
    rttr::method m_method;             // +0x60  the resolved method handle
};
static_assert(sizeof(C_FunctionBase) == 0x68, "C_FunctionBase must be 0x68");

}  // namespace wh::conceptmodule
