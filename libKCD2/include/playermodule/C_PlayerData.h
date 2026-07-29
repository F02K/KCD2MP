#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryString.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::playermodule::C_PlayerData -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x30.
// -----------------------------------------------
// RTTI .?AVC_PlayerData@playermodule@wh@@ (TD 0x184DCEB78, COL 0x18411E2C8, single base
// descriptor 0x18442C010). vtable 0x183A84260 -- exactly 4 slots (the next symbol,
// ??_7C_FastTravel, begins at 0x183A84288; a wider read runs past the end into it).
// ctor sub_180BE9B54, dtor sub_182DDE140.
//
// One row of the PLAYABLE-CHARACTER definition table -- the static registry of who the
// player may be. Rows are stored BY VALUE in the global sorted array xmmword_1853325C0
// {begin,end,cap}, built by the registrar sub_1812BD484. One instance is also embedded in
// C_PlayerManager @+0x18.
//
// sizeof 0x30 is the ARRAY STRIDE (rows live inside the vector, so there is no standalone
// operator new to read a size from). Stride PROVEN two ways: the binary-search divide in
// C_PlayerManager::FindPlayerDataById sub_180473288 (`sar rcx,4` then
// `imul rcx, 0AAAAAAAAAAAAAAABh` == /0x30), and the `+= 0x30` row walks in sub_180C96710
// and sub_18193C72C.
//
// ctor stores, with their EXACT widths read from the disassembly (widths matter -- see the
// tag-width lesson in feedback_layout_assert_offsets):
//   180BE9B68  mov   [rcx], &vtable
//   180BE9B61  mov   dword ptr [rcx+08h], 0FFFFFFFFh
//   180BE9B79  movdqu xmmword ptr [rcx+0Ch], xmm0     <- 16 bytes, UNALIGNED move
//   180BE9B75  mov   byte  ptr [rcx+1Ch], 0
//   180BE9B87  mov   [rbx+20h], rax                   <- empty CryStringT
//   180BE9B8E  mov   dword ptr [rbx+28h], 1010000h    <- bytes +28=00 +29=00 +2A=01 +2B=01
//   180BE9B95  mov   byte  ptr [rbx+2Ch], 1
//
// m_guid is written with MOVDQU and read by sub_180C96710 as two UNALIGNED qwords
// (`[i+0x0C] != key[0] || [i+0x14] != key[1]`). It is therefore NOT a CryGUID/S_LocationId
// member: those are 8-byte aligned and the compiler would slide them to +0x10. Declared as
// raw bytes so the compiled offset matches the binary; its internal tiling is UNVERIFIED.

namespace wh::playermodule {

class C_PlayerData {
public:
    virtual ~C_PlayerData();                                 // [0] 0x182DDE140
    // RTTR trio [1..3]: get_type 0x181A6CDE8, get_ptr 0x1805F5DA0 (`return this`),
    // get_derived_info 0x1819CB7C0. Registered BASELESS (creator 0x18176988C,
    // sizeof imm 0x30 -- the binary's own confirmation of this header's size).
    RTTR_ENABLE()

    int32_t          m_id;         // +0x08  ctor -1; the sorted-array key (sub_180473288),
                                   //        min-reduced by sub_18193C72C to pick a default
    uint8_t          m_guid[0x10]; // +0x0C  16-byte identity, UNALIGNED; matched as two
                                   //        qwords against an external key by sub_180C96710
    uint8_t          m_byte1C;     // +0x1C  ctor 0 [role UNVERIFIED]
    uint8_t          _pad1D[3];    // +0x1D
    CryStringT<char> m_name;       // +0x20  the only string the dtor frees
    uint8_t          m_flag28;     // +0x28  ctor 0 [role UNVERIFIED]
    uint8_t          m_flag29;     // +0x29  ctor 0 [role UNVERIFIED]
    // Gates fast-travel MAP POINTS for this playable character. Read -- and only read --
    // by sub_181F4A2A0, which returns `GetGameMode() != Hardcore && this->m_flag2A`.
    // Defaults to 1 (allowed). NAME DESCRIBES THE ONE OBSERVED USE; no source name was
    // recovered, and no writer other than the ctor was found.
    uint8_t          m_flag2A;     // +0x2A  ctor 1
    uint8_t          m_flag2B;     // +0x2B  ctor 1 [role UNVERIFIED]
    uint8_t          m_byte2C;     // +0x2C  ctor 1 [role UNVERIFIED]
    uint8_t          _pad2D[3];    // +0x2D
};
static_assert(sizeof(C_PlayerData) == 0x30, "C_PlayerData must be 0x30 (array stride, sub_180473288)");
static_assert(offsetof(C_PlayerData, m_id) == 0x08, "id at 0x08");
static_assert(offsetof(C_PlayerData, m_guid) == 0x0C, "guid at 0x0C (unaligned -- movdqu)");
static_assert(offsetof(C_PlayerData, m_name) == 0x20, "name at 0x20");
static_assert(offsetof(C_PlayerData, m_flag2A) == 0x2A, "fast-travel-point gate byte at 0x2A");

}  // namespace wh::playermodule
