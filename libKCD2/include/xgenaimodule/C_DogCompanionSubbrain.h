#pragma once
#include <cstdint>
#include "C_BehaviorTreeSUBB.h"

// -----------------------------------------------
// wh::xgenaimodule::C_DogCompanionSubbrain : C_BehaviorTreeSUBB -- the dog
// companion's subbrain (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x130 (create-site
// proven: factory sub_18041A4DC operator new(304) -> ctor sub_1819B1DC0).
// -----------------------------------------------
// RTTI TD rva 0x4EFDE78; COL rva 0x40E7660; vtable 0x183B1F100, 40 slots.
// ctor: sub_180413C50(this,0); +0x120=0; +0x128 word=0; registers a state
// listener into base m_listeners via sub_18206DB30({sub_18171D9D0, _, this,
// sub_18041C37C}).  Extra overrides vs BehaviorTreeSUBB: [1]dtor sub_18116C228,
// [5]Init=sub_180BC9B00, [6]Deinit=sub_183208A38, [27]type=sub_181A74260,
// [29]sub_18320A21C, [30]sub_1832098C0, [38]sub_18066CD10, [39]sub_1819B57C8
// (companion event: sets m_companionState, forwards to m_companionTarget via
// sub_18330416C / sub_1819B5818).

namespace wh::xgenaimodule {

namespace activitysystem { class C_DogCompanion; }   // RTTI .?AVC_DogCompanion@activitysystem@xgenaimodule@wh@@

class C_DogCompanionSubbrain : public C_BehaviorTreeSUBB {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DogCompanionSubbrain;
    void  Init(void* owner) override;         // [5]  sub_180BC9B00
    void  Deinit() override;                  // [6]  sub_183208A38
    void* GetSubbrainType() override;         // [27] sub_181A74260
    void  Serialize(void* stream) override;   // [29] sub_18320A21C
    void  Serialize2(void* stream) override;  // [30] sub_1832098C0
    bool  _vf38() override;                   // [38] sub_18066CD10 (return 0)
    void  _vf39() override;                   // [39] sub_1819B57C8 -- companion event

    activitysystem::C_DogCompanion* m_companionTarget;  // +0x120  Init (sub_180BC9B00) allocs a C_DogCompanion (0x1E30) via sub_180BC9BC4 and stores it here; slot [39] forwards state to it
    uint8_t  m_128;               // +0x128  0 (ctor word-write covers 0x128/0x129; dossier's
                                  //         "u16@0x128 + u8@0x129" overlaps -> split as 2 bytes [U])
    bool     m_companionState;    // +0x129  slot [39] sub_1819B57C8 writes only 0/1
    uint8_t  _pad12A[6];          // +0x12A
};
static_assert(sizeof(C_DogCompanionSubbrain) == 0x130, "C_DogCompanionSubbrain must be 0x130 (operator new(304))");

}  // namespace wh::xgenaimodule
