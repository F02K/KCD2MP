#pragma once
#include <cstdint>

// ============================================================================
//  CEntitySystem  (KCD2 WHGame.dll 1.5.6, image base 0x180000000)
// ============================================================================
//  RTTI type descriptor : .?AVCEntitySystem@@   (global namespace)
//  primary vtable       : 0x184077048  (98 slots)
//  constructor          : 0x180D25458  (thunk 0x180D25488)
//  destructor           : 0x1838090F0  (scalar deleting dtor slot0 = 0x18380959C)
//  allocation size      : 0x601BB0     (from operator-new-tracker at 0x1819B0994:
//                                        qword_18549D378(6298544=0x601BB0, &sz);
//                                        _InterlockedAdd(&dword_1851DFB18, 0x601BB0))
//
//  CEntitySystem is the CryEntitySystem manager. It is ~6 MB because it embeds
//  the fixed CEntity* salt-buffer/slot table inline:
//    m_saltBuffer @0x200 : ~0x40000 slots, 24 bytes each ({CEntity*, salt, free}),
//    indexed by (id & 0x3FFFF); salt = (id >> 18). Verified in SpawnEntity
//    (0x1803D1E94) via  a1[3*(id&0x3FFFF) + 66]  (qword index 66 = byte 0x210).
//
//  Layout recovered by decompiling the constructor (member stores -> offsets),
//  the destructor (per-pointer frees -> which offsets are heap sub-managers),
//  and the vtable simple-accessor fingerprints. Sub-object *types* are opaque
//  (represented as sized blobs / void* like KCD1's C_Actor.h); their offsets
//  and sizes are verified. Manager pointees marked /* tentative */ where role
//  is inferred rather than proven.
// ============================================================================

typedef unsigned int EntityId;
typedef uint64_t     EntityGUID;

struct IEntity;
struct ISystem;
struct ICrySizer;
struct IEntityIt;
struct SEntitySpawnParams;
class CComponentEventDistributer;
class CScriptBind_Entity;
class CEntityClassRegistry;
class CAreaManager;
class CEntityPoolManager;
class CBreakableManager;
namespace wh { class C_EntityStreamingManager; }

