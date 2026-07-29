#pragma once
#include <cstdint>
#include "Scaleform/GFxMovieView.h"
#include "Scaleform/GFxStateBag.h"
#include "Scaleform/GFxLoadQueueEntry.h"

// -----------------------------------------------
// GFxMovieRoot -- Scaleform GFx movie instance (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Statically linked Scaleform GFx 3.x; no RTTI, SDK-correlated name. THE movie
// object behind every CFlashPlayer: created by GFxMovieDef::CreateInstance
// 0x181C622D0 (heap Alloc(11256, 0) -> ctor 0x181CA0840(mem, movieDef)) and
// stored raw at CFlashPlayer+0xD0. sizeof == 0x2BF8 VERIFIED (the alloc size).
// Bases (ctor 0x181CA0868..0x181CA087F): GRefCountImplCore/GFxMovieView
// primary at +0x00 (vtable 0x183DA0CB0, 73 slots), GFxStateBag at +0x10
// (vtable 0x183DA0EF8). dtor 0x181CA13B0 (scalar 0x181CA2820).
//
// ===== The AS2 loadMovie pipeline (all VERIFIED, incl. at runtime 2026-07-09) =====
// AS builtin loadMovie 0x181CB8420 -> loader 0x181CA3010 (builds a
// GFxLoadQueueEntry) -> AddLoadQueueEntry 0x181CA3900. The enqueue takes the
// immediate path 0x181CA3610 ONLY when GFxStateBag::GetStateAddRef(23) returns
// a state -- and in KCD2 that state is NEVER installed (runtime probe: null for
// every native and injected loadMovie). So every entry parks in
// m_pLoadQueueHead and is serviced by the NEXT GFxMovieView::Advance
// (0x181CA4D90 calls ProcessLoadQueue at all 3 exit paths: 0x181CA519D /
// 0x181CA58BA / 0x181CA5A5D).
//
// CONSEQUENCE for injectors: a movie whose timeline is stopped in steady state
// (the KCD2 HUD) still Advances, so native loads work -- but an entry enqueued
// from C++ BETWEEN frames only loads on the next frame-step, and a movie that
// is reset first (0x181CB1490) or destroyed (dtor) FREES the pending queue
// without ever loading it. Fix: call ProcessLoadQueue() yourself right after
// Invoke("loadMovie") -- that is exactly what the game's own Advance does.
// (CompassOverhaul does this; the earlier "state-23 live-context gate" theory
// is DEAD -- the gate exists but never fires in KCD2.)
//
// Related, not wired: reset 0x181CB1490 (frees pending queue), normal-SWF
// handler 0x181CAE530 (.swf->.gfx rewrite, img:// branch, disk load via
// 0x181C28C30, logs `Error loading URL "%s"`), vars/XML/CSS handlers
// 0x181CAF1C0 / 0x181CAF370 / 0x181CAE3B0, img:// protocol 0x18080B14C,
// CryGFxFileOpener::OpenFile 0x180890D1C (logs unconditionally -- its absence
// in kcd.log proves no open was attempted).

struct GFxMovieDef;   // opaque movie definition (CFlashPlayer+0xC8 holds the player's copy)
class  GMemoryHeap;   // opaque GFx heap; vf+0x50 = Alloc(size, align), vf+0x60 = Free

