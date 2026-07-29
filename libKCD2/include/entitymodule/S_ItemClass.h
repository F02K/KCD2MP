#pragma once
#include <cstddef>
#include <cstdint>
#include <vector>
#include "../CryEngine/CryCommon/CryString.h"
#include "../CryEngine/CryCommon/CryExtension/CryGUID.h"
#include "rttr/rttr_enable.h"

// -----------------------------------------------
// wh::entitymodule::S_ItemClass -- the item CLASS-definition root (KCD2 WHGame.dll 1.5.6, utem).
// sizeof 0x38.  KCD2's rename of KCD1's S_ItemData (RTTI .?AUS_ItemClass@entitymodule@wh@@).
// -----------------------------------------------
// A 30-type polymorphic tree (root + S_TypedItemClass mixin + 28 CRTP leaves via
// S_ItemClassWrapper<Derived, Parent, TypeId>, RTTR-registered -- NO integer-switch factory).
// Tree + per-type vtables/ctors/sizes + registry internals: analysis/item_data/s_itemdata_re.md.
// TypeId (E_ItemType, vf[1]/vf[3]): Misc 0, Melee 1, Missile 2, Ammo 3, Armor 4, Food 5, Money 6,
// DiceBadge 7, Document 8, CraftingMaterial 9, Herb 10, AlchemyBase 11, NPCTool 12, Ointment 13,
// Poison 14, Die 15, Helmet 16, Key 17, KeyRing 18, QuickSlotContainer 19; intermediates
// Pickable 22, Divisible 23, WeaponEquip 24, Player 25, Equippable 26, Weapon 27, Consumable 28,
// Hood 29; 21 = this root.
//
// Instances live in the global item-class database 0x185325820 (GUID-sorted vector keyed by
// m_guid, comparator 0x18046A5AC; lookup sub_180468340, register sub_18097054C, loader
// sub_1809702AC).  Reached from a live item via C_Item::m_pClassData (+0x48); accessor
// sub_1804695B4 substitutes the static default row &qword_1854AFDE0 while that is null.
// Base vtable 0x183A4EC78 (55 slots), ctor sub_180754CE4, copy sub_180D290F8, dtor sub_181AB90BC.
namespace wh::entitymodule {

class S_HerbItemClass;
class S_PickableItemClass;
class S_PlayerItemClass;

class S_ItemClass {
public:
    // Element of m_properties: 0x20-byte record (dtor 0x18110E100); purpose UNVERIFIED
    // (candidate: tagged attribute/modifier row -- vf[17] applies values by tag).
    struct S_Property { uint8_t _opaque[0x20]; };

