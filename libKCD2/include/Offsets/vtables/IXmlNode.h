#pragma once

// -----------------------------------------------
// IXmlNode -- CrySystem XML DOM node, KCD2 binary vtable order (impl CXmlNode).
// -----------------------------------------------
// 80+ slots -- this header declares the
// prefix [0..78] (mostly-unmapped tail = the remaining attr overload block).
// Never sizeof/derive from this; it exists for slot-correct virtual calls on nodes the
// engine hands out (XmlNodeRef payloads).  The stock CryCommon IXml.h interfuscator order
// does NOT match this binary -- always call through this header.
//   defaultActionHelp parser 0x180A13558, CActionMapManager::LoadFromXML 0x180A13E3C,
//   CActionMap::LoadFromXMLNode 0x180A12514, CActionMap::SaveRebindingDataToXML 0x183582EB0,
//   XmlNodeRef release helper 0x1804FB980.
// Methods returning XmlNodeRef use the MSVC hidden out-pointer, declared explicitly.
//
// Second implementation: CXMLReadOnlyNode, vtable 0x183A3CF28, SAME slot order
// (produced by the pooled/read-only parse path the config loaders use, flag a5=1 of
// ISystem::LoadXmlFromFile). Its accessors are real but every mutator ([39] newChild,
// [41] insertChild, [42] replaceChild, the [60..79] setAttr block) is nullsub_1 --
// writes to such trees are silent no-ops. Convert with [52] clone before mutating.

