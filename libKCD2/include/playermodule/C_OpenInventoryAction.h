#pragma once
#include <cstdint>
#include <cstddef>
#include "C_AlchemyActionBase.h"
#include "S_InventoryParams.h"

// -----------------------------------------------
// wh::playermodule::C_OpenInventoryAction -- open the ingredient picker (KCD2 1.5.6, kd7u).
// sizeof 0xA0 [alloc "0x9C+" -- exact allocation size UNVERIFIED; modeled 8-aligned].
// -----------------------------------------------
// RTTI .?AVC_OpenInventoryAction@playermodule@wh@@ (TD 0x184DD4A18); vtable 0x183BD10D0; ctor
// sub_1815FCC6C.  Opens the inventory item-picker filtered by m_params.m_pItemFilter (default
// qword_185332870); the chosen item lands in the RESULT fields below and drives the follow-up
// take/place.  Completion hashes "Attach"/"Detach"/"Use".

namespace wh::playermodule {

class C_OpenInventoryAction : public C_AlchemyActionBase<S_InventoryParams> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_OpenInventoryAction;

    uint64_t _pad68;        // +0x68
    uint32_t m_evAttach;    // +0x70  ~CRC32("attach")
    uint32_t m_evDetach;    // +0x74  ~CRC32("detach")
    uint32_t m_evUse;       // +0x78  ~CRC32("use")
    uint32_t _pad7C;        // +0x7C
    uint64_t m_uiResult0;   // +0x80  picker result (UI-written)
    uint64_t m_uiResult1;   // +0x88  picker result (UI-written)
    void*    m_pIndexer;    // +0x90  item indexer/registry (sub_1808D3DC4)
    uint16_t _pad98;        // +0x98
    uint8_t  m_category;    // +0x9A  ctor = 10 (picker category)
    uint8_t  _pad9B[5];     // +0x9B
};
static_assert(sizeof(C_OpenInventoryAction) == 0xA0, "C_OpenInventoryAction modeled 0xA0");
static_assert(offsetof(C_OpenInventoryAction, m_uiResult0) == 0x80, "UI result at 0x80");

}  // namespace wh::playermodule
