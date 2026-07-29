#pragma once
#include <cstdint>

// -----------------------------------------------
// wh::shared::C_Signal<...> -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x10 (non-polymorphic POD).
// -----------------------------------------------
// A signal head embedded by value in its owner (e.g. ~22 heads inside C_CombatActor from +0x38).
//
// [FUNDAMENTAL vs KCD1] De-virtualized and shrunk. KCD1 C_Signal was polymorphic (0x30: virtual
// dtor + inline std::vector<S_Delegate> + a sorted S_SortedConnectionEntry list). KCD2 is a 0x10
// POD: a single pointer to a header-prefixed relocatable delegate array plus two int16 guards.
// [FUNDAMENTAL vs KCD1] Connections are bare S_Delegate (0x10, no per-entry slot); Connect
// (sub_180C57A1C) de-dups by {instance, invoke} and appends. Per-signal Connect thunks now live in
// the owner's vtable (e.g. C_CombatActor slots ~109-174), forwarding into C_Signal::Connect.
//
// [EMIT WALK -- brew-result instantiation, instruction-verified 2026-07-18] Emit (alchemy twin
// 0x182E15404, REL::ID 375516) claims the FIRST idle cursor (-1 scan over +0x08/+0x0A), then
// walks NEWEST -> OLDEST: starting at i = count-1 it stores i-1 into the claimed cursor BEFORE
// invoking entry[i], and RE-READS that cursor after the callback returns as the next index
// (negative stops the walk; the cursor is then released to -1).  The re-read is what makes
// mid-emit Disconnect safe (twin 0x181732108, id 134142: linear {instance,invoke} find, then
// `if (idx <= cursor) --cursor` on BOTH cursors before the in-place erase 0x18173238C -- element
// shrink NEVER reallocates, only append grows) -- and it equally lets a callback stop the
// remaining walk by writing -1 into the cursors.  Connect twin for the same instantiation:
// 0x1817321AC (id 134144; de-dup, empty -> reserve 5 via 0x1817322DC + alloc 0x180396724, store
// 0x182420480).  The owner's dtor tears the list down by resize(0) + free of the old pool
// (C_Alchemy dtor 0x180FC0BA8).  ICF folds one body per signature family, so every instantiation
// family has its own twin addresses AND its own static empty sentinel (brew result: 0x185669440).

namespace wh::shared {

// One subscriber: an instance ("this") plus its bound free-function pointer. Field ORDER
// {instance @+0, invoke @+8} is proven by the delegate index-writer sub_182420480 (stores a3[0]
// -> entry+0, a3[1] -> entry+8) and the subscribe helper sub_180EC8510 (round-3 verify pass).
template<typename... Args>
struct S_Delegate {
    void*  m_pInstance;                  // +0x00
    void (*m_pInvoke)(void*, Args...);   // +0x08

    void operator()(Args... args) const { m_pInvoke(m_pInstance, args...); }
    bool operator==(const S_Delegate& o) const { return m_pInstance == o.m_pInstance && m_pInvoke == o.m_pInvoke; }
    bool operator!=(const S_Delegate& o) const { return !(*this == o); }
};
static_assert(sizeof(S_Delegate<>) == 0x10);

// The subscriber list: a single pointer to Entry[] whose 8-byte header sits BEFORE the data --
//   [m_connections-8] uint32 capacity   [m_connections-4] uint32 count   [m_connections+0] Entry[]
// with Entry stride 0x10. While empty, m_connections points at a shared zero-filled static sentinel
// (capacity == count == 0). This is CryEngine-style header-prefixed relocatable storage -- NOT a
// std::vector, boost container, or the KCD1 {begin,end,cap} triple.
template<typename... Args>
struct C_Signal {
    using Entry = S_Delegate<Args...>;

    // The two int16 cursors are in-flight emit iterators supporting up to 2 NESTED emits:
    // disconnect (sub_18119E00C) decrements a run of exactly 2 int16 slots at +0x08 when a
    // listener below the cursor is erased mid-emit. Both init -1 (idle).
    Entry*   m_connections;   // +0x00  -> Entry[] (header-prefixed; shared empty sentinel while idle)
    int16_t  m_emitCursor0;   // +0x08  outer emit cursor, -1 == idle
    int16_t  m_emitCursor1;   // +0x0A  nested emit cursor, -1 == idle
    uint8_t  _pad0C[4];       // +0x0C  alignment to 0x10

    // Header reads: valid for the empty sentinel too (its header is zero-filled).
    uint32_t Count() const    { return reinterpret_cast<const uint32_t*>(m_connections)[-1]; }
    uint32_t Capacity() const { return reinterpret_cast<const uint32_t*>(m_connections)[-2]; }

    // Linear scan by invoke fn (the entries have no index -- Disconnect scans exactly like this).
    Entry* Find(void (*pInvoke)(void*, Args...)) {
        for (uint32_t i = 0, n = Count(); i < n; ++i)
            if (m_connections[i].m_pInvoke == pInvoke)
                return &m_connections[i];
        return nullptr;
    }
};
static_assert(sizeof(C_Signal<>) == 0x10);

}  // namespace wh::shared
