#pragma once
#include <cstdint>
#include <vector>
#include "I_ObjectDatabase.h"

// ===========================================================================
// wh::databasemodule::C_ObjectDatabaseBase -- typed-table container base
// (KCD2 WHGame.dll 1.5.6, kd7u).
// ===========================================================================
// RTTI .?AVC_ObjectDatabaseBase@databasemodule@wh@@ (TD 0x184AF0B38)  vtable 0x183E3D888, 21 slots.
// Base extent 0x30 (ctor sub_180EF4F30(this, int type) writes +0x08 and +0x18..+0x28; +0x10 is
// NOT initialized by any ctor in the chain).
// The ctor SELF-REGISTERS into C_ObjectDatabaseManager::GetInstance() (accessor sub_180EF4F8C,
// TLS-guarded static @0x1854A49A0), pushing `this` into the manager's database vector (+0x48).
//
// [FUNDAMENTAL vs KCD1] The KCD1 base (0x90, two C_Signals, vtable overwritten by the loader
// post-load) is GONE. KCD2 databases stay live objects; concrete tables are GLOBAL STATIC
// instances constructed at static-init (e.g. C_ReputationChangeDatabase @0x18532F700), and the
// row storage lives in the C_ObjectDatabase<T> layer (+0x30 vector).
//
// Slot map (base defaults; overrides observed on C_ReputationChangeDatabase 0x183F18B10):
//   [0]  dtor            [1]  GetVersion = 1        [2]  GetTableName (pure here)
//   [3]  nullsub         [4]  GetRowCount (pure; I_DynamicEnum[0] forwards to it)
//   [5]  nullsub         [6]  IsLoaded (pure; concrete: byte @+0x94)
//   [7]  Register (pure; concrete 0x1819434E0: I_DatabaseModule slot[7] register + slot[12]
//        GetTableMetadata(name, rowStride) -> handle stored @+0x48)
//   [8]  ret stub        [9]  Load (base returns 3; concrete 0x180EF3A4C: module load-by-name,
//        verify handle type @handle+0x38 vs m_type, call the [17] hook, set +0x94)
//   [10] ParseRows (pure; C_PerkDatabase 0x18115C994: clear the +0x30 vector, module-load if
//        handle flags bit3, convert every raw table row into a parsed row, then [16])
//   [11] nullsub         [12] ret stub / push a static descriptor into an out-vector (0x180EF5154)
//   [13] nullsub (concrete 0x181FA8CE0)             [14]/[15] ret stubs
//   [16] RebuildIndices (C_PerkDatabase 0x18115CB90, repdb 0x1813345B0)
//   [17] post-load hook -- ret stub in both observed concretes
//                                                   [18] invalid-id out-param (0x182744920)
//   [19] returns this (0x1805F5DA0)                 [20] {this, invalid-id} pair out (0x181207C24)
// Purecall stub = 0x181D93E5D; ret stubs 0x1803B6E80 / 0x180838AE0. Names for [7]/[9]/[17] come
// from the concrete implementations; the pure/no-op leaves are [INFERRED-thin].

namespace wh::databasemodule {

class C_ObjectDatabaseBase : public I_ObjectDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ObjectDatabaseBase;
    int32_t  m_type;           // +0x08  ctor arg (C_ReputationChangeDatabase passes 1); Load
                               //        verifies it against the table handle's type dword
    uint32_t _pad0C;           // +0x0C
    uint64_t m_unk10;          // +0x10  VERIFIED uninit: ctor sub_180EF4F30 writes +0x08 (m_type) then vec +0x18/+0x20/+0x28, skips +0x0C/+0x10; Load 0x180EF3A4C never touches it; no writer found -- type/role unresolved
    std::vector<void*> m_vec18;// +0x18  ctor zeroes begin/end/cap; element type UNRESOLVED
                               //        (listener/pending list candidate)
};
static_assert(sizeof(C_ObjectDatabaseBase) == 0x30, "C_ObjectDatabaseBase must be 0x30");

}  // namespace wh::databasemodule
