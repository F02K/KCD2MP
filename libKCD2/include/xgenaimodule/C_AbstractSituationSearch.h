#pragma once
#include <cstdint>
#include <vector>
#include "I_SituationSearch.h"

// -----------------------------------------------
// wh::xgenaimodule::C_AbstractSituationSearch : I_SituationSearch -- shared
// base of the two constraint-search algorithms (KCD2 WHGame.dll 1.5.6, kd7u).
// ABSTRACT (slots [0] and [3] stay pure).  Base region 0x38 (vptr + two
// std::vector); base ctor sub_180A21FA8(this, a2=variable-array).
// -----------------------------------------------
// Vtable rva 0x3A6B850 (7 slots): [0]_purecall [1]sub_1832BC294
// [2]sub_181A71D30 [3]_purecall [4]sub_181A73160 [5]sub_1824301CC (inherited
// iface body) [6]dtor sub_1832B73FC.  Slots [1][2][4] are concrete here and
// inherited unchanged by both derived classes.  Own COL/TD not directly
// captured (inferred from the LazyNC/FC RTTI base arrays at mdisp=0) [U].
// Vector element types not pinned [U].

namespace wh::xgenaimodule {

class C_AbstractSituationSearch : public I_SituationSearch {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_AbstractSituationSearch;
    void _vf1() override;               // [1] sub_1832BC294
    void _vf2() override;               // [2] sub_181A71D30
    void _vf4() override;               // [4] sub_181A73160
    ~C_AbstractSituationSearch() override; // [6] dtor sub_1832B73FC

    std::vector<void*> m_assignments;   // +0x08  variable/assignment list (zeroed by base ctor) [U elem type]
    std::vector<void*> m_domains;       // +0x20  reserved by sub_1803E67A4(this+0x20, count) [U elem type]
};
static_assert(sizeof(C_AbstractSituationSearch) == 0x38, "base region: vptr + 2 vectors");

}  // namespace wh::xgenaimodule
