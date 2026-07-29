#pragma once
#include <cstdint>
#include <cstddef>
#include "../CryEngine/CryCommon/CryString.h"
#include "C_PickableItem.h"
#include "E_DocumentViewMode.h"

// -----------------------------------------------
// wh::entitymodule::C_DocumentItem -- the readable-document item extension (KCD2 WHGame.dll
// 1.5.6, utem).  sizeof 0x178.
// -----------------------------------------------
// RTTI .?AVC_DocumentItem@entitymodule@wh@@.  One of exactly THREE item classes registered with
// the framework item system (IGameFramework::CItemCreator<T> vtables: C_DocumentItem 0x1840B6F20,
// C_MissileWeaponItem, C_PickableItem) -- creator Create 0x1812765B0 allocates 0x178 and runs
// ctor 0x181276680 (C_PickableItem ctor sub_1803F08EC first).
//
// This is the object CItemSystem::GetItem(entityId) returns for book/document entities (the
// alchemy recipe book included: C_OpenBookAction::Activate 0x180A95AEC caches it at
// bookAction+0xA0).  The READING STATE lives in the derived tail and persists as long as the
// item exists -- ctor zero-inits verified at 0x18127672D (+0x12C word), 0x181276734 (+0x130),
// 0x181276751 (+0x170).  Readers: GetOpenRecipeId 0x1808C3C68 (recipe id iff view==RecipeDetail
// && !m_herbViewActive), reading-hint refresher 0x1808C33E8, autocook gate 0x1808C3AF4.

namespace wh::entitymodule {

class C_DocumentItem : public C_PickableItem {   // +0x000..+0x0D8
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_DocumentItem;

    void*    _unkD8;                 // +0x0D8  ctor 0; the reading ctrl stores its arg here (0x180A96C6B)
    uint32_t _unkE0;                 // +0x0E0  ctor 0
    uint32_t _padE4;                 // +0x0E4
    CryStringT<char> _strE8;         // +0x0E8  ctor empty
    void*    _unkF0;                 // +0x0F0  ctor 0
    float    _unkF8;                 // +0x0F8  ctor 1.0f
    uint32_t _padFC;                 // +0x0FC
    uint16_t _unk100;                // +0x100  ctor 0
    uint8_t  _unk102;                // +0x102  ctor 0
    uint8_t  _pad103[5];             // +0x103
    CryStringT<char> _str108;        // +0x108  ctor empty
    CryStringT<char> _str110;        // +0x110  ctor empty
    CryStringT<char> _str118;        // +0x118  ctor empty
    CryStringT<char> _str120;        // +0x120  ctor empty
    uint32_t _unk128;                // +0x128
    uint8_t  _flag12C;               // +0x12C  ctor 0 (word store with +0x12D)
    // 1 while the recipe book shows the HERB/ingredient page instead of the recipe page: the
    // hint refresher (0x1808C33E8 via sub_1808C3D20 == 1) then offers "switch to recipe"
    // ("ui_help_alchemy_reading_t_recip") and disables read_select, and GetOpenRecipeId
    // (0x1808C3C68) returns 0 -- the open recipe is not showing.
    uint8_t  m_herbViewActive;       // +0x12D
    uint8_t  _pad12E[2];             // +0x12E
    // The recipe currently/last opened in the book's detail view; written by the read_next/
    // read_prev page-navigation handlers, consumed by autocook (and Autobrew).
    uint32_t m_lastOpenRecipeId;     // +0x130
    uint8_t  _unk134[16];            // +0x134  ctor = xmmword_183A38B00 sentinel (CryGUID-shaped)
    uint32_t _unk144;                // +0x144
    uint8_t  _obj148[0x28];          // +0x148  embedded member (ctor sub_1807046CC)
    // The document's UI view kind (asset-static; see E_DocumentViewMode.h).
    E_DocumentViewMode::Type m_viewMode; // +0x170
    uint8_t  _pad171[7];             // +0x171
};
static_assert(sizeof(C_DocumentItem) == 0x178, "C_DocumentItem must be 0x178 (creator alloc 0x1812765FC)");
static_assert(offsetof(C_DocumentItem, m_lastOpenRecipeId) == 0x130, "recipe id at 0x130");
static_assert(offsetof(C_DocumentItem, m_viewMode) == 0x170, "view mode at 0x170");

}  // namespace wh::entitymodule
