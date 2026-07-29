#pragma once

// -----------------------------------------------
// GFxStateBag -- Scaleform GFx per-movie state provider (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Statically linked Scaleform GFx 3.x; no RTTI, SDK-correlated name (the GFx 3
// interface that hands out GFxState instances by StateType via GetStateAddRef).
// GFxMovieRoot embeds it as its second base at +0x10 (ctor 0x181CA0840 binds
// the 5-slot vtable 0x183DA0EF8 at this+0x10; the base dtor 0x181CA13B0
// rebinds the same one).
//
// Slot [3] is the only slot with verified behaviour: AddLoadQueueEntry
// (0x181CA3900) gates the immediate-load path on
//   root->stateBag.GetStateAddRef(23)          (call site 0x181CA3A68)
// releasing the returned state with the refcount helper 0x181C20770.
// Runtime-verified (2026-07-09 trace build): state 23 is NEVER installed in
// KCD2 -- the call returns null for every loadMovie, native or injected, so
// entries always park in GFxMovieRoot::m_pLoadQueueHead until the next
// Advance drains them.

struct GFxState;   // opaque refcounted state object (refcount at +0x08)

class GFxStateBag {
public:
    virtual void      _vf0() = 0;                     // [0] 0x181CA95B0 (not a defined func in the idb)
    virtual void      _vf1() = 0;                     // [1] 0x181CA2094 (9-byte thunk)
    virtual void      _vf2() = 0;                     // [2] 0x180890708
    virtual GFxState* GetStateAddRef(int state) = 0;  // [3] 0x180D5EA1C  VERIFIED (enqueue gate, arg 23)
    virtual void      _vf4() = 0;                     // [4] 0x1817DE218
};
static_assert(sizeof(GFxStateBag) == 0x8, "GFxStateBag is a bare interface");
