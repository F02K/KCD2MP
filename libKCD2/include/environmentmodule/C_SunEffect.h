#pragma once
#include <cstdint>
#include "../conceptmodule/C_Effect.h"
#include "../conceptmodule/C_TypedPortRef.h"

// -----------------------------------------------
// wh::environmentmodule::C_SunEffect -- concept-graph sun/celestial effect node
// (KCD2 1.5.6, kd7u).  modeled sizeof 0x198 [alloc UNPROVEN -- rttr-factory created].
// -----------------------------------------------
// Own vtable 0x183C2B208; ctor sub_181851BB0 (base chain: conceptmodule::C_Effect ->
// C_AutoTriggerable<C_Node> -> C_Node -> C_SharedResource, base ctor sub_1806B19C0).
// Cheat surface: sun angle / celestial control through the four ports below (port ROLES are
// UNVERIFIED -- named by offset until the rttr property table is walked). The tail writes
// at +0x18C were decompiler-read as a misaligned qword; modeled as two dwords.

namespace wh::environmentmodule {

class C_SunEffect : public conceptmodule::C_Effect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SunEffect;
    conceptmodule::C_TypedPortRef<bool>  m_port88;    // +0x88   [role UNVERIFIED]
    conceptmodule::C_TypedPortRef<float> m_portC8;    // +0xC8   [role UNVERIFIED -- angle/intensity candidate]
    conceptmodule::C_TypedPortRef<bool>  m_port108;   // +0x108  [role UNVERIFIED]
    conceptmodule::C_TypedPortRef<float> m_port148;   // +0x148  [role UNVERIFIED]
    uint8_t  m_flag188;    // +0x188  ctor 0
    uint8_t  _pad189[3];   // +0x189
    float    m_savedParamA; // +0x18C  saved orig sun celestial param (system vtbl[104]) restored on deactivate; setter 0x182AD7164, reader 0x182AD72B0
    float    m_savedParamB; // +0x190  saved orig sun celestial param (system vtbl[96]) restored on deactivate; setter 0x182AD7164, reader 0x182AD72B0
    int32_t  m_unk194;     // +0x194  ctor 0; no writer beyond ctor across 36 vtable methods (scanned) -- reserved/tail pad
};
static_assert(sizeof(C_SunEffect) == 0x198, "modeled extent (ctor field extent; alloc UNPROVEN)");

}  // namespace wh::environmentmodule
