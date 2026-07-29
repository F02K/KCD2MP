#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::shared::C_RegularGridObjectWrapperBase -- spatial-grid object wrapper base
// (KCD2 1.5.6, kd7u).  modeled sizeof 0x28 [extent UNPROVEN].
// -----------------------------------------------
// TD 0x184F49EF8, vtable 0x183A42FA8. Only the dtor sub_180631668 was located: it frees a
// std::vector-style buffer at +0x10 (begin/end/cap via sub_181AB5490) and restamps the
// vtable. Ctor not located, so +0x08's role and the true allocation size are UNVERIFIED --
// the 0x28 below is the dtor-visible field extent.

namespace wh::shared {

class C_RegularGridObjectWrapperBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_RegularGridObjectWrapperBase;
    virtual ~C_RegularGridObjectWrapperBase();   // [0] sub_180631668; further slots not enumerated

    void*    m_unk08;                // +0x08  ptr to lazily-alloc'd pooled object (sub_1805B1510); pointee class UNIDENTIFIED
    std::vector<void*> m_objects;    // +0x10  grid object list [element type UNVERIFIED]
};
static_assert(sizeof(C_RegularGridObjectWrapperBase) == 0x28, "modeled extent (dtor-visible fields; alloc UNPROVEN)");

}  // namespace wh::shared
