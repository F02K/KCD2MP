#pragma once
#include <cstdint>
#include <vector>
#include <unordered_map>

// -----------------------------------------------
// wh::xgenaimodule::C_MailboxDatabase -- hand-written table-backed mailbox DB
// (NOT a template DB) (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x78 (create-site
// proven: sub_181125078: operator new(120) -> ctor sub_1811250E8; the runtime
// type is the trivial subclass C_GlobalMailBoxDatabase, singleton
// qword_185493110).
// -----------------------------------------------
// RTTI TD rva 0x4EFD740, COL 0x451DB90.  3-slot "table reload listener"
// vtable rva 0x3FD7A10 ([0] dtor sub_183208768, [1]sub_1832099A0,
// [2]sub_180C753F8 -- non-dtor roles [U]).  Caches reflection-registry table
// handles: "mailbox" (sub_1811257F4), "mailbox_filter" (sub_1811257B8), and a
// third written by sub_180C74334.  Index map key/value types [U].

namespace wh::xgenaimodule {

class C_MailboxDatabase {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_MailboxDatabase;
    virtual ~C_MailboxDatabase();   // [0] dtor sub_183208768
    virtual void MdbVf1();          // [1] sub_1832099A0 (table-reload hook) [U role]
    virtual void MdbVf2();          // [2] sub_180C753F8 (table-reload hook) [U role]

    void*              m_tblMailbox;        // +0x08  ctor 0x1811251DD: sub_1811257F4(reflectDB, CCryName("mailbox")) -> reflection-registry table handle (also pushed into m_tables)
    void*              m_tblMailboxFilter;  // +0x10  ctor 0x1811251F9: sub_1811257B8(reflectDB, CCryName("mailbox_filter")) -> reflection-registry table handle (also pushed into m_tables)
    void*              m_tblThird;          // +0x18  ctor 0x181125200: sub_180C74334(reflectDB, &m_tblThird) -> reflection-registry table handle (also pushed into m_tables) [table name U]
    std::vector<void*> m_tables;            // +0x20  the 3 table handles
    std::unordered_map<uint64_t, uint64_t> m_index;   // +0x38  (0x40) key/value types [U]
};
static_assert(sizeof(C_MailboxDatabase) == 0x78, "C_MailboxDatabase must be 0x78 (new 120 at sub_181125078)");

}  // namespace wh::xgenaimodule