class GFxMovieRoot : public GFxMovieView, public GFxStateBag {
public:
    uint8_t             _unk18[8];                  // +0x18  GFxStateBag delegate/state storage (not written by ctor/dtor; unmapped)
    void*               _unk20;                     // +0x20  owned 8-byte heap cell holding a GFxMovieRoot* back-ref (ctor 0x181CA0840 Alloc(8) + *cell=this; dtor 0x181CA13B0 frees the cell)
    GFxMovieDef*        m_pMovieDef;                // +0x28  VERIFIED (ctor 0x181CA0897)
    void*               _unk30;                     // +0x30  ctor 0x181CA0840 inits 0; no writer/reader in ctor/dtor/ProcessLoadQueue (role UNVERIFIED)
    GMemoryHeap*        m_pHeap;                    // +0x38  VERIFIED (loader allocs entries from it, 0x181CA314B)
    uint8_t             _unk40[0x18];               // +0x40
    void*               _unk58;                     // +0x58  non-owning polymorphic handle; drain 0x181CAEF50 & dtor 0x181CA13B0 test it, then call m_pLoadStates vf[12](+0x60) and this handle's vf @+0x150 (not released -> non-owning; role UNVERIFIED)
    void*               m_pLoadStates;              // +0x60  owning polymorphic load-state provider (dtor 0x181CA13B0 releases via sub_181C270D0; drain 0x181CAEF50 reads +0x28 & calls vf[12]/+0x60; name SDK-correlated)
    uint8_t             _unk68[0xD8];               // +0x68  intrusive dlist head @+0x68/+0x70 (self-ref next/prev), embedded obj ctor sub_181C20BF0 @+0xE8, 1.0f cxform floats @+0xA0..+0xAC, FLT_MAX rect @+0x110..+0x11C -- NOT FULLY MAPPED
    void*               _unk140;                    // +0x140 owning GPtr to a 96-byte GRefCount obj (vtables off_183D979B8/off_183D979C0/off_183D979E8 @+0/+0x10/+0x20, refcnt@+0x8, refptr@+0x28, CRITICAL_SECTION@+0x38); ctor 0x181CA0840 Alloc(96)+link, dtor releases via sub_181C20770; drain 0x181CAEF50 hands (_unk140+0x10) to movie-load task ctor 0x181C685E0
    uint8_t             _unk148[0x2BD0 - 0x148];    // +0x148 timeline / heaps / listeners; incl. 4x sub_181D6C900 objs @+0xA88/+0x1110/+0x1798/+0x1E20, a GRefCount obj @+0x178 (off_183DA0AF8), LoadLibrary HMODULE @+0x2BC0 (dtor FreeLibrary) -- NOT FULLY MAPPED
    GFxLoadQueueEntry*  m_pLoadQueueHead;           // +0x2BD0 pending FIFO head (a1[1402] in the drain)
    uint32_t            m_lastLoadQueueEntryCnt;    // +0x2BD8 entry-id stamp counter (0x181CA3B07)
    uint32_t            _pad2BDC;                   // +0x2BDC
    void*               m_pLoadProgressHead;        // +0x2BE0 head of intrusive dlist {vtable@+0, next@+0x8, prev@+0x10} of polymorphic load-progress nodes (node+0x20 -> obj whose +0xC8 byte = cancel); drain 0x181CAEF50 polls vf[2], culls via vf[1], destroys via vf[0](this,1)
    void*               _unk2BE8;                   // +0x2BE8 owning GPtr to a polymorphic provider (dtor 0x181CA13B0 releases via sub_181C20770); when set, 0x181D370D0 samples its vf @+0x20/+0x30/+0x38/+0x48/+0x40 -> int/float/float/float/int (role UNVERIFIED)
    void*               _unk2BF0;                   // +0x2BF0 owning GPtr to a GFx obj (ctor 0x181CA0840 inits 0; dtor 0x181CA13B0 releases via sub_181C20770); role UNVERIFIED
    // ---- wired (forwarders in RE/src/Scaleform/GFxMovieRoot.cpp) ----
    // Pops and services every pending m_pLoadQueueHead entry NOW (the exact
    // call Advance makes), then culls finished m_pLoadProgressHead tasks.
    void ProcessLoadQueue();                        // 0x181CAEF50
    // Tail-appends entry to the FIFO and stamps m_entryId -- unless the
    // (never-installed, see banner) state-23 immediate path takes it.
    void AddLoadQueueEntry(GFxLoadQueueEntry* entry);  // 0x181CA3900
};
static_assert(sizeof(GFxMovieRoot) == 0x2BF8, "GFxMovieRoot must be 11256 bytes (alloc at 0x181C622F8)");
static_assert(offsetof(GFxMovieRoot, m_pMovieDef) == 0x28, "movie def at 0x28");
static_assert(offsetof(GFxMovieRoot, m_pLoadQueueHead) == 0x2BD0, "load queue head at 0x2BD0");
static_assert(offsetof(GFxMovieRoot, m_pLoadProgressHead) == 0x2BE0, "load progress list at 0x2BE0");
