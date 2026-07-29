#pragma once
#include <cstdint>

// -----------------------------------------------
// IEntitySystem -- CryEngine entity system, KCD2 binary vtable order (impl CEntitySystem).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  CEntitySystem primary vtable 0x184077048, 98 slots
// (KCD1: 104 -- independently interfuscated, NOT a truncation). Reached via
// gEnv->pEntitySystem (gEnv+0xA8). Index pin independent of strings: FindEntityByName
// (slot 16) virtual-calls vtbl+0x70 (= slot 14) to convert the mapped id -> GetEntity.
// Full 98-row evidence table: KCD2/analysis/vtables_reslot/IEntitySystem.md.
// Unnamed _vfN slots are real functions whose role is UNVERIFIED -- do not call them.
//
// [FUNDAMENTAL vs KCD1] SpawnEntity 10->12, InitEntity 11->13, GetEntity 12->14,
// FindEntityByName 15->16, RemoveEntity 18->19, GetNumEntities 19->21, GetEntityIterator
// 20->22, SendEventToAll 21->23, AddEntityEventListener 27->45, RemoveEntityEventListener
// 28->46, PauseTimers 29->31, GetMemoryStatistics 31->33, LoadEntities 38/39->40/41,
// ReserveEntityId 41->58, FindEntityByGuid 46->47, GenerateEntityId 48->18 (jumped to the
// early block). New first-class layer block [64..92]; slots [35..44]+[48] are thin
// CEntityLoadManager (+0x600268) delegates.

