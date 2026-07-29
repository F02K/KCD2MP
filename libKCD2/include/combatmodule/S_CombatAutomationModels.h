#pragma once
#include <cstdint>
#include "../framework/C_Signal.h"
#include "../framework/C_ModelProperty.h"
#include "../framework/ModelPropertyTraits.h"
#include "CombatModelTraits.h"

// -----------------------------------------------
// wh::combatmodule::S_CombatAutomationModels -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x1B0.
// -----------------------------------------------
// NAME IS SYNTHETIC (block is non-polymorphic, no RTTI; no "CombatAutomationModels" string exists).
// Heap-allocated in the C_CombatAutomation ctor (alloc 432, built by sub_1810EEA54(block, actor);
// pointer stored at C_CombatAutomation+0x48; reset helper sub_1810EDFB0). Holds the automation's
// data models: three bespoke SIGNAL-FIRST inline models (C_Signal at offset 0; broadcast helper
// sub_180393524 is called with the model base), a config/state cluster, and four full
// C_ModelProperty sub-objects whose template args come verbatim from their RTTI vtable names.

namespace wh::combatmodule {

class I_CombatActor;
namespace E_ApproachingMode { enum Type : uint8_t; }   // (values not recovered)

// Battlement configuration value (8B; reset writes {0, -1}). Field names tentative.
struct S_BattlementConfiguration {
    int32_t a;   // +0x00  init 0
    int32_t b;   // +0x04  init -1
};
static_assert(sizeof(S_BattlementConfiguration) == 8, "S_BattlementConfiguration must be 8");

// One lazily-allocated change-tracking record (24B, array of 3 hung off m_pChangeTracking;
// settled + freed by sub_1810EE200). Field roles inferred from the settle logic.
struct S_ChangeRecord {
    uint64_t current;    // +0x00
    uint64_t previous;   // +0x08  settle copies current -> previous
    uint64_t _u10;       // +0x10
};

struct S_CombatAutomationModels {
    // --- model A: signal-first, 4 slot handles + trailing ids (bespoke; semantics unresolved) ---
    wh::shared::C_Signal<> m_modelAChanged;    // +0x000  (sentinel unk_185665AC0)
    uint8_t  m_modelAPendingNotify;            // +0x010  if set, reset clears + broadcasts
    uint8_t  _pad11[7];                        // +0x011
    void*    m_modelASlots[4];                 // +0x018  inline array of 4x raw 8B handles; builder sub_1810EEA54 default-constructs the 4 elems via sub_18050B734(base,8,4,sub_1803F1EB0=*p=0); reset sub_1810EDFB0 clears each with a bare store (NOT smart ptrs)
    uint64_t m_modelA_h38;                     // +0x038  cleared by reset
    uint64_t m_modelA_h40;                     // +0x040  PERSISTENT (survives reset)
    uint64_t m_modelA_h48;                     // +0x048  cleared by reset
    uint64_t m_modelA_h50;                     // +0x050  cleared by reset
    // --- model B: signal-first {flags, float} ---
    wh::shared::C_Signal<> m_modelBChanged;    // +0x058
    uint8_t  m_modelBPendingNotify;            // +0x068
    bool     m_modelBFlag1;                    // +0x069  default 1
    bool     m_modelBFlag2;                    // +0x06A  default 0
    uint8_t  _pad6B;                           // +0x06B
    float    m_modelBValue;                    // +0x06C  default -1.0 (range/distance-with-validity candidate)
    // --- model C: signal-first pointer/handle ---
    wh::shared::C_Signal<> m_modelCChanged;    // +0x070
    uint8_t  m_modelCPendingNotify;            // +0x080
    uint8_t  _pad81[7];                        // +0x081
    uint64_t m_modelCValue;                    // +0x088  ptr/handle (target type unresolved)
    // --- int model property (RTTI: C_ModelProperty<int, CustomDefault, NewValue<int>, ...>) ---
    wh::shared::C_ModelProperty<int, wh::shared::traits::C_CustomDefaultValueTrait<int>>
             m_intProp;                        // +0x090  value @+0x98 (default 0)
    // --- config/state cluster (real fields; names/enums unresolved) ---
    int32_t  m_cfgA;                           // +0x0C0  default 3
    int32_t  m_cfgB;                           // +0x0C4  default 3
    int32_t  m_cfgC;                           // +0x0C8  default 3
    uint8_t  m_cfgD;                           // +0x0CC  default 4
    uint8_t  _padCD[3];                        // +0x0CD
    wh::shared::C_Signal<> m_cfgModelChanged;  // +0x0D0  (sentinel unk_1855D4D40)
    uint8_t  m_cfgModelState;                  // +0x0E0  default 4; broadcasts m_cfgModelChanged on change (real state, value set UNRECOVERED)
    uint8_t  _padE1[3];                        // +0x0E1
    int32_t  m_cfgE;                           // +0x0E4  default -1
    int32_t  m_cfgF;                           // +0x0E8  default -1
    int32_t  m_cfgG;                           // +0x0EC  default 0
    int32_t  m_cfgH;                           // +0x0F0
    int32_t  m_cfgI;                           // +0x0F4
    int32_t  m_cfgJ;                           // +0x0F8
    int32_t  m_cfgK;                           // +0x0FC
    int32_t  m_cfgL;                           // +0x100
    int32_t  _unset104;                        // +0x104  never written (malloc garbage)
    uint64_t m_cfgM;                           // +0x108  ptr/handle (default 0)
    // --- target / approach / battlement model properties (RTTI-named templates) ---
    wh::shared::C_ModelProperty<I_CombatActor*>                  m_targetProp;      // +0x110  automation target
    wh::shared::C_ModelProperty<E_ApproachingMode::Type,
        wh::shared::traits::C_StaticDefaultValueTrait<E_ApproachingMode::Type, (E_ApproachingMode::Type)0>>
                                                                 m_approachProp;    // +0x140
    wh::shared::C_ModelProperty<S_BattlementConfiguration>       m_battlementProp;  // +0x170
    // --- tail ---
    S_ChangeRecord* m_pChangeTracking;         // +0x1A0  lazy heap array of 3 (freed when all settled)
    I_CombatActor*  m_pOwnerActor;             // +0x1A8  = ctor arg
};
static_assert(sizeof(S_CombatAutomationModels) == 0x1B0, "S_CombatAutomationModels must be 0x1B0");

}  // namespace wh::combatmodule
