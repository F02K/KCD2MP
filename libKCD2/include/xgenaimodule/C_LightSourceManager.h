#pragma once
#include <cstdint>
#include "../game/I_EntitySideEffectCallback.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LightSourceManager : game::I_EntitySideEffectCallback --
// light-source (torch etc.) tracking SINGLETON at qword_1854927C8 (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0xE0 (getter sub_1810A6150 @0x1810a61a0:
// sub_18549D378(224,...), += 0xE0, ctor sub_1810A6D98).  dtor sub_1833BD8F0
// (unregisters the cvar via console vtbl+80).
// -----------------------------------------------
// RTTI TD rva 0x4FC69C0.  Vtable rva 0x3B03488 -- 3 slots ([0] dtor
// sub_181025124, [1]sub_1810250D8, [2]sub_1833BDA70): one MORE than the
// 2-slot iface -> [2] modeled as an appended own virtual [U].  ctor interns
// name-ids "interior"/"settlement"/"city" (+0x08/+0x0C/+0x10) and binds cvar
// wh_ai_TorchDropBeforeMoveEndDistance to +0x14 (sub_1823D8ABC).  Container
// regions typed as blobs [U interior].

namespace wh::xgenaimodule {

class C_LightSourceManager : public game::I_EntitySideEffectCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LightSourceManager;
    void OnEntitySideEffectAdded(void* sideEffect) override;    // [0] sub_181025124 (previously misread as dtor; ESEC has no vdtor -- RTM proof)
    void OnEntitySideEffectRemoved(void* sideEffect) override;  // [1] sub_1810250D8
    virtual void _vf2();                                  // [2] sub_1833BDA70 (appended own virtual) [U role]

    uint32_t m_nameIdInterior;    // +0x08  interned "interior"
    uint32_t m_nameIdSettlement;  // +0x0C  interned "settlement"
    uint32_t m_nameIdCity;        // +0x10  interned "city"
    float    m_cvarTorchDropDist; // +0x14  cvar wh_ai_TorchDropBeforeMoveEndDistance target
    bool     m_active;            // +0x18  gate flag: set 1 @0x1820C3478, read as bool in sub_181025410/sub_18073765C/sub_18090427C
    uint8_t  m_cachedState;       // +0x19  caches (uint8)sub_180653344() @0x1820C347C; compared @0x1820C3471 to re-run subscriber loop [U role]
    uint8_t  _pad1A[6];           // +0x1A  align pad (never written)
    float    m_hashMaxLoadFactor; // +0x20  std::unordered_set _Max_bucket_size (1.0f); the hash container REALLY begins here, not +0x28 (see note)
    uint8_t  _pad24[4];           // +0x24  STL _Traitsobj align pad before list head @+0x28
    uint8_t  m_hashSet28[0x38];   // +0x28..+0x60  WH hash-set (mask 7 @+0x50, count 8 @+0x58) [U interior]
    uint8_t  m_sub60[0x40];       // +0x60..+0xA0  container (sub_18047937C) [U interior]
    uint8_t  m_subA0[0x40];       // +0xA0..+0xE0  container (sub_18047937C) [U interior]
};
static_assert(sizeof(C_LightSourceManager) == 0xE0, "must be 0xE0 (alloc 224 at sub_1810A6150)");

}  // namespace wh::xgenaimodule
