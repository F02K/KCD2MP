#pragma once
#include <cstdint>
#include "I_CrimeLevelAggregator.h"
#include "../game/I_GameSideEffectCallback.h"
#include "../game/I_EntitySideEffectCallback.h"
#include "../rpgmodule/I_LocationListener.h"

// -----------------------------------------------
// wh::xgenaimodule::C_CrimeLevelAggregator -- per-district crime/wanted-level
// accrual, embedded at +0x60 of C_CrimeResolver (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0xA0 (ctor-proven: sub_180E5CF08 spans resolver +0x60..+0x100).
// -----------------------------------------------
// RTTI TD rva 0x4F48310; FOUR vtables written by the ctor:
//   +0x00 primary (I_CrimeLevelAggregator) 0x183ABEA48, 9 slots;
//   +0x08 game::I_GameSideEffectCallback 0x183ABEA18 (2x sub_1832A6414);
//   +0x10 game::I_EntitySideEffectCallback 0x183ABEA30 ([0] sub_181785754 dtor
//         thunk, [1] sub_1817856D0);
//   +0x18 rpgmodule::I_LocationListener 0x183ABEA98, 10 slots ([6] = [9]
//         sub_180B57328, rest nullsub_1).
// This is the parallel wanted-level accrual path: the three mailbox handles feed
// crime events in; ctor tail sub_180E5D080 recomputes the total level and
// notifies via sub_18039B9B8 on change.  All inherited pures are overridden below
// (the class is embedded by value, so this mirror must be concrete).

namespace wh::xgenaimodule {

class C_CrimeLevelAggregator : public I_CrimeLevelAggregator,
                               public game::I_GameSideEffectCallback,
                               public game::I_EntitySideEffectCallback,
                               public rpgmodule::I_LocationListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_CrimeLevelAggregator;
    // I_CrimeLevelAggregator impls (primary vtable 0x183ABEA48)
    int32_t GetEffectiveCrimeLevel() override;             // [0] sub_18082DBFC -- bit-scan m_levelFlags, threshold m_levelThreshold
    void _cla_vf1() override;                              // [1] sub_181AA5650 [U role]
    void _cla_vf2() override;                              // [2] sub_180EC9720 [U role]
    void _cla_vf3() override;                              // [3] sub_180EC9430 [U role]
    void RemoveDistrictEntry(uint64_t districtKey) override;  // [4] sub_180EC7CB8
    void _cla_vf5() override;                              // [5] sub_180EC9408 [U role]
    void _cla_vf6() override;                              // [6] sub_180EC7E3C [U role]
    void _cla_vf7() override;                              // [7] sub_180EC93E0 [U role]
    void _cla_vf8() override;                              // [8] sub_180EC7E64 [U role]
    // game::I_GameSideEffectCallback impls (both -> sub_1832A6414)
    void OnSideEffectDeactivated(char sideEffectId) override;  // GSEC [0] sub_1832A6414
    void OnSideEffectActivated(char sideEffectId) override;    // GSEC [1] sub_1832A6414
    // game::I_EntitySideEffectCallback impls (interface shape corrected: 2 handlers, no vdtor)
    void OnEntitySideEffectAdded(void* sideEffect) override;   // ESEC [0] [U EA -- previously misread as a dtor thunk]
    void OnEntitySideEffectRemoved(void* sideEffect) override; // ESEC [1] sub_1817856D0
    // rpgmodule::I_LocationListener impls (subobject vt 0x183ABEA98)
    void _vf0() override;   // LL [0] nullsub_1
    void _vf1() override;   // LL [1] nullsub_1
    void _vf2() override;   // LL [2] nullsub_1
    void _vf3() override;   // LL [3] nullsub_1
    void _vf4() override;   // LL [4] nullsub_1
    void _vf5() override;   // LL [5] nullsub_1
    void _vf6() override;   // LL [6] sub_180B57328 (same body as [9])
    void OnRegistered(rpgmodule::I_RPGLocationManager* mgr) override;    // LL [7] nullsub_1
    void OnUnregistered(rpgmodule::I_RPGLocationManager* mgr) override;  // LL [8] nullsub_1
    void OnLocationOccupied(rpgmodule::I_RPGLocationManager* mgr, rpgmodule::I_Location* loc) override;  // LL [9] sub_180B57328

    uint8_t  m_mailbox1[0x10];      // +0x20  {descr* = &unk_18566BEF0, i16 = -1, i16 = -1} mailbox handle 1 [U descriptor role]
    uint8_t  m_mailbox2[0x10];      // +0x30  {descr* = &unk_185666D10, -1, -1} mailbox handle 2
    uint8_t  m_mailbox3[0x10];      // +0x40  {descr* = &unk_185665AC0, -1, -1} mailbox handle 3
    uint8_t  m_districtTable[0x40]; // +0x50..+0x8F  per-district crime-level table (init sub_180E5CFC0; stride-16 {u64, u64 districtKey} + parallel i16 counts) [U interior]
    float    m_levelThreshold;      // +0x90  ctor: 0; float threshold read by GetEffectiveCrimeLevel [U exact type]
    uint8_t  m_byte94;              // +0x94  ctor: 0 [U role]
    uint8_t  _pad95[3];             // +0x95
    uint32_t m_levelFlags;          // +0x98  ctor: 1; bit-scanned by GetEffectiveCrimeLevel [V]
    uint8_t  m_byte9C;              // +0x9C  ctor: 0 [U role]
    uint8_t  _pad9D[3];             // +0x9D
};
static_assert(sizeof(C_CrimeLevelAggregator) == 0xA0, "C_CrimeLevelAggregator must be 0xA0 (resolver+0x60..+0x100)");

}  // namespace wh::xgenaimodule
