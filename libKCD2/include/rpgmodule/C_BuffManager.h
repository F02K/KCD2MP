#pragma once
#include <cstdint>
#include <unordered_map>
#include "I_BuffManager.h"
#include "../framework/C_Signal.h"
#include "../framework/WUID.h"
#include "../framework/HashPrimitives.h"

// -----------------------------------------------
// wh::rpgmodule::C_BuffManager : I_BuffManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x68.
// -----------------------------------------------
// RTTI .?AVC_BuffManager@rpgmodule@wh@@ (TD 0x184CE4ED0)  vtable 0x183EFA3B0 (16 slots)
// ctor sub_180E82180  dtor sub_182BC5378.  Singleton qword_18548E360 (allocator sub_180E820D4:
// alloc 104 + _InterlockedAdd 0x68 -- sizeof hard-confirmed).
//
// [FUNDAMENTAL vs KCD1] A lightweight manager: 2 signals + 1 WUID->instance index. Souls OWN
// their buffs (vector in the soul's buff list @C_Soul+0xAF8); this map is only the global
// by-WUID fast index. Buff DEFINITIONS are NOT stored here either: GetBuffDefinition binary-
// searches a GLOBAL sorted vector (xmmword_18532CD70 {begin,end}, 0x98-byte records keyed by
// CryGUID @rec+0; default record &qword_18532CCA0). And there is NO C_BuffFactory class --
// creation delegates to the global RTTR reflection factory ("wh.rpgmodule.buff.<typename>").
// Insert site for m_activeBuffsByWuid was not decompiled (read path verified); map value could
// be the more-derived C_SoulBuffInstance* -- conservative base type kept (flagged).

namespace wh::rpgmodule {

class C_BuffManager : public I_BuffManager {   // +0x00  vptr (I_BuffManager base is data-less)
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BuffManager;

    // Forwards to the engine's lazy allocator sub_180E820D4 (mutex-guarded; instance
    // qword_18548E360), so first-call init runs. Impl in src/rpgmodule/rpgmodule.cpp.
    static C_BuffManager* GetInstance();
    wh::shared::C_Signal<> m_signalA;   // +0x08  buff removed/changed (fired by RemoveBuff sub_18063A664
                                        //        + exclusivity resolver sub_18063B300); connect [11]/disconnect [12].
                                        //        Published name UNVERIFIED ("rpgModule:buffTagChanged" family?)
    wh::shared::C_Signal<> m_signalB;   // +0x18  buff-change event; connect [13]/disconnect [14]
    // Key = 8-byte WUID (FNV-1a lookup sub_18046D6C4, basis 0xCBF29CE484222325); node value_type
    // 0x10 {WUID@+0x10, ptr@+0x18} matches the observed 0x20 list nodes / 0x20 sentinel alloc.
    std::unordered_map<wh::framework::WUID, C_BuffInstanceBase*,
                       wh::shared::S_DefaultHash<wh::framework::WUID>> m_activeBuffsByWuid;  // +0x28
};
static_assert(sizeof(C_BuffManager) == 0x68, "C_BuffManager must be 0x68");

}  // namespace wh::rpgmodule
