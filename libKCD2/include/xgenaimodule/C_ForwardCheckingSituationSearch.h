#pragma once
#include <cstdint>
#include <vector>
#include "C_AbstractSituationSearch.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::xgenaimodule::C_ForwardCheckingSituationSearch : C_AbstractSituationSearch
// -- forward-checking constraint search (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof
// 0xA8 (create-site proven: factory sub_180A21DD0: qword_18549D378(168,..),
// dword_1851DFB18 += 0xA8 @0x180a21eb5; ctor sub_1832B6488(this, a2=situation,
// a3=domainArray)).  Picked when #variables > 2 AND maxDomainSize >= 100.
// -----------------------------------------------
// RTTI TD rva 0x4F4DB50, COL 0x4550FE0.  Vtable rva 0x4767290 (7 slots):
// overrides [0]sub_1832BBE90 [3]unknown_libname_89 (rva 0x1A84720; IDA lib
// thunk, body not inspected [U]) [6]deleting dtor sub_1832B7558; [1][2][4][5]
// inherited.  ctor fills the base +0x08 vector via sub_1823D17D8(varcount).
// The "LazyNC-FC" string at +0xA0 is a debug/telemetry label only.

namespace wh::xgenaimodule {

class C_ForwardCheckingSituationSearch : public C_AbstractSituationSearch {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ForwardCheckingSituationSearch;
    void _vf0() override;                          // [0] sub_1832BBE90 (algorithm)
    void _vf3() override;                          // [3] unknown_libname_89 (0x1A84720) [U body]
    ~C_ForwardCheckingSituationSearch() override;  // [6] deleting dtor sub_1832B7558

    int8_t             m_38;          // +0x38  ctor: 0 [U role]
    uint8_t            _pad39[7];     // +0x39  alignment padding (uninit by ctor)
    uint64_t           m_iterLimit;  // +0x40  prepare sub_1832B9F70@0x1832ba1da: product of per-var domain sizes (search-space budget)
    uint64_t           m_iterCount;  // +0x48  step counter; prepare@0x1832ba19d=0, Search@0x1832bbf1f ++, ret 2 when >= m_iterLimit
    void*              m_situation;   // +0x50  ctor a2 = the CSP problem/situation-definition (NOT C_Situation): read at +0x90 var-vector, +0xD8/+0xF0 per-var constraint vec-of-vecs [U class]
    int32_t            m_varCount;    // +0x58
    uint8_t            _pad5C[4];     // +0x5C
    std::vector<void*> m_domainRows;  // +0x60  elem 56 (domain rows) [U elem type]
    std::vector<void*> m_queue;       // +0x78  elem 24 (constraint/queue) [U elem type]
    int32_t            m_currentVarIndex;  // +0x90  ctor 0; current search depth/var, indexes domain rows (56*idx), Search@0x1832bc1eb ++
    int32_t            m_94;          // +0x94  ctor: 0 [U role]
    int32_t            m_98;          // +0x98  ctor: 1 [U role]
    uint8_t            _pad9C[4];     // +0x9C
    CryStringT<char>   m_name;        // +0xA0  "LazyNC-FC" (sub_1804FDA1C)
};
static_assert(sizeof(C_ForwardCheckingSituationSearch) == 0xA8, "C_ForwardCheckingSituationSearch must be 0xA8 (alloc 168 in factory)");

}  // namespace wh::xgenaimodule
