#include <Windows.h>
#include <cstddef>
#include <cstring>
#include "REL.h"

// CryEngine SDK glue the vendored CryCommon headers reference but no game module
// provides in this build. Declarations come from the kcd.h PCH (which fixes the
// extern-C linkage of the CryModule* allocators).

void CryCreateCriticalSectionInplace(void* p) { InitializeCriticalSection(static_cast<CRITICAL_SECTION*>(p)); }
void CryDeleteCriticalSectionInplace(void* p) { DeleteCriticalSection(static_cast<CRITICAL_SECTION*>(p)); }

struct IMemoryManager;
IMemoryManager* CryGetIMemoryManager() { return nullptr; }

// Forward to WHGame.dll's OWN CryModule allocator -- the retail-compiled
// _CryMemoryManagerPoolHelper backed by the process-global BucketAllocator
// (kd7u-verified; the pool-helper fn table at 0x18549D348..78 is filled by
// sub_18045AAA0 with exactly these):
//   CryMalloc  0x4F7610  (size, &allocated, alignment) -- align 0 & size<=0x200 ->
//              bucket SLIST; bigger -> game ucrt malloc; aligned -> malloc + shift
//              byte count stored at [p-4]
//   CryRealloc 0x490CFC  (p, size, &allocated, &oldSize, alignment)
//   CryFree    0x4FD8D0  (p, alignment) -- routes BY ADDRESS: bucket-segment scan
//              (push back to SLIST) with ucrt free fallback (un-shifting via [p-4]
//              when alignment != 0), so it correctly frees ANYTHING CryMalloc
//              returns, whichever module allocated it.
// This is what makes refcounted Cry types (CryStringT!) safe to OWN across the
// module boundary in both directions: the game's CryStringT buffers come from
// this allocator, and freeing them with anything else corrupts the heap (that
// exact crash: game bucket block released by a plugin-compiled ~CryStringT
// while the stubs still pointed at plain ucrt free).
__declspec(dllexport) void* CryModuleMalloc(size_t size) throw()
{
    static REL::Relocation<void* (__fastcall*)(size_t, size_t*, size_t)> f{ REL::ID(5) };
    size_t allocated;
    return f(size, &allocated, 0);
}
__declspec(dllexport) void* CryModuleCalloc(size_t count, size_t size)
{
    void* p = CryModuleMalloc(count * size);
    if (p)
        memset(p, 0, count * size);
    return p;
}
__declspec(dllexport) void* CryModuleRealloc(void* p, size_t size) throw()
{
    static REL::Relocation<void* (__fastcall*)(void*, size_t, size_t*, size_t*, size_t)> f{ REL::ID(3) };
    size_t allocated, oldSize;
    return f(p, size, &allocated, &oldSize, 0);
}
__declspec(dllexport) void CryModuleFree(void* p) throw()
{
    static REL::Relocation<void (__fastcall*)(void*, size_t)> f{ REL::ID(6) };
    f(p, 0);
}

// Aligned pair (used by CryArray's NAlloc::StandardAlloc / StdAllocator for
// over-aligned element types). Same allocator: CryMalloc's aligned path either
// returns a naturally-aligned bucket block or a shifted malloc block; CryFree
// with alignment != 0 handles both (the shift lives at [p-4], read by the free
// itself -- callers only signal "was an aligned alloc").
__declspec(dllexport) void* CryModuleMemalign(size_t size, size_t alignment)
{
    static REL::Relocation<void* (__fastcall*)(size_t, size_t*, size_t)> f{ REL::ID(5) };
    size_t allocated;
    return f(size, &allocated, alignment);
}
__declspec(dllexport) void CryModuleMemalignFree(void* p)
{
    static REL::Relocation<void (__fastcall*)(void*, size_t)> f{ REL::ID(6) };
    f(p, 1);
}

// WHGame retail-DynArray bridge (NArray::WHDynStorage, WHDynStorage.h).  Our blocks must be
// byte-identical to the game's NArray blocks, so alloc/free go straight to the game's bucket
// allocator.

void* WHDynArrayAlloc(size_t nBytes, size_t* nActualBytes)
{
    // bucket alloc sub_1804F7610(size, &outActual, align) -- outActual is MANDATORY
    static REL::Relocation<void* (__fastcall*)(size_t, long long*, unsigned long long)> f{ REL::ID(5) };
    long long actual = 0;
    void* p = f(nBytes, &actual, 8);
    *nActualBytes = (size_t)actual;
    return p;
}

void WHDynArrayFree(void* pBlockBase)
{
    // bucket free sub_1804FD8D0(rawBase, adjusted): bucket ptrs ignore the flag; >0x200
    // malloc-fallback blocks carry a shift at [base-4] that nonzero unadjusts (pairs with align=8)
    static REL::Relocation<size_t (__fastcall*)(void*, long long)> f{ REL::ID(6) };
    f(pBlockBase, 1);
}

void* WHDynArrayDefaultAllocFn()
{
    // sub_1804A2C50 -- the single default realloc fn every game DynArray block carries at
    // [D-16]; stamped into our blocks so the game frees/grows them exactly like its own
    static void* const s_fn = (void*)REL::ID(4).address();
    return s_fn;
}
