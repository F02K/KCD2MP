#include "entitymodule/C_ItemManager.h"
#include "Offsets/Offsets.h"

// C_ItemManager singleton accessor + registry lookup (KCD2 WHGame.dll 1.5.6 RVAs, kd7u).

namespace wh::entitymodule {

C_ItemManager* C_ItemManager::GetInstance()
{
    // qword_185487A78: the C_EntityModule ctor (sub_180BE8968) mirrors a1[25] here right
    // after constructing the manager (alloc 0x5B9070 == sizeof).
    static REL::Relocation<C_ItemManager**> p{ REL::ID(2353) };
    return *p;
}

C_Item* C_ItemManager::LookupByWUID(const wh::framework::WUID& wuid)
{
    // sub_181E3E090(&m_itemTable, &wuid) -- the exact call every item resolver makes
    // (e.g. sub_18082CFB0 passes this+0x168). WUID type byte must be 2 (item),
    // idx = WUID & 0x3FFFF, generation = WUID >> 18; the slot's C_Item must store the
    // same full WUID at +0x30.
    using Fn = C_Item* (__fastcall*)(void*, const wh::framework::WUID*);
    static REL::Relocation<Fn> fn{ REL::ID(64) };
    return fn(&m_itemTable, &wuid);
}

}  // namespace wh::entitymodule
