#pragma once
#include <cstdint>
#include "C_SUBBBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_BehaviorTreeSUBB : C_SUBBBase -- subbrain executing a
// behavior tree (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x120 (create-site
// proven: factory sub_18041A4DC "BehaviorTree" branch operator new(288)).
// -----------------------------------------------
// RTTI TD rva 0x4EFD7B0; COL rva 0x40E7638; vtable 0x183A34270, 40 slots --
// concrete (fills all 5 purecalls) and ADDS 3 new slots [37..39].
// ctor sub_180413C50(this, bool a2) (a2 -> m_btFlags bit1; Dialog passes 1).
// Overrides vs base: [1]dtor, [7]SetGoals, [17], [18], [22]STEP=sub_180760A44
// (switches on m_state), [23]sub_18041A688, [24]Configure=sub_1804175F0,
// [27]type, [28], [29]Serialize, [30], [35], [36].

namespace wh::xgenaimodule {

namespace BehaviorTree { class C_BehaviorTree; }   // RTTI .?AVC_BehaviorTree@BehaviorTree@xgenaimodule@wh@@ (Warhorse BT, not CryCommon ::BehaviorTree)

class C_BehaviorTreeSUBB : public C_SUBBBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_BehaviorTreeSUBB;
    void  Step() override;                    // [22] sub_180760A44 -- per-state STEP processor
    bool  IsRunning() override;               // [23] sub_18041A688
    void  Configure(void* desc) override;     // [24] sub_1804175F0
    void* GetSubbrainType() override;         // [27]
    void  Serialize(void* stream) override;   // [29]
    void  RequestOwnerTick() override;        // [35]
    void  CancelOwnerTick() override;         // [36]
    virtual void* GetTreeName();              // [37] NEW sub_18206EDB0 -> sub_18205B420(this+0xE0) [U name]
    virtual bool  _vf38();                    // [38] NEW sub_181A72470 (return 1) [U role]
    virtual void  _vf39();                    // [39] NEW nullsub_1 [U role]

    CryStringT<char> m_btName1;   // +0xC8  refcounted string (ctor = shared empty static; Configure copies from desc+0x48)
    CryStringT<char> m_btName2;   // +0xD0  refcounted string (ctor = shared empty static; Configure copies from desc+0x50)
    BehaviorTree::C_BehaviorTree* m_behaviorTree;  // +0xD8  BT instance (setter sub_180412884; created via sub_1804176D8)
    uint32_t m_e0;             // +0xE0  init 2 [U role -- also read by [37]]
    uint8_t  _padE4[4];        // +0xE4
    void*    m_btRuntime[5];   // +0xE8..+0x110  runtime/exec ptrs (0) [U roles]
    void*    m_unk110;         // +0x110  runtime slot; ctor 0x180413CC4 writes 0 [U role]
    uint8_t  m_btFlags;        // +0x118  (a2&0xFD)|..; bit1 = ctor arg (Dialog=1)
    uint8_t  _pad119[7];       // +0x119
};
static_assert(sizeof(C_BehaviorTreeSUBB) == 0x120, "C_BehaviorTreeSUBB must be 0x120 (operator new(288))");

}  // namespace wh::xgenaimodule
