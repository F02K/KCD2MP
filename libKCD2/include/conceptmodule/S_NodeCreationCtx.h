#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::conceptmodule::S_NodeCreationCtx -- naming context for one node creation
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x20.
// -----------------------------------------------
// Built by C_ConceptGraphDeserializer vtable slot 4 (0x1804F20BC) from the XML
// element, and on the stack by the composite instantiator 0x1806906E0.  Passed to
// C_ConceptNodeFactory::Create (r8).  scopedName is the fully-qualified name from
// walking Name= attributes up the document ("Project::Module::Tag", 0x180696530);
// namespacedName comes from the Namespace= attribute with "." -> "::".

namespace wh::conceptmodule {

struct S_NodeCreationCtx {
    CryStringT<char> typeName;        // +0x00  the raw XML tag
    CryStringT<char> scopedName;      // +0x08  "<enclosing scope>::<tag>" -- key into the composite-definition map
    CryStringT<char> namespacedName;  // +0x10  "<Namespace attr>::<tag>"
    bool isGraphNode;                 // +0x18  parent element is <Nodes>
    bool isStaticInstance;            // +0x19  attr InstanceType == "Static" -> C_StaticInstancePlaceholder reroute
    uint8_t _pad1A[6];                // +0x1A
};
static_assert(sizeof(S_NodeCreationCtx) == 0x20, "S_NodeCreationCtx must be 0x20");

}  // namespace wh::conceptmodule
