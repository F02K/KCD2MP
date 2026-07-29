#pragma once
#include <cstdint>
#include "C_SmartEntity.h"
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::xgenaimodule::C_InteractiveSmartEntity : C_SmartEntity -- interactive
// smart-entity level (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x1A0 (inferred:
// both C_SmartObject variants embed C_Ownership at +0x1A0).  No standalone
// heap ctor observed -- constructed only as a base of C_SmartObject (ctor
// inlined in sub_180A2B1FC).
// -----------------------------------------------
// RTTI TD rva 0x4F5A478 -- same 4 MI subobjects at the same offsets as
// C_SmartEntity.  Primary vtable rva 0x3A6BCC8 (40 slots; differs from
// C_SmartEntity ONLY at [0]dtor 0x3442150, [1]0x1AABDD0, [29]0xA2C160).
// Secondary vtables +0x20 0x3A6BBD8 / +0xA8 0x3A6BBF8 / +0xB0 0x3A6BC58
// (bodies identical to C_SmartEntity's).

namespace wh::xgenaimodule {

class C_InteractiveSmartEntity : public C_SmartEntity {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InteractiveSmartEntity;
    ~C_InteractiveSmartEntity() override;  // [0]  deleting dtor 0x3442150
    uint32_t GetTypeMask() override;       // [1]  0x1AABDD0
    void _vf29() override;                 // [29] 0xA2C160

    framework::WUID m_wuid188;   // +0x188  fresh WUID (sub_1804FD80C+3)
    int32_t         m_190;       // +0x190  ctor: -1 [U role]
    int32_t         m_194;       // +0x194  ctor: -1 [U role]
    uint8_t         m_flags198;  // +0x198  bits from ctor args a4/a5 [U meaning]
    uint8_t         _pad199[7];  // +0x199
};
static_assert(sizeof(C_InteractiveSmartEntity) == 0x1A0, "C_InteractiveSmartEntity must be 0x1A0 (C_Ownership @+0x1A0 in variants)");

}  // namespace wh::xgenaimodule
