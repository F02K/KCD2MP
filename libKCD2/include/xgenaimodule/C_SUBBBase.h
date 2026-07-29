#pragma once
#include <cstdint>
#include <vector>
#include "I_CastableIface.h"
#include "C_VariableIndex.h"

// -----------------------------------------------
// wh::xgenaimodule::C_SUBBBase : I_CastableIface : I_NoRTTITypeInfo -- shared
// base of the whole SUBB (subbrain) fleet (KCD2 WHGame.dll 1.5.6, kd7u).
// ABSTRACT (slots 22,23,27,35,36 _purecall).  sizeof 0xC8 (create-site proven:
// factory sub_18041A4DC "Default"/"" branch memset(p,0,0xC8) + ctor).
// -----------------------------------------------
// RTTI TD rva 0x4EFDA08; COL rva 0x40E7610; vtable 0x183A34010, 37 slots.
// ctor sub_18041414C; body dtor sub_180413AD8; deleting dtor = slot [1]
// sub_18320867C.  Factory: C_SUBBBase::CreateSubbrainByName_18041A4DC
// (string -> ctor; the single size-proof site for every subbrain).
// Central state machine SetState = sub_180BDBD18: stores m_state, fires each
// listener in m_listeners ((*(L+0x18))(L,this,old,new)), and if
// m_pendingOwnerNotify calls owner.vtbl[256].  State bitmask values observed:
// 2,4,8,16,32,128,256,512,1024.  Slot [11] getter sub_181A7F820 (IDA
// "Concurrency::GetRootCollection" label is a COMDAT-folding false name).
// Slots [8..21] are lifecycle commands funneling through SetState; individual
// roles beyond those below UNRESOLVED [U].

namespace wh::xgenaimodule {

class C_SUBBBase : public I_CastableIface {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SUBBBase;
    virtual void  Init(void* owner);        // [5]  sub_18041A758 -- m_owner = owner
    virtual void  Deinit();                 // [6]  sub_183208A14 -- m_owner = 0
    virtual void  SetGoals(void* goalsVec); // [7]  sub_1810B2DFC -> m_goals, SetState(256)
    virtual void  _vf8();                   // [8]  lifecycle cmd (SetState funnel) [U role]
    virtual void  _vf9();                   // [9]  lifecycle cmd [U role]
    virtual void  _vf10();                  // [10] lifecycle cmd [U role]
    virtual void* GetCurrent();             // [11] sub_181A7F820 -- return m_current
    virtual void  _vf12();                  // [12] lifecycle cmd [U role]
    virtual void  _vf13();                  // [13] lifecycle cmd [U role]
    virtual void  _vf14();                  // [14] lifecycle cmd [U role]
    virtual void  _vf15();                  // [15] lifecycle cmd [U role]
    virtual void  _vf16();                  // [16] lifecycle cmd [U role]
    virtual void  _vf17();                  // [17] lifecycle cmd (touches m_pendingOwnerNotify) [U role]
    virtual void  _vf18();                  // [18] lifecycle cmd [U role]
    virtual void  _vf19();                  // [19] clears m_field48 [U role]
    virtual void  _vf20();                  // [20] lifecycle cmd [U role]
    virtual void  _vf21();                  // [21] lifecycle cmd [U role]
    virtual void  Step() = 0;               // [22] PURE -- per-tick state processor (BT impl sub_180760A44)
    virtual bool  IsRunning() = 0;          // [23] PURE -- run-query [U name]
    virtual void  Configure(void* desc);    // [24] sub_180418A68 -- reads desc+0x18/+0x28/+0x2C
    virtual void  _vf25();                  // [25] [U role]
    virtual void  _vf26();                  // [26] [U role]
    virtual void* GetSubbrainType() = 0;    // [27] PURE -- per-subclass const stub
    virtual void  _vf28();                  // [28] [U role]
    virtual void  Serialize(void* stream);  // [29] sub_18320A388 (stream tags 4700/4701/4702)
    virtual void  Serialize2(void* stream); // [30] sub_183209970 (callback-record wrapper)
    virtual void  _vf31();                  // [31] return-stub in base (live in SmartObject) [U role]
    virtual void  _vf32();                  // [32] return-stub in base (live in Scheduler) [U role]
    virtual void  _vf33();                  // [33] return-stub in base (live in Scheduler) [U role]
    virtual void  _vf34();                  // [34] return-stub in base (live in Scheduler) [U role]
    virtual void  RequestOwnerTick() = 0;   // [35] PURE -- common impl sub_18075EDB8: m_pendingOwnerNotify=1; owner.vtbl[248]
    virtual void  CancelOwnerTick() = 0;    // [36] PURE -- common impl sub_1807F8434: clear flag; owner.vtbl[256]

