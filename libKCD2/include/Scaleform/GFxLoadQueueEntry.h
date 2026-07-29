#pragma once
#include <cstdint>
#include "Scaleform/GASString.h"

// -----------------------------------------------
// GFxLoadQueueEntry -- one queued AS2 load request (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Statically linked Scaleform GFx 3.x; no RTTI, SDK-correlated name. 208 bytes
// (0xD0), allocated from the movie's GMemoryHeap (root+0x38 vf+0x50 Alloc(208,0),
// loader 0x181CA314B / 0x181CA307D) and freed by ProcessLoadQueue through the
// global GFx heap qword_1850C85C0 vf+0x60 (0x181CAF0E9).
//
// Producers -- both branches of the loadMovie loader 0x181CA3010 (called from
// the AS builtin 0x181CB8420 and friends):
//   * target branch  (sprite/mc target): m_type = url-is-unload-sentinel ? 1 : 0,
//     m_level = -1, m_pCharacter = target's addref'd GFxCharacterHandle
//     (entry+0x20; the handle's leading int is ++'d at 0x181CA31C8).
//   * _levelN branch (target resolves to a level id, 0x181CA304E..0x181CA30ED):
//     m_type = 3 - (url != sentinel)  (i.e. LTF_LevelFlag | maybe LTF_UnloadFlag),
//     m_level = level id, m_pCharacter = null.
// Consumer -- GFxMovieRoot::ProcessLoadQueue 0x181CAEF50 pops the FIFO and
// dispatches on m_type: &4 -> 0x181CAF1C0, &8 -> 0x181CAF370, &0x10 ->
// 0x181CAE3B0, else (plain movie, 0/1/2/3) -> 0x181CAE530 (the normal-SWF
// handler: .swf->.gfx rewrite, img:// branch, disk load via 0x181C28C30,
// logs `Error loading URL "%s"` on failure).
//
// The four 0x20 "optional payload" blocks (+0x30/+0x50/+0x78/+0xA0) each start
// with a presence byte the loader zeroes; the drain releases them via
// 0x181C71270. +0x30 receives a copy of the loader's 5th arg via 0x181C72900
// (0x181CA326F). Their individual roles (vars text, post data, ...) are
// UNVERIFIED -- kept opaque.

struct GFxCharacterHandle;   // opaque; leading int32 refcount (++/-- at entry creation/free)

class GFxLoadQueueEntry {
public:
    // Load method requested by the AS call (loadMovie/loadVariables 3rd arg).
    enum LoadMethod : int32_t {
        LM_None = 0,
        LM_Get  = 1,
        LM_Post = 2,
    };

    // m_type bits. Values 0..3 are written by the loader (see banner); the
    // flag NAMES are correlated from GFx 3 source -- the dispatch itself is
    // VERIFIED from ProcessLoadQueue (0x181CAF010..0x181CAF042).
    enum TypeFlags : uint32_t {
        LTF_None       = 0x00,   // plain loadMovie into a target character
        LTF_UnloadFlag = 0x01,   // url was the unload sentinel (tagged value & ~3 == -12)
        LTF_LevelFlag  = 0x02,   // _levelN load (m_level >= 0, m_pCharacter null)
        LTF_VarsFlag   = 0x04,   // -> handler 0x181CAF1C0
        LTF_XMLFlag    = 0x08,   // -> handler 0x181CAF370
        LTF_CSSFlag    = 0x10,   // -> handler 0x181CAE3B0
    };

    GFxLoadQueueEntry*  pNext;          // +0x00  singly-linked FIFO (tail-appended by AddLoadQueueEntry)
    uint32_t            m_type;         // +0x08  TypeFlags
    LoadMethod          m_method;       // +0x0C  loader arg a4
    GASString           m_url;          // +0x10  request URL ("img://" / "imgps://" tested here by the enqueue)
    int32_t             m_level;        // +0x18  _levelN id; -1 = target-character mode
    uint32_t            _pad1C;         // +0x1C
    GFxCharacterHandle* m_pCharacter;   // +0x20  addref'd target handle (null for level loads)
    uint8_t             _unk28;         // +0x28  flag/presence byte; both producers write 0 (0x181CA3210, 0x181CA35A5/0x181CA34A8), no reader found -- role UNVERIFIED
    uint8_t             _pad29[7];      // +0x29
    uint8_t             m_opt30[0x20];  // +0x30  optional payload block (copy of loader arg 5)
    uint8_t             m_opt50[0x20];  // +0x50  optional payload block
    uint32_t            m_entryId;      // +0x70  loader inits -1; AddLoadQueueEntry stamps ++root.m_lastLoadQueueEntryCnt (0x181CA3B07)
    uint32_t            _pad74;         // +0x74
    uint8_t             m_opt78[0x20];  // +0x78  optional payload block
    GRefCountImplCore*  _unk98;         // +0x98  GFx refcounted obj; XML/vars handler calls derived vtable[1] setter (0x181CAF40D), drain releases via 0x181C20770 (0x181CAF070)
    uint8_t             m_optA0[0x20];  // +0xA0  optional payload block
    GRefCountImplCore*  _unkC0;         // +0xC0  GFx refcounted obj; drain releases via 0x181C20770 (0x181CAF053)
    uint8_t             _unkC8;         // +0xC8  flag/presence byte; both producers write 0 (0x181CA3214, 0x181CA35B5/0x181CA34B3), no reader found -- role UNVERIFIED
    uint8_t             _padC9[7];      // +0xC9
};
static_assert(sizeof(GFxLoadQueueEntry) == 0xD0, "GFxLoadQueueEntry must be 208 bytes (alloc size at 0x181CA314B)");
static_assert(offsetof(GFxLoadQueueEntry, m_url) == 0x10, "url at 0x10");
static_assert(offsetof(GFxLoadQueueEntry, m_pCharacter) == 0x20, "target handle at 0x20");
static_assert(offsetof(GFxLoadQueueEntry, m_entryId) == 0x70, "entry id at 0x70");
