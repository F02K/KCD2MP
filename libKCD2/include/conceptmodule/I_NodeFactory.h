#pragma once
#include <cstdint>
#include "S_NodeCreationCtx.h"
#include "rttr/type.h"
#include "rttr/variant.h"

class XmlNodeRef;

// -----------------------------------------------
// wh::conceptmodule::I_NodeFactory -- generic name-prefixed rttr object factory
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x20, vtable 0x183A486C0 (11 slots).
// [class name UNVERIFIED -- taken from the deserializer's m_pFactory role; the base
// has no located RTTI name of its own]
// -----------------------------------------------
// Base of C_ConceptNodeFactory, ctor sub_1806E474C(prefix, separator).  Maps rttr
// type names <-> XML spellings: slot [6] prepends m_namespacePrefix and decodes the
// template encoding "t__"/"__t" -> "<"/">" (so State<int> is spelled Statet__int__t
// in XML), slot [7] is the exact inverse ("boolean" stands in for the invalid
// type).  The generic deserialize step (0x181E25110) drives creation through
// slot [1].

namespace wh::conceptmodule {

class I_NodeFactory {
public:
    virtual ~I_NodeFactory();       // [0]  base 0x1825CDB74
    virtual rttr::variant Create(S_NodeCreationCtx const& ctx, void* args, bool& ok, XmlNodeRef node, bool& skipped, bool flag);  // [1] base 0x181E24390; node consumed by value [args/flag roles U]
    virtual void unk02();           // [2]  0x1825D0770 [U]
    virtual void unk03();           // [3]  0x1825CF044 [U]
    virtual CryStringT<char> GetTypeName(rttr::type const& expected);  // [4] used when a member has no xsi:type attr (call @0x181E2528A) [name/sig LIKELY]
    virtual void Init();            // [5]  base nullsub; C_ConceptNodeFactory resolves its 8 preset type names
    virtual rttr::type ResolveType(CryStringT<char> const& name);  // [6] 0x1806A4C1C: "t__/__t" decode, prefix + get_by_name (fallback: bare)
    virtual CryStringT<char> GetName(rttr::type type);             // [7] 0x1806A4AD0: inverse of [6]
    RTTR_ENABLE()                   // [8..10]

    CryStringT<char> m_namespacePrefix;  // +0x08  C_ConceptNodeFactory: "wh::conceptmodule::"
    CryStringT<char> m_separator;        // +0x10  "::"
    bool m_useShortNames;                // +0x18  ctor false; read by [7] [name LIKELY]
    uint8_t _pad19[7];                   // +0x19
};
static_assert(sizeof(I_NodeFactory) == 0x20, "I_NodeFactory must be 0x20");

}  // namespace wh::conceptmodule
