#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::xgenaimodule::S_LinkEdge -- polymorphic edge record of the linkable graph
// -----------------------------------------------
// RTTI TD rva 0x4F5C580 (.?AUS_LinkEdge@... -- struct in source; class here per
// project convention for polymorphic types).  vtable 0x183A398C0, 2 slots:
// [0] deleting dtor sub_18047DD64(this, flag), [1] nullsub_1.
// Field evidence from the edge walker sub_18047AFA4(obj, key, filter):
//   for e in [obj+0x28, obj+0x30):  match if *(e+0x10) == key;
//   filter searches the target range [*(e+0x18), *(e+0x20)) via sub_181C1D7F0.
// Edges live in C_LinkableObject::m_links (the +0x28 vector).

namespace wh::xgenaimodule {

class S_LinkEdge {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_LinkEdge;
    virtual ~S_LinkEdge();       // [0] deleting dtor sub_18047DD64(this, flag)
    virtual void _vf1();         // [1] nullsub_1 [U role]

    uint64_t m_unk08;            // +0x08  plain scalar (untouched by dtor sub_18047CA04; no writer traced) [U role]
    uint64_t m_key;              // +0x10  owner/name key compared by sub_18047AFA4 [V]
    std::vector<void*> m_targets;   // +0x18  {first,last,cap} freed by dtor sub_1804134B4; walked [begin,end) by sub_18047AFA4; 8-byte elems compared in sub_181C1C010
};
// sizeof >= 0x30; not create-site proven -> no static_assert [U]

}  // namespace wh::xgenaimodule
