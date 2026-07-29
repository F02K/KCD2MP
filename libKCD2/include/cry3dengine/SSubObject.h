#pragma once
#include <cstddef>
#include <cstdint>

namespace Offsets {
struct IStatObj;
struct IFoliage;
}

// -----------------------------------------------
// SSubObject -- CStatObj compound sub-object record, KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Element of CStatObj::m_subObjects (+0x200/+0x208/+0x210). sizeof == 0xB0 proven by
// GetSubObjectCount's `(end-begin)/0xB0` (0x18078F7C8) and GetSubObject's `begin + 0xB0*i`.
// Field evidence (two independent dossiers):
//   +0x00 nType          -- Invalidate counts nType==0 with a statobj (0x1835254E6)
//   +0x08 name           -- FindSubObject* string compares against subobj+0x08
//   +0x10 properties     -- "group"/"pieces"/"breaker" property reads
//   +0x1C tm             -- GetHelperTM returns subobj+0x1C; GetHelperPos reads +0x28/+0x38/+0x48
//                           (the translation column of a 4-aligned Matrix34)
//   +0x4C localTM        -- CBrush::PhysicalizeFoliage composes m_Matrix with subobj+0x4C
//   +0x80 pStatObj       -- AddSubObject stores + AddRefs here; RemoveSubObject releases
//   +0xA0 pFoliage       -- CBrush::PhysicalizeFoliage passes &subobj->pFoliage (+0xA0)
// The unlabeled gaps carry no observed access; they are explicit unknowns, not proven padding.

struct SSubObject
{
    int32_t          nType = 0;          // +0x00  0 = mesh
    uint32_t         _unk04 = 0;         // +0x04
    CryStringT<char> name;               // +0x08
    CryStringT<char> properties;         // +0x10
    uint32_t         _unk18 = 0;         // +0x18
    Matrix34         tm;                 // +0x1C  world/helper transform
    Matrix34         localTM;            // +0x4C
    uint32_t         _unk7C = 0;         // +0x7C
    Offsets::IStatObj* pStatObj = nullptr;  // +0x80  ref-counted (refcount at CStatObj+0x50)
    uint8_t          _unk88[0x18] = {};  // +0x88 .. 0x9F  unknown
    Offsets::IFoliage* pFoliage = nullptr;  // +0xA0  per-subobject foliage (PhysicalizeFoliage out)
    uint8_t          _unkA8[0x08] = {};  // +0xA8 .. 0xAF  unknown (+0xA0 sibling cleared on CopySubObject)
};
static_assert(sizeof(SSubObject) == 0xB0, "stride proven by GetSubObjectCount/GetSubObject");
static_assert(offsetof(SSubObject, tm) == 0x1C, "tm @+0x1C (GetHelperTM)");
static_assert(offsetof(SSubObject, localTM) == 0x4C, "localTM @+0x4C (PhysicalizeFoliage)");
static_assert(offsetof(SSubObject, pStatObj) == 0x80, "pStatObj @+0x80");
static_assert(offsetof(SSubObject, pFoliage) == 0xA0, "pFoliage @+0xA0");
