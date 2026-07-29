#pragma once
#include <cstdint>

// -----------------------------------------------
// GRefCountImplCore -- Scaleform GFx refcount root (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// Statically linked Scaleform GFx 3.x; no RTTI. Named vtable survives in the
// binary: ??_7GRefCountImplCore@@6B@ (the GFxMovieRoot ctor 0x181CA0840 stores
// it at this+0x00 before rebinding the derived vtable, and inits the refcount
// dword at this+0x08 to 1 -- disasm 0x181CA0868..0x181CA0883).
//
// Refcounting is NON-virtual (AddRef/Release helpers touch +0x08 directly via
// _InterlockedExchangeAdd; release helper frees through the global GFx heap
// qword_1850C85C0 vf+0x60). Only the destructor is virtual.

class GRefCountImplCore {
public:
    virtual ~GRefCountImplCore() = default;   // [0] scalar deleting dtor in every derived vtable

    volatile int32_t m_refCount;              // +0x08  ctor inits 1
    uint32_t         _padC;                   // +0x0C
};
static_assert(sizeof(GRefCountImplCore) == 0x10, "GRefCountImplCore must be 0x10");