    CryStringT<char>     m_name;            // +0x08  subbrain name; empty = shared _emptyHeader atom
                                            //        (ctor sub_1804FD80C()+3); Configure assigns desc+0x18; dtor sub_1804FD898 decrefs ptr-12
    uint32_t             m_field10;         // +0x10  init 0; no writer/reader found; not serialized [U role]
    uint32_t             m_state;           // +0x14  state bitmask (init 8); SetState target
    void*                m_owner;           // +0x18  owning brain/host back-ptr (slot [5]/[6])
    std::vector<void*>   m_names;           // +0x20  name-handle elems (dtor sub_1803F70D8 frees each-12)
    uint8_t              m_flagsA;          // +0x38  |=1 (bits 1,2,4,8) [U bit meanings]
    uint8_t              _pad39[7];         // +0x39
    uint64_t             m_field40;         // +0x40  low32 = Configure copies (uint32)desc+0x28; serialized as int32 (tag 4700/sub-tag 1, field 0) [U role]
    uint64_t             m_field48;         // +0x48  serialized as int32 (tag 4700/sub-tag 1, field 1); only ever written 0 (ctor & slot [19]); no non-zero writer found [U role]
    uint8_t              m_field50;         // +0x50  init 1; serialized as 1 byte (tag 4700/sub-tag 1, field 2) [U role]
    uint8_t              _pad51[7];         // +0x51
    void*                m_current;         // +0x58  active task/goal ptr; GetCurrent [11] returns it; SetState clears it; Serialize maps it to a 16B persistent handle via owner->vtbl[29] [U pointee]
    std::vector<uint64_t> m_goals;          // +0x60  serialize iterates (>>3)
    uint32_t             m_countCache;      // +0x78  = m_goals count
    uint32_t             m_status;          // +0x7C  constant 3 in all observed writers (not a live FSM)

    // 32-byte state-listener record appended via sub_18206DB30 (2x OWORD);
    // SetState fires (*(rec+0x18))(rec, this, oldState, newState).  Fields [U].
    struct S_StateListener {
        void* m_dispatch; // +0x00  fn ptr (SmartObject sub_180BDBE88 / DogCompanion sub_18171D9D0)
        void* _unk8;      // +0x08  flag/unused (int 0 in SmartObject ctor; unset in Dog ctor) [U]
        void* m_context;  // +0x10  callback context = owning subbrain 'this'
        void* m_callback; // +0x18  fn ptr; SetState calls (rec, this, oldState, newState)
    };
    std::vector<S_StateListener> m_listeners;   // +0x80

    C_VariableIndex      m_varIndex;        // +0x98  embedded (0x28); parent = qword_185496678
    uint8_t              m_pendingOwnerNotify; // +0xC0  gate for [35]/[36] owner notify
    uint8_t              _padC1[7];         // +0xC1
};
static_assert(sizeof(C_SUBBBase) == 0xC8, "C_SUBBBase must be 0xC8 (factory memset 0xC8)");
static_assert(sizeof(C_SUBBBase::S_StateListener) == 0x20, "listener record is 32 bytes");

}  // namespace wh::xgenaimodule
