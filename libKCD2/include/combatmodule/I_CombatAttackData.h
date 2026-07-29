#pragma once

// -----------------------------------------------
// wh::combatmodule::I_CombatAttackData -- single-slot interface exposing an attack's aim-zone
// mapping (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vtable only).
// -----------------------------------------------
// RTTI .?AVI_CombatAttackData@combatmodule@wh@@ (TD 0x4B130E0). Like KCD1 the interface has NO
// standalone vtable -- the single slot is owned by the implementer. The only recovered implementer
// is S_CombatActionAttackData, which embeds I_CombatAttackData as a base at +0x40 (BCD mdisp 0x40);
// there is also an mdisp-0 base use. The +0x40 subobject vtable (0x183A65D70) is 1 slot:
//   [0] sub_181A77FD0 = "mov rax, [rcx+0x108]; ret"  (returns the aim zone-mapping FK pointer)
// so the interface is a single pure virtual, exactly as in KCD1 (the KCD1 return offset was +0xE0;
// KCD2 reads +0x108 relative to the interface subobject).

namespace wh::combatmodule {

class C_CombatZoneMappingData;

class I_CombatAttackData {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatAttackData;
    // Returns the attack's AIM zone-mapping FK (KCD2 impl: *(this+0x108)).
    virtual C_CombatZoneMappingData* GetAimZoneMapping() = 0;   // [0] sub_181A77FD0
};

}  // namespace wh::combatmodule
