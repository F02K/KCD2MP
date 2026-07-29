#pragma once
#include <cstdint>
#include "C_ObjectManagerProxyStorage.h"
#include "../crysystem/IEntitySystemSink.h"
#include "../framework/I_WUIDMappingProvider.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DynamicLinkablesManager -- registry of all
// C_DynamicLinkableObject instances (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof >= 0x78,
// NOT create-site proven -> no size assert [U].
// -----------------------------------------------
// RTTI TD rva 0x4FF5840; THREE COLs.  Primary vtable 0x184014058, 3 slots
// ([0] deleting dtor sub_18341F834, [1] nullsub_1, [2] sub_18047B660).  MI bases:
//   +0x00  C_ObjectManagerProxyStorage<C_DynamicLinkableObject,
//          C_DynamicLinkablesManager, C_AIObjectManager>  (0x68)
//   +0x68  ::IEntitySystemSink -- subobject vtable 0x184014010 has 8 SLOTS vs the
//          7-slot stock declaration ([0] sub_18213D730, [1] sub_18041A6A0 ret 1,
//          [3] sub_18047E8D0, [7] sub_181AABE10, rest nullsub).  Interfuscator-
//          shuffled + extra slot => layout-only base; pures intentionally NOT
//          overridden here (class stays abstract in this mirror, pointer-only use),
//          same policy as C_NPCManager.
//   +0x70  framework::I_WUIDMappingProvider -- subobject vtable 0x184014078 has
//          only 2 SLOTS ([0] sub_1832A96F0 GetValueForWuid, [1] sub_1834432F0
//          GetWuidForKey) vs the 3-slot declaration in the existing header (dtor
//          last) -- DISCREPANCY flagged; the 2 data slots match, the trailing dtor
//          slot may be folded/absent in this subobject [U].
// Dynamic objects register in q_1854961A0 (C_DynamicLinkableObject ctor
// sub_18047BCFC).  Members past +0x78 unresolved.

namespace wh::xgenaimodule {

class C_DynamicLinkableObject;
class C_AIObjectManager;

class C_DynamicLinkablesManager
    : public C_ObjectManagerProxyStorage<C_DynamicLinkableObject, C_DynamicLinkablesManager, C_AIObjectManager>,
      public IEntitySystemSink,
      public framework::I_WUIDMappingProvider {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DynamicLinkablesManager;
    ~C_DynamicLinkablesManager() override;   // [0] deleting dtor sub_18341F834
    void _vf2() override;                    // [2] sub_18047B660 -- remove/unregister object [U role]
    // framework::I_WUIDMappingProvider impls
    void* GetValueForWuid(const void* wuid) override;          // sink vt [0] sub_1832A96F0
    void  GetWuidForKey(void* out, const void* key) override;  // sink vt [1] sub_1834432F0
    // IEntitySystemSink pures NOT overridden (see banner) -- abstract mirror.
};
// sizeof >= 0x78 (base 0x68 + two MI vptrs); total size not create-site proven -> no static_assert [U]

}  // namespace wh::xgenaimodule
