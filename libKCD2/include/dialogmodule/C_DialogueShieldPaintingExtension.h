#pragma once
#include <cstdint>
#include <cstddef>
#include "I_DialogueExtension.h"

// -----------------------------------------------
// wh::dialogmodule::C_DialogueShieldPaintingExtension -- shield-painting minigame
// extension (KCD2 1.5.6, kd7u).  sizeof ~0xA8 UNVERIFIED.
// -----------------------------------------------
// RTTI TD 0x184B4FCC0; vtable 0x183E67C20 (11 slots); ctor sub_181E6B780; overrides
// [3] OnUpdate and [5] Reset. Interior observations CONFLICT: a "+0x18 worker object
// (112 bytes, built by sub_181E6EA10)" would span +0x18..+0x88, overlapping the
// "+0x68 intrusive-list node" observation -- the interior is therefore modeled as an
// opaque byte region pending a layout dump. Do NOT rely on interior offsets.

namespace wh::dialogmodule {

class C_DialogInstance;

class C_DialogueShieldPaintingExtension : public I_DialogueExtension {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DialogueShieldPaintingExtension;
    bool    m_active;        // +0x08  [per-extension convention; UNVERIFIED]
    uint8_t _pad09[7];       // +0x09
    C_DialogInstance* m_pOwner;  // +0x10  owning C_DialogInstance (ctor a2 arg; owner+640/+668 read)
    void*    m_pWorker;      // +0x18  owning ptr to heap 112B worker (alloc sub_1804F75C0(112), ctor sub_181E6EA10, dtor sub_181E6B960=dtor+free) [pointee UNVERIFIED]
    uint8_t  m_b20;          // +0x20  byte flag, ctor 0 (bool candidate; single memset-0)
    uint8_t  _pad21[7];      // +0x21
    void*    m_pItem;        // +0x28  owning wh::entitymodule::C_ItemWrapper (sub_1808D3DC4 allocs 24B + sets its vftable); released in dtor & Reset
    void*    m_field30;      // +0x30  cached global qword_185322558 (static value 0; runtime-init singleton/handle) [role UNVERIFIED]
    std::vector<void*>      m_vec38;   // +0x38  {first,last,end}; 8B elems (dtor sub_181DE9FA0; OnUpdate (last-first)>>3) [elem UNVERIFIED]
    std::vector<void*>      m_vec50;   // +0x50  {first,last,end}; 8B elems (dtor sub_181DE9FA0) [elem UNVERIFIED]
    std::map<CryStringT<char>, std::vector<void*>> m_map68;  // +0x68  {_Myhead,_Mysize}; node 64B=32hdr+8key+24val (dtor sub_181E6B8C0; OnUpdate CryStringT-keyed lookup returns vector at node+40)
    std::vector<void*>      m_vec78;   // +0x78  {first,last,end}; 8B ptr elems, polymorphic (dtor sub_181DE9FA0; OnUpdate calls vtbl[2] on each element) [pointee UNVERIFIED]
    int      m_cursor90;     // +0x90  ring index into m_vec38 (OnUpdate)
    int      m_cursor94;     // +0x94  ring index into m_map68 value vector (OnUpdate)
    int      m_cursor98;     // +0x98  ring index into m_vec78 (OnUpdate)
    int      m_field9C;      // +0x9C  int, ctor 0 [role UNVERIFIED]
    int      m_fieldA0;      // +0xA0  int (DWORD), ctor 0 [role UNVERIFIED]
    uint8_t  _padA4[4];      // +0xA4  tail pad to sizeof 0xA8
};
static_assert(offsetof(C_DialogueShieldPaintingExtension, m_pWorker) == 0x18);
// sizeof == 0xA8 by construction; total size ~0xA8 UNVERIFIED (see header note).

}  // namespace wh::dialogmodule
