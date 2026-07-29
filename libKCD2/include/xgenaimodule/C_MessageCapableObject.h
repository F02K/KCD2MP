#pragma once
#include <cstdint>
#include "C_LinkableObject.h"

// -----------------------------------------------
// wh::xgenaimodule::C_MessageCapableObject : C_LinkableObject -- message-capable
// spine level (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x60 -- ZERO own data members
// (ctor proven: rewrites vptrs + registers, nothing else; G2_supplement §3.4).
// Abstract (slot [19] pure).
// -----------------------------------------------
// RTTI TD rva 0x4EFD8C8; primary vtable 0x183A6C330, 20 slots (adds pure [19]);
// I_RWLocked subobject vtable @+0x20 = 0x183A6C310. Real ctor
// sub_180A2BD74(this, const WUID*): hashmap-registers in the
// C_MessageCapableObjectManager (q_185496198, node+0x18 = this).
// C_NPCMessageDispatcher::Dispatch looks the target up in that registry and invokes
// slot [19] (vf +0x98), treating result 3 as rejected.

namespace wh::xgenaimodule {

class C_MessageCapableObject : public C_LinkableObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MessageCapableObject;
    ~C_MessageCapableObject() override;         // [0]  deleting dtor sub_18344222C
    uint32_t GetTypeMask() override;            // [1]  sub_181AABDE0
    virtual int64_t OnMessage(void* msg) = 0;   // [19] _purecall here; impl C_IntelligentObject sub_18070DBD0; invoked by C_NPCMessageDispatcher::Dispatch (vf +0x98) [U exact signature]
};
static_assert(sizeof(C_MessageCapableObject) == 0x60, "C_MessageCapableObject adds no data to C_LinkableObject");

}  // namespace wh::xgenaimodule
