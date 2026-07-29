#pragma once
#include <cstdint>
#include "I_Intellect.h"
#include "C_VariableIndex.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AIBrain : I_Intellect -- abstract AI brain base (KCD2
// WHGame.dll 1.5.6, kd7u).  ABSTRACT: 15 _purecall slots
// (7,12,13,14,17,21,23,24,26..32).  sizeof 0xF8 (proven: C_AIBrainSingleSubb
// operator new(264) places its first own field at +0xF8).
// -----------------------------------------------
// RTTI TD rva 0x4EFD828; COL rva 0x40E72C8; vtable 0x183A37C70, 35 slots.
// ctor sub_180418CF4(this, owner) -- NOTE: sub_1804137F8 is the DESTRUCTOR, not
// the ctor (brief corrected by G4A).  Deleting dtor = slot [0] sub_183208590.
// Brain factory sub_180419AA8(a4=isSingleSubb): Single=new(264), Multi=new(408).
// Slot roles beyond [0] UNRESOLVED [U] -- addresses from g4a_dump_vtables.txt.
// [8] sub_181A9C2B0 is a real xgenaimodule fn (IDA "Concurrency::GetLocation"
// label is a COMDAT collision).  [34] = nullsub_1.

namespace wh::xgenaimodule {

class C_AIBrain : public I_Intellect {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AIBrain;
    // [0] dtor declared on I_Intellect (impl sub_183208590 -> body sub_1804137F8)
    virtual void _vf1();        // [1]  sub_1813030D8 [U role]
    virtual void _vf2();        // [2]  sub_1816235F4 [U role]
    virtual void _vf3();        // [3]  sub_18070DC14 [U role]
    virtual void _vf4();        // [4]  sub_181A73160 [U role]
    virtual void _vf5();        // [5]  sub_18041B0C4 [U role]
    virtual void _vf6();        // [6]  sub_18041B0FC [U role]
    virtual void _vf7() = 0;    // [7]  PURE
    virtual void _vf8();        // [8]  sub_181A9C2B0 (shared by all 3 brains)
    virtual void _vf9();        // [9]  sub_18066CD10 (return 0 stub)
    virtual void _vf10();       // [10] sub_183209D4C [U role]
    virtual void _vf11();       // [11] sub_1832097B0 [U role]
    virtual void _vf12() = 0;   // [12] PURE
    virtual void _vf13() = 0;   // [13] PURE
    virtual void _vf14() = 0;   // [14] PURE
    virtual void _vf15();       // [15] sub_1832097A8 [U role]
    virtual void _vf16();       // [16] sub_181850504 [U role]
    virtual void _vf17() = 0;   // [17] PURE
    virtual void _vf18();       // [18] sub_18320979C [U role]
    virtual void _vf19();       // [19] sub_183209790 [U role]
    virtual void _vf20();       // [20] sub_181A72EC0 [U role]
    virtual void _vf21() = 0;   // [21] PURE
    virtual void _vf22();       // [22] sub_1832094CC [U role]
    virtual void _vf23() = 0;   // [23] PURE
    virtual void _vf24() = 0;   // [24] PURE
    virtual void _vf25();       // [25] sub_18041A65C [U role]
    virtual void _vf26() = 0;   // [26] PURE
    virtual void _vf27() = 0;   // [27] PURE
    virtual void _vf28() = 0;   // [28] PURE
    virtual void _vf29() = 0;   // [29] PURE
    virtual void _vf30() = 0;   // [30] PURE
    virtual void _vf31() = 0;   // [31] PURE
    virtual void _vf32() = 0;   // [32] PURE
    virtual void _vf33();       // [33] sub_181180180 [U role]
    virtual void _vf34();       // [34] nullsub_1 (no-op)

    C_NPC*           m_pOwner;               // +0x08  owning NPC (ctor a2); a2->vf[8]() supplies the C_VariableIndex root passed to sub_1805B66DC(this+0xC8,...)
    CryStringT<char> m_name;                 // +0x10  = emptyHdr+12; released via sub_1804FC624 [U name]
    bool             m_flag18;               // +0x18  ctor: 1 [U name/meaning]
    uint8_t          _pad19[7];              // +0x19
    uint8_t          m_variableStore[0xA8];  // +0x20  embedded store (ctor sub_180418D9C; dtor sub_180413610
                                             //        + sub_18041B1C0): std::list + unordered_map(8 buckets)
                                             //        + vector + 3 inner sub-vtables [U exact class]
    C_VariableIndex  m_variableIndex;        // +0xC8  embedded (0x28); parent = qword_185496678
    bool             m_boolF0;               // +0xF0  ctor: 0 [U role; G4A originally folded this into the
                                             //        index -- 0x28 index size proven by G4B/G5E]
    uint8_t          _padF1[7];              // +0xF1
};
static_assert(sizeof(C_AIBrain) == 0xF8, "C_AIBrain must be 0xF8 (SingleSubb extends at +0xF8)");

}  // namespace wh::xgenaimodule
