// Trampoline implementation — moved into kcd_re so REL::Relocation::write_branch/
// write_call (and every plugin linking kcd_re) can reach it, mirroring how
// CommonLibSSE keeps SKSE::Trampoline inside the library rather than the loader.
// The header (KCSE/Trampoline.h) and the inline KCSE::AllocTrampoline/GetTrampoline
// helpers are unchanged.
#include "KCSE/Trampoline.h"
#include <Windows.h>
#include <cassert>
#include <cstring>

bool Trampoline::create(std::size_t a_size)
{
    return create(a_size, GetModuleHandleA("WHGame.dll"));
}

bool Trampoline::create(std::size_t a_size, void* a_module)
{
    if (m_data) return false;

    auto base = reinterpret_cast<std::uintptr_t>(a_module);
    constexpr std::size_t kGiB2 = std::size_t(2) * 1024 * 1024 * 1024;

    SYSTEM_INFO si;
    GetSystemInfo(&si);
    auto granularity = static_cast<std::uintptr_t>(si.dwAllocationGranularity);

    std::uintptr_t minAddr = base > kGiB2
        ? base - kGiB2
        : reinterpret_cast<std::uintptr_t>(si.lpMinimumApplicationAddress);

    std::uintptr_t maxAddr = base + kGiB2;

    for (auto addr = base - granularity; addr >= minAddr; addr -= granularity) {
        auto* mem = VirtualAlloc(reinterpret_cast<void*>(addr), a_size,
            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (mem) {
            set_trampoline(mem, a_size, [](void* p, std::size_t) { VirtualFree(p, 0, MEM_RELEASE); });
            return true;
        }
    }

    for (auto addr = base + granularity; addr <= maxAddr; addr += granularity) {
        auto* mem = VirtualAlloc(reinterpret_cast<void*>(addr), a_size,
            MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (mem) {
            set_trampoline(mem, a_size, [](void* p, std::size_t) { VirtualFree(p, 0, MEM_RELEASE); });
            return true;
        }
    }

    return false;
}

void Trampoline::set_trampoline(void* a_trampoline, std::size_t a_size, deleter_type a_deleter)
{
    release();
    m_data = static_cast<std::byte*>(a_trampoline);
    m_capacity = a_size;
    m_size = 0;
    m_deleter = std::move(a_deleter);
}

void Trampoline::destroy()
{
    unpatch_all();
    release();
}

void* Trampoline::allocate(std::size_t a_size)
{
    assert(m_size + a_size <= m_capacity);
    auto* result = m_data + m_size;
    m_size += a_size;
    return result;
}

void Trampoline::write_5patch(std::uintptr_t a_src, std::uintptr_t a_dst, uint8_t a_opcode)
{
    auto* relay = static_cast<std::byte*>(allocate(14));

    PatchRecord rec{};
    std::memcpy(rec.original, reinterpret_cast<void*>(a_src), 5);
    rec.trampoline = relay;
    rec.size = 5;
    m_patches[a_src] = rec;

    // relay: FF 25 00 00 00 00 [8-byte absolute address]
    relay[0] = std::byte{0xFF};
    relay[1] = std::byte{0x25};
    std::memset(relay + 2, 0, 4);
    auto absAddr = static_cast<std::uint64_t>(a_dst);
    std::memcpy(relay + 6, &absAddr, 8);

    DWORD oldProt;
    VirtualProtect(reinterpret_cast<void*>(a_src), 5, PAGE_EXECUTE_READWRITE, &oldProt);
    auto* site = reinterpret_cast<std::byte*>(a_src);
    site[0] = std::byte{a_opcode};
    auto rel = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(relay) - (a_src + 5));
    std::memcpy(site + 1, &rel, 4);
    VirtualProtect(reinterpret_cast<void*>(a_src), 5, oldProt, &oldProt);
}

void Trampoline::write_6patch(std::uintptr_t a_src, std::uintptr_t a_dst, uint8_t a_modrm)
{
    auto* slot = static_cast<std::byte*>(allocate(8));

    PatchRecord rec{};
    std::memcpy(rec.original, reinterpret_cast<void*>(a_src), 6);
    rec.trampoline = slot;
    rec.size = 6;
    m_patches[a_src] = rec;

    auto absAddr = static_cast<std::uint64_t>(a_dst);
    std::memcpy(slot, &absAddr, 8);

    DWORD oldProt;
    VirtualProtect(reinterpret_cast<void*>(a_src), 6, PAGE_EXECUTE_READWRITE, &oldProt);
    auto* site = reinterpret_cast<std::byte*>(a_src);
    site[0] = std::byte{0xFF};
    site[1] = std::byte{a_modrm};
    auto rel = static_cast<std::int32_t>(reinterpret_cast<std::uintptr_t>(slot) - (a_src + 6));
    std::memcpy(site + 2, &rel, 4);
    VirtualProtect(reinterpret_cast<void*>(a_src), 6, oldProt, &oldProt);
}

void Trampoline::unpatch_all()
{
    for (auto& [addr, rec] : m_patches) {
        DWORD oldProt;
        VirtualProtect(reinterpret_cast<void*>(addr), rec.size, PAGE_EXECUTE_READWRITE, &oldProt);
        std::memcpy(reinterpret_cast<void*>(addr), rec.original, rec.size);
        VirtualProtect(reinterpret_cast<void*>(addr), rec.size, oldProt, &oldProt);
    }
    m_patches.clear();
}

void Trampoline::release()
{
    if (m_data) {
        if (m_deleter)
            m_deleter(m_data, m_capacity);
        m_data = nullptr;
        m_capacity = 0;
        m_size = 0;
    }
}
