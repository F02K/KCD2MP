#pragma once
#include <cstdint>
#include <unordered_map>
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::xgenaimodule::C_MessageCapableObjectManager -- the message-routing registry
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xA0 (create-site proven: new 160 in
// sub_180BECAC4).
// -----------------------------------------------
// RTTI TD rva 0x4FE8B60; vtable 0x183A84820, 3 slots ([0] dtor sub_18341F70C,
// [1] nullsub_1, [2] sub_180A2D924). Global qword_185496198; NOT hub-reached
// (confirmed twice, G1 §2.3 / G2_dossier §11.6). Same template base stack as
// C_AIObjectManager (C_ObjectManagerOwnStorage<C_MessageCapableObject,...>, modeled
// flat). C_MessageCapableObject's ctor sub_180A2BD74 hashmap-registers here
// (node+0x18 = this); C_NPCMessageDispatcher::Dispatch resolves targets here.
// Layout: the 0xA0 total = the 0x60 base-stack prefix (identical shape to
// C_AIObjectManager) + the registry map @+0x60 -- prefix interior [U], map [V]
// (registration inserts at mgr+0x60).

namespace wh::xgenaimodule {

class C_MessageCapableObject;

class C_MessageCapableObjectManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MessageCapableObjectManager;
    virtual ~C_MessageCapableObjectManager();   // [0] deleting dtor sub_18341F70C
    virtual void _vf1();                        // [1] nullsub_1 [U role]
    virtual void _vf2();                        // [2] sub_180A2D924 [U role -- likely Remove/Unregister]

    uint64_t m_callbackHead;   // +0x08  C_CallbackListHolder list head = 0
    uint8_t  m_trackSeq;       // +0x10  C_TrackSequencing<1> flag = 0
    uint8_t  _pad11[7];        // +0x11
    uint8_t  m_seqBlock[0x40]; // +0x18..+0x57  callback/track-sequencing block [U interior]
    uint8_t  m_byte58;         // +0x58  [U role]
    uint8_t  _pad59[7];        // +0x59
    std::unordered_map<framework::WUID, C_MessageCapableObject*> m_objects;   // +0x60  the routing registry (node+0x18 = object) [V]
};
static_assert(sizeof(C_MessageCapableObjectManager) == 0xA0, "C_MessageCapableObjectManager must be 0xA0");

}  // namespace wh::xgenaimodule
