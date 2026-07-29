#pragma once
#include <cstdint>
#include <vector>
#include "I_MessageDispatcher.h"

// -----------------------------------------------
// wh::xgenaimodule::C_NPCMessageDispatcher : I_MessageDispatcher -- routes AI
// messages to message-capable objects; SINGLETON at qword_1855A9E10 (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x70 (getter sub_1830DC5A8 @0x1830dc5fa:
// qword_18549D378(112,...), dword_1851DFB18 += 0x70; ctor sub_181173200, also
// inlined in the getter).  Pure message router -- no external registration.
// -----------------------------------------------
// RTTI TD rva 0x4F5D548; vtable 0x183FEE260, 3 slots: [0] deleting dtor
// sub_1832F5660, [1] nullsub_1, [2] Dispatch = sub_18070D668 [V]:
//   if the message has a target: look it up in C_MessageCapableObjectManager
//   (q_185496198) by msg.target+0x28, invoke the target's OnMessage (spine
//   slot 19, vf +0x98), return (result != 3).
// dtor sub_1832F52EC frees the +0x58 vector as a pointer array.

namespace wh::xgenaimodule {

class C_NPCMessageDispatcher : public I_MessageDispatcher {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NPCMessageDispatcher;
    ~C_NPCMessageDispatcher() override;  // [0] deleting dtor sub_1832F5660
    bool Dispatch(void* msg) override;   // [2] sub_18070D668 [V]

    int32_t            m_08;           // +0x08  ctor: 2 [U meaning]
    uint8_t            _pad0C[4];      // +0x0C
    uint64_t           m_10;           // +0x10  ctor: 0 [U role]
    uint64_t           m_18;           // +0x18  ctor: 0 [U role]
    uint8_t            m_zero20[0x30]; // +0x20..+0x50  3x OWORD zeroed (vector-like) [U types]
    int32_t            m_50;           // +0x50  ctor: -1 [U role]
    int32_t            m_54;           // +0x54  ctor: 0 [U role]
    std::vector<void*> m_vec58;        // +0x58  freed as ptr array in dtor sub_1832F52EC [U elem type]
};
static_assert(sizeof(C_NPCMessageDispatcher) == 0x70, "C_NPCMessageDispatcher must be 0x70 (alloc 112 at sub_1830DC5A8)");

}  // namespace wh::xgenaimodule
