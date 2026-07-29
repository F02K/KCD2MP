#pragma once
#include "I_NPCAttachListener.h"

// -----------------------------------------------
// wh::xgenaimodule::C_MountRiderLinkManager : I_NPCAttachListener -- links
// mounts and riders on attach events; SINGLETON at qword_185496158 (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x10 (getter sub_181822290 @0x1818222dd:
// sub_18549D378(16,...), += 0x10, ctor sub_18182232C).
// -----------------------------------------------
// RTTI TD rva 0x4F5C500.  Vtable rva 0x3C26B38 (2 slots: [0] dtor
// sub_180A9E5D4, [1] sub_180A9E6C8).  ctor stores the attach subsystem
// (*(qword_185338CC8+360)) at +0x08 and registers via its vtbl+128.

namespace wh::xgenaimodule {

class C_MountRiderLinkManager : public I_NPCAttachListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MountRiderLinkManager;
    ~C_MountRiderLinkManager() override;   // [0] sub_180A9E5D4
    void OnNPCAttachEvent() override;      // [1] sub_180A9E6C8

    void* m_attachSubsystem;   // +0x08  cached NPC-attach subsystem = *(g_systems qword_185338CC8 + 0x168); non-null gates listener add/remove into C_NPCManager (q_185494490 vf+128/+136); passed to attach/detach handlers [U pointee type]
};
static_assert(sizeof(C_MountRiderLinkManager) == 0x10, "must be 0x10 (alloc 16 at sub_181822290)");

}  // namespace wh::xgenaimodule
