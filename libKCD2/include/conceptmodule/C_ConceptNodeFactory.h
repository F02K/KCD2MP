#pragma once
#include <cstdint>
#include <map>
#include "Offsets/vtables/XmlNodeRef.h"
#include "I_NodeFactory.h"

// -----------------------------------------------
// wh::conceptmodule::C_ConceptNodeFactory -- XML tag -> node object
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x88 [LIKELY], vtable 0x183A80510
// (11 slots).  Ctor 0x180BC3708(owner) -- embedded BY VALUE at
// C_ConceptGraphDeserializer+0xD8.
// -----------------------------------------------
// NOT a registry of its own: a thin adapter over the rttr type registry.
// Create [1] = 0x181E38FA0 (self-named via profiler literal):
//  1. prune gate 0x1804F1F60: under <Nodes>, not an <Output>, CVar-armed
//     RequiredForOutput filter fails -> *skipped = true, empty variant (success!).
//  2. composite lookup (isGraphNode): m_definitions.find(scopedName), else
//     best-suffix/nearest-scope resolve 0x1806A4328 (+ recursion guard 0x1806A4894)
//     -> hit => recursive Create on the definition root + re-deserialize its body
//     per instance (0x1806906E0; Level/RandomEventPlace toggle m_levelName/
//     m_randomEventPlaceMode around it; InstanceType="Static" skips the body).
//  3. name normalize: "." -> "::" (globals 0x185577120/0x185577190).
//  4. structural remap 0x180C80854: <Port>/<StaticPort> (+Type="trigger") ->
//     Interface/Static Trigger|Data port; <ConstantPort>/<AssetPort> with Type= ->
//     the Interface twin; <Type> -> runtime::C_RuntimeType; InstanceType="Static"
//     -> C_StaticInstancePlaceholder (the 8 preset names below).
//  5. rttr lookup 0x1806A5798: get_by_name, else the lazily-built short-name index
//     (last "::" component, 0x180621A54); filter to types derived from C_Resource/
//     C_SharedResource (0x1804F4B14, fallback to unfiltered); require EXACTLY ONE.
//  6. construct 0x18069C708: default-ctor constructor_wrapper (class_data +0x80)
//     -> rttr::variant holding _smart_ptr<node> (as_wh_smart_ptr).
// Tag->class truth is the rttr REGISTRATION name "wh::<module>::<Tag>" -- NOT
// always class-minus-C_ (State -> C_StateVariable, Watch -> C_StateWatch,
// Layer -> entitymodule::C_ProfileEffect).

namespace wh::conceptmodule {

class C_ConceptGraphDeserializer;

class C_ConceptNodeFactory : public I_NodeFactory {
public:
    rttr::variant Create(S_NodeCreationCtx const& ctx, void* args, bool& ok, XmlNodeRef node, bool& skipped, bool flag) override;  // [1] 0x181E38FA0 (args unused here)
    void Init() override;   // [5] 0x180BC2F64 resolves the 8 preset rttr type names + clears m_definitions
    RTTR_ENABLE(I_NodeFactory)   // [8..10] trio overrides

    bool RegisterDefinition(CryStringT<char> const& name, XmlNodeRef node);  // 0x1809797D0: refused if already present or an rttr type owns the name

    std::map<CryStringT<char>, XmlNodeRef> m_definitions;  // +0x20  composite <Definition> types: scoped name -> definition doc root (REVERSE-lexicographic order -- suffix scans)
    C_ConceptGraphDeserializer* m_pOwner;   // +0x30  the embedding deserializer
    CryStringT<char> m_levelName;           // +0x38  set while instantiating a <Level> definition (read by the prune gate)
    bool m_randomEventPlaceMode;            // +0x40  set while instantiating a <RandomEventPlace> definition
    uint8_t _pad41[7];                      // +0x41
    CryStringT<char> m_nameRuntimeType;             // +0x48  rttr name of runtime::C_RuntimeType   (<Type> tag)
    CryStringT<char> m_nameInterfaceDataPort;       // +0x50  ... of C_InterfaceDataPort            (<Port>)
    CryStringT<char> m_nameInterfaceTriggerPort;    // +0x58  ... of C_InterfaceTriggerPort         (<Port Type="trigger">)
    CryStringT<char> m_nameStaticDataPort;          // +0x60  ... of C_StaticDataPort               (<StaticPort>)
    CryStringT<char> m_nameStaticTriggerPort;       // +0x68  ... of C_StaticTriggerPort            (<StaticPort Type="trigger">)
    CryStringT<char> m_nameConstantInterfacePort;   // +0x70  ... of C_ConstantInterfacePort        (<ConstantPort Type=...>)
    CryStringT<char> m_nameAssetInterfacePort;      // +0x78  ... of C_AssetInterfacePort           (<AssetPort Type=...>)
    CryStringT<char> m_nameStaticInstancePlaceholder;  // +0x80  ... of C_StaticInstancePlaceholder (InstanceType="Static")
};
static_assert(sizeof(C_ConceptNodeFactory) == 0x88, "C_ConceptNodeFactory must be 0x88 (last member at +0x80; extent LIKELY)");

}  // namespace wh::conceptmodule
