#pragma once
#include <cstdint>
#include "../framework/C_BaseModule.h"   // declares framework::I_ModuleMessageListener

// -----------------------------------------------
// wh::xgenaimodule::C_AreaProximityManager : framework::I_ModuleMessageListener
// -- area proximity subscription manager SINGLETON at qword_1854960D8 (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x20 (create sub_181468E5C: operator
// new(32) -> ctor sub_181468EF8).
// -----------------------------------------------
// RTTI TD rva 0x4F4DEE0, COL 0x4551290.  Vtable rva 0x3FE8720 (2 slots:
// [0] deleting dtor sub_1832B7430, [1] OnModuleMessage sub_18047A408: msg 65
// -> register with the event bus (sub_1803EE208) + walk subscribers; msg
// 52/63/77 -> unregister (sub_1803ED5F0)).

namespace wh::xgenaimodule {

class C_AreaProximityManager : public framework::I_ModuleMessageListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AreaProximityManager;
    ~C_AreaProximityManager() override;          // [0] sub_1832B7430
    void OnModuleMessage(void* msg) override;    // [1] sub_18047A408

    void*    m_pSubscribers;   // +0x08  MSVC std::_Tree head node (std::set/map): sentinel 0x70 (_Left/_Parent/_Right=self @+0/+8/+16, _Color=_Isnil=1 -> WORD 0x0101 @+24, _Myval @+0x20, value 0x50); companion _Mysize @+0x10 [element type U]
    uint64_t m_10;             // +0x10  ctor: 0 [U role]
    bool     m_bRegistered;    // +0x18  toggled by OnModuleMessage (65 -> 1; 52/63/77 -> 0)
    uint8_t  _pad19[7];        // +0x19
};
static_assert(sizeof(C_AreaProximityManager) == 0x20, "must be 0x20 (new 32 at sub_181468E5C)");

}  // namespace wh::xgenaimodule
