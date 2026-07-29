#pragma once
#include <cstdint>
#include "S_DealDamageParams.h"
#include "../framework/C_Signal.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::rpgmodule::C_CombatSoul -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xA8.
// -----------------------------------------------
// Vtable 0x183F45670, 52 slots, RTTI-CERTIFIED (COL 0x1843F6AE8 -> TD .?AVC_CombatSoul@rpgmodule@wh@@;
// slot 52 = the adjacent C_MissileHitCause COL). Ctor sub_18070F6CC writes the vtable; base dtor
// sub_18096712C is NOT in the vtable (no deleting-dtor slot; slot 0 is a setter) -> dtor declared
// non-virtual here to keep the slot map exact. Embedded in C_Soul @+0xF0.
// Verified anchors: 7 C_Signal heads at +0x08..+0x68, owner @+0x78, smart ptrs @+0x98/+0xA0
// (sub_1803F2814 / sub_18117A950); subscribes mailboxes 522 / 279 / 96.
//
// Slots 42..45 are the damage-pipeline PRODUCERS: each stack-builds an S_DamageEventData
// (ctor sub_180C65E64), fills it, submits it to the central dispatcher S_DamageEventData::
// Dispatch (IDA-renamed from sub_18072704C), and
// returns an 8-byte ref-counted cause handle through the MSVC hidden return slot (declared
// here as `void* (void* resultSlot, ...)` to match the raw ABI).

namespace wh::rpgmodule {

class C_Soul;
class C_CombatSoulModel;   // RTTI .?AVC_CombatSoulModel@rpgmodule@wh@@ (160B, factory sub_1803F2814)
class I_CombatSoul;        // rttr base of C_CombatSoul (fwd-decl only -- no RE header yet)

class C_CombatSoul {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatSoul;

