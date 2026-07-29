#pragma once
#include <cstdint>
#include "I_SEBehaviorConditionManager.h"
#include "../game/I_EntitySideEffectCallback.h"
#include "C_SEConditionDatabase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SituationManager : I_SEBehaviorConditionManager (@+0x0) +
// wh::game::I_EntitySideEffectCallback (@+0x8) -- global situation manager
// SINGLETON at qword_185493960 (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x120
// (create-site proven: getter sub_1807D2460: qword_18549D378(288,&v5),
// dword_1851DFB18 += 0x120, ctor sub_1806010EC(result)).
// -----------------------------------------------
// RTTI TD rva 0x4F4E308; COLs 0x4551330 (@+0) / 0x4551308 (@+8).  Primary
// vtable rva 0x3FE8708 (2 slots: [0]sub_1832B98D8 [1]dtor sub_1832B760C);
// secondary vtable rva 0x3FE86F0 (2 slots: [0]dtor thunk sub_1815FFE3C
// [1]sub_1815FFE30).  ctor registers cvars wh_ai_SituationInterruptPriority
// (+0x108, default 10), wh_ai_SituationInterruptUrgency (+0x110, default
// "Slow"), wh_ai_SituationManagerSearchBudget (+0x11C); inits the registry at
// +0x38 (sub_1805FFC9C); intrusive-list sentinel at +0xC8; registers a
// side-effect callback via qword_185494490 vtbl+256 (functor sub_1805F5DA0).

namespace wh::xgenaimodule {

class C_SituationManager
    : public I_SEBehaviorConditionManager,
      public game::I_EntitySideEffectCallback {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SituationManager;
    void BcmVf0() override;                        // primary [0] sub_1832B98D8
    ~C_SituationManager() override;                // primary [1] sub_1832B760C
    void OnEntitySideEffectAdded(void* sideEffect) override;    // ESEC secondary [0] sub_1815FFE3C (adjustor thunk; previously misread as dtor thunk)
    void OnEntitySideEffectRemoved(void* sideEffect) override;  // ESEC secondary [1] sub_1815FFE30

    std::vector<std::array<uint8_t, 40>> m_ctrlEntries10; // +0x10..+0x28  first/last/cap; 40B elems {u64 key; _8; std::vector<T*>@+0x10}; per-elem dtor sub_1811921D0
    uint8_t  _unk28[8];               // +0x28  [U] NOT written by ctor (disasm-verified); no writer in ctor/dtor/add(sub_180D9BC5C)/remove(sub_180D9AEC8)
    int32_t  m_30;                    // +0x30  ctor: 0 [U role]
    uint8_t  _pad34[4];               // +0x34
    C_SEConditionDatabase m_seConditionDb; // +0x38..+0xA0  embedded situation-condition DB
                                      //   (ctor sub_1806010EC calls sub_1805FFC9C(this+0x38); G6I)
    uint8_t  m_zeroA0[0x10];          // +0xA0..+0xB0  [U] (+0xA0 = lazy hash-map ptr, alloc'd in BcmVf0 sub_1832B98D8; +0xA8 [U])
    std::vector<std::pair<uint64_t, void*>> m_sortedByKeyB0; // +0xB0..+0xC8  first/last/cap; sorted flat-map {u64 key, T* val} (16B elems); bin-search insert sub_180D9BC5C / erase sub_180D9AEC8
    std::map<uint64_t, void*> m_treeByKeyC8; // +0xC8..+0xD8  std::_Tree: head node@+0xC8, size@+0xD0; 48B nodes (K/V sizes 8/8 inferred; map-vs-set unconfirmed)
    uint8_t  m_zeroD8[0x20];          // +0xD8..+0xF8  zeroed ptrs [U roles]
    uint64_t m_hashCountF8;           // +0xF8  element count of custom hash container at +0xD8 (iter end = *(+0xF0)+this; sub_180D9CA04 / insert sub_182089950)
    int32_t  m_100;                   // +0x100 [U role]
    int16_t  m_104;                   // +0x104 [U role]
    uint8_t  _pad106[2];              // +0x106
    int32_t  m_cvarInterruptPriority; // +0x108 cvar wh_ai_SituationInterruptPriority (default 10)
    uint8_t  _pad10C[4];              // +0x10C
    char*    m_cvarInterruptUrgency;  // +0x110 cvar wh_ai_SituationInterruptUrgency (default "Slow")
    int8_t   m_priorityByte;          // +0x118 clamped from +0x108 (if <= 0xFF)
    uint8_t  _pad119[3];              // +0x119
    int32_t  m_cvarSearchBudget;      // +0x11C cvar wh_ai_SituationManagerSearchBudget
};
static_assert(sizeof(C_SituationManager) == 0x120, "C_SituationManager must be 0x120 (alloc 288 at sub_1807D2460)");

}  // namespace wh::xgenaimodule
