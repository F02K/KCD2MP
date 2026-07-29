#pragma once
#include <cstdint>
#include "C_ConceptNodeFactory.h"
#include "S_ResourceRef.h"
#include "framework/C_RTTRXMLDeserializer.h"

// -----------------------------------------------
// wh::conceptmodule::C_ConceptGraphDeserializer -- one Skald concept database
// loader (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x2E0, primary vtable
// 0x183A804B0, secondary vtable 0x183A804A0 at +0xC0.  PARTIAL MODEL.
// -----------------------------------------------
// Ctor 0x180BC34A4 (allocators sub_180BC2118 / sub_1819DE270, one instance per
// named concept database -- get-or-create sub_180BC2068 keys on m_name).  Owns the
// whole load path: document loader 0x180979068 (Database > Skald > single child;
// caches by path at +0x1A0, doc-node -> scope at +0x220; broadcasts each loaded doc
// over the subscriber vector qword_18531EBF8), <Definitions>/<Definition> handler
// 0x180979560 (loads File= docs, registers scope-qualified names into the embedded
// factory's m_definitions), pending-edge/constant vectors filled per node
// (0x180699FE0), then CreatePorts 0x1806A03D4 (any authored pin name -> runtime
// port; kind 0 Constant / 1 Asset; definition IsInterface picks the Interface twin)
// and CreateEdges 0x180698054 (ResolvePortPath 0x1806988CC handles "..", ".",
// module hops; Connect 0x1806986AC writes producer/consumer links).
// The secondary base at +0xC0 is unidentified -- modeled as an explicit vftable
// slot [UNVERIFIED].

namespace wh::conceptmodule {

class C_ConceptGraphDeserializer : public framework::C_RTTRXMLDeserializer {
public:
    struct S_PendingEdge {            // 0x18 -- one <Edge From= To=>, stored on the CONSUMING node
        S_ResourceRef    node;        // +0x00
        CryStringT<char> from;        // +0x08  "otherNode.OutPort" | "modulePort" | "..(parent)"
        CryStringT<char> to;          // +0x10  the node's own pin
    };
    struct S_PendingConstant {        // 0x20 -- one <Constant Name= Value=> (kind 0) / <Asset> (kind 1)
        int32_t          kind;        // +0x00
        uint8_t          _pad04[4];   // +0x04
        S_ResourceRef    node;        // +0x08
        CryStringT<char> portName;    // +0x10
        CryStringT<char> value;       // +0x18  raw literal text (conversion deferred to the consumer)
    };

    void* m_secondaryVftable;         // +0xC0  vft 0x183A804A0 -- unidentified secondary base [U]
    CryStringT<char> m_name;          // +0xC8  database name (get-or-create key, _stricmp)
    void* m_unkD0;                    // +0xD0  ctor 0 [U]
    C_ConceptNodeFactory m_factory;   // +0xD8  BY VALUE (ctor 0x180BC3708(this))
    uint8_t _unk160[0x40];            // +0x160 loaded-definition-file set (paths) [container U]
    uint8_t _unk1A0[0x40];            // +0x1A0 map: normalized path -> {XmlNodeRef doc @+0x18} (0x180E70178) [container U]
    uint8_t _unk1E0[0x40];            // +0x1E0 container (ctor sub_180BC368C) [U]
    uint8_t _unk220[0x40];            // +0x220 map: doc node -> {CryStringT scope @+0x18} (0x180BC4510) [container U]
    uint8_t _unk260[0x40];            // +0x260 std::unordered_map<CryStringT, ?> (node 0x48) -- module-scoped port buckets for ResolvePortPath [value type U]
    std::vector<S_PendingEdge> m_pendingEdges;         // +0x2A0  drained by CreateEdges
    std::vector<S_PendingConstant> m_pendingConstants; // +0x2B8  drained by CreatePorts
    uint8_t _unk2D0[0x10];            // +0x2D0 list head [U]
};
static_assert(sizeof(C_ConceptGraphDeserializer) == 0x2E0, "C_ConceptGraphDeserializer must be 0x2E0 (alloc size)");
static_assert(sizeof(C_ConceptGraphDeserializer::S_PendingEdge) == 0x18, "S_PendingEdge stride");
static_assert(sizeof(C_ConceptGraphDeserializer::S_PendingConstant) == 0x20, "S_PendingConstant stride");

}  // namespace wh::conceptmodule
