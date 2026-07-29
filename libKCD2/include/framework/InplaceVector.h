#pragma once
#include <cstddef>
#include <new>
#include <vector>

// -----------------------------------------------
// wh::shared::InplaceVector<T,N> -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof = N*sizeof(T) + 0x30.
// -----------------------------------------------
// A std::vector<T> with inline storage for N elements (small-buffer optimization): an arena plus a
// std::vector whose stateful allocator points back at it (Howard Hinnant short_alloc idiom). Serves
// the inline buffer while it fits and is free, else the heap. Non-copyable (the allocator is
// self-referential). Used where the game keeps a fixed-capacity inline list that can spill to heap.

namespace wh::shared {

// External inline buffer an InplaceVector allocates from -- a sibling of the vector, NOT embedded in
// the allocator (the allocator merely points here).
template <class T, int N>
struct InplaceStorage {
    T    buffer[N];  // +0x00                 inline element storage
    bool inUse;      // +N*sizeof(T)          buffer currently handed out?
    T*   self;       //                       = buffer
};

// Thin stateful allocator: a pointer to an InplaceStorage. Serves the inline buffer while it fits
// and is free, else the heap; deallocate frees heap blocks only. Non-empty, so the owning vector
// keeps a leading allocator slot.
template <class T, int N>
struct InplaceAllocator {
    using value_type = T;
    template <class U>
    struct rebind { using other = InplaceAllocator<U, N>; };

    InplaceStorage<T, N>* storage;  // +0x00  -> the arena (null = always heap)

    InplaceAllocator() noexcept : storage(nullptr) {}
    explicit InplaceAllocator(InplaceStorage<T, N>* a_storage) noexcept : storage(a_storage) {}
    template <class U>
    InplaceAllocator(const InplaceAllocator<U, N>& a_rhs) noexcept
        : storage(reinterpret_cast<InplaceStorage<T, N>*>(a_rhs.storage)) {}

    [[nodiscard]] T* allocate(std::size_t a_count) {
        if (storage && !storage->inUse && a_count <= N) {
            storage->inUse = true;
            return storage->buffer;
        }
        return static_cast<T*>(::operator new(a_count * sizeof(T)));
    }
    void deallocate(T* a_ptr, std::size_t) noexcept {
        if (storage && a_ptr == storage->buffer) storage->inUse = false;
        else ::operator delete(a_ptr);
    }
    template <class U>
    bool operator==(const InplaceAllocator<U, N>& a_rhs) const noexcept {
        return static_cast<const void*>(storage) == static_cast<const void*>(a_rhs.storage);
    }
    template <class U>
    bool operator!=(const InplaceAllocator<U, N>& a_rhs) const noexcept { return !(*this == a_rhs); }
};

// The arena + a std::vector whose stateful allocator points back at it. Layout: N*sizeof(T) arena +
// 0x30 (bool/pad + self ptr + the vector's {alloc, first, last, end}).
template <class T, int N>
struct InplaceVector {
    InplaceStorage<T, N>                   _storage;  // +0x00
    std::vector<T, InplaceAllocator<T, N>> _vec;      //         { alloc(-> _storage), first, last, end }

    InplaceVector() : _vec(InplaceAllocator<T, N>{ &_storage }) {
        _storage.inUse = false;
        _storage.self = _storage.buffer;
    }
    InplaceVector(const InplaceVector&) = delete;
    InplaceVector& operator=(const InplaceVector&) = delete;

    [[nodiscard]] auto begin() { return _vec.begin(); }
    [[nodiscard]] auto end() { return _vec.end(); }
    [[nodiscard]] auto size() const { return _vec.size(); }
    [[nodiscard]] bool empty() const { return _vec.empty(); }
    [[nodiscard]] T&   operator[](std::size_t a_pos) { return _vec[a_pos]; }

    template <class... Args>
    T& emplace_back(Args&&... a_args) { return _vec.emplace_back(static_cast<Args&&>(a_args)...); }
    void push_back(const T& a_val) { _vec.push_back(a_val); }
    void clear() { _vec.clear(); }
};

}  // namespace wh::shared
