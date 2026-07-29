#pragma once
#include <cstdint>

#ifndef KCD2_ENTITYID_DEFINED
#define KCD2_ENTITYID_DEFINED
typedef unsigned int EntityId;
#endif

// -----------------------------------------------
// IActorSystem -- CryAction actor system interface (KCD2 WHGame.dll 1.5.6, kd7u)
// -----------------------------------------------
// Concrete CActorSystem RTTI .?AVCActorSystem@@ (TypeDescriptor 0x18501E9F0), size 0xB8.
// Primary vtable ??_7CActorSystem@@6B@ @ 0x184048340 (COL 0x1845E8EF0), 19 slots (0x00..0x90).
// All 19 slots VERIFIED from their function bodies; slot ORDER is IDENTICAL to KCD1 (no shuffle).
//
// CActorSystem multiple inheritance (CHD 0x1845E85D8):
//   IActorSystem        @ +0x00  primary   vtable 0x184048340 (19 slots)
//   IEntityPoolListener @ +0x08  secondary vtable 0x184048300 ( 7 slots)
//   IEntitySystemSink   @ +0x10  tertiary  vtable 0x1840482B8 ( 8 slots)
// Only IActorSystem (primary) is modeled here; the actor system is reached via
// S_GameContext+0x180, and GetActor is slot 3 (+0x18) on this primary vtable.
//
// Key CActorSystem data members: m_actorMap @+0x38 (EntityId->IActor*), m_actorCount @+0x48,
// m_pEntitySystem @+0x30, actor-class set @+0x78, m_actorParams @+0xA0, m_scanFolder @+0xB0.

struct IActor;
struct IActorIterator;
struct IItemParamsNode;
struct IEntityClass;

namespace Offsets {

struct IActorSystem {
    virtual void  Dtor(char flags) = 0;                                       // [0]  0x183560B10  scalar-deleting dtor (frees 0xB8)
    virtual void  Reset() = 0;                                                // [1]  0x180B6DF14  purge actor list + iterator pool
    virtual void  Reload() = 0;                                               // [2]  0x18357F534  Reset() then Scan(m_scanFolder)
    virtual IActor* GetActor(EntityId id) = 0;                                // [3]  0x180647E40  m_actorMap lookup by EntityId          VERIFIED
    virtual IActor* GetActorByChannelId(uint16_t channelId) = 0;              // [4]  0x1812AD520  linear scan by channelId
    virtual IActor* CreateActor(uint16_t channelId, const char* name,         // [5]  0x180B86120  spawn entity + actor extension
        const char* actorClass, const void* pos /*Vec3&*/,                    //       pos/rot/scale are Vec3&/Quat&/Vec3&
        const void* rot /*Quat&*/, const void* scale /*Vec3&*/, EntityId id) = 0;
    virtual int   GetActorCount() const = 0;                                  // [6]  0x181A727A0  returns *(this+0x48)
    virtual IActorIterator* CreateActorIterator() = 0;                        // [7]  0x181794CB0  allocs CActorIterator (0x20)
    virtual void  SetDemoPlaybackMappedOriginalServerPlayer(EntityId id) = 0; // [8]  0x181A750B0  sets *(this+0x20)
    virtual EntityId GetDemoPlaybackMappedOriginalServerPlayer() const = 0;   // [9]  0x181A727F0  returns *(this+0x20)
    virtual void  SwitchDemoSpectator(EntityId id) = 0;                       // [10] 0x183586A78  updates m_currentDemoSpectator (this+0x1C)
    virtual IActor* GetCurrentDemoSpectator() = 0;                            // [11] 0x1835711DC  map lookup keyed by this+0x1C
    virtual IActor* GetOriginalDemoSpectator() = 0;                           // [12] 0x183572678  map lookup keyed by this+0x18
    virtual void  AddActor(EntityId id, IActor* pActor) = 0;                  // [13] 0x181099150  inserts into m_actorMap
    virtual void  RemoveActor(EntityId id) = 0;                               // [14] 0x1808E0B1C  erases from m_actorMap
    virtual void  Scan(const char* folderName) = 0;                           // [15] 0x180E46CE8  CryPak scan for actor-param xml; stores folder@+0xB0
    virtual bool  ScanXML(void* xmlNodeRoot, const char* xmlFile) = 0;        // [16] 0x180E46FD4  parses "ActorParams"/"EntityClassParams"
    virtual const IItemParamsNode* GetActorParams(const char* actorClass) const = 0; // [17] 0x180BFD890  m_actorParams lookup (this+0xA0)
    virtual bool  IsActorClass(IEntityClass* pClass) const = 0;               // [18] 0x180BFA288  searches actor-class set (this+0x78)
};

}  // namespace Offsets