    virtual ~S_ItemClass();                                  // [0]  deleting dtor
    virtual int32_t GetTypeId();                             // [1]  root returns 21; leaves their E_ItemType
    virtual S_ItemClass* Clone();                            // [2]  new(sizeof(T)) + copy-ctor (source of the size proofs)
    virtual bool IsType(int32_t typeId);                     // [3]  IsKindOf: matches self + ancestors
    virtual uint32_t unk4();                                 // [4]  IsType() ? 0 : 0x80000000 [INFERRED]
    virtual int64_t unk5();                                  // [5]  base: return 0
    virtual bool unk6();                                     // [6]  base: return 1
    virtual int64_t unk7();                                  // [7]  base: return 0
    virtual int64_t unk8();                                  // [8]  base: return 0
    virtual uint32_t unk9();                                 // [9]  base: return -1 (invalid-id default)
    virtual uint32_t unk10();                                // [10] base: return -1
    virtual uint32_t unk11();                                // [11] base: return -1
    virtual float GetPrice(uint32_t priceContext);           // [12] base = (float)priceContext identity; leaves override
    virtual float unk13();                                   // [13] base: return 0.0f
    virtual std::vector<S_Property>* GetProperties();        // [14] returns &m_properties
    virtual void unk15();                                    // [15] base: nullsub
    virtual void unk16();                                    // [16] class-def reload/diff [INFERRED]
    virtual void unk17();                                    // [17] tagged property applicator (serialization record)
    virtual void unk18();                                    // [18] serialize/diff helper [INFERRED]
    virtual void InitItemInstance(const CryGUID& classGuid, void* srcItem, void* newItem); // [19] item factory hook (0x180468B70)
    virtual void InitItemDefaults(void* item);               // [20] health/amount/durability from the def (0x180467FFC)
    virtual int64_t unk21();                                 // [21] base: return 0
    virtual int64_t unk22();                                 // [22] getter [INFERRED]
    virtual int64_t unk23();                                 // [23] base: return 0
    // Typed-downcast family: returns `this` when the object IS that subclass, else a shared
    // static default instance (e.g. [24] -> 0x1855DD170, [25] -> 0x1855DD210).
    virtual S_PickableItemClass* GetAsPickableItemClass();   // [24] 0x182A44E98
    virtual S_PlayerItemClass* GetAsPlayerItemClass();       // [25] 0x182A44F08 -- the UI name/icon view
    // [26]..[50]: the remaining GetAs<Subclass>ItemClass() downcasts, one per hierarchy class.
    // Per-slot identity SAMPLED not certified (adjacent same-shape run) -- keep by index only.
    virtual S_ItemClass* GetAsUnk26();  virtual S_ItemClass* GetAsUnk27();
    virtual S_ItemClass* GetAsUnk28();  virtual S_ItemClass* GetAsUnk29();
    virtual S_ItemClass* GetAsUnk30();  virtual S_ItemClass* GetAsUnk31();
    virtual S_ItemClass* GetAsUnk32();  virtual S_ItemClass* GetAsUnk33();
    virtual S_ItemClass* GetAsUnk34();  virtual S_ItemClass* GetAsUnk35();
    virtual S_ItemClass* GetAsUnk36();  virtual S_ItemClass* GetAsUnk37();
    virtual S_ItemClass* GetAsUnk38();
    // [39] certified: herb override = `return this` (ICF 0x1805F5DA0), default 0x182A44A00 =
    // TLS-lazy static default herb 0x1855DE0D0 (null m_driedItemId) -- callers: the alchemy
    // ingredient counter 0x18175A4BC + autocook substitute retry 0x181FFE040.
    virtual S_HerbItemClass* GetAsHerbItemClass();
    virtual S_ItemClass* GetAsUnk40();  virtual S_ItemClass* GetAsUnk41();
    virtual S_ItemClass* GetAsUnk42();  virtual S_ItemClass* GetAsUnk43();
    virtual S_ItemClass* GetAsUnk44();  virtual S_ItemClass* GetAsUnk45();
    virtual S_ItemClass* GetAsUnk46();  virtual S_ItemClass* GetAsUnk47();
    virtual S_ItemClass* GetAsUnk48();  virtual S_ItemClass* GetAsUnk49();
    virtual S_ItemClass* GetAsUnk50();
    virtual void OnRegister();                               // [51] finalize hook: registrar calls it pre-insert; base nullsub
    RTTR_ENABLE()  // [52..54]: get_type 0x181A6DB4C, get_derived 0x182A23F6C

    CryGUID m_guid;                        // +0x08  THE class guid: registry key, FindItemByClass
                                           //        compare (0x1808D319D), bark/presentation id
    CryStringT<char> m_name;               // +0x18  internal class name/key (name index at reg+0x90)
    std::vector<S_Property> m_properties;  // +0x20  exposed by vf[14]; element purpose UNVERIFIED
};

static_assert(sizeof(S_ItemClass) == 0x38, "S_ItemClass must be 0x38");
static_assert(offsetof(S_ItemClass, m_guid) == 0x08, "class guid at 0x08");
static_assert(offsetof(S_ItemClass, m_properties) == 0x20, "properties at 0x20");

}  // namespace wh::entitymodule
