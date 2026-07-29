#include "framework/GuidUtils.h"
#include "Offsets/Offsets.h"

// wh GUID text utilities -- KCD2 WHGame.dll 1.5.6 RVAs (verified in utem).

namespace wh {

bool ParseGuid(const char* text, CryGUID& out)
{
    // sub_180719B1C (RVA 0x719B1C): bool(char* text, GUID16* out).
    using Fn = bool(__fastcall*)(const char*, CryGUID*);
    static REL::Relocation<Fn> fn{ REL::ID(39331) };
    return fn(text, &out);
}

}  // namespace wh
