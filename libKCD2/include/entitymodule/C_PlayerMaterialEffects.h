#pragma once
#include <cstdint>
#include <vector>
#include <functional>
#include <mutex>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::entitymodule::C_ActorMaterialEffects (0xB0) / C_PlayerMaterialEffects (0xB8) -- KCD2 1.5.6 (kd7u).
// -----------------------------------------------
// Footstep/surface FX state. C_PlayerMaterialEffects : C_ActorMaterialEffects adds only the owner
// back-pointer (embedded in C_Player at +0xBF0; ctor sub_180BC870C -> base sub_180BC89CC; player
// vtable 0x183A81678: [1] IsEnabled, [3] GetFxGroupName -> "player_fx").
//
// [CORRECTION vs prior recon] The old "m_transform[0x30] Matrix34 hypothesis" @+0x48 is REFUTED:
// that region is the interior of an embedded std::mutex spanning +0x30..+0x80 (type field @+0x30 = 2,
// SRWLOCK/critsec storage @+0x48, owner-thread @+0x78 = -1, recursion-count @+0x7C = 0; only
// _Mtx_lock/_Mtx_unlock touch it). SMfxInstance is now fully tiled (uint16 effectId + a nullary
// std::function returning the effect's live params).

namespace wh::entitymodule {

class C_Player;

// The 12-byte struct the SMfxInstance callback returns by value (via hidden sret). The three fields
// are pushed to the effect's SetCustomParameter as "Intensity"/"Param1"/"Param2"; float-vs-int is
// MED (decompiler saw int; CryEngine MFX custom params are float).
struct S_MfxParams {
    float Intensity;   // +0x00
    float Param1;      // +0x04
    float Param2;      // +0x08
};
static_assert(sizeof(S_MfxParams) == 0xC, "S_MfxParams must be 0xC");

// One material-FX instance (0x48; stride proven by range-dtor sub_182857308). effectId + a nullary
// std::function<S_MfxParams()> providing the effect's current params (MSVC 0x40 std::function, inner
// _Ptr at elem+0x40).
struct SMfxInstance {
    uint16_t effectId;                       // +0x00  TMFXEffectId (material-effect id/handle)
    uint8_t  _pad02[6];                      // +0x02  alignment for the std::function
    std::function<S_MfxParams()> paramProvider;  // +0x08  (0x40) live-params callback
};
static_assert(sizeof(SMfxInstance) == 0x48, "SMfxInstance must be 0x48");
static_assert(offsetof(SMfxInstance, paramProvider) == 0x08, "paramProvider at 0x08");

class C_ActorMaterialEffects {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ActorMaterialEffects;
    virtual ~C_ActorMaterialEffects() = default;               // [0]
    virtual bool IsEnabled() const { return m_enabled; }       // [1]  sub_181A7DE10: *(u8*)(this+8)
    virtual void _vf2() { m_flag2 = false; }                   // [2]  sub_181A81F40 (clears +0x09)
    virtual const char* GetFxGroupName() const { return ""; }  // [3]  player override returns "player_fx"

    bool     m_enabled;                   // +0x08
    bool     m_flag2;                     // +0x09  "all-active" toggle (read in flush)
    uint8_t  _pad0A[6];                   // +0x0A
    CryStringT<char> m_name;              // +0x10  (empty-string sentinel at construct; fx-group/material name)
    std::vector<SMfxInstance> m_active;   // +0x18  active FX instances
    std::mutex m_lock;                    // +0x30  (0x50; guards the queues)
    std::vector<SMfxInstance> m_pendingStart;  // +0x80  queued starts
    std::vector<uint16_t> m_pendingStop;       // +0x98  queued stop effect-ids (2-byte elements)
};
static_assert(sizeof(C_ActorMaterialEffects) == 0xB0, "C_ActorMaterialEffects must be 0xB0");
static_assert(offsetof(C_ActorMaterialEffects, m_lock) == 0x30, "mutex at 0x30");
static_assert(offsetof(C_ActorMaterialEffects, m_pendingStart) == 0x80, "pending-start at 0x80");

class C_PlayerMaterialEffects : public C_ActorMaterialEffects {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_PlayerMaterialEffects;
    const char* GetFxGroupName() const override { return "player_fx"; }   // [3] sub_182AA7374

    C_Player* m_pOwner;                  // +0xB0  back-ptr (abs C_Player+0xCA0)
};
static_assert(sizeof(C_PlayerMaterialEffects) == 0xB8, "C_PlayerMaterialEffects must be 0xB8");

}  // namespace wh::entitymodule
