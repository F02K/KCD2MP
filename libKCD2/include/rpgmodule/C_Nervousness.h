#pragma once
#include <cstdint>
#include "I_Negativity.h"

// -----------------------------------------------
// wh::rpgmodule::C_Nervousness : I_Negativity -- per-NPC nervousness value
// (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x28 (embedded at C_NPCFactionNode+0x90,
// node end 0xB8; init inlined in the node make_shared ctor sub_1803F21DC
// @0x1803f224e..0x1803f2270).
// -----------------------------------------------
// RTTI TD rva 0x4D4E230; COL 0x18412A438; vtable 0x3A33478 (11 slots 0..10 --
// ends at the C_DebugReputationLog COL).  Slot impls:
//  [0] sub_180F857B8 deleting dtor  [1] sub_182CA9474 thunk->[4]
//  [2] sub_180474F08 composite ([8]+[5])  [3] sub_1803F3910 SetValue
//  [4] sub_1814A6084 Reset (lastUpdate=now; value=Add(0); opt [10])
//  [5] sub_180474FB8 Add = clamp(value+delta, NervousnessMinimum 0xB60,
//      NervousnessMaximum 0xB64)  [6] sub_182CC7D6C ClampAdd math helper
//  [7] sub_180474FA0 GetDecayRate = NervousnessDecayRate 0xB6C
//  [8] sub_180474F40 ResolveOwner (game ctx sub_1809155C8)
//  [9] sub_180474F70 Decay = rate*(now-lastUpdate)*0.001
//  [10] sub_1814A6100 Propagate (NervousnessPropagationCoef 0xB70, spatial
//       query, spreads to nearby faction nodes)
// Consumed as soul property 100 ("DerivStat_Nervousness") by the perception
// inner-cone worker (C_RPGPerception slot 26); raised by the BT AffectFeelings
// ("AffectNervousness") node and every hearing-subscription event.

namespace wh::rpgmodule {

class C_NPCFactionNode;

class C_Nervousness : public I_Negativity {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_Nervousness;
    ~C_Nervousness() override;                          // [0] sub_180F857B8
    void  ResetThunk() override;                        // [1] sub_182CA9474
    float UpdateAndGet() override;                      // [2] sub_180474F08
    void  SetValue(float v) override;                   // [3] sub_1803F3910
    void  Reset(void* a2, bool propagate) override;     // [4] sub_1814A6084
    float Add(float delta) override;                    // [5] sub_180474FB8
    float ClampAdd(float a, float b) override;          // [6] sub_182CC7D6C
    float GetDecayRate() override;                      // [7] sub_180474FA0
    void* ResolveOwner() override;                      // [8] sub_180474F40
    float Decay(uint64_t now) override;                 // [9] sub_180474F70
    void  Propagate(void* a2) override;                 // [10] sub_1814A6100

    float              m_value;       // +0x08  current nervousness (init NervousnessMinimum @0x1803f2258)
    uint32_t           _pad0C;        // +0x0C
    uint64_t           m_lastUpdate;  // +0x10  world-time of last update (Decay reads; SetValue zeroes)
    C_NPCFactionNode*  m_owner;       // +0x18  back-ptr to enclosing faction node (@0x1803f2269)
    int32_t            m_flag20;      // +0x20  init 0 [role UNVERIFIED -- no non-ctor writer found]
    uint32_t           _pad24;        // +0x24
};
static_assert(sizeof(C_Nervousness) == 0x28, "C_Nervousness must be 0x28 (node+0x90..+0xB8)");

}  // namespace wh::rpgmodule
