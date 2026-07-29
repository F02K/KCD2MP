#pragma once
#include <cstdint>

// -----------------------------------------------
// IPlayerProfileListener -- KCD2 binary vtable order (kd7u)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6. RTTI: .?AVIPlayerProfileListener@@ (global namespace;
// CryAction IPlayerProfiles.h). Profile save/load sink; registered through
// IPlayerProfileManager::AddListener (framework vf +0xF8 -> object vf +0x128 ->
// AddListener vf +0xF8(listener, updateNow) -- C_UISettings ctor 0x18060104b..7e).
//
// Shape evidence: C_UISettings' listener vtable (subobject +0x58, slots): [0] adjustor
// thunk 0x18213A758 into the class dtor, [1] 0x181A71808 -> sub_18079CF88(base),
// [2] 0x180A6A4D8 -> sub_18079C4A8(base). Matches the stock SDK declaration order
// {~dtor, SaveToProfile, LoadFromProfile}; which body saves vs loads is INFERRED from
// that slot order only.

namespace Offsets {

struct IPlayerProfile;   // opaque (CryAction profile object)

struct IPlayerProfileListener {
    virtual ~IPlayerProfileListener() = default;                                               // [0]
    virtual void SaveToProfile(IPlayerProfile* pProfile, bool online, unsigned int reason) = 0;   // [1]
    virtual void LoadFromProfile(IPlayerProfile* pProfile, bool online, unsigned int reason) = 0; // [2]
};
static_assert(sizeof(IPlayerProfileListener) == 8);

}  // namespace Offsets
