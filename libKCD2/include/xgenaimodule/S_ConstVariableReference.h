#pragma once
#include <cstdint>
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::xgenaimodule::S_ConstVariableReference -- read-only reference/path to an AI
// variable (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x60.  RTTI kind is struct
// (.?AU) but the type is POLYMORPHIC (10-slot vtable) -> declared class per
// project convention.
// -----------------------------------------------
// RTTI TD rva 0x4E8E660; COL rva 0x40CCF68; vtable 0x183A43AA0, 10 slots (roles
// NOT individually decompiled [U]).  ctors: default sub_180638164, from-owner
// sub_1806380CC, move sub_180638994; dtor sub_1806382BC (frees the two CryStrings
// + m_extra via sub_180D132C8).  The two path blocks are NOT std::vectors: begin
// is seeded with the static sentinel &xmmword_18533C060 -- modeled as raw ptrs.

namespace wh::xgenaimodule {

class S_ConstVariableReference {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_ConstVariableReference;
    virtual ~S_ConstVariableReference();   // [0] [U placement]
    virtual void _vf1();                   // [1] [U role]
    virtual void _vf2();                   // [2] [U role]
    virtual void _vf3();                   // [3] [U role]
    virtual void _vf4();                   // [4] [U role]
    virtual void _vf5();                   // [5] [U role]
    virtual void _vf6();                   // [6] [U role]
    virtual void _vf7();                   // [7] [U role]
    virtual void _vf8();                   // [8] [U role]
    virtual void _vf9();                   // [9] [U role]

    uint16_t         m_kind;        // +0x08  only 0 observed (readers test !m_kind, sub_1805C68EC); the real flag bits live in the +0x0A byte
    uint8_t          m_flag;        // +0x0A  (0) [U meaning]
    uint8_t          _pad0B[5];     // +0x0B
    CryStringT<char> m_nameA;       // +0x10  (empty default via sub_1804FD80C()+3) [U role]
    CryStringT<char> m_nameB;       // +0x18  (empty default) [U role]
    void*            m_owner;       // +0x20  ctor a2; passed as eval context to m_extra vslot2 (sub_18059A4A0); only null-checked, pointee class unresolved
    void*            m_aux28;       // +0x28  polymorphic provider iface, bound post-ctor (0 at ctor); vcalls [1]@+8 ret int (==3 test), [7]@+56 name->entry lookup, [22]@+176 ret count; class unresolved
    void*            m_pathA;       // +0x30  provider vslot7 lookup result (variable-entry ptr); &xmmword_18533C060 = shared empty {begin,end} descriptor (16-byte elems) when unresolved
    void*            m_pathAEnd;    // +0x38  ptr to value descriptor {void* @0, int32 kind @8}; kind==7 && ptr==0 => empty/invalid (sub_1805C6EF4, sub_1805C6854)
    void*            m_pathB;       // +0x40  second {begin,end} range ptr; reset to &xmmword_18533C060 (shared empty) together with m_pathA
    void*            m_pathBAux;    // +0x48  aux slot for the m_pathB range; reset to 0 with the empty-path block
    int32_t          m_count;       // +0x50  = (sentinel.size >> 4) - 1
    uint8_t          _pad54[4];     // +0x54
    void*            m_extra;       // +0x58  owned polymorphic evaluator (vtable; vslot2@+16 evaluates a value, sub_18059A4A0); move-ctor nulls source; dtor calls pool getter sub_180D132C8 then nulls; class unresolved
};
static_assert(sizeof(S_ConstVariableReference) == 0x60, "S_ConstVariableReference must be 0x60");

}  // namespace wh::xgenaimodule
