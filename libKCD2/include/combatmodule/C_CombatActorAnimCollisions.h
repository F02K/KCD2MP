#pragma once
#include <cstddef>
#include <cstdint>
#include "C_CombatActorUpdatedObject.h"
#include "S_CombatAnimCollision.h"
#include "../framework/C_Signal.h"

// -----------------------------------------------
// wh::combatmodule::C_CombatActorAnimCollisions : C_CombatActorUpdatedObject
// (KCD2 WHGame.dll 1.5.6). sizeof 0x40.
// -----------------------------------------------
// RTTI verifies the sole base. Ctor 0x180917708 registers update phase 1. Reset clears m_enabled
// and m_hitIndex; UpdatePhase1 emits a deferred collision once the owner timer permits it. The
// signal payload type is corroborated by KCD1 template RTTI and the KCD2 producer/consumer layout.

namespace wh::combatmodule {

class C_CombatActorAnimCollisions : public C_CombatActorUpdatedObject {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CombatActorAnimCollisions;
    ~C_CombatActorAnimCollisions() override = default;
    void Reset() override;                                                     // 0x18277F9D4
    E_CombatSubsystem GetSubsystemId() const override { return static_cast<E_CombatSubsystem>(16); }
    const char* GetName() const override { return "ANIM_COLLISION"; }
    void UpdatePhase1() override;                                              // 0x180D53094

    wh::shared::C_Signal<const S_CombatAnimCollision&> m_onCollision;          // +0x18
    bool        m_enabled;                                                     // +0x28
    bool        m_deferredCollisionPending;                                    // +0x29
    bool        m_unk2A;                                                       // +0x2A
    uint8_t     _pad2B;                                                        // +0x2B
    int32_t     m_hitIndex;                                                    // +0x2C
    uint32_t    m_unk30;                                                       // +0x30
    uint32_t    _pad34;                                                        // +0x34
    void*       m_pCollisionQuery;                                             // +0x38  concrete interface type unverified
};
static_assert(sizeof(C_CombatActorAnimCollisions) == 0x40,
              "C_CombatActorAnimCollisions must be 0x40");
static_assert(offsetof(C_CombatActorAnimCollisions, m_onCollision) == 0x18);
static_assert(offsetof(C_CombatActorAnimCollisions, m_enabled) == 0x28);
static_assert(offsetof(C_CombatActorAnimCollisions, m_hitIndex) == 0x2C);
static_assert(offsetof(C_CombatActorAnimCollisions, m_pCollisionQuery) == 0x38);

}  // namespace wh::combatmodule
