#pragma once
#include <cstdint>
#include "../game/I_EntitySideEffectCallback.h"

// -----------------------------------------------
// wh::xgenaimodule::C_RecognizingThresholdManager : game::I_EntitySideEffectCallback
// -- per-entity recognition-threshold overrides, embedded at +0x198 of
// C_PerceptionManager (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x90 (ctor-proven:
// spans PM+0x198..+0x228).
// -----------------------------------------------
// RTTI TD rva 0x4EDD760; vtable 0x183FD06C0, 2 slots -- both are DISTINCT
// side-effect handlers, NEITHER is a dtor (embedded object, never
// polymorphically deleted; this proved the ESEC interface shape):
// [0] sub_1808ACF2C OnEntitySideEffectAdded: resolves the entity record
// (sub_1808AD98C) and push_backs a threshold float into its vector at rec+64
// (sub_180F993D0 @0x180f993db).  [1] sub_1808AC6F4 OnEntitySideEffectRemoved:
// erase-removes every float == m_recognizingThreshold (PM+0xD0, read
// @0x1808ac720) from rec+64..+72.
// Hash container evidence (init sub_1803B5774, cap 0x10): mask 7 @+0x50, bucket
// count 8 @+0x58 => hashmap<entityId, per-entity recognition threshold>; a
// parallel vector sits @+0x40 -- interior layout not fully resolved [U].

namespace wh::rpgmodule { class C_RPGPerception; }

namespace wh::xgenaimodule {

struct S_PerceptorSensingContext;

class C_RecognizingThresholdManager : public game::I_EntitySideEffectCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RecognizingThresholdManager;
    void OnEntitySideEffectAdded(void* sideEffect) override;    // [0] sub_1808ACF2C -- push per-entity threshold float
    void OnEntitySideEffectRemoved(void* sideEffect) override;  // [1] sub_1808AC6F4 -- drop per-entity thresholds matching PM+0xD0

    uint8_t  _unk08[8];           // +0x08  [U] left uninit by ctor sub_180FB74DC and untouched by PM-init sub_180F661D0; no reader/writer seen from any PM-getter caller -- reserved/unresolved
    wh::rpgmodule::C_RPGPerception* m_rpgPerception;   // +0x10  PM-init sub_180F661D0 @0x180f6642a stores &qword_1855E63B0 = the C_RPGPerception singleton (guarded ctor sub_1819DD880); THE recognition evaluator the FSM calls (eval->vt[6/10/11]); non-owning
    S_PerceptorSensingContext* m_sensingContext;       // +0x18  PM-init @0x180f66314 stores heap 0x160 perceptor sensing aggregate (+0x00 = C_PerceptionManager* back-ref); ticked by sub_18061AFB8; non-owning
    float    m_thresholdScale;    // +0x20  ctor: 1.0f -- per-manager threshold scale [V]
    uint8_t  _pad24[4];           // +0x24
    uint64_t m_listHead[2];       // +0x28  intrusive circular-list head (32B nodes)
    uint8_t  m_entityThresholds[0x58];  // +0x38..+0x8F  hashmap<entityId, threshold> (init sub_1803B5774; mask 7 @+0x50, buckets 8 @+0x58) + vector @+0x40 [U interior]
};
static_assert(sizeof(C_RecognizingThresholdManager) == 0x90, "C_RecognizingThresholdManager must be 0x90 (PM+0x198..+0x228)");

}  // namespace wh::xgenaimodule
