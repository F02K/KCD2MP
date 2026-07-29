#pragma once
#include <cstdint>
#include "conceptmodule/S_NodeCreationCtx.h"

class XmlNodeRef;

// -----------------------------------------------
// wh::framework::C_RTTRXMLDeserializer -- generic rttr-driven XML deserializer base
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0xC0 (primary-base extent of
// C_ConceptGraphDeserializer).  PARTIAL MODEL -- slots [1..3] and +0x10..+0x8F
// unwalked.
// -----------------------------------------------
// Ctor sub_1806E4514(this, factory, 1).  The generic per-element step
// sub_181E25110(this, node, out, isGraphNode, flag): tag "null" -> ok; build an
// S_NodeCreationCtx (graph mode via vtable slot [4]; plain mode from the xsi:type
// attr, else factory slot [4] GetTypeName); dispatch factory Create; skipped
// counts as success; then deserialize the created object's reflected members and
// children (sub_181E25520).

namespace wh::conceptmodule { class I_NodeFactory; }

namespace wh::framework {

class C_RTTRXMLDeserializer {
public:
    virtual ~C_RTTRXMLDeserializer();   // [0]
    virtual void unk01();               // [1] [U]
    virtual void unk02();               // [2] [U]
    virtual void unk03();               // [3] [U]
    virtual void BuildNodeCreationCtx(XmlNodeRef node, wh::conceptmodule::S_NodeCreationCtx& ctx);  // [4] concept impl 0x1804F20BC (tag/scoped/namespaced names + flags)
    // further slots unmapped [U]

    wh::conceptmodule::I_NodeFactory* m_pFactory;  // +0x08  -> the embedded factory (deserializer+0xD8)
    uint8_t _unk10[0x80];               // +0x10  unwalked base state [U]
    bool    m_unk90;                    // +0x90  ctor writes [role U]
    int32_t m_unk94;                    // +0x94  ctor 1 [role U]
    uint8_t _unk98[0x28];               // +0x98  intrusive list links (ctor-inited) [U]
};
static_assert(sizeof(C_RTTRXMLDeserializer) == 0xC0, "C_RTTRXMLDeserializer primary-base extent is 0xC0");

}  // namespace wh::framework
