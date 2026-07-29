#include "entitymodule/C_InventoryManager.h"
#include "Offsets/Offsets.h"

// C_InventoryManager singleton accessor + registry lookup (KCD2 WHGame.dll 1.5.6 RVAs, kd7u).

namespace wh::entitymodule {

C_InventoryManager* C_InventoryManager::GetInstance()
{
    // qword_185487A58: the C_EntityModule ctor (sub_180BE8968) mirrors a1[29] here
    // (alloc 0x100280 == sizeof).
    static REL::Relocation<C_InventoryManager**> p{ REL::ID(2352) };
    return *p;
}

C_Inventory* C_InventoryManager::LookupByWUID(const wh::framework::WUID& wuid)
{
    // sub_181EC6B20(this+0x28, &wuid): WUID type byte must be 3 (inventory), idx = low
    // 16 bits (0 reserved), generation = WUID bits 16..31 against m_slots[idx]; the found
    // C_Inventory must report the same WUID via the virtual getter on its +0xE8 subobject.
    using Fn = C_Inventory* (__fastcall*)(void*, const wh::framework::WUID*);
    static REL::Relocation<Fn> fn{ REL::ID(65) };
    return fn(&m_poolCurBlock, &wuid);
}

}  // namespace wh::entitymodule
