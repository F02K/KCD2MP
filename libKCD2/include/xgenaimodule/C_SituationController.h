#pragma once
#include <cstdint>
#include <vector>

// -----------------------------------------------
// wh::xgenaimodule::C_SituationController -- runs the constraint search for one
// situation (KCD2 WHGame.dll 1.5.6, kd7u).  NO base class (standalone
// polymorphic, RTTI numBase=1).  sizeof 0x118 (create-site proven:
// sub_180D9BC5C: qword_18549D378(280,&v16), dword_1851DFB18 += 0x118, ctor
// sub_180D9C8D4(this, a2=situation, a3=owner)).
// -----------------------------------------------
// RTTI TD rva 0x4F4E208, COL 0x4551268.  Vtable rva 0x3FE8650 with exactly ONE
// slot: [0] virtual deleting dtor sub_180D9ADE0 (body dtor sub_180D9AC2C).
// ctor tail helper sub_180D9CA6C(this).  Interior container template types NOT
// expanded [U] -- opaque regions sized by the gaps between known offsets.

namespace wh::xgenaimodule {

class C_AbstractSituationSearch;
class C_Situation;
class C_SituationManager;

class C_SituationController {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_SituationController;
    virtual ~C_SituationController();   // [0] deleting dtor sub_180D9ADE0 (body sub_180D9AC2C)

    C_SituationManager* m_owner;            // +0x08  ctor a3 = C_SituationManager singleton (qword_185493960)
    C_Situation*       m_situation;         // +0x10  ctor a2
    uint8_t            _unk18[8];           // +0x18  [U]
    std::vector<void*> m_vec20;             // +0x20  elem 8 (dtor free-mask 0xF8)
    std::vector<void*> m_vec38;             // +0x38  elem 24 [U elem type]
    uint8_t            m_sub50[0x18];       // +0x50..+0x68  opaque subobject (dtor sub_1804134B4) [U interior]
    float              m_68;                // +0x68  ctor: 1.0f
    uint8_t            _pad6C[4];           // +0x6C
    void*              m_listSentinel70;    // +0x70  std::unordered _List._Myhead: 32-byte self-linked sentinel (sub_1804F75C0(32)); element-list head of hash @+0x68
    uint64_t           m_hash68_size;       // +0x78  std::unordered _List._Mysize (element count of hash @+0x68)
    uint8_t            m_hash80[0x28];      // +0x80..+0xA8  hash container (sub_1803B5774 init; +0x98=7, +0xA0=8 bucket params) [U interior]
    uint8_t            m_subA8[8];          // +0xA8  subobject (sub_1806030E4 init) [U interior]
    void*              m_B0;                // +0xB0  std::unordered _List._Myhead: 32-byte self-linked sentinel (sub_1804F75C0(32)); element-list head of hash @+0xA8
    uint64_t           m_hashA8_size;       // +0xB8  std::unordered _List._Mysize (element count of hash @+0xA8)
    std::vector<void*> m_vecC0;             // +0xC0  elem 8
    uint64_t           m_hashA8_mask;       // +0xD8  std::unordered _Mask (ctor/init 7)
    uint64_t           m_hashA8_maxidx;     // +0xE0  std::unordered _Maxidx (ctor/init 8)
    std::vector<void*> m_vecE8;             // +0xE8  elem 8
    uint8_t            _unk100[8];          // +0x100 [U]
    C_AbstractSituationSearch* m_search;    // +0x108 search strategy; destroyed via its virtual slot [6]
    int32_t            m_searchCounter110;  // +0x110 zeroed (search/step counter) [U role]
    int32_t            m_searchCounter114;  // +0x114 zeroed; LAST member (dossier's "+0x11C DWORD" claim exceeds the proven 280-byte alloc -- treated as off-by-one) [U]
};
static_assert(sizeof(C_SituationController) == 0x118, "C_SituationController must be 0x118 (alloc 280 at sub_180D9BC5C)");

}  // namespace wh::xgenaimodule
