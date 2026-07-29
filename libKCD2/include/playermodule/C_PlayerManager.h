#pragma once
#include <cstdint>
#include <cstddef>
#include "C_PlayerData.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::playermodule::C_PlayerManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0xD8.
// -----------------------------------------------
// RTTI .?AVC_PlayerManager@playermodule@wh@@ (TD 0x184E062D8). TWO vtables:
// primary 0x183A84208 (5 slots; the next symbol aPlayerswitcher begins at 0x183A84248)
// and secondary 0x183A841E8 for the base subobject at +0x08 (COL ...6B@_0).
// ctor sub_180BE9868 (alloc 0xD8 at C_PlayerModule ctor 0x180BE9483, stored at
// C_PlayerModule+0x78); registers itself under the literal name "C_PlayerManager".
//
// Owns the notion of WHICH playable character is current. Reach it as
//   S_GameContext::GetInstance()->m_pPlayerModule->m_pPlayerManager
// (i.e. *(*(S_GameContext+0x128)+0x78)) -- this is the exact chain the fast-travel-point
// gate sub_181F4A2A0 walks before reading C_PlayerData::m_flag2A.
//
// ctor sequence (sub_180BE9868 -> sub_180BE9A1C on this+0x10):
//   +0x00 / +0x08   the two vtables
//   +0x10           byte 0
//   +0x18           embedded C_PlayerData, in place (ctor sub_180BE9B54)
//   +0x48           sub-object seeded with the literal "PlayerSwitcher" (sub_180B3FF14)
//   +0xB8 / +0xC0   zeroed, then +0xB8 = a self-linked intrusive list node
//                   (node[0]=node[1]=node[2]=node, node+0x18 = 0x0101)
//   +0xC8           = sub_18193C72C()  -- scans the global row array and returns the
//                     MINIMUM m_id, i.e. the default/first playable character
//   +0xCC           = copy of +0xC8
//   +0xD0           empty CryStringT
//
// GetCurrentPlayerData (slot [1], sub_180473260) is a two-liner:
//     return this->vf[2](this->m_currentPlayerId);          // [rax+10h] on m_currentId
// and slot [2] (sub_180473270) forwards to the free binary search sub_180473288 over the
// global row array xmmword_1853325C0. Returns nullptr when the id is absent -- every
// caller must null-check (sub_181F4A2A0 does, defaulting its gate to "allowed").
//
// [UNVERIFIED] the +0x48..+0xB7 block and the +0xB8 list are not decoded; padded out
// rather than guessed. Slots [3] and [4] are likewise unnamed.

namespace rttr {
class type;
namespace detail { struct derived_info; }
}  // namespace rttr

namespace wh::playermodule {

class C_PlayerManager {
public:
    // ---- primary vtable 0x183A84208 ----
    // [0] 0x181A73A70 -- shares an address with a Concurrency runtime thunk (ICF fold).
    virtual int32_t _vf0();
    // Look up the CURRENT character's row: vf[2](m_currentPlayerId). May return nullptr.
    virtual C_PlayerData* GetCurrentPlayerData();            // [1] 0x180473260
    // Binary-search the global row array by id; nullptr when absent.
    virtual C_PlayerData* FindPlayerDataById(int32_t id);    // [2] 0x180473270 -> sub_180473288
    virtual void _vf3();                                     // [3] 0x182EABE14  role UNVERIFIED
    virtual void _vf4();                                     // [4] 0x182EB2E1C  role UNVERIFIED

    uint8_t          _pad08[0x08];   // +0x08  second base subobject (vtable 0x183A841E8)
    uint8_t          m_byte10;       // +0x10  ctor 0 [role UNVERIFIED]
    uint8_t          _pad11[0x07];   // +0x11
    C_PlayerData     m_playerData;   // +0x18  embedded row, constructed in place (0x30)
    uint8_t          _pad48[0x70];   // +0x48  "PlayerSwitcher" sub-object [contents UNVERIFIED]
    void*            m_pListNode;    // +0xB8  self-linked intrusive list node (ctor-allocated)
    void*            m_ptrC0;        // +0xC0  ctor 0 [role UNVERIFIED]
    int32_t          m_currentPlayerId;  // +0xC8  ctor = min row id (sub_18193C72C); the key
                                         //        GetCurrentPlayerData feeds to the search
    int32_t          m_initialPlayerId;  // +0xCC  ctor = copy of m_currentPlayerId
    CryStringT<char> m_str;              // +0xD0  ctor empty [role UNVERIFIED]
};
static_assert(sizeof(C_PlayerManager) == 0xD8, "C_PlayerManager must be 0xD8 (alloc at 0x180BE9483)");
static_assert(offsetof(C_PlayerManager, m_playerData) == 0x18, "embedded C_PlayerData at 0x18");
static_assert(offsetof(C_PlayerManager, m_currentPlayerId) == 0xC8, "current player id at 0xC8");

}  // namespace wh::playermodule
