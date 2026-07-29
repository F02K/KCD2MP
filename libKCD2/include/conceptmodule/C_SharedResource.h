#pragma once
#include <cstdint>
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::conceptmodule::C_SharedResource -- refcounted node-graph resource root
// (KCD2 WHGame.dll Steam 1.5.6, e4cp).  sizeof 0x10, vtable 0x183A46568 (8 slots).
// -----------------------------------------------
// Root of the whole concept-graph object family: C_Node AND I_Port both derive from it
// (plain single inheritance, every subobject at offset 0 -- RTTI base-array proven).
// Intrusively refcounted AND id-registered: +0x08 holds an id in the shared-resource
// registry at S_GameContext+0x120 -> +0x18 -> +0x10 (accessor sub_180646DD4; registry
// vfuncs +0x00 Register(ptr)->id, +0x08 Unregister(id), +0x10 Resolve(id)->ptr,
// +0x18 AllocateId, +0x20 GetGeneration(id) -- ports weak-ref nodes through it).
// AddRef 0x181E38F20 (registers this on the first ref), Release 0x181E39320
// (unregisters + deletes via slot [0] on the last ref); both Interlocked16 on +0x0C.
// Ctor 0x1806B2850.  rttr passes graph objects around as _smart_ptr<C_SharedResource>.

namespace wh::conceptmodule {

class C_SharedResource {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SharedResource;

    enum E_Flag : uint16_t {                // m_flags bits
        Flag_Activated          = 0x1,      // Activate() already ran (idempotence latch, 0x180e544c1)
        Flag_AutoTriggerable    = 0x2,      // set for life by the C_AutoTriggerable ctor (0x1806b214b)
        Flag_AutoTriggerEnabled = 0x4,      // auto-trigger currently registered (slots 25/37/38)
    };

    virtual ~C_SharedResource();            // [0]  scalar deleting dtor (Release invokes it with flag 1)
    virtual void unk01();                   // [1]  nullsub in every sampled vtable [U]
    virtual void unk02();                   // [2]  nullsub in every sampled vtable [U]
    virtual void unk03();                   // [3]  nullsub in every sampled vtable [U]
    virtual void Activate();                // [4]  nodes: COMDAT-shared 0x180E544B4 -- one-shot (Flag_Activated) rttr port wiring, body sub_1804F4BB0 [root/port impls unsampled]
    RTTR_ENABLE()                           // [5..7]  get_ptr = the shared 0x1805F5DA0

    uint32_t m_resourceId;   // +0x08  registry id (allocated in ctor, re-registered on first AddRef)
    int16_t  m_refCount;     // +0x0C  _InterlockedIncrement16 / _InterlockedExchangeAdd16
    uint16_t m_flags;        // +0x0E  E_Flag bits
};
static_assert(sizeof(C_SharedResource) == 0x10, "C_SharedResource must be 0x10 (C_Node/I_Port members start at +0x10)");

}  // namespace wh::conceptmodule
