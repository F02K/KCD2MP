#pragma once
#include <cstdint>
#include <unordered_map>
#include "../framework/WUID.h"
#include "../framework/C_Listeners.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIObjectManager -- the linkable/AIObject registry, HUB SLOT 1
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x110 (create-site proven: new 272 in
// sub_180BECBAC).
// -----------------------------------------------
// RTTI TD rva 0x4FE92C0; vtable 0x18400F960, 3 slots. Singleton global
// qword_185481FC8 (hub getter sub_181A9DC40 = literal return; G1 §2.2/§2.3 corrected
// by G2_dossier §11.2). ctor sub_180BEC6CC (base sub_180BEC304). Real base chain
// (template object-manager stack, SCRIPTABLE batch -- modeled flat here):
//   C_AIObjectManager : C_ObjectManagerOwnStorage<C_AIObject,C_AIObjectManager>
//   : C_ObjectManager<...> : Callbacks::C_NoDataExtension<Functor2<E_AIONotifyEvent::
//   Type, WUID&>> : ... with C_CallbackListHolder @+0x08, C_TrackSequencing<1> @+0x10.
// Find-by-linkableId: sub_18047C1F8 -- FNV-1a-64(id, 8) over the map @+0x60, value at
// node+0x18. Every spine ctor level registers here (insert + notify sub_18047C238);
// the C_AIObject dtor and the NPC-factory error path unregister via slot [2].
// NOTE: the supplement's "+0xC0 small 2-elem init (sub_180A03994(this+0xC0, 2, ...))"
// is modeled as the C_Listeners<...,2> dispatch-cursor pair -- C_Listeners<L,2> is
// 0x28 (vptr + vector + int32[2]), which lands the cursors exactly at +0xC0 and the
// second map at +0xC8 [deduction; flagged].

namespace wh::xgenaimodule {

class C_AIObject;
class I_AIObjectNotifyListener;   // notify-listener interface (not yet RE'd; pointers only)

class C_AIObjectManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIObjectManager;
    virtual ~C_AIObjectManager();                            // [0] deleting dtor sub_18341F7CC
    virtual void _vf1();                                     // [1] nullsub_1 [U role]
    virtual void RemoveObject(const framework::WUID& id);    // [2] sub_18047B810 -> sub_18047B818; erases from m_objects [V, U exact signature]

    uint64_t m_callbackHead;   // +0x08  C_CallbackListHolder list head = 0
    uint8_t  m_trackSeq;       // +0x10  C_TrackSequencing<1> flag = 0
    uint8_t  _pad11[7];        // +0x11
    uint8_t  m_seqBlock[0x40]; // +0x18..+0x57  callback/track-sequencing block (sub_180BEC624) [U interior]
    uint8_t  m_byte58;         // +0x58  ctor: 0 [U role]
    uint8_t  _pad59[7];        // +0x59
    std::unordered_map<framework::WUID, C_AIObject*> m_objects;   // +0x60  THE registry (Find sub_18047C1F8; MSVC _Hash: ctrl @+0x60, max_load 1.0f, sentinel node, size @+0x70, buckets @+0x78, mask 7, maxidx 8) [V]
    shared::C_Listeners<I_AIObjectNotifyListener, 2> m_listeners; // +0xA0  C_Listeners<I_AIObjectNotifyListener,2> (IDB-named vtable); cursor pair @+0xC0
    uint8_t  m_mapC8[0x40];    // +0xC8..+0x107  second std::unordered_map (same _Hash shape; node alloc 0x40 => pair 0x30 = key 8 + value 0x28); key/value types [U]
    int32_t  m_dword108;       // +0x108 ctor: 0 [U role]
    uint8_t  _pad10C[4];       // +0x10C
};
static_assert(sizeof(std::unordered_map<wh::framework::WUID, void*>) == 0x40, "std::unordered_map must be 0x40");
static_assert(sizeof(wh::shared::C_Listeners<void, 2>) == 0x28, "C_Listeners<L,2> must be 0x28");
static_assert(sizeof(C_AIObjectManager) == 0x110, "C_AIObjectManager must be 0x110");

}  // namespace wh::xgenaimodule
