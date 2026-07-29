#pragma once

#include <cstdint>
#include "../Offsets/vtables/IEntity.h"   // Offsets::IEntity (158-slot binary vtable) + EntityId

struct IEntityClass;
struct IEntityArchetype;

// -----------------------------------------------
// CEntity - CryEntitySystem concrete entity (CryEngine 3 + Warhorse, KCD2)
// -----------------------------------------------
// RTTI:   .?AVCEntity@@                 TypeDescriptor 0x185042118
// vtable: 0x184072c78  (158 slots; single base = IEntity)   COL 0x184626b88
// ctor:   0x1803ce3ec  (body sub_1803CE394)
// dtor:   0x1808e2e00  scalar-deleting -> operator delete(this, 248)  => sizeof = 0xF8
// Target: WHGame.dll 1.5.6 (kd7u), image base 0x180000000
//
// CEntity single-inherits IEntity, so the object starts with one vptr
// (CEntity::`vftable') at +0x00 and the first data member lands at +0x08.
// Layout recovered by decompiling the constructor (member stores -> offsets/types)
// and confirming the simple accessor slots against the vtable fingerprints
// (GetId@0xC, GetGuid@0x10, GetClass@0x20, GetArchetype@0x28, GetFlags@0x18,
//  GetFlagsExtended@0x1C, GetWorldTMPtr->0x58, GetLocal{Pos,Rot,Scale}Ptr->0x30/0x3C/0x4C,
//  GetWorldPos reads 0x64/0x74/0x84, AI-id triad@0x94, GetName->0xE0, keep-alive@0xF0).
//
// Vec3 / Quat / Matrix34 are the real Cry_Math.h SDK types, provided (complete) by the
// KCD2 kcd.h umbrella that force-includes this header (mirrors libKCD1's C_Actor.h).
// The two name fields are CryStringT<char> (a single char* each); kept as void* here,
// exactly like C_Actor.h's CryString members, so the header stands alone pre-umbrella.

class CEntity : public Offsets::IEntity
{
public:
    // +0x00 : vptr (CEntity::`vftable') supplied by Offsets::IEntity

    // Packed internal state bitfield (active / bounds-valid / hidden / invisible /
    // initialized / xform-dirty ...). bit22 (0x400000) = world-TM / forward-dir dirty.
    // ctor: |=0x2000000, then &=0xFE400000, |=0x400000.
    uint32_t     m_internalFlags;      // +0x08   internal state bits (slots 16,21,53,54,64,66,73,126,140,153)
    EntityId     m_id;                 // +0x0C   GetId()            [slot 1]              VERIFIED
    uint64_t     m_guid;               // +0x10   GetGuid() (EntityGUID) [slot 2]          VERIFIED
    uint32_t     m_flags;              // +0x18   EEntityFlags;   Get/Set/Add/Clear/Check  VERIFIED
    uint32_t     m_flagsExtended;      // +0x1C   EEntityFlagsExtended; Get/Set/CheckExt   VERIFIED
    IEntityClass* m_pClass;            // +0x20   GetClass() [slot 3]; ctor <- SpawnParams+0x18          VERIFIED
    IEntityArchetype* m_pArchetype;    // +0x28   GetArchetype [slot 4]; ctor <- SpawnParams+0x20        VERIFIED

    // Local transform (initialised from SpawnParams vPosition / qRotation / vScale).
    Vec3         m_localPos;           // +0x30   GetLocalPosPtr()   [slot 40]             VERIFIED
    Quat         m_localRot;           // +0x3C   GetLocalRotPtr()   [slot 42]             VERIFIED
    Vec3         m_localScale;         // +0x4C   GetLocalScalePtr() [slot 44]             VERIFIED

    // World transform: row-major 3x4; translation column m03/m13/m23 = +0x64/+0x74/+0x84.
    Matrix34     m_worldTM;            // +0x58   GetWorldTMPtr()    [slot 32]             VERIFIED
    Vec3         m_forwardDir;         // +0x88   cached forward dir; GetForwardDir [slot 49]
                                       //         (recomputed from m_localRot when bit 0x400000 is set)

    uint32_t     m_aiObjectID;         // +0x94   tAIObjectID; HasAI / Get / SetAIObjectID [slots 68-70] VERIFIED

    void*        m_pProxyContainer;    // +0x98   ptr to heap {first,last,..} range of 8-byte elems; slot25 sub_18041F6B4 returns (last-first)>>3; read by slots 25-29; ctor=0  /* pointee unresolved */
    void*        m_pParent;            // +0xA0   owned refcounted iface; dtor sub_1803A7D74 Release via pointee vtbl slot3; getter slot85; ctor=0  /* pointee unresolved; NOT confirmed a parent */
    // +0xA8..+0xB7  std::map (MSVC _Tree, always 0x10) = entity proxy/component map, keyed by
    //   proxy-type id (proxy vfn slot7). Built in ctor via sub_1803CEDA0 / sub_1803CE9A8 (keys 2, 19, ..);
    //   dtor _Tidy sub_1808E3168 @0x1808E30F4 recursively frees nodes then frees the sentinel.
    //   NOTE: this is the proxy MAP, not an event-listener list (prior guess was wrong).
    struct SProxySlot { void* _p0; void* _pOwner; };   // 0x10 mapped_type; +8 is a CryEngine-refcounted ptr (Release=sub_1804F6588)
    std::map<int32_t, SProxySlot>  m_proxyMap;         // +0xA8   _Tree head node @+0xA8, _Mysize @+0xB0     /* value class unresolved */
    std::vector<SProxySlot>        m_proxyVec;         // +0xB8   {first@+0xB8, last@+0xC0, end@+0xC8} (24 bytes)
                                                       //         dtor destroys range (sub_1805B8164, stride 0x10, Release elem+8) then frees buffer  /* elem class unresolved */
    void*        m_pPhysics;           // +0xD0   IPhysicalEntity*? GetPhysics (slots 117/157)   /* tentative */
    void*        m_unkD8;              // +0xD8   ctor=0; no accessor references it in the 158-slot vtable  /* unresolved */

    CryStringT<char> m_name;               // +0xE0   CryStringT<char>; GetName() = *(char**)0xE0 [slot 18] VERIFIED
    CryStringT<char> m_str_E8;             // +0xE8   CryStringT<char> (second string; ctor = empty) /* tentative */

    uint16_t     m_keepAliveCounter;   // +0xF0   Inc/Dec/Reset/IsKeptAlive [slots 129-132] (NOT a refcount)
    uint16_t     m_hideCounter;        // +0xF2   read as word by slots 14/15                     /* tentative */
    uint16_t     m_word_F4;            // +0xF4   get/set word by slots 135/136                   /* tentative */
    uint8_t      m_byte_F6;            // +0xF6   bool getter slot 13; ctor <- SpawnParams+0x48   /* tentative */
    uint8_t      m_byte_F7;            // +0xF7   ctor <- SpawnParams+0x49                          /* tentative */
};
static_assert(sizeof(CEntity) == 0xF8, "CEntity size must be 0xF8 (operator delete 248)");