// ----------------------------------------------------------------------------
// IEntitySystem — lean vtable base (binary order of CEntitySystem::`vftable').
// 98 slots. Named where identified from decompilation / accessor fingerprints;
// terse _vfN for the rest. Order is the KCD2 interfuscated binary order (close
// to KCD1's but with a few insertions, e.g. SpawnEntity moved [10]->[12]).
// ----------------------------------------------------------------------------
struct IEntitySystem
{
    virtual ~IEntitySystem() = default;                                          // [0]  scalar deleting dtor 0x18380959C
    virtual void      _vf1() = 0;                                                // [1]  op on m_obj600700
    virtual void      _vf2() = 0;                                                // [2]  set byte m_flag600758
    virtual bool      _vf3() = 0;                                                // [3]  get byte m_flag600758
    virtual void      _vf4() = 0;                                                // [4]
    virtual void      _vf5() = 0;                                                // [5]  reads m_pMgr200 ([+0x90]&1)
    virtual void      _vf6() = 0;                                                // [6]
    virtual void      _vf7() = 0;                                                // [7]
    virtual void      _vf8() = 0;                                                // [8]  ClearEntitySinks (also called by dtor)  /* tentative */
    virtual void      _vf9() = 0;                                                // [9]  frees triple-vector m_vec601B88
    virtual void      _vf10() = 0;                                               // [10] thunk
    virtual void*     _vf11() = 0;                                               // [11] getter m_pMgr248
    virtual IEntity*  SpawnEntity(SEntitySpawnParams& params, bool bAutoInit = true) = 0; // [12] 0x1803D1E94 ("CEntitySystem::SpawnEntity")
    virtual bool      InitEntity(IEntity* pEntity, SEntitySpawnParams& params) = 0;       // [13] 0x1805CB0F8 (fires OnSpawn sinks)  /* tentative */
    virtual IEntity*  GetEntity(EntityId id) const = 0;                          // [14] slot called by FindEntityByName
    virtual void      _vf15() = 0;                                               // [15] wraps FindEntityByGuid
    virtual IEntity*  FindEntityByName(const char* name) const = 0;              // [16] 0x180A6D6F4 (_stricmp name map -> GetEntity)
    virtual void      _vf17() = 0;                                               // [17]
    virtual void      _vf18() = 0;                                               // [18] thunk
    virtual void      RemoveEntity(EntityId entity, bool bForceRemoveNow = false) = 0;    // [19] 0x18047EC24 ("CEntitySystem::RemoveEntity")
    virtual void      _vf20() = 0;                                               // [20]
    virtual void      _vf21() = 0;                                               // [21]
    virtual void      _vf22() = 0;                                               // [22]
    virtual void      _vf23() = 0;                                               // [23]
    virtual void      _vf24() = 0;                                               // [24] op on m_pMgr200
    virtual void      _vf25() = 0;                                               // [25]
    virtual void      _vf26() = 0;                                               // [26] guarded by m_pMgrB10
    virtual void      _vf27() = 0;                                               // [27]
    virtual void      _vf28() = 0;                                               // [28]
    virtual void      _vf29() = 0;                                               // [29]
    virtual void      _vf30() = 0;                                               // [30]
    virtual void      _vf31() = 0;                                               // [31]
    virtual void      _vf32() = 0;                                               // [32]
    virtual void      _vf33() = 0;                                               // [33]
    virtual ISystem*  GetSystem() const = 0;                                     // [34] 0x181A74220  return m_pSystem (+0x128)
    virtual void      _vf35() = 0;                                               // [35] delegate -> m_pDelegateMgr268
    virtual void      _vf36() = 0;                                               // [36] delegate -> m_pDelegateMgr268
    virtual void      _vf37() = 0;                                               // [37] delegate -> m_pDelegateMgr268
    virtual void      _vf38() = 0;                                               // [38] delegate -> m_pDelegateMgr268
    virtual void      _vf39() = 0;                                               // [39] delegate -> m_pDelegateMgr268
    virtual void      _vf40() = 0;                                               // [40] delegate -> m_pDelegateMgr268
    virtual void      _vf41() = 0;                                               // [41]
    virtual void      _vf42() = 0;                                               // [42] delegate -> m_pDelegateMgr268
    virtual void      _vf43() = 0;                                               // [43] delegate -> m_pDelegateMgr268
    virtual void      _vf44() = 0;                                               // [44] delegate -> m_pDelegateMgr268
    virtual void      AddEntityEventListener(EntityId id, int event, void* pListener) = 0;    // [45] 0x1807086F4
    virtual void      RemoveEntityEventListener(EntityId id, int event, void* pListener) = 0; // [46] 0x18060829C
    virtual EntityId  FindEntityByGuid(const EntityGUID& guid) const = 0;        // [47] 0x18047AB4C (GUID hash map @m_guidMap680)  /* tentative */
    virtual void      _vf48() = 0;                                               // [48] delegate -> m_pDelegateMgr268
    virtual void      _vf49() = 0;                                               // [49]
    virtual void*     _vf50() = 0;                                               // [50] getter m_pMgr258
    virtual void*     _vf51() = 0;                                               // [51] getter m_pMgr270
    virtual void*     _vf52() = 0;                                               // [52] getter m_pMgrAF0
    virtual void*     _vf53() = 0;                                               // [53] getter m_pMgr278
    virtual void*     _vf54() = 0;                                               // [54] getter m_pMgrAF8
    virtual void      _vf55() = 0;                                               // [55]
    virtual void*     _vf56() = 0;                                               // [56] getter m_pMgrAF8
    virtual void      _vf57() = 0;                                               // [57]
    virtual void      _vf58() = 0;                                               // [58]
    virtual void      _vf59() = 0;                                               // [59] uses m_pMgr258
    virtual void      _vf60() = 0;                                               // [60] GetEntityIterator?  /* tentative */
    virtual void      _vf61() = 0;                                               // [61]
    virtual void      LockSpawning(bool lock) = 0;                               // [62] 0x181AB0D60  set byte m_bSpawnLock (+0x601B24)
    virtual void      _vf63() = 0;                                               // [63]
    virtual void      _vf64() = 0;                                               // [64]
    virtual void      _vf65() = 0;                                               // [65]
    virtual void      _vf66() = 0;                                               // [66]
    virtual void      _vf67() = 0;                                               // [67]
    virtual void      _vf68() = 0;                                               // [68]
    virtual void      _vf69() = 0;                                               // [69]
    virtual uint32_t  _vf70() const = 0;                                         // [70] getter dword @+0x601B40
    virtual void      _vf71() = 0;                                               // [71]
    virtual void      _vf72() = 0;                                               // [72]
    virtual void      _vf73() = 0;                                               // [73]
    virtual void      _vf74() = 0;                                               // [74]
    virtual void      _vf75() = 0;                                               // [75]
    virtual void      _vf76() = 0;                                               // [76]
    virtual void      _vf77() = 0;                                               // [77]
    virtual void      _vf78() = 0;                                               // [78]
    virtual void      _vf79() = 0;                                               // [79]
    virtual bool      _vf80() const = 0;                                         // [80] getter byte m_flag601BA0
    virtual void      _vf81() = 0;                                               // [81] setter byte m_flag601BA0
    virtual void      _vf82() = 0;                                               // [82]
    virtual void      _vf83() = 0;                                               // [83]
    virtual void      _vf84() = 0;                                               // [84] stub (retn 0)
    virtual void      _vf85() = 0;                                               // [85]
    virtual void      _vf86() = 0;                                               // [86]
    virtual void      _vf87() = 0;                                               // [87] uses m_str601B38
    virtual void      _vf88() = 0;                                               // [88]
    virtual void      _vf89() = 0;                                               // [89]
    virtual void      _vf90() = 0;                                               // [90]
    virtual void      _vf91() = 0;                                               // [91]
    virtual void      _vf92() = 0;                                               // [92]
    virtual void      _vf93() = 0;                                               // [93]
    virtual void      _vf94() = 0;                                               // [94]
    virtual void      _vf95() = 0;                                               // [95]
    virtual void      _vf96() = 0;                                               // [96]
    virtual void      GetMemoryStatistics(ICrySizer* pSizer) const = 0;          // [97] 0x180D4F370 (walks m_pMgr200)  /* tentative */
};