    // ---- vtable 0x183F45670 (52 slots; unnamed slots = behavior not yet analyzed) ----
    virtual void  Unk00(const void* delegate16);  // [0]  0x1807D08B0  copies a 16B pair -> sub_180904FF0(&m_signal08, ..)
    virtual void  Unk01();                        // [1]  0x1809048E8
    virtual void  Unk02();                        // [2]  0x180A1CF20
    virtual void  Unk03();                        // [3]  0x180A1C480
    virtual void  Unk04();                        // [4]  0x182D260CC
    virtual void  Unk05();                        // [5]  0x182D267C0
    virtual void  Unk06();                        // [6]  0x1806159A4  (40/130B getter-setter pair pattern with [7])
    virtual void  Unk07();                        // [7]  0x180615330
    virtual void  Unk08();                        // [8]  0x18061597C  (pair with [9])
    virtual void  Unk09();                        // [9]  0x1806153B4
    virtual void  Unk10();                        // [10] 0x180615774  (pair with [11])
    virtual void  Unk11();                        // [11] 0x1806156F0
    virtual void  Unk12();                        // [12] 0x181293EB4  (pair with [13])
    virtual void  Unk13();                        // [13] 0x181293E30
    virtual void  Unk14();                        // [14] 0x181704280
    virtual void  Unk15();                        // [15] 0x1814C8978
    virtual void  Unk16();                        // [16] 0x1809D14D4
    virtual void  Unk17();                        // [17] 0x182D29BF8
    virtual void  Unk18();                        // [18] 0x18046E4E4
    virtual void  Unk19();                        // [19] 0x1816B1D60
    virtual void  Unk20();                        // [20] 0x1816B1D10
    virtual void  Unk21();                        // [21] 0x18072C6FC
    virtual void  Unk22();                        // [22] 0x1818364F0
    virtual void  Unk23();                        // [23] 0x18072C098
    virtual void  Unk24();                        // [24] 0x181797EDC
    virtual void  Unk25();                        // [25] 0x1807275CC
    virtual void  Unk26();                        // [26] 0x1807275E0
    // [27] 0x180EE031C  script-facing entry (Lua soul:DealDamage / actor:TakeDamage handlers land
    // here): packs an S_DealDamageParams (effect payload copied via sub_180EE0098) and forwards
    // to DealDamageInternal.
    virtual void  DealDamage(float stamina, float health, uint64_t source, bool flag10, const void* effect48);
    // [28] 0x1807256D4  guard (victim health > 0), builds the OnScriptedHit descriptor, then
    // dispatches the UI/event record via sub_1809155C8() mgr+0x1A0 vfn+0x528.
    virtual void  DealDamageInternal(const S_DealDamageParams& params);
    virtual void  Unk29();                        // [29] 0x182D289E4
    virtual void  Unk30();                        // [30] 0x18180901C
    virtual void  Unk31();                        // [31] 0x182D27008
    virtual void  Unk32();                        // [32] 0x1817AECAC
    virtual void  Unk33();                        // [33] 0x181772E84
    virtual void  Unk34();                        // [34] 0x1815863C4
    virtual void  Unk35();                        // [35] 0x182D2A4C0
    virtual void  Unk36();                        // [36] 0x181A8A560  shared 8B ret-stub (same fn as [37])
    virtual void  Unk37();                        // [37] 0x181A8A560
    virtual void  Unk38();                        // [38] 0x182D28E90
    virtual void  Unk39();                        // [39] 0x182D28E9C
    virtual void  Unk40();                        // [40] 0x182D29108
    virtual void  Unk41();                        // [41] 0x180D543A4
    // ---- damage-pipeline producers (S_DamageEventData -> S_DamageEventData::Dispatch) ----
    virtual void* OnMeleeHit(void* resultSlot, const void* meleeHitDesc);        // [42] 0x180726DF4  type MeleeHit (fill sub_180727B0C)
    virtual void* OnMissileHit(void* resultSlot, const void* missileHitDesc);    // [43] 0x182D291D0  type MissileHit (missile WUID @desc+96)
    virtual void* OnCollisionHit(void* resultSlot, const void* collisionDesc);   // [44] 0x1816C79F8  type Collision; submits TWO contexts (both participants)
    virtual void* OnScriptedHit(void* resultSlot, const void* scriptedDesc);     // [45] 0x1807258DC  type ScriptedDamage (from DealDamageInternal)
    virtual void  Unk46();                        // [46] 0x180686FDC
    virtual void  Unk47();                        // [47] 0x1806875BC
    virtual void  Unk48();                        // [48] 0x180728C30
    RTTR_ENABLE(I_CombatSoul)  // [49..51]: get_type 0x182D2BFC0, get_derived 0x182D2BF28; base lambda 0x1818A7CDC

    wh::shared::C_Signal<> m_signal08;    // +0x08  purpose unresolved
    wh::shared::C_Signal<> m_signal18;    // +0x18
    wh::shared::C_Signal<> m_signal28;    // +0x28
    wh::shared::C_Signal<> m_signal38;    // +0x38
    wh::shared::C_Signal<> m_signal48;    // +0x48
    wh::shared::C_Signal<> m_signal58;    // +0x58
    wh::shared::C_Signal<> m_signal68;    // +0x68
    C_Soul*  m_pOwner;                    // +0x78  back-ptr
    uint64_t m_unk80[3];                  // +0x80..+0x98  unresolved
    C_CombatSoulModel* m_pModel;          // +0x98  OWNED; factory sub_1803F2814 (160B alloc, sets C_CombatSoulModel vftable); dtor releases via vtbl[0](this,1)
    void*    m_pTreeA0;                   // +0xA0  OWNED heap std::set/map (MSVC std::_Tree, 0x10 {node*,size}); factory sub_18117A950; element value_type ~0x58 bytes, type unresolved
};
static_assert(sizeof(C_CombatSoul) == 0xA8, "C_CombatSoul must be 0xA8");

}  // namespace wh::rpgmodule
