#pragma once
#include <cstdint>

typedef unsigned int EntityId;

// CryAction item extension interface (SDK `struct IItem`).  Pointees are the per-entity item
// game-object extensions.  KCD2 registers exactly THREE item classes (IGameFramework::
// CItemCreator<T> vtables): wh::entitymodule::C_PickableItem (0xD8), C_DocumentItem (0x178,
// RE'd -- entitymodule/C_DocumentItem.h; books/documents), C_MissileWeaponItem.  The IItem
// vtable itself is not certified, so the interface stays opaque here.
struct IItem;

// -----------------------------------------------
// IItemSystem - KCD2 binary vtable order  (CryAction CItemSystem, interface subobject @+0x08)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (IDA instance "utem").
// RTTI:   .?AUIItemSystem@@ / concrete .?AVCItemSystem@@ (primary vtable 0x184052150).
// vtable: ??_7CItemSystem@@6B@_0 @ 0x1840521B0 -- exactly 22 slots ([22] = next COL).
// CCryAction::GetIItemSystem (IGameFramework slot [26]) returns CItemSystem+0x08 = this
// interface pointer.  The item registry is a hash-map at interface+0x70 keyed by EntityId
// (value = IItem* at node+0x28), mirrored in a list at interface+0xB0.
// Interfuscator caveat: SDK declaration order is unreliable for Cry* interfaces -- this order
// is the BINARY's.  Unverified slots stay _vfN (impl address as the hint).
//
// Live consumer (in-game verified): C_OpenBookAction::Activate 0x180A95AEC resolves the
// alchemy recipe book's reading state via GetItem(bookPropEntityId) @0x180A95C95.

namespace Offsets {

struct IItemSystem {
    virtual void _vf0() = 0;                                     // [0]  0x18213DD60
    virtual void _vf1() = 0;                                     // [1]  0x180DA8340
    virtual void _vf2() = 0;                                     // [2]  0x1836C1D50
    virtual void _vf3() = 0;                                     // [3]  0x1836BCAF0
    virtual void _vf4() = 0;                                     // [4]  0x1836BC80C
    virtual void _vf5() = 0;                                     // [5]  0x1811A76F0
    virtual void _vf6() = 0;                                     // [6]  0x180E47418
    virtual void _vf7() = 0;                                     // [7]  0x1813B660C
    virtual void _vf8() = 0;                                     // [8]  0x1836B4874
    virtual void _vf9() = 0;                                     // [9]  0x181A73370
    virtual void _vf10() = 0;                                    // [10] 0x1836B47E0
    virtual void _vf11() = 0;                                    // [11] 0x1836B48DC
    virtual void _vf12() = 0;                                    // [12] 0x1836B4778
    virtual void _vf13() = 0;                                    // [13] 0x1836B4D28
    virtual void _vf14() = 0;                                    // [14] 0x1836B83A4
    virtual void _vf15() = 0;                                    // [15] 0x1836B40DC
    virtual void _vf16() = 0;                                    // [16] 0x1836B5BEC
    virtual void _vf17() = 0;                                    // [17] 0x1836C141C
    virtual void _vf18() = 0;                                    // [18] 0x1836CBAD4
    // [19] 0x18070743C -- insert into the entity->item registry map.               VERIFIED
    virtual void AddItem(EntityId entityId, IItem* pItem) = 0;
    // [20] 0x1808E0320 -- erase from the registry map + the mirror list @+0xB0.    VERIFIED
    virtual void RemoveItem(EntityId entityId) = 0;
    // [21] 0x180707FA8 -- registry lookup; miss -> nullptr, hit -> node+0x28.      VERIFIED
    virtual IItem* GetItem(EntityId entityId) = 0;
};

}  // namespace Offsets