// ----------------------------------------------------------------------------
// CEntitySystem — concrete class. Data members at verified offsets; opaque
// sub-objects as sized blobs, sub-managers as void* (pointee /* tentative */).
// ----------------------------------------------------------------------------
class CEntitySystem : public IEntitySystem
{
public:
    // --- header / sinks / small sub-objects (0x08 .. 0x200) ---
    uint8_t     m_sinks[0xF0];              // +0x008  5 sink lists (SinkMaxEventSubscriptionCount=5, 0x30 each); ctor sub_18050B734(,48,5,)
    uint8_t     m_obj0F8[0x18];             // +0x0F8  ctor sub_1817E3188 / dtor sub_181AD6BD0
    uint8_t     m_eventListenerLock[0x10];  // +0x110  SRW-guarded (Add/RemoveEntityEventListener lock this)  /* tentative */
    uint32_t    m_flags120;                 // +0x120
    uint32_t    _pad124;                    // +0x124
    ISystem*    m_pSystem;                  // +0x128  ctor a2 store; GetSystem() [slot 34]
    uint8_t     m_vec130[0x18];             // +0x130  vector; dtor sub_1804134B4
    uint8_t     m_vec148[0x18];             // +0x148  vector; dtor sub_1804134B4
    uint8_t     m_obj160[0x10];             // +0x160  ctor sub_1804DD0DC / dtor sub_180454920 (string-family)
    uint8_t     m_obj170[0x38];             // +0x170  ctor sub_18091865C / dtor sub_182416E88
    uint8_t     m_obj1A8[0x10];             // +0x1A8  ctor sub_180A2B984 / dtor sub_1805BA73C
    uint8_t     m_obj1B8[0x30];             // +0x1B8  ctor sub_18091865C / dtor sub_182416E88
    uint8_t     m_obj1E8[0x18];             // +0x1E8  dtor sub_180454920 (string-family)

