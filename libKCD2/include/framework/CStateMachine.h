#pragma once
#include <cstdint>
#include <cstddef>
#include "C_Signal.h"

// -----------------------------------------------
// ::CStateMachine<T> -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x48.
// -----------------------------------------------
// GLOBAL-namespace template (RTTI: the bound transition callback demangles to
// CStateMachine<wh::entitymodule::C_Actor>::* -- CryAction State.h pattern, NOT wh::shared).
// Non-polymorphic: the first qword is the transition C_Signal's private empty sentinel
// (unk_185667088, all-zero, 2 xrefs), not a vtable. Embedded at C_Actor+0x58 (boundary-proven:
// next sub-object ctor at +0xA0). ctor sub_1809CEFA0; dtor sub_1808E1A34.
// Dispatch (sub_1809CF9BC / sub_18181332C): when m_isProcessing, events are queued into the ring
// FIFO and drained front-to-back afterwards; handlers are function pointers inside the current
// state's SStateIndex records, and the transition signal fires with (old, new) index at
// transition end. The ring FIFO is a bespoke CryEngine queue (pow2 capacity + head + count +
// heap slot array + a 16-byte {&self, 0} control node) -- not any MSVC STL shape.

// One state-registry record (0x28; lives inside the state hierarchy object, referenced by the
// transition signal's delegate signature).
template<class T>
struct SStateIndex {
    int32_t  m_id;         // +0x00  state id (id - 100 indexes the global state registry)
    uint32_t _pad04;       // +0x04
    void*    m_pHandler;   // +0x08  event-handler function pointer
    void*    m_links[3];   // +0x10  hierarchy links (roles unresolved)
};
static_assert(sizeof(SStateIndex<void>) == 0x28, "SStateIndex must be 0x28");

// Polymorphic heap state node (child state @+72, id @+84; exact class layout not mapped).
template<class T>
class SState;

// One queued event batch (104B heap entries; 8B id + a CryFixedArray-shaped 5-slot event array of
// 16-byte SStateEventData records; internals not fully opened).
struct SStateEventEntry;

template<class T>
class CStateMachine {
public:
    wh::shared::C_Signal<const SStateIndex<T>&, const SStateIndex<T>&>
                       m_transitionSignal;   // +0x00  fired (old, new) at transition end
    SState<T>*         m_pCurrentState;      // +0x10  current active state node (nullable)
    // --- bespoke ring-FIFO event queue (0x28) ---
    void*              m_queueCtrlNode;      // +0x18  heap 16B {&self, 0} control node
    SStateEventEntry** m_queueRing;          // +0x20  ring storage (slot -> 104B entry or null)
    size_t             m_queueCapacity;      // +0x28  power-of-2; indexed idx & (cap-1)
    size_t             m_queueHead;          // +0x30
    size_t             m_queueSize;          // +0x38
    bool               m_isProcessing;       // +0x40  reentrancy guard (set -> defer to queue)
    uint8_t            _pad41[7];            // +0x41
};
static_assert(sizeof(CStateMachine<void>) == 0x48, "CStateMachine must be 0x48");
