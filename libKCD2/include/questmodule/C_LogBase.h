#pragma once
#include <cstdint>
#include "../framework/C_LocalizedString.h"

// -----------------------------------------------
// wh::questmodule::C_LogBase -- quest log entry base (KCD2 1.5.6, kd7u).  sizeof 0x30.
// -----------------------------------------------
// Own vtable 0x183ED65B8; ctor sub_181F43240. Base of C_Log / C_EnumLog. Size PROVEN by
// C_Log (adds nothing, alloc 48) and C_EnumLog's first own member at +0x30.

namespace wh::questmodule {

class C_LogBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LogBase;
    virtual ~C_LogBase();                  // [0]; slot set not mapped

    uint32_t m_category;                   // +0x08  ctor 3 [log-type/category enum, UNVERIFIED]
    uint32_t _pad0C;                       // +0x0C
    CryStringT<char> m_name;               // +0x10  name/key (interned empty ctor)
    framework::C_LocalizedString m_text;   // +0x18  embedded localized text
    void*    m_unk28;                      // +0x28  cached ptr: vtable[4] sub_181F43970 sets = *(*(RPGModule+0x88)+0x50) when m_category!=0 [pointee UNVERIFIED]
};
static_assert(sizeof(C_LogBase) == 0x30, "C_LogBase must be 0x30 (C_Log alloc 48)");

}  // namespace wh::questmodule
