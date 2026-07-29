#pragma once
#include <cstdint>
#include <unordered_map>
#include "../conceptmodule/C_NodeManager.h"

// -----------------------------------------------
// wh::dialogmodule::data::C_DialogueDataManager -- dialogue node/index registry
// (KCD2 1.5.6, kd7u).  sizeof 0x158 (alloc 344 + memset 0x158 in factory sub_180E2FDE4).
// -----------------------------------------------
// RTTI TD 0x184B4DED0; primary vtable 0x183AB7D00 (9 slots, shared with the
// conceptmodule::C_NodeManager base -- single inheritance, mdisp 0); ctor sub_180E2F638;
// deleting-dtor [0] sub_182803B88. NOT a global singleton: owned by C_DialogManager
// (stored at manager +0x68; created in the manager ctor sub_180E2FEFC).
// Five 0x40 MSVC unordered_maps (init helpers sub_1806030E4 / sub_180AD7AD4): +0x18 is
// the C_NodeManager node graph (id -> data::C_DialogueNode*); +0x58..+0x118 are
// derived-owned dialogue indices. The exact C_NodeManager-base vs derived member split
// is UNVERIFIED (the ctor inlines the whole chain; C_NodeManager is modeled vptr-only,
// so all fields are declared here). Map key/value types UNVERIFIED (modeled
// uint64 -> ptr; layout-identical).

namespace wh::dialogmodule::data {

class C_DialogueNode;

class C_DialogueDataManager : public conceptmodule::C_NodeManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueDataManager;
    CryStringT<char> m_str08;   // +0x08  empty string (ctor stores &unk_185666F60 = shared empty-string static)
    uint16_t m_u10;          // +0x10  ctor -1
    uint16_t m_u12;          // +0x12  ctor -1
    uint8_t  _pad14[4];      // +0x14
    std::unordered_map<uint64_t, C_DialogueNode*> m_nodeIndex;   // +0x18  id -> node (C_NodeManager graph)
    std::unordered_map<uint64_t, void*> m_index58;               // +0x58  derived index [role UNVERIFIED]
    std::unordered_map<uint64_t, void*> m_index98;               // +0x98  derived index [role UNVERIFIED]
    std::unordered_map<uint64_t, void*> m_indexD8;               // +0xD8  derived index [role UNVERIFIED]
    std::unordered_map<uint64_t, void*> m_index118;              // +0x118 derived index (elem-size-1 init) [role UNVERIFIED]
};
static_assert(sizeof(C_DialogueDataManager) == 0x158, "C_DialogueDataManager must be 0x158 (alloc 344)");
static_assert(offsetof(C_DialogueDataManager, m_nodeIndex) == 0x18);
static_assert(offsetof(C_DialogueDataManager, m_index118) == 0x118);

}  // namespace wh::dialogmodule::data
