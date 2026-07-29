#pragma once

#include <cstdint>
#include <cstring>
#include <functional>
#include <map>
#include <string>
#include <string_view>

template<typename To, typename From>
To unrestricted_cast(From a_from)
{
    union { From from; To to; } u;
    static_assert(sizeof(From) == sizeof(To));
    u.from = a_from;
    return u.to;
}

class Trampoline {
public:
    using deleter_type = std::function<void(void* a_mem, std::size_t a_size)>;

    Trampoline() = default;
    explicit Trampoline(std::string_view a_name) : m_name(a_name) {}
    Trampoline(const Trampoline&) = delete;
    Trampoline& operator=(const Trampoline&) = delete;
    Trampoline(Trampoline&& a_rhs) noexcept { move_from(std::move(a_rhs)); }
    Trampoline& operator=(Trampoline&& a_rhs) noexcept { if (this != &a_rhs) move_from(std::move(a_rhs)); return *this; }
    ~Trampoline() { release(); }

    bool create(std::size_t a_size);
    bool create(std::size_t a_size, void* a_module);

    void set_trampoline(void* a_trampoline, std::size_t a_size, deleter_type a_deleter = {});

    // Undo all patches (restore original bytes) and free the pool.
    // Call this before unloading a plugin.
    void destroy();

    [[nodiscard]] void* allocate(std::size_t a_size);

    template<class T>
    [[nodiscard]] T* allocate() { return static_cast<T*>(allocate(sizeof(T))); }

    [[nodiscard]] std::size_t capacity() const   { return m_capacity; }
    [[nodiscard]] std::size_t allocated() const   { return m_size; }
    [[nodiscard]] std::size_t free_size() const   { return m_capacity - m_size; }
    [[nodiscard]] bool empty() const              { return m_capacity == 0; }

    template<std::size_t N>
    std::uintptr_t write_branch(std::uintptr_t a_src, std::uintptr_t a_dst)
    {
        uint8_t data;
        if constexpr (N == 5) { data = 0xE9; }
        else if constexpr (N == 6) { data = 0x25; }
        else { static_assert(N == 5 || N == 6, "invalid branch size"); }
        return write_patch<N>(a_src, a_dst, data);
    }

    template<std::size_t N, class F>
    std::uintptr_t write_branch(std::uintptr_t a_src, F a_dst)
    {
        return write_branch<N>(a_src, unrestricted_cast<std::uintptr_t>(a_dst));
    }

    template<std::size_t N>
    std::uintptr_t write_call(std::uintptr_t a_src, std::uintptr_t a_dst)
    {
        uint8_t data;
        if constexpr (N == 5) { data = 0xE8; }
        else if constexpr (N == 6) { data = 0x15; }
        else { static_assert(N == 5 || N == 6, "invalid call size"); }
        return write_patch<N>(a_src, a_dst, data);
    }

    template<std::size_t N, class F>
    std::uintptr_t write_call(std::uintptr_t a_src, F a_dst)
    {
        return write_call<N>(a_src, unrestricted_cast<std::uintptr_t>(a_dst));
    }

private:
    struct PatchRecord {
        std::byte   original[6];    // saved original bytes at patch site
        std::byte*  trampoline;     // pointer into pool
        uint8_t     size;           // 5 or 6
    };

    void write_5patch(std::uintptr_t a_src, std::uintptr_t a_dst, uint8_t a_opcode);
    void write_6patch(std::uintptr_t a_src, std::uintptr_t a_dst, uint8_t a_modrm);
    void unpatch_all();

    template<std::size_t N>
    std::uintptr_t write_patch(std::uintptr_t a_src, std::uintptr_t a_dst, uint8_t a_data)
    {
        auto disp = reinterpret_cast<std::int32_t*>(a_src + N - 4);
        auto nextOp = a_src + N;
        bool isNop = *reinterpret_cast<uint8_t*>(a_src) == 0x90;
        auto original = isNop ? std::uintptr_t(0) : nextOp + *disp;

        if constexpr (N == 5) { write_5patch(a_src, a_dst, a_data); }
        else if constexpr (N == 6) { write_6patch(a_src, a_dst, a_data); }

        return original;
    }

    void move_from(Trampoline&& a_rhs)
    {
        release();
        m_patches  = std::move(a_rhs.m_patches);
        m_name     = std::move(a_rhs.m_name);
        m_deleter  = std::move(a_rhs.m_deleter);
        m_data     = a_rhs.m_data;     a_rhs.m_data = nullptr;
        m_capacity = a_rhs.m_capacity;  a_rhs.m_capacity = 0;
        m_size     = a_rhs.m_size;      a_rhs.m_size = 0;
    }

    void release();

    std::map<std::uintptr_t, PatchRecord>   m_patches;
    std::string     m_name{ "Trampoline" };
    deleter_type    m_deleter;
    std::byte*      m_data{ nullptr };
    std::size_t     m_capacity{ 0 };
    std::size_t     m_size{ 0 };
};

// ---- KCSE per-plugin trampoline helpers ----

namespace KCSE {

inline Trampoline g_pluginTrampoline;

inline void AllocTrampoline(std::size_t size) {
    g_pluginTrampoline.create(size);
}

inline Trampoline& GetTrampoline() {
    return g_pluginTrampoline;
}

}  // namespace KCSE
