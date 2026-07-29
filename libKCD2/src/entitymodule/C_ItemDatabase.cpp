#include "entitymodule/C_ItemDatabase.h"
#include "Offsets/Offsets.h"

// C_ItemDatabase engine-function forwarders (KCD2 WHGame.dll 1.5.6;
// ids = kcd2 address library).

namespace wh::entitymodule {

C_ItemDatabase* C_ItemDatabase::GetInstance()
{
    // The database object itself is the static global 0x185325820 (not a pointer slot).
    static REL::Relocation<C_ItemDatabase*> inst{ REL::ID(1248956) };
    return reinterpret_cast<C_ItemDatabase*>(inst.address());
}

S_ItemClass* C_ItemDatabase::FindClassByGuid(const CryGUID& classGuid)
{
    using Fn = S_ItemClass* (__fastcall*)(C_ItemDatabase*, const CryGUID*);
    static REL::Relocation<Fn> fn{ REL::ID(26755) };
    return fn(this, &classGuid);
}

}  // namespace wh::entitymodule
