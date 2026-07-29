#pragma once
#include "../crysystem/CScriptableBase.h"

// -----------------------------------------------
// wh::xgenaimodule::C_ScriptBindXGenAIModule -- Lua "XGenAIModule" global table
// (KCD2 1.5.6, kd7u).  sizeof 0x78 (alloc 120).
// -----------------------------------------------
// RTTI TD rva 0x4FE89C0; vtable 0x183ab8000 : ::CScriptableBase (0x60).
// ctor sub_180E30A4C: SetGlobalName("XGenAIModule") via sub_180B85140, Init flag 0
// (global bind), method registration sub_180A4B8F4. Creator sub_180E30920 = lazy
// module-global singleton qword_1854961E0 (allocator fn-ptr qword_18549D378(120)) --
// NOT stored in C_XGenAIModule.
//
// Lua fn table (registered by sub_180A4B8F4; * = cheat lever):
//   OnSpawn/OnDestroy/OnPropertyChange/OnStart          sub_180C820A0 (shared stub)
//   SendMessageToEntity(wuid,type,value)                sub_18196F4D0
//   SendMessageToEntityData(...)                        sub_1819DD418
//   SendMessageToEntityArray(...)                       sub_1819DD478
//   GetEntityByWUID(wuid)                               sub_18082B548
//   GetEntityIdByWUID(wuid)                             sub_183421DB0
//   SpawnPerceptibleVolume(pos,r,h,vis,consp,label,timer,worldTime)      sub_180D4515C
//   SpawnPerceptibleVolumeOnWUID(wuid,r,h,vis,consp,label,timer,wTime)   sub_183423BA8
//   DespawnPerceptibleVolume(wuid)                      sub_1819DFBDC
//   IgnorePerception(wuid,wuid,timeout,worldTime,uninterruptible)        sub_183422D7C  *
//   IsPointInAreaWithLabel(pos,label)                   sub_183422F18
//   IsPointInAreaWithLabelWUID(wuid,label)              sub_183423068
//   LootInventoryBegin(wuid) / LootEnd(wuid)            sub_183423380 / sub_183423340
//   GetWuidDebugString(wuid)                            sub_183422B90
//   GetMyWUID(table)                                    sub_1819771B8
//   GetBrainVariable(wuid,name)                         sub_183421CE8  * brain-var read
//   SetBrainVariable(wuid,name,data)                    sub_1834239D4  * brain-var write
//   MakeTableFromType(typeName)                         sub_1834233EC
//   _GetDataVariable(name) / _SetDataVariable(name)     sub_18041C484 / sub_1812E0FCC
//   ProduceSound(int,pos,mult)                          sub_180B6F7FC  * fake noise
//   ProduceSoundWUID(int,wuid,mult)                     sub_180B6F76C  *
//   AddRecordedIntellectForFaderProfiling(wuid)         sub_180C820A0 (stub)
//   PlaceToSlotFromInventory(itemWuid,slotWuid)         sub_183423590
//   SetPlayerDogMode(sMode)                             sub_182138CE4  *
//   AddLink(fromId,toId,linkTag)                        sub_1817D08EC  *
//   FindLinks(fromId,linkTag)                           sub_1804D100C  *
//   HasClothingLink(wuid)                               sub_183422BE8
//   GetOwner(linkableId)                                sub_1816544D4  * ownership queries
//   IsStanceAvailable/IsUnstanceAvailable/IsOneshotAvailable          sub_1834230FC/...31CC/sub_18087072C
//   IsStanceBlocked/IsUnstanceBlocked/IsOneshotBlocked                sub_183423168/...3284/sub_18153DE34
// Plus a 28-iteration loop registering 28 enum constants from off_18492EC80 into the
// table (XGenAIModule.<CONST>; enum identity UNRESOLVED).

namespace wh::xgenaimodule {

class C_ScriptBindXGenAIModule : public ::CScriptableBase   // +0x00  (0x60)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ScriptBindXGenAIModule;
    Offsets::ISystem* m_pSystem;   // +0x60  ISystem* (gEnv->pSystem); ctor sub_180E30A4C stores it; its vf[0x278]=GetIScriptSystem feeds +0x68
    Offsets::IScriptSystem* m_pScriptSystem;   // +0x68  pSS (duplicate of base m_pSS @+0x50); passed to Init sub_18144CE0C (CreateTable[13]+AddRef)
    void* m_p70;             // +0x70  ctor 0 [role UNVERIFIED]
};
static_assert(sizeof(C_ScriptBindXGenAIModule) == 0x78, "C_ScriptBindXGenAIModule must be 0x78 (alloc 120)");

}  // namespace wh::xgenaimodule
