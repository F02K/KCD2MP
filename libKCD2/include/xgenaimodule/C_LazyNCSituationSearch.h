#pragma once
#include <cstdint>
#include <vector>
#include "C_AbstractSituationSearch.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LazyNCSituationSearch : C_AbstractSituationSearch --
// lazy no-check constraint search (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x88
// (create-site proven: factory sub_180A21DD0: qword_18549D378(136,..),
// dword_1851DFB18 += 0x88 @0x180a21f16; ctor inlined in the factory).  Picked
// when (#variables <= 2) OR (maxDomainSize < 100), else ForwardChecking.
// -----------------------------------------------
// RTTI TD rva 0x4F4DB10, COL 0x40DFA28.  Vtable rva 0x3A6B788 (7 slots):
// overrides [0]sub_1803BAEC8 [3]sub_181A73610 [6]deleting dtor sub_180A21C1C;
// [1][2][4][5] inherited from the abstract base.  The "LazyNC" string at +0x80
// is a debug/telemetry label, not a class-name source.

namespace wh::xgenaimodule {

class C_LazyNCSituationSearch : public C_AbstractSituationSearch {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LazyNCSituationSearch;
    void _vf0() override;                 // [0] sub_1803BAEC8 (algorithm)
    void _vf3() override;                 // [3] sub_181A73610 (algorithm)
    ~C_LazyNCSituationSearch() override;  // [6] deleting dtor sub_180A21C1C

    void*              m_situation;   // +0x38  CSP problem/holder (factory a2, stored @0x180a21f36); owns std::vector<C_Variable*> @prob+0x90 (elem flag@+0x41, domain via vf slot5) + per-var constraint rows @prob+0xD8; NOT C_Situation (0x78); concrete class [U]
    int32_t            m_varCount;    // +0x40  (situation+152 - situation+144) >> 3
    uint8_t            _pad44[4];     // +0x44
    std::vector<void*> m_vec48;       // +0x48  [U elem type]
    int32_t            m_60;          // +0x60  ctor: 0 [U role]
    uint8_t            _pad64[4];      // +0x64  alignment before +0x68 qword (never written by ctor/_vf0)
    uint64_t           m_nodeCounter;  // +0x68  _vf0 backtrack-step counter (writer 0x1803bb131 ++, reset 0x1803bb100); returns 2 (abort) when >= m_nodeLimit
    uint64_t           m_nodeLimit;    // +0x70  search-space node bound = product over vars of domainSize*(acc+1)+1 (init loop 0x1803bb0be)
    int8_t             m_78;          // +0x78  ctor: 0 [U role]
    uint8_t            _pad79[7];     // +0x79
    CryStringT<char>   m_name;        // +0x80  "LazyNC" (sub_1804F66CC)
};
static_assert(sizeof(C_LazyNCSituationSearch) == 0x88, "C_LazyNCSituationSearch must be 0x88 (alloc 136 in factory)");

}  // namespace wh::xgenaimodule
