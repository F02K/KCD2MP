#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::C_IntelligentObjectManager -- per-level intelligent-object
// registry SINGLETON at qword_1854930D0 (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof
// 0xB0 (inlined create sub_18197934C @0x181979374: qword_18549D378(176,&v5),
// += 0xB0, body init sub_180BEC184).
// -----------------------------------------------
// Real base stack (RTTI TD 0x4EFE0D0): C_ObjectManager<C_IntelligentObject,..>
// + Callbacks bases @+0x8/+0x10, PLUS a VIRTUAL base I_DebugDraw (RTTI pdisp
// 0xA0, vdisp 4): vbtable ptr @+0xA0 (&unk_183FD76E0), I_DebugDraw subobject
// vptr @+0xA8 (vtable rva 0x3FD76B0: [0]sub_18213D49C [1]nullsub).  Modeled
// FLAT with the virtual-base tail as raw pointers -- pragmatic escape hatch,
// virtual inheritance NOT reproduced [U].  Primary vtable rva 0x3FD76F0
// (3 slots: [0] dtor sub_183208724, [1] nullsub, [2] sub_1810D772C).

namespace wh::xgenaimodule {

class C_IntelligentObjectManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_IntelligentObjectManager;
    virtual ~C_IntelligentObjectManager();   // [0] sub_183208724
    virtual void _vf1();                     // [1] nullsub [U role]
    virtual void _vf2();                     // [2] sub_1810D772C [U role]

    uint64_t    m_callbackHead;   // +0x08  Callbacks::C_CallbackListHolder list head
    uint8_t     m_trackSeq;       // +0x10  Callbacks::C_TrackSequencing<1> flag
    uint8_t     _pad11[7];        // +0x11
    uint8_t     m_objectManagerState[0x88]; // +0x18..+0xA0  registry incl. hashmap @+0x60 (sub_180BEC184) [U interior]
    const void* m_vbtable;        // +0xA0  vbtable ptr (&unk_183FD76E0) -- raw-modeled virtual-base machinery [U]
    const void* m_debugDrawVptr;  // +0xA8  virtual-base I_DebugDraw subobject vptr (vtable rva 0x3FD76B0) -- raw-modeled [U]
};
static_assert(sizeof(C_IntelligentObjectManager) == 0xB0, "must be 0xB0 (alloc 176 at sub_18197934C)");

}  // namespace wh::xgenaimodule
