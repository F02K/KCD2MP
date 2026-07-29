#pragma once

#include <cstdint>
#include <type_traits>

#include "Offsets.h"       // Offsets::GetBase()
#include "Offsets_RTTI.h"  // Offsets::RTTI_<Class> TypeDescriptor RVAs

// ---------------------------------------------------------------------------
// kcd_cast — a dynamic_cast that works across the mod/engine boundary using
// the game's own RTTI, modelled on CommonLibSSE's skyrim_cast.
//
// __RTDynamicCast is MSVC's dynamic_cast implementation, exported by
// vcruntime140.dll (ordinal 25). That DLL is already loaded in the WHGame
// process, and libKCD2 links it from vcruntime140.lib — so the declaration
// below resolves to the exact same routine the engine's own CRT uses.
//
// Crucially, __RTDynamicCast discovers the source module from the *object's*
// vtable[-1] -> CompleteObjectLocator -> self-RVA (verified: COL.selfRVA yields
// WHGame's image base). It never consults the caller's module. So calling it
// from libKCD2 with WHGame's TypeDescriptors behaves identically to a
// dynamic_cast compiled inside the engine — which Warhorse never did (WHGame.dll
// does not import __RTDynamicCast), so we borrow it from the shared CRT.
// ---------------------------------------------------------------------------

// __RTDynamicCast is MSVC's dynamic_cast runtime helper. The compiler PREDEFINES it
// (it's what the dynamic_cast keyword lowers to) whenever RTTI is enabled (/GR), and
// the implementation is supplied by vcruntime140.dll — already loaded in the WHGame
// process. So we don't (and must not) redeclare it: doing so trips C2382 against the
// compiler-internal declaration. We just call it. The canonical signature, for
// reference (VC\Tools\MSVC\...\include\rttidata.h):
//
//   extern "C" void* __cdecl
//   __RTDynamicCast(void* inptr, long VfDelta, void* srcType, void* targetType,
//                   int isReference) noexcept(false);

namespace Offsets::detail {
    template <class T, class = void>
    struct has_rtti : std::false_type {};
    template <class T>
    struct has_rtti<T, std::void_t<decltype(T::RTTI)>> : std::true_type {};
}

// kcd_cast<To*>(from) — returns `from` as To* if the runtime type derives from To,
// otherwise nullptr (and nullptr in -> nullptr out). To and From must be RE'd
// polymorphic types that carry a static RTTI member (Offsets_RTTI.h).
template <class To, class From>
To kcd_cast(From* from) {
    using ToClass = std::remove_pointer_t<To>;
    static_assert(std::is_pointer_v<To>, "kcd_cast: To must be a pointer type");
    static_assert(Offsets::detail::has_rtti<From>::value,
                  "kcd_cast: source type has no static RTTI member");
    static_assert(Offsets::detail::has_rtti<ToClass>::value,
                  "kcd_cast: target type has no static RTTI member");

    if (!from) return nullptr;
    // TypeDescriptor RVAs resolve per-distribution via the address library.
    return static_cast<To>(__RTDynamicCast(
        const_cast<void*>(static_cast<const volatile void*>(from)),
        0,
        reinterpret_cast<void*>(From::RTTI.address()),
        reinterpret_cast<void*>(ToClass::RTTI.address()),
        0));
}