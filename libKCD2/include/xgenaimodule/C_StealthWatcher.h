#pragma once
#include <cstdint>
#include "I_StealthWatcher.h"

// -----------------------------------------------
// wh::xgenaimodule::C_StealthWatcher : XGenPublic::I_StealthWatcher -- the
// trespass/stealth-context watcher singleton (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x30 (create-site proven: alloc 48 in the lazy-init getter sub_180F667DC
// -> qword_185493928).
// -----------------------------------------------
// RTTI TD rva 0x4F5B598; vtable 0x183AE0A28, 3 slots.  Registers via
// sub_180F668CC(sub_1832AE82C()).
// [2] sub_180F668C0 -> sub_180460B88(this, *(this+0x20)) = the trespass check:
// for each object in the +0x20 list, GetOwnership tags "private" / "personal" /
// "prohibited"; returns private && (personal || prohibited).
// NOTE: NOT the sneak-XP ticker -- sneak XP is C_RPGPerception vtable slot 23
// (sub_180C5C114), invoked only virtually; independent system.  Exposed to the
// rpg side via S_RPGPerceptionHelper slot [1] GetStealthWatcher.

namespace wh::xgenaimodule {

class C_StealthWatcher : public XGenPublic::I_StealthWatcher {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_StealthWatcher;
    ~C_StealthWatcher() override;   // [0] deleting dtor sub_1832EE0C8
    void _vf1() override;           // [1] sub_181A71D30 [U role]
    void _vf2() override;           // [2] sub_180F668C0 -- trespass/stealth-context check (see banner) [U exact signature]

    boost::container::vector<void*> m_watchList;  // +0x08  flat vector {data,size,cap}; 8-byte elems, sorted insert (add sub_18087889C) / clear (sub_1809DD670); elem type [U]
    void*    m_watchedTarget;   // +0x20  ptr to polymorphic watched target; trespass sub_180460B88 calls its vslot2 & builds an ownership list, update sub_18061D1D4 reads *(t+8); class [U]
    uint64_t m_unk28;   // +0x28  ctor: 0 [U role]
};
static_assert(sizeof(C_StealthWatcher) == 0x30, "C_StealthWatcher must be 0x30 (48)");

}  // namespace wh::xgenaimodule
