#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::I_LinkNetworkQuerySystem -- link-graph query interface (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable only).
// -----------------------------------------------
// RTTI TD rva 0x4ECD920; own vtable 0x183FCDFE0, 3 slots: [0] _purecall,
// [1] _purecall, [2] sub_1831A26E8 (shared trivial impl).  NO dtor slot -- slot [0]
// is GetLinks in the implementor (C_LinkNetworkQuerySystem), not a dtor.

namespace wh::xgenaimodule {

class I_LinkNetworkQuerySystem {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_LinkNetworkQuerySystem;
    // [0] resolve entity -> linkable object -> collect its outgoing edges into a
    //     vector of 3-qword entries [U exact signature]
    virtual void GetLinks(void* outVec, uint64_t entityId) = 0;
    // [1] resolve entity, walk its link-name list, strcmp against nameKey
    //     [U exact signature]
    virtual bool HasLink(uint64_t entityId, const char* nameKey) = 0;
    virtual void _vf2();   // [2] sub_1831A26E8 -- shared trivial [U role]
};
static_assert(sizeof(I_LinkNetworkQuerySystem) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
