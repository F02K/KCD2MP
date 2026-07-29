#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::C_ParticleEffectPuppetManager -- particle-effect puppet
// pool SINGLETON at qword_1854961A8 (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof
// 0xA0 (inlined create sub_180BECB38 @0x180becb5b: qword_18549D378(160,&v5),
// += 0xA0, body init sub_180BEC3C4).
// -----------------------------------------------
// Real base stack (RTTI TD 0x4FFB500): C_ObjectManagerOwnStorage<...> ->
// C_ObjectManager<C_ParticleEffectPuppet, C_ParticleEffectPuppetManager> with
// Callbacks::C_CallbackListHolder<...> @+0x8 and Callbacks::C_TrackSequencing<1>
// @+0x10 -- modeled FLAT here (precedent: C_ObjectManagerProxyStorage.h);
// interior driven by the un-decompiled template base [U interior].
// Vtable rva 0x3A84840 (3 slots: [0] dtor sub_18344F534, [1] nullsub,
// [2] sub_1813666F4 -- C_ObjectManager virtual, e.g. OnObjectDestroyed [U]).

namespace wh::xgenaimodule {

class C_ParticleEffectPuppetManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ParticleEffectPuppetManager;
    virtual ~C_ParticleEffectPuppetManager();   // [0] sub_18344F534
    virtual void _vf1();                        // [1] nullsub [U role]
    virtual void _vf2();                        // [2] sub_1813666F4 [U role]

    uint64_t m_callbackHead;   // +0x08  Callbacks::C_CallbackListHolder list head
    uint8_t  m_trackSeq;       // +0x10  Callbacks::C_TrackSequencing<1> flag
    uint8_t  _pad11[7];        // +0x11
    uint8_t  m_objectManagerState[0x88]; // +0x18..+0xA0  ObjectManager pool/WUID map (sub_180BEC3C4) [U interior]
};
static_assert(sizeof(C_ParticleEffectPuppetManager) == 0xA0, "must be 0xA0 (alloc 160 at sub_180BECB38)");

}  // namespace wh::xgenaimodule
