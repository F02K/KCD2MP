#pragma once
#include <cstdint>
#include <list>
#include "C_SmartEntitySubbrain.h"
#include "I_SmartAreaListener.h"
#include "S_VariableReference.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SmartAreaSubBrain : C_SmartEntitySubbrain +
// I_SmartAreaListener (@+0xD0; the subobject covers BOTH I_SmartAreaListener and
// I_SmartEntityListener -- both at mdisp 0xD0 in RTTI, modeled as derivation
// I_SmartAreaListener : I_SmartEntityListener [U]) -- smart-area subbrain (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x2A0 (create-site proven: factory
// sub_18041A4DC "SmartArea" branch @0x18041A62F operator new(672) -> ctor
// sub_18148BF08).
// -----------------------------------------------
// RTTI TD rva 0x4EFF478.  Primary vtable rva 0x3FD7E88 (37 slots, COL 0x451F140);
// combined listener subobject vtable rva 0x3FD7FB8 (12 slots, COL 0x451F4D8).
// ctor: SmartEntity ctor + 2 vptrs; five (atom+3,atom+3,0) id-triples @+0xE0..;
// list node new(64) @+0x180; two S_VariableReference @+0x1C8/+0x228;
// sub_1803C4794(+0x288); +0x298 = 2; if (base flag & 1) SetState(16).
// Primary overrides: [0]/[2] cast thunks sub_1811D12B0/8, [1]dtor, [5]Init,
// [17], [18], [22]Step=sub_1804120A4, [23], [24]Configure, [27]type, [28],
// [29]Serialize, [30], [31], [35], [36].  Interior typing [U].

namespace wh::xgenaimodule {

struct S_SmartAreaEventEntry;   // [SYNTHETIC] 0x40 list node payload of m_eventQueue

class C_SmartAreaSubBrain
    : public C_SmartEntitySubbrain,
      public I_SmartAreaListener {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SmartAreaSubBrain;
    void* TypeQuery0() override;            // [0]  cast thunk sub_1811D12B0
    void* CastQuery2() override;            // [2]  cast thunk sub_1811D12B8
    void  Init(void* owner) override;       // [5]
    void  Step() override;                  // [22] sub_1804120A4
    bool  IsRunning() override;             // [23]
    void* GetSubbrainType() override;       // [27]
    void  Serialize(void* stream) override; // [29]
    void  _vf31() override;                 // [31]
    void  RequestOwnerTick() override;      // [35]
    void  CancelOwnerTick() override;       // [36]
    // combined listener impls (12-slot subobject vtable) [U roles]
    void  SelVf1() override; void SelVf2() override; void SelVf3() override;
    void  SelVf4() override; void SelVf5() override; void SelVf6() override;
    void  SelVf7() override; void SelVf8() override; void SelVf9() override;
    void  SalVf10() override; void SalVf11() override;

    void*               m_subjectD8;       // +0xD8  raw non-owning ptr; polymorphic subject/handler dispatched via sub_180FD1C34 / sub_1805B38C0 in Step; NOT released in dtor
    uint8_t             m_idTriples[0xA0]; // +0xE0..+0x180  five (atom,atom,0) id-triples / WUID pairs [U types]
    std::list<S_SmartAreaEventEntry> m_eventQueue; // +0x180  {sentinel*, size}; sentinel new(0x40); empty=(size==0) sub_18040D1C0; pop_front sub_1823C959C [std::list INFERRED]
    // +0x188 = m_eventQueue._Mysize (belongs to the std::list above)
    void*               m_activeSubject;   // +0x190  raw non-owning ptr to in-flight subject (same role/type as m_subjectD8); reset to 0 in Step
    int32_t             m_reqSlot198;      // +0x198  -1 default sentinel (result of sub_18320D0C0; index/enum) [U meaning]
    uint32_t            _pad19C;           // +0x19C  (never written)
    int64_t             m_req1A0;          // +0x1A0  0 default [U role]
    uint64_t            m_reqHandle1A8;    // +0x1A8  8-byte handle; default -1 (qword_1853381E0 invalid sentinel) [WUID? INFERRED]
    CryStringT<char>    m_reqName1B0;      // +0x1B0  request name; default empty (Str); assign sub_1804F3344, dtor sub_1804FC624
    int64_t             m_1B8;             // +0x1B8  0 [U role]
    int64_t             m_1C0;             // +0x1C0  0 [U role]
    S_VariableReference m_varRefA;         // +0x1C8  embedded (0x60)
    S_VariableReference m_varRefB;         // +0x228  embedded (0x60)
    uint8_t             m_embedded288[0x10]; // +0x288..+0x298  (sub_1803C4794) [U interior]
    uint32_t            m_298;             // +0x298  ctor: 2 [U role]
    uint32_t            m_flags29C;        // +0x29C  flags [U meaning]
};
static_assert(sizeof(C_SmartAreaSubBrain) == 0x2A0, "C_SmartAreaSubBrain must be 0x2A0 (operator new(672))");

}  // namespace wh::xgenaimodule
