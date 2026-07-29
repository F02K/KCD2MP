#pragma once
#include <cstdint>
#include <vector>
#include "WUID.h"

// -----------------------------------------------
// wh::shared::C_Listeners<Listener,N> / C_DependentListeners<Listener,Key,N> -- KCD2 1.5.6 (kd7u).
// -----------------------------------------------
// The engine's reusable safe-iteration listener registry (verified against sub_1803A4BF0 /
// sub_1803A4CA0 and the dispatch helpers sub_181EDBFC0 / sub_180E60A74).
//
//   C_Listeners<Listener,N>          = a std::vector<Listener*> plus an inline int32 cursor stack
//                                      (one slot per possible reentrant-dispatch nesting level,
//                                      -1 == free). Polymorphic (one virtual: the deleting dtor).
//                                      sizeof = roundup8(0x20 + 4*N).
//   C_DependentListeners<L,Key,N>    = C_Listeners<L,N> + two parallel per-depth keyed guard
//                                      stacks (S_DispatchScope[N] each), so a listener keyed by a
//                                      subject WUID auto-unsubscribes when that subject dies.
//                                      sizeof = 0x20 + 0x24*N.  (N=4 -> 0xB0, N=2 -> 0x68.)

namespace wh::shared {

// One reentrancy-guard record (0x10). The primary stack uses m_state (0/1) + m_flags
// ((m_flags & 5) == 5 => live & key-installed); the secondary stack reuses m_state's low byte as an
// "active" liveness flag. Key is wh::framework::WUID for both concrete instantiations.
template<typename Key>
struct S_DispatchScope {
    Key      m_key;         // +0x00  keyed subject (compared as a full qword)
    int32_t  m_state;       // +0x08  primary: 0/1; secondary: low byte = active
    uint8_t  m_flags;       // +0x0C  bit0=dispatching, bit2=keyInstalled
    uint8_t  _pad0D[3];     // +0x0D
};

template<typename Listener, int N>
class C_Listeners {
public:
    virtual ~C_Listeners() = default;              // +0x00  (one virtual; deleting dtor)
    std::vector<Listener*> m_delegates;            // +0x08  subscriber pointers
    int32_t                m_dispatchCursor[N];    // +0x20  per-nesting-level iteration cursor (-1=free)
};

template<typename Listener, typename Key, int N>
class C_DependentListeners : public C_Listeners<Listener, N> {
public:
    S_DispatchScope<Key> m_primaryScopes[N];       // +0x20+4N   keyed dispatch guard stack
    S_DispatchScope<Key> m_secondaryScopes[N];     // +0x20+4N+0x10N  secondary keyed guard stack
};

// Size formulas proven at the live instantiations (deleting dtors free 48 / 176; the C_ItemCollection
// ctor field span pins the N=2 dependent variant at 0x68).
static_assert(sizeof(C_Listeners<void, 4>) == 0x30, "C_Listeners<L,4> must be 0x30");
static_assert(sizeof(C_DependentListeners<void, wh::framework::WUID, 4>) == 0xB0, "C_DependentListeners<L,K,4> must be 0xB0");
static_assert(sizeof(C_DependentListeners<void, wh::framework::WUID, 2>) == 0x68, "C_DependentListeners<L,K,2> must be 0x68");

}  // namespace wh::shared