namespace Offsets {

struct IXmlNode {
    virtual void _vf0() = 0;  // [0] 0x1804597FC  dtor region
    virtual void _vf1() = 0;  // [1] 0x180459818
    virtual void _vf2() = 0;  // [2] 0x1804584C4
    virtual void _vf3() = 0;  // [3] 0x181A72630  (AddRef by stock shape [conf LOW] -- do not call)
    // [4] 0x1804597EC -- drop one reference, self-deletes at zero.  VERIFIED: the engine's
    // XmlNodeRef-release helper 0x1804FB980 is exactly `if (*p) (*p)->vf[+0x20]()`.
    virtual void Release() = 0;
    virtual void _vf5() = 0;  // [5] 0x181A72970
    // [6] 0x1809DD2D0 -- tag string of this element.  VERIFIED: both parsers strcmp the
    // result against "action_help_set"/"actionmap"/"action"/"include".
    virtual const char* getTag() const = 0;
    virtual void _vf7() = 0;  // [7] 0x182488BC0
    virtual void _vf8() = 0;  // [8] 0x1804584A4
    // [9] 0x18045868C -- attribute count.  [conf MED]: LoadFromXMLNode gates inline-input
    // parsing on `result > 1` right after reading the action node's "name".
    virtual int getNumAttributes() const = 0;
    // [10] 0x180EFB604 -- attribute key/value by index, bool return.
    virtual bool getAttributeByIndex(int i, const char** key, const char** value) = 0;
    virtual void _vf11() = 0;  // [11] 0x180719E3C
    virtual void _vf12() = 0;  // [12] 0x180CBAAC4
    virtual void _vf13() = 0;  // [13] 0x182487388
    virtual void _vf14() = 0;  // [14] 0x182487158
    virtual void _vf15() = 0;  // [15] 0x1803C308C
    virtual void _vf16() = 0;  // [16] 0x1803C3494
    virtual void _vf17() = 0;  // [17] 0x1803C2F3C
    virtual void _vf18() = 0;  // [18] 0x1824872A4
    virtual void _vf19() = 0;  // [19] 0x18248730C
    virtual void _vf20() = 0;  // [20] 0x1803C3400
    virtual void _vf21() = 0;  // [21] 0x1824870F0
    virtual void _vf22() = 0;  // [22] 0x182487250
    virtual void _vf23() = 0;  // [23] 0x1824871F0
    virtual void _vf24() = 0;  // [24] 0x1824870C8
    virtual void _vf25() = 0;  // [25] 0x1803C3118
    virtual void _vf26() = 0;  // [26] 0x1824873B4
    virtual void _vf27() = 0;  // [27] 0x18079D964
    virtual void _vf28() = 0;  // [28] 0x182487080
    virtual void _vf29() = 0;  // [29] 0x182487050
    virtual void _vf30() = 0;  // [30] 0x1803C33A8
    // NOTE: [31]-[33] are C++ overloads of getAttr in the engine, but MUST carry distinct
    // names here (see the reverse-order rule in the file header).
    // [31] 0x1803C33D8 -- int attribute.  VERIFIED: LoadFromXML reads root "version" into a
    // stack int through this slot (bool return in al).
    virtual bool getAttrInt(const char* key, int& value) const = 0;
    // [32] 0x1803C30EC -- string attribute via out-param, bool return.  [conf MED]:
    // LoadFromXML's priorities loop pairs it with [31] ("name" then "value").
    virtual bool getAttrOut(const char* key, const char** value) const = 0;
    // [33] 0x1803C3474 -- string attribute.  Returns the shared static empty string
    // 0x183A3D1E0 when absent, NEVER NULL (both impls: CXmlNode 0x1803C3474,
    // CXMLReadOnlyNode 0x1804FBD54).  VERIFIED: every "actionmap"/"name"/"text"/"action"
    // read in the parsers.
    virtual const char* getAttr(const char* key) const = 0;
    // [34] 0x180CBAB18 -- attribute presence.  VERIFIED: help parser probes "text"/"type"/...
    // through this slot before reading them with [33].
    virtual bool haveAttr(const char* key) const = 0;
    virtual void _vf35() = 0;  // [35] 0x182488D08
    virtual void _vf36() = 0;  // [36] 0x182488DA0
    virtual void _vf37() = 0;  // [37] 0x182488D6C
    virtual void _vf38() = 0;  // [38] 0x18045A414
    // [39] 0x18079E544 -- create a child element and return a NEW reference to it in *out.
    // VERIFIED: SaveRebindingDataToXML builds its "action"/"rebind" children through this.
    virtual IXmlNode** newChild(IXmlNode** out, const char* tag) = 0;
    virtual void _vf40() = 0;  // [40] 0x182488078
    // [41] 0x1824879EC -- insert a child at the given index (takes XmlNodeRef* and AddRefs
    // internally).
    virtual void insertChild(int idx, IXmlNode** node) = 0;
    // [42] 0x1824880DC -- replace the child at the given index (takes XmlNodeRef*).
    // VERIFIED: XMLPatcher 0x18247546C lands its "replacewith" clone through this slot.
    virtual void replaceChild(int idx, IXmlNode** node) = 0;
    virtual void _vf43() = 0;  // [43] 0x182488070
    // [44] 0x180458574 -- child element count.  VERIFIED: every parser loop bound.
    virtual int getChildCount() const = 0;
    // [45] 0x180A69D24 -- i-th child as a NEW reference written to *out (hidden-pointer
    // XmlNodeRef return;
    virtual IXmlNode** getChild(IXmlNode** out, int i) const = 0;
    // [46] 0x18045840C -- first child with the given tag, NEW reference to *out (NULL *out
    // when absent).
    virtual IXmlNode** findChild(IXmlNode** out, const char* tag) const = 0;
    virtual void _vf47() = 0;  // [47] 0x180459EDC
    virtual void _vf48() = 0;  // [48] 0x181A73BE0
    virtual void _vf49() = 0;  // [49] 0x1819A2A90
    virtual void _vf50() = 0;  // [50] 0x182488BA0
    // [51] 0x18045A3F4 (shared by both vtables) -- internal deep-clone taking the string-
    // intern-table context as arg (impl 0x18045A7A0: allocs 0x48, plants the CXmlNode
    // vtable, copies tag/content/line, copyAttributes, recurses children through their own
    // [51]).  Do not call directly -- use [52], which supplies the context.
    virtual void _vf51() = 0;  // [51] 0x18045A3F4
    // [52] -- deep-clone this subtree into a NEW MUTABLE CXmlNode tree (hidden-pointer
    // XmlNodeRef return; caller owns one reference).  shareStringTable=false allocs a fresh
    // 0x90-byte intern table for the clone; true reuses this node's (CXmlNode impl
    // 0x1824865BC only -- the CXMLReadOnlyNode impl 0x180A6E960 ignores the flag and always
    // creates a fresh table).  This is the sanctioned read-only -> writable conversion.
    virtual IXmlNode** clone(IXmlNode** out, bool shareStringTable) const = 0;
    virtual void _vf53() = 0;  // [53] 0x180638480
    virtual void _vf54() = 0;  // [54] 0x1805F5DA0
    virtual void _vf55() = 0;  // [55] 0x181A72820
    virtual void _vf56() = 0;  // [56] 0x181A72930
    virtual void _vf57() = 0;  // [57] 0x180F688C8
    virtual void _vf58() = 0;  // [58] 0x180A6B53C
    virtual void _vf59() = 0;  // [59] 0x180A6B2F0
    virtual void _vf60() = 0;  // [60] 0x181A73BC0
    virtual void _vf61() = 0;  // [61] 0x1824888B0
    virtual void _vf62() = 0;  // [62] 0x1824884B0
    virtual void _vf63() = 0;  // [63] 0x182488538
    virtual void _vf64() = 0;  // [64] 0x182488780
    virtual void _vf65() = 0;  // [65] 0x18248880C
    virtual void _vf66() = 0;  // [66] 0x1824886E4
    virtual void _vf67() = 0;  // [67] 0x182488418
    virtual void _vf68() = 0;  // [68] 0x182488664
    virtual void _vf69() = 0;  // [69] 0x1824885DC
    virtual void _vf70() = 0;  // [70] 0x182488AC4
    virtual void _vf71() = 0;  // [71] 0x182488A50
    virtual void _vf72() = 0;  // [72] 0x182488B34
    virtual void _vf73() = 0;  // [73] 0x18079E2A4
    virtual void _vf74() = 0;  // [74] 0x1824889E4
    virtual void _vf75() = 0;  // [75] 0x182488988
    virtual void _vf76() = 0;  // [76] 0x182488918
    virtual void _vf77() = 0;  // [77] 0x18079D994
    // [78] 0x18079E300 -- set/overwrite a string attribute.  VERIFIED:
    // SaveRebindingDataToXML writes name/device/input/defaultInput through this slot.
    virtual void setAttr(const char* key, const char* value) = 0;
};

}  // namespace Offsets
