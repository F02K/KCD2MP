#pragma once
#include <cstdint>
#include "C_SmartEntitySubbrain.h"
#include "I_SmartEntityListener.h"
#include "S_VariableReference.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartObjectSubBrain : C_SmartEntitySubbrain +
// I_SmartEntityListener (@+0xD0) -- smart-object subbrain (KCD2 WHGame.dll
// 1.5.6, kd7u).  sizeof 0x288 (create-site proven: factory sub_18041A4DC
// "SmartObject" branch operator new(648) -> ctor sub_180413E88).
// -----------------------------------------------
// RTTI TD rva 0x4EFF290.  Primary vtable rva 0x3FD8260 (37 slots, COL 0x451F6B8);
// I_SmartEntityListener subobject vtable rva 0x3FD8208 (10 slots, COL 0x451F690).
// ctor: SmartEntity ctor sub_180414120, 2 vptrs; (atom+3,atom+3,0) id-triples
// @+0xE0..+0x138; self-linked list node new(64) @+0x168; two S_VariableReference
// (0x60) @+0x1B0/+0x210; sub_1803C4794(+0x270); +0x280 = 2; registers a state
// listener into base m_listeners.  Primary overrides: [0]/[2] cast thunks
// sub_1811D1280/8, [1]dtor, [17], [18], [22]Step=sub_1807F9544, [23],
// [24]Configure, [27]type, [28], [29]Serialize, [30], [31] (live), [35], [36].
// Interior typing of the id-triple block / list node element [U].

namespace wh::xgenaimodule {

class C_SmartObjectSubBrain
    : public C_SmartEntitySubbrain,
      public I_SmartEntityListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartObjectSubBrain;
    void* TypeQuery0() override;            // [0]  cast thunk sub_1811D1280
    void* CastQuery2() override;            // [2]  cast thunk sub_1811D1288
    void  Step() override;                  // [22] sub_1807F9544
    bool  IsRunning() override;             // [23]
    void* GetSubbrainType() override;       // [27]
    void  Serialize(void* stream) override; // [29]
    void  _vf31() override;                 // [31] live here (base return-1 stub)
    void  RequestOwnerTick() override;      // [35]
    void  CancelOwnerTick() override;       // [36]
    // I_SmartEntityListener impls [U roles]
    void  SelVf1() override; void SelVf2() override; void SelVf3() override;
    void  SelVf4() override; void SelVf5() override; void SelVf6() override;
    void  SelVf7() override; void SelVf8() override; void SelVf9() override;

    uint8_t             _unkD8[8];        // +0xD8  [U]
    uint8_t             m_idTriples[0x88]; // +0xE0..+0x168  (atom+3,atom+3,0) id-triples [U types]
    void*               m_requestQueueHead; // +0x168  std::list<> _Myhead: self-linked 64-byte sentinel (operator new(64)); element @node+16; size @+0x170
    uint8_t             _unk170[0x40];    // +0x170..+0x1B0  [U]
    S_VariableReference m_varRefA;        // +0x1B0  embedded (0x60)
    S_VariableReference m_varRefB;        // +0x210  embedded (0x60)
    uint8_t             m_embedded270[0x10]; // +0x270..+0x280  (sub_1803C4794) [U interior]
    uint32_t            m_280;            // +0x280  ctor: 2 [U role]
    uint8_t             _pad284[4];       // +0x284
};
static_assert(sizeof(C_SmartObjectSubBrain) == 0x288, "C_SmartObjectSubBrain must be 0x288 (operator new(648))");

}  // namespace wh::xgenaimodule
