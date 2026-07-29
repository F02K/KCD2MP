#pragma once

// -----------------------------------------------
// wh::xgenaimodule::I_NPCAttachListener -- NPC attach/detach notification
// listener; base of C_MountRiderLinkManager (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x08.
// -----------------------------------------------
// RTTI mangled .?AVI_NPCAttachListener@xgenaimodule@wh@@.  Shape (2 slots)
// read from the sole implementor C_MountRiderLinkManager (vtable rva
// 0x3C26B38: [0] dtor sub_180A9E5D4, [1] sub_180A9E6C8 -- the single
// attach/detach callback).  Registration: attach subsystem
// (qword_185338CC8+360) vtbl+128.  Exact signature [U].

namespace wh::xgenaimodule {

class I_NPCAttachListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_NPCAttachListener;
    virtual ~I_NPCAttachListener();          // [0]
    virtual void OnNPCAttachEvent() = 0;     // [1] attach/detach notification [U name/signature]
};
static_assert(sizeof(I_NPCAttachListener) == 0x08, "interface: vtable pointer only");

}  // namespace wh::xgenaimodule
