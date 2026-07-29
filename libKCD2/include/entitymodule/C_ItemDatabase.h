#pragma once
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "../databasemodule/C_ObjectTreeDatabase.h"
#include "S_ItemClass.h"

// -----------------------------------------------
// wh::entitymodule::C_ItemDatabase -- THE global item-class database (KCD2 WHGame.dll 1.5.6).
// -----------------------------------------------
// RTTI .?AVC_ItemDatabase@entitymodule@wh@@; concrete vtable 0x183BB0DF8 over the tree base's
// 0x183C6FD98.  STATIC-INIT SINGLETON @0x185325820 (ctor 0x1815416BC -> tree registration ctor
// 0x1819AB960: ns "wh::entitymodule::", name "Item", group "item"; atexit dtor 0x18216C020).
// Rows are std::shared_ptr<S_ItemClass> in the tree layer (matches the RTTR as_std_shared_ptr
// constructor wrappers); m_guidIndex is the raw-pointer lookup side (comparator 0x18046A5AC,
// register sub_18097054C calls vf[51] OnRegister first, loader sub_1809702AC two-pass).
// Tail past +0x88 NOT modeled: name-index map ~+0xA8, money-guid cache +0xB8, aux vector
// +0xC8 -- use by pointer only, never sizeof.
namespace wh::entitymodule {

class C_ItemDatabase
    : public wh::databasemodule::C_ObjectTreeDatabase<std::shared_ptr<S_ItemClass>> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ItemDatabase;

    // The static singleton @0x185325820 (src/entitymodule/C_ItemDatabase.cpp).
    static C_ItemDatabase* GetInstance();

    // Binary-search m_guidIndex; nullptr when absent (null guid early-outs).  0x180468340 --
    // the lookup behind FindItemByClass's substitute retry, GetItemUiPresentation, the
    // alchemy ingredient counter and ~80 other sites.
    S_ItemClass* FindClassByGuid(const CryGUID& classGuid);

    std::vector<std::pair<CryGUID, S_ItemClass*>> m_guidIndex;   // +0x78  guid-sorted, stride 24
};

static_assert(offsetof(C_ItemDatabase, m_guidIndex) == 0x78, "guid index at 0x78");

}  // namespace wh::entitymodule
