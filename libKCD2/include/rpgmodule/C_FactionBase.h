#pragma once
#include <cstdint>
#include <memory>
#include <vector>

// -----------------------------------------------
// wh::rpgmodule::C_FactionBase -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x78.
// -----------------------------------------------
// Base of C_Faction AND C_NPCFactionNode (RTTI CHD 0x1843D6350, mdisp 0 -- the derived vtable
// sits in the shared vptr @+0x00). ctor sub_1803F2288; dtor sub_181242778. An embedded
// C_DebugReputationLog begins @+0x08 (its own vtable is written there; RTTI lists it at
// disp +0x08, but whether it is an MI base or a plain member is UNRESOLVED -- represented as
// an embedded region; its interior +0x10..+0x3F is ctor-zeroed and untiled).

namespace wh::rpgmodule {

class C_FactionBase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FactionBase;
    virtual ~C_FactionBase() = default;   // +0x00  vptr (C_Faction vtable 0x183B1DF10 in the derived object)
    void*          m_debugRepLogVptr;     // +0x08  vtable of embedded C_DebugReputationLog (RTTI 2nd base disp+0x08, vtbl 0x183A334D8); interior +0x10..+0x40 below, owner back-ptr @+0x40
    uint64_t       m_unk10[6];            // +0x10..+0x40  C_DebugReputationLog interior / unresolved (ctor-zeroed)
    C_FactionBase* m_pSelf;               // +0x40  self back-ptr (INFERRED intrusive)
    int32_t        m_i48;                 // +0x48  (ctor-zeroed; role unresolved)
    uint32_t       _pad4C;                // +0x4C
    std::weak_ptr<void> m_weak50;         // +0x50  (0x10) RUNTIME weak ref -- _Decwref-released
                                          //        (sub_1803FB78C); pointee type UNRESOLVED
    std::vector<void*> m_vec60;           // +0x60  (0x18) 72-BYTE elements (elem dtor
                                          //        sub_1810C77A8) -- element type UNRESOLVED
                                          //        (INFERRED reputation records); void* misstates
                                          //        the stride, kept only for the container shape
};
static_assert(sizeof(C_FactionBase) == 0x78, "C_FactionBase must be 0x78");
static_assert(offsetof(C_FactionBase, m_weak50) == 0x50, "weak at 0x50");
static_assert(offsetof(C_FactionBase, m_vec60) == 0x60, "vector at 0x60");

}  // namespace wh::rpgmodule
