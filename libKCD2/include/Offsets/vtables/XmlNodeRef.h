#pragma once

// -----------------------------------------------
// XmlNodeRef -- minimal layout-true stand-in for CryCommon's intrusive XML node ref
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 8.
// -----------------------------------------------
// One pointer to the engine's IXmlNode (RE'd devirtualized struct in
// Offsets/vtables/IXmlNode.h -- the STOCK CryCommon IXml.h must not be used: its
// interfuscator vfunc order does not match the shipped binary and its IXmlNode
// collides with the RE'd one).  The engine's release helper 0x1804FB980 is exactly
// `if (*p) (*p)->vf[+0x20]()`; copy helper 0x18041EBF4 AddRefs.  Declared at global
// scope to match the engine spelling used across the conceptmodule headers
// (C_Node::Save/Load, the factory's definition map, the deserializer).  Modeled
// WITHOUT ctor/dtor semantics -- tool code must AddRef/Release through the binary
// helpers; treat held refs as borrowed.

namespace Offsets { struct IXmlNode; }

class XmlNodeRef {
public:
    Offsets::IXmlNode* m_ptr = nullptr;   // +0x00  intrusively refcounted engine node
};
static_assert(sizeof(XmlNodeRef) == 8, "XmlNodeRef is one IXmlNode*");
