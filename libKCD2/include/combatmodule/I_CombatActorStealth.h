#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::combatmodule::I_CombatActorStealth -- interface of the per-actor stealth
// takedown subsystem (KCD2 WHGame.dll 1.5.6, kd7u).  Pure interface (CHD
// numBases 1); subobject at C_CombatActorStealth+0x18.
// -----------------------------------------------
// RTTI TD rva 0x4B00AC8; standalone COL 0x184160290; abstract vtable
// 0x183B0A4E0 (slot 0 dtor sub_18275CD30, slots 1-9 _purecall).  Concrete
// override vtable = 0x183B0A538 (see C_CombatActorStealth.h).
// Delegate = 16-byte {void* boundThis, void* fn} pair.  Signal names A/B/C are
// structural (target member offsets verified; event semantics [U]).
// Can-checker sub_18072F2A8(actor, reqType 3|4, victim): gated by the global
// stealth-actions enable (sub_1809D5994()[327]); dagger check for kill
// (sub_1817B5460), KO check sub_1817AA920; returns E_StealthActionType.

namespace wh::combatmodule {

class I_CombatActorStealth {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_CombatActorStealth;
    virtual ~I_CombatActorStealth();                                 // [0] sub_18275CD30
    virtual int32_t CanStealthKill(uint32_t victimEntityId) = 0;     // [1] -> sub_18072F2A8(this, 3, victim); returns E_StealthActionType
    virtual int32_t CanStealthKnockout(uint32_t victimEntityId) = 0; // [2] -> sub_18072F2A8(this, 4, victim)
    virtual char Arm(uint32_t victimEntityId, int32_t satType) = 0;  // [3] sub_18275EF4C -- arms +0x70/74/75/78/7C
    virtual void ConnectSignalA(void* delegate2q) = 0;               // [4] add -> m_signalA (+0x20)
    virtual void DisconnectSignalA(void* delegate2q) = 0;            // [5] remove <- m_signalA
    virtual void ConnectSignalB(void* delegate2q) = 0;               // [6] add -> m_signalB (+0x30)
    virtual void DisconnectSignalB(void* delegate2q) = 0;            // [7] remove <- m_signalB
    virtual void ConnectSignalC(void* delegate2q) = 0;               // [8] add -> m_signalE (+0x60)
    virtual void DisconnectSignalC(void* delegate2q) = 0;            // [9] remove <- m_signalE
};
static_assert(sizeof(I_CombatActorStealth) == 0x08, "I_CombatActorStealth is vptr-only");

}  // namespace wh::combatmodule
