#pragma once
#include <cstdint>
#include "I_SmartAreasManagerListener.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AreaCallbackDispatch : I_SmartAreasManagerListener --
// area event -> callback dispatcher SINGLETON at qword_185496108 (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0xA0 (create inside sub_180F80358:
// operator new(160) -> ctor sub_180F80414).
// -----------------------------------------------
// RTTI TD rva 0x4FE8D48, COL 0x45BB618.  Vtable rva 0x400F148 (4 slots:
// [0]nullsub, [1]OnSmartAreaRemoved-slot = sub_180D9B370 (forwards
// sub_18070A68C(this, msg+392)), [2]nullsub, [3] deleting dtor sub_18341F800
// -- appended own virtual dtor).  C_AreaLabelManager registers itself here
// via thunk sub_18070A630.

namespace wh::xgenaimodule {

class C_AreaCallbackDispatch : public I_SmartAreasManagerListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AreaCallbackDispatch;
    void _vf0() override;                          // [0] nullsub
    void OnSmartAreaRemoved(void* area) override;  // [1] sub_180D9B370
    void _vf2() override;                          // [2] nullsub
    virtual ~C_AreaCallbackDispatch();             // [3] deleting dtor sub_18341F800 (appended)

    void*    m_pRecord;         // +0x08  COW-refcounted multicast-delegate buffer: entries {void* target, void* pmf}[], cap@ptr-8 / count@ptr-4, empty-shared static unk_1856681E0; dispatched by sub_1803A524C (calls pmf(target,arg)) using m_handleA/m_handleB as N=2 re-entrancy cursors -- +0x08..+0x18 form one embedded delegate object [pointee struct unnamed: no header exists]
    int16_t  m_handleA;         // +0x10  ctor: -1 [U role]
    int16_t  m_handleB;         // +0x12  ctor: -1 [U role]
    uint8_t  _pad14[4];         // +0x14
    uint8_t  _zero18[0x30];     // +0x18..+0x48  zeroed [U roles]
    std::unordered_map<framework::WUID, void*> m_callbacks;  // +0x48..+0x88  MSVC std::_Hash: max_load 1.0f @+0x48, self-ref 32B list sentinel @+0x50, size @+0x58, bucket vec @+0x60, mask 7 @+0x78, maxidx 8 @+0x80; key hashed FNV-1a-64/8B (std::hash<WUID>); value = 8B listener/callback obj ptr [U exact value class]
    uint8_t  _tail88[0x18];     // +0x88..+0xA0  std::vector (data/size/cap; elem size 16, per sub_18070A68C a1[17]/a1[18]) [U elem type]
};
static_assert(sizeof(C_AreaCallbackDispatch) == 0xA0, "must be 0xA0 (new 160 at sub_180F80358)");

}  // namespace wh::xgenaimodule