namespace Offsets {

struct IEntity;
typedef uint64_t EntityGUID;

struct IEntitySystem {
    virtual void Dtor(char flags) = 0;  // [0] 0x18380959C  scalar deleting dtor
    virtual void _vf1() = 0;  // [1]
    virtual void SetFlag600758(bool b) = 0;  // [2] 0x1804BE818  byte +0x600758 setter [LIKELY]
    virtual bool GetFlag600758() = 0;  // [3] 0x1805092F0  byte +0x600758 getter [LIKELY]
    virtual void _vf4() = 0;  // [4]
    virtual void PrePhysicsUpdate() = 0;  // [5] 0x180FC5F64  feeds entity-event 33 on timer [LIKELY; KCD1 3]
    virtual void Update() = 0;  // [6] 0x180A56568  per-frame; layer-heap sweep + deletions [LIKELY; KCD1 4]
    virtual void Reset() = 0;  // [7] 0x180AE2E44  "EntitySystem::Reset" [KCD1 5]
    virtual void Release() = 0;  // [8] 0x180A5F998  Reset + teardown [LIKELY; KCD1 6]
    virtual void _vf9() = 0;  // [9]
    virtual void DeletePendingEntities() = 0;  // [10] 0x180C82030  wrapper -> slot 20 [LIKELY; KCD1 8]
    virtual void* GetBreakableManager() = 0;  // [11] 0x180708440  returns +0x600248 [name UNVERIFIED]
    virtual IEntity* SpawnEntity(void* params, bool bInit) = 0;  // [12] 0x1803D1E94  "CEntitySystem::SpawnEntity" + spawn-lock refusal (EntitySystem.cpp:918)
    virtual bool InitEntity(IEntity* pEntity, void* params) = 0;  // [13] 0x1805CB0F8  init + OnSpawn sink broadcast [LIKELY; KCD1 11]
    virtual IEntity* GetEntity(uint32_t id) = 0;  // [14] 0x1804B4F98  salt math idx=id&0x3FFFF, salt=id>>18 [KCD1 12]
    virtual IEntity* GetEntityByGuidWrapper() = 0;  // [15] 0x180622598  FindEntityByGuid -> GetEntity [LIKELY]
    virtual IEntity* FindEntityByName(const char* name) = 0;  // [16] 0x180A6D6F4  SRW-locked name map _stricmp -> slot 14 [KCD1 15]
    virtual IEntity* GetEntityReservedRange(uint32_t id) = 0;  // [17] 0x1803D1CF8  reserved-id-range salt variant [UNVERIFIED; KCD1 13?]
    virtual uint32_t GenerateEntityId(bool bStaticId) = 0;  // [18] 0x1810B021C  pulls free id from salt array [LIKELY; KCD1 48!]
    virtual bool RemoveEntity(uint32_t id, bool forceNow) = 0;  // [19] 0x18047EC24  "CEntitySystem::RemoveEntity" (EntitySystem.cpp:1112) [KCD1 18]
    virtual void UpdateDeletedEntities() = 0;  // [20] 0x1820FA710  "EntitySystem::UpdateDeletedEntities" worker [KCD1 8]
    virtual uint32_t GetNumEntities() = 0;  // [21] 0x180AE310C  salt-list counter (disasm-proven) [KCD1 19]
    virtual void* GetEntityIterator() = 0;  // [22] 0x180AE2C34  allocates CEntityItMap [KCD1 20]
    virtual void SendEventToAll(void* event) = 0;  // [23] 0x180D1E26C  per-slot event dispatch [KCD1 21]
    virtual void _vf24() = 0;  // [24]
    virtual int QueryProximity(void* query) = 0;  // [25] 0x180C887A4  AABB partition-grid query [LIKELY; KCD1 24]
    virtual void _vf26() = 0;  // [26]
    virtual void _vf27() = 0;  // [27]
    virtual void _vf28() = 0;  // [28]
    virtual void AddSink(void* sink, uint32_t subscriptions) = 0;  // [29] 0x180DC006C  5 sink lists @+0x08+0x30*i, SRW-locked [KCD1 sinks]
    virtual void RemoveSink(void* sink) = 0;  // [30] 0x180DA68BC  removes from all sink lists [KCD1 sinks]
    virtual void PauseTimers(bool bPause, bool bResume) = 0;  // [31] 0x180A55964  +0x600230; shifts queued timers on unpause [LIKELY; KCD1 29]
    virtual bool IsIDUsed(uint32_t id) = 0;  // [32] 0x18380C574  locked salt-word match [LIKELY; KCD1 30]
    virtual void GetMemoryStatistics(void* pSizer) = 0;  // [33] 0x18380BF78  ICrySizer sections "Entities"/"Partition Grid"/... [KCD1 31]
    virtual void* GetSystem() = 0;  // [34] 0x181A74220  returns m_pSystem (+0x128) [LIKELY]
    virtual void _vf35() = 0;  // [35]
    virtual void _vf36() = 0;  // [36]
    virtual void BeginCreateEntities(int count) = 0;  // [37] 0x180A77E0C  "CEntityLoadManager::PrepareBatchCreation(%d)" [LIKELY]
    virtual void _vf38() = 0;  // [38]
    virtual void EndCreateEntities() = 0;  // [39] 0x180A77E00  "CEntityLoadManager::OnBatchCreationCompleted()" [LIKELY]
    virtual bool LoadEntities(void* xmlNode) = 0;  // [40] 0x18380C6F8  "...Not all entities were loaded correctly." [LIKELY; KCD1 38]
    virtual bool LoadEntitiesEx(void* xmlNode, void* seg) = 0;  // [41] 0x180A57998  LoadEntities + entity-pool prep (EntitySystem.cpp:2828) [KCD1 39]
    virtual void _vf42() = 0;  // [42]
    virtual void _vf43() = 0;  // [43]
    virtual void _vf44() = 0;  // [44]
    virtual void AddEntityEventListener(uint32_t id, int event, void* listener) = 0;  // [45] 0x1807086F4  bucket @+0x600280+0x10*event [KCD1 27]
    virtual void RemoveEntityEventListener(uint32_t id, int event, void* listener) = 0;  // [46] 0x18060829C  same bucket erase [KCD1 28]
    virtual uint32_t FindEntityByGuid(const EntityGUID& guid) = 0;  // [47] 0x18047AB4C  FNV-1a over 8B guid, map @+0x600680 [KCD1 46]
    virtual void _vf48() = 0;  // [48]
    virtual uint32_t RegisterEntityGuid(const void* guid) = 0;  // [49] 0x18380BD8C  get-or-add in guid map @+0x6003C0 [LIKELY]
    virtual void _vf50() = 0;  // [50]
    virtual void _vf51() = 0;  // [51]
    virtual void _vf52() = 0;  // [52]
    virtual void _vf53() = 0;  // [53]
    virtual void* GetEntityArchetype(const char* name) = 0;  // [54] 0x180A6E1E8  archetype mgr @+0x601AF8 [LIKELY]
    virtual void* LoadEntityArchetype(void* xmlNode) = 0;  // [55] 0x18380C72C  "EntityPrototype" xml [LIKELY]
    virtual void _vf56() = 0;  // [56]
    virtual void SerializeTimersAndLayers(void* writer) = 0;  // [57] 0x18380D90C  "Timers"/"Layers" xml [LIKELY]
    virtual void ReserveEntityId(uint32_t id) = 0;  // [58] 0x18380DD88  forced-id store @+0x600DC8 [LIKELY; KCD1 41]
    virtual void _vf59() = 0;  // [59]
    virtual void _vf60() = 0;  // [60]
    virtual void DumpEntities() = 0;  // [61] 0x18380B834  "%d entities (%d active)" (EntitySystem.cpp:3318)
    virtual void LockSpawning(bool bLock) = 0;  // [62] 0x181AB0D60  byte +0x601B24 -- the exact gate SpawnEntity checks
    virtual void _vf63() = 0;  // [63]
    virtual void* CreateLayer(const char* name) = 0;  // [64] 0x1809B5A00  200B layer alloc + name/id map insert [LIKELY]
    virtual void LoadLayers(void* xmlNode) = 0;  // [65] 0x1809B572C  "Layer" nodes: Name/Id/Physics/Specs [LIKELY]
    virtual void _vf66() = 0;  // [66]
    virtual void _vf67() = 0;  // [67]
    virtual void _vf68() = 0;  // [68]
    virtual void ResetLayers() = 0;  // [69] 0x1809B65D4  "[Sliced loading] Reset" [LIKELY]
    virtual uint32_t GetNumLayers() = 0;  // [70] 0x181AB0D50  dword +0x601B40 [LIKELY]
    virtual void* FindLayer(const char* name) = 0;  // [71] 0x1809B5C08  layer name-map lookup [LIKELY]
    virtual void* FindLayerById(uint16_t id) = 0;  // [72] 0x1808E3748  layer id-map lookup [LIKELY]
    virtual void _vf73() = 0;  // [73]
    virtual void EnableLayer(const char* name, bool enable, bool serialized) = 0;  // [74] 0x1809B5544  toggle + physics side effects [LIKELY]
    virtual bool IsLayerEnabled(const char* name, bool includeParent) = 0;  // [75] 0x18380C5F4  layer flag bits +32/+34 [LIKELY]
    virtual void _vf76() = 0;  // [76]
    virtual void _vf77() = 0;  // [77]
    virtual void _vf78() = 0;  // [78]
    virtual void _vf79() = 0;  // [79]
    virtual bool GetGuidMapEnabled() = 0;  // [80] 0x1803CF840  byte +0x6003E0 [LIKELY]
    virtual void SetGuidMapEnabled(bool b) = 0;  // [81] 0x181AB0D70  byte +0x6003E0 (gate for slot 49) [LIKELY]
    virtual void _vf82() = 0;  // [82]
    virtual void _vf83() = 0;  // [83]
    virtual void _vf84() = 0;  // [84]
    virtual void SerializeEntities(void* ser) = 0;  // [85] 0x18380C8D8  save/load "ExistingEntity: ..." [LIKELY]
    virtual uint16_t GetLayerId(const char* name) = 0;  // [86] 0x1809B56A8  layer name-map -> u16 id [LIKELY]
    virtual void _vf87() = 0;  // [87]
    virtual int GetLayerEntityCount(const char* name) = 0;  // [88] 0x18380BEAC  layer entity-vector size [LIKELY]
    virtual uint32_t GetLayerEntity(const char* name, int idx) = 0;  // [89] 0x18380BE3C  layer entity-vector element [LIKELY]
    virtual int GetLayerBitmask(void* out) = 0;  // [90] 0x18380C414  bit per active layer id [LIKELY]
    virtual void _vf91() = 0;  // [91]
    virtual void EnableLayersBySubstring(const char* substr, bool enable) = 0;  // [92] 0x18380DF78  strstr over layer names [LIKELY]
    virtual void* CreateBSPTree3D() = 0;  // [93] 0x18051E378  16B CBSPTree3D factory [LIKELY]
    virtual void _vf94() = 0;  // [94]
    virtual void _vf95() = 0;  // [95]
    virtual void _vf96() = 0;  // [96]
    virtual void _vf97() = 0;  // [97]
};

}  // namespace Offsets
