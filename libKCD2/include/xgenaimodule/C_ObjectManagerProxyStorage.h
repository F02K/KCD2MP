#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::xgenaimodule::C_ObjectManagerProxyStorage<TObject,TManager,TOwner> -- the
// proxy-storage object-manager template stack, modeled FLAT (KCD2 WHGame.dll 1.5.6,
// kd7u).  sizeof 0x68 per instantiation.
// -----------------------------------------------
// Real base chain (template callback machinery, SCRIPTABLE batch -- collapsed here):
//   C_ObjectManagerProxyStorage<TObject,TManager,TOwner>
//     : C_ObjectManager<TObject,TManager>
//     : Callbacks::C_NoDataExtension<Functor2<E_AIONotifyEvent::Type,
//       framework::WUID const&>,1> : Callbacks::C_Extension<...>
//     : Callbacks::C_NoDataExtensionBase<...>
//   with Callbacks::C_CallbackListHolder<C_Callback<...>> @+0x08 and
//   Callbacks::C_TrackSequencing<1> @+0x10 (RTTI CHD of both instantiations).
// Layout proven by the two instantiation ctors: sub_180BEC09C (C_LinkablesManager)
// and the C_DynamicLinkablesManager ctor -- identical first-0x68 shape as the
// own-storage variant flattened into C_AIObjectManager.  3-slot primary vtable per
// instantiation ([0] deleting dtor, [1] nullsub_1, [2] remove/unregister).

namespace wh::xgenaimodule {

template <class TObject, class TManager, class TOwner>
class C_ObjectManagerProxyStorage {
public:
    virtual ~C_ObjectManagerProxyStorage();   // [0] deleting dtor (instantiation-specific)
    virtual void _vf1();                      // [1] nullsub_1 [U role]
    virtual void _vf2();                      // [2] remove/unregister object (instantiation-specific) [U role]

    uint64_t m_callbackHead;    // +0x08  C_CallbackListHolder list head, ctor: 0
    uint8_t  m_trackSeq;        // +0x10  C_TrackSequencing<1> flag, ctor: 0
    uint8_t  _pad11[7];         // +0x11
    uint8_t  m_registry[0x40];  // +0x18..+0x57  object-registry storage (init sub_180BEC624, same helper as C_AIObjectManager's block) [U interior]
    uint8_t  m_byte58;          // +0x58  ctor: 0 [U role]
    uint8_t  _pad59[7];         // +0x59
    uint64_t m_liveCount;       // +0x60  live-object count (ctor: 0; ++ on register sub_18047C238/sub_18047C398) [V]
};
static_assert(sizeof(C_ObjectManagerProxyStorage<int, int, int>) == 0x68,
              "C_ObjectManagerProxyStorage instantiations must be 0x68");

}  // namespace wh::xgenaimodule
