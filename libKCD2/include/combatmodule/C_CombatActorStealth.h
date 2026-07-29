#pragma once
#include <cstdint>
#include "C_CombatActorUpdatedObject.h"
#include "I_CombatActorStealth.h"
#include "E_CombatSubsystem.h"
#include "E_StealthActionType.h"
#include "E_StealthKillResult.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorStealth : C_CombatActorUpdatedObject +
// I_CombatActorStealth (@+0x18) -- the pre-strike ARMED stealth-attack state
// machine, per-actor combat subsystem at C_CombatActor::m_pStealth (+0x3F8)
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0xC8 (deleting dtor sub_1810EF560:
// operator delete(this, 200) @0x1810ef581; matches C_CombatActor.h).
// -----------------------------------------------
// RTTI TD rva 0x4B00688; COLs 0x1841602B8@0 / 0x1841602E0@0x18.  Primary vtable
// 0x183B0A590 (10 slots, pure UpdatedObject shape -- adds NO own primary
// virtuals): [1] Reset sub_1810EFFAC, [2] GetSubsystemId -> 27 (sub_181A7DFA0),
// [3] GetName -> "STEALTH" (sub_181A7DEA0), [7] UpdatePhase2 sub_181418AEC.
// Interface subobject vtable 0x183B0A538 (see I_CombatActorStealth.h; Arm
// sub_18275EF4C, Can* -> sub_18072F2A8).
// ctor sub_1810F0630 (base sub_180914754(actor, flags=2)); complete dtor
// sub_1810EFB7C (the prior dossier's "ctor" label was WRONG); built by the
// C_CombatActor subsystem builder sub_180916ECC (actor vtbl slot 90).
// STATE MACHINE = 5 free-function callbacks the ctor registers on
// C_CombatActor::m_pState (+0x210) signal heads:
//  cb1 arm      sub_181706C60 (head +0x60):  SAT=KnockoutEnabled, armed=1, victim/eventId set
//  cb2 attack   sub_181794B10 (head +0x7A0): reads victim SAT; SAT4 -> KO roll
//      (sub_1827C7680 -> C_RPGCombat slot 46 sub_182CF8358), SAT3 -> kill roll
//      (sub_1827C78C0 -> slot 47 sub_182CF8620); success -> dispatch action
//      sub_181483580; fail path -> result writer sub_1827C7C3C(+0x9C)
//  cb3 execute  sub_1818C91C0 (head +0xBF0): C_RPGCombat slot 51 sub_182CFBFE4
//      with m_resultCode (fail alerts victim); resets self+victim (sub_18275D150)
//  cb4 value    sub_180911350 (head +0x30):  &2 stealth value (slot 48
//      sub_1827C73E8 -> m_valueSmooth); &8 timewarp (S_Constants[852]=0xD50
//      StealthTimeWarpSpeed); &0x10 finalize (m_resultCode = (m_flag99 != 0))
//  cb5 gate     sub_180C5CA50 (head +0x00):  a3==0 -> execute; else clear m_armGate
// The five +0x20..+0x70 signal members are 0x10 C_ModelArrayProperty<
// framework::WUID, entitymodule::E_HandSlot, ..., C_CombatSignalArrayTrait<...>>
// properties (member COL @0x183B0A5E0) -- kept opaque here [U template].

namespace wh::combatmodule {

class C_CombatActorStealth : public C_CombatActorUpdatedObject,
                             public I_CombatActorStealth {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorStealth;
    // primary (C_CombatActorObject/UpdatedObject) overrides
    void Reset() override;                        // [1] sub_1810EFFAC -- zero arm scalars + m_wuidSlots
    E_CombatSubsystem GetSubsystemId() const override;  // [2] sub_181A7DFA0 -> 27 (COMBAT_SUB_STEALTH)
    const char* GetName() const override;         // [3] sub_181A7DEA0 -> "STEALTH"
    void GetDebugLabel(void* outLabel) override;  // [4] sub_1827603E4
    void GetDebugInfo(void* outInfo) override;    // [5] sub_182757E04
    void UpdatePhase2() override;                 // [7] sub_181418AEC (flags=2; only live phase)
    // I_CombatActorStealth overrides (subobject vtable 0x183B0A538)
    int32_t CanStealthKill(uint32_t victimEntityId) override;      // [1] sub_18072F280
    int32_t CanStealthKnockout(uint32_t victimEntityId) override;  // [2] sub_18072F294
    char Arm(uint32_t victimEntityId, int32_t satType) override;   // [3] sub_18275EF4C
    void ConnectSignalA(void* d) override;        // [4] sub_180A46B10 -> m_signalA
    void DisconnectSignalA(void* d) override;     // [5] sub_1809639F0
    void ConnectSignalB(void* d) override;        // [6] sub_180C57618 -> m_signalB
    void DisconnectSignalB(void* d) override;     // [7] sub_18193B6A4
    void ConnectSignalC(void* d) override;        // [8] sub_180C5A0D0 -> m_signalE
    void DisconnectSignalC(void* d) override;     // [9] sub_1810ED8D8

    // (+0x18 = I_CombatActorStealth vptr)
    uint8_t  m_signalA[0x10];        // +0x20  signal-array prop {desc unk_185666A10, i16 -1, i16 -1} (iface slots 4/5)
    uint8_t  m_signalB[0x10];        // +0x30  {desc unk_185666A10, -1, -1} (iface slots 6/7)
    uint8_t  m_signalC[0x10];        // +0x40  {desc unk_185665AC0, -1, -1} internal
    uint8_t  m_signalD[0x10];        // +0x50  {desc unk_185665430, -1, -1} internal
    uint8_t  m_signalE[0x10];        // +0x60  {desc unk_185665430, -1, -1} (iface slots 8/9)
    E_StealthActionType m_satState;  // +0x70  armed stealth-action type (Reset=0; cb1=KnockoutEnabled; Arm=satType)
    uint8_t  m_armGate;              // +0x74  cleared by cb5 gate (@0x180c5ca55); Arm writes 0x0101 word @+0x74
    uint8_t  m_armed;                // +0x75  cb1/Arm = 1
    uint8_t  m_flag76;               // +0x76  Reset = 0 [U role]
    uint8_t  _pad77;                 // +0x77
    uint32_t m_victimEntityId;       // +0x78  cb1/Arm; Reset = 0
    int32_t  m_eventId;              // +0x7C  -1 idle; cb1 = event id
    float    m_timewarpSmooth;       // +0x80  smoother current (init -100000 sentinel; sub_1809D6A34)
    float    m_timewarpSmoothAux;    // +0x84  smoother aux
    float    m_timewarpRaw;          // +0x88  cb4: (cur+0.2) * StealthTimeWarpSpeed (S_Constants 0xD50)
    uint8_t  m_flag8C;               // +0x8C  [U role]
    uint8_t  _pad8D[3];              // +0x8D
    float    m_valueSmooth;          // +0x90  stealth-value smoother (cb4 &2: C_RPGCombat slot 48)
    float    m_valueSmoothAux;       // +0x94
    uint8_t  m_flag98;               // +0x98  [U role]
    uint8_t  m_flag99;               // +0x99  read by cb4 finalize -> m_resultCode = (m_flag99 != 0)
    uint8_t  m_flag9A;               // +0x9A  [U role]
    uint8_t  m_flag9B;               // +0x9B  cb1 = 1 [U role]
    E_StealthKillResult m_resultCode;// +0x9C  default fail(3); cb2/cb4 write; cb3 passes to C_RPGCombat slot 51
    uint8_t  _pad9D[3];              // +0x9D
    void*    m_pOwnedSub;            // +0xA0  optional owned subobject (ctor sub_1810EB4A8 if entity valid; custom delete)
    uint8_t  m_wuidSlots[2][0x10];   // +0xA8  2 x 16-byte WUID/model default records (ctor sub_18050B734(this+0xA8,16,2,sub_18192BB90); default qword_18531EC48)
};
static_assert(sizeof(C_CombatActorStealth) == 0xC8, "C_CombatActorStealth must be 0xC8 (delete(this,200))");

}  // namespace wh::combatmodule