    // --- embedded CEntity* salt/slot table (the ~6 MB bulk) ---
    // ctor sub_1823D5148(this+0x200). SpawnEntity indexes  slot[id & 0x3FFFF]
    // as this+0x210 + 24*idx; each 24-byte slot = { CEntity* pEntity, salt, free }.
    // this+0x200 is the CSaltBufferArray control block (checked via sub_1804B5028).
    uint8_t     m_saltBuffer[0x5FFFE8];     // +0x200 .. 0x6001E8

    uint8_t     m_vec6001E8[0x18];          // +0x6001E8  vector; dtor sub_1823C8BF0

    // --- managers / tail members (0x600200 .. 0x601BB0) ---
    CComponentEventDistributer* m_pMgr200;  // +0x600200  ctor sub_180C20DB0 (0x98); RTTI .?AVCComponentEventDistributer@@; read by slots 5/24/97
    uint8_t     m_obj600208[0x10];          // +0x600208  ctor sub_180A5448C / dtor sub_183808F10
    uint8_t     m_vec600218[0x18];          // +0x600218  vector; dtor sub_1807D5ACC
    uint8_t     m_flag600230;               // +0x600230
    uint8_t     _pad600231[7];              // +0x600231
    int64_t     m_int600238;                // +0x600238  ctor init = -100000
    CScriptBind_Entity* m_pMgr240;          // +0x600240  lazy ctor sub_18144DCB4 (0x388); RTTI .?AVCScriptBind_Entity@@; virtual dtor
    CEntityClassRegistry* m_pMgr248;        // +0x600248  lazy ctor sub_180D24580 (0x58); RTTI .?AVCEntityClassRegistry@@; getter [slot 11]
    void*       m_pMgr250;                  // +0x600250  lazy 0x10 obj {CEntitySystem* owner, void* h}; ctor sub_180E1C8D0; non-polymorphic; dtor sub_183809674  /* type unnamed */
    CAreaManager* m_pMgr258;                // +0x600258  ctor sub_1810D4FF0 (0x180); RTTI .?AVCAreaManager@@; getter [slots 50/59]
    wh::C_EntityStreamingManager* m_pMgr260; // +0x600260  ctor sub_181684A64 (0x158); RTTI .?AVC_EntityStreamingManager@wh@@
    void*       m_pDelegateMgr268;          // +0x600268  ctor sub_180D24984 (0x90); non-polymorphic POD (owner back-ptr @+0x60, 2x sub_1803F7FAC lock @+0x50/+0x80); delegate slots 35-44,48  /* type unnamed */
    void*       m_pMgr270;                  // +0x600270  ctor sub_180D24D84 (0x70); non-polymorphic POD (owner back-ptr @+0x00); getter [slot 51]  /* type unnamed */
    CEntityPoolManager* m_pMgr278;          // +0x600278  ctor sub_1816C931C (0xA8); RTTI .?AVCEntityPoolManager@@; getter [slot 53]; used by RemoveEntity
    uint8_t     m_eventListeners[0x400];    // +0x600280  64 event-listener buckets (0x10 each); ctor sub_18050B734(,16,64,)
    uint8_t     m_guidMap680[0x40];         // +0x600680  GUID->EntityId hash map (FindEntityByGuid); ctor sub_1806030C0
    uint8_t     m_obj6006C0[0x40];          // +0x6006C0  ctor sub_1806030C0 / dtor sub_1809B71D8
    uint8_t     m_obj600700[0x58];          // +0x600700  ctor sub_180D25E9C; slot 1
    uint8_t     m_flag600758;               // +0x600758  slots 2/3 (get/set)
    uint8_t     _pad600759[7];              // +0x600759
    uint8_t     m_obj600760[0x1390];        // +0x600760  large embedded sub-object; ctor sub_180D24A14; used in SpawnEntity (reserved-id path)  /* tentative */
    CBreakableManager* m_pMgrAF0;           // +0x601AF0  ctor sub_180D25E6C (0x48); RTTI .?AVCBreakableManager@@; getter [slot 52]
    void*       m_pMgrAF8;                  // +0x601AF8  ctor sub_180D24A6C (0x18); {node* head, size, void* cmp=&unk_18566A0C8}; std::set/map-like _Tree; getter [slots 54/56]  /* elem type unnamed */
    void*       m_pMgrB00;                  // +0x601B00  ptr to 0x18 3-ptr container (empty-init unknown_libname_15); dtor sub_1838095E0  /* elem type unnamed */
    void*       m_pMgrB08;                  // +0x601B08  ptr to 0x18 3-ptr container (empty-init unknown_libname_15); dtor sub_1838094BC  /* elem type unnamed */
    void*       m_pMgrB10;                  // +0x601B10  ctor sub_180D24154 (0x58); non-polymorphic (+0x00=1.0f load-factor, embeds sub_18091865C obj @+0x28); slot 26  /* type unnamed */
    void*       m_pMgrB18;                  // +0x601B18  ctor sub_18193C984 (0x138); non-polymorphic manager (allocs 2x 0x20 buckets w/ 0x80000000 sentinel + 0x68 pool)  /* type unnamed */
    EntityId    m_idForced;                 // +0x601B20  SetNextSpawnId value (consumed by SpawnEntity)
    uint8_t     m_bSpawnLock;               // +0x601B24  LockSpawning() [slot 62]; checked by Spawn/RemoveEntity
    uint8_t     _pad601B25[3];              // +0x601B25
    uint8_t     m_obj601B28[0x10];          // +0x601B28  singleton-registered (qword_185499A10); ctor sub_180D24C7C / dtor sub_183809408
    uint8_t     m_str601B38[0x10];          // +0x601B38  ctor sub_1804DD0DC; slot 87
    uint8_t     m_str601B48[0x10];          // +0x601B48  ctor sub_1804DD0DC / dtor sub_180454920
    uint8_t     m_vec601B58[0x18];          // +0x601B58  vector; dtor sub_1804134B4
    uint8_t     m_vec601B70[0x18];          // +0x601B70  vector; dtor sub_1804134B4
    uint8_t     m_vec601B88[0x18];          // +0x601B88  triple-stride vector; dtor sub_18165D134; cleared by slot 9
    uint8_t     m_flag601BA0;               // +0x601BA0  slots 80/81 (get/set)
    uint8_t     _pad601BA1[7];              // +0x601BA1
    uint8_t     m_flag601BA8;               // +0x601BA8
    uint8_t     _pad601BA9[7];              // +0x601BA9
};
// WARNING / UNVERIFIED: CEntitySystem embeds a huge inline fixed-size entity array, so the
// object is genuinely multi-MB. But its deleting-destructor (vtable[0]=0x18380959c) frees via a
// size-tracking allocator (qword_18549D358, size arg = 0), so there is NO operator-delete size to
// confirm the exact extent -- and the base dtor (0x1838090f0) touches members past 0x601C48, i.e.
// BEYOND this value. Treat 0x601BB0 as an approximate LOWER BOUND; pin the true size from the
// allocation site before relying on any tail member. (All other foundation classes' sizeof come
// from a real operator delete(this,N) and are verified.)
static_assert(sizeof(CEntitySystem) == 0x601BB0, "CEntitySystem size is APPROXIMATE/unverified");
