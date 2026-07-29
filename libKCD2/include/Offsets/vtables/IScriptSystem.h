#pragma once
#include <cstdint>
#include <cstddef>

// -----------------------------------------------
// IScriptSystem - KCD2 binary vtable order  (CryScriptSystem CScriptSystem)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (base 0x180000000).
// RTTI:   .?AVCScriptSystem@@     primary vtable 0x183b8c610 (69 slots)
// Singleton: global CScriptSystem* @ 0x18549CFB0 (set by ctor); Lua VM @ 0x18549CFA0.
//
// The interface is interfuscator-shuffled; slot identities below were recovered by
// decompiling each thunk on kd7u (instance "kd7u"), NOT ported blindly from KCD1.
// Order is mostly preserved vs KCD1's 67-slot table with two insertions
// (a func-ref slot ~[25] and a returns-0 stub at [67]); ~40 slots are behaviourally
// confirmed (VERIFIED), the rest are inferred from KCD1 position/fingerprint (tentative).
//
// Every value-passing method drives the shared Lua stack held at this+0x10 (lua_State*).
// ScriptAnyValue is 0x18 bytes here (see CryCommon/IScriptSystem.h); EndCallAnyN [21]
// iterates results with stride 0x18, confirming that size.

typedef void*          HSCRIPTFUNCTION;   // opaque Lua registry function reference
typedef struct lua_State lua_State;

struct ScriptAnyValue;   // 0x18-byte type-discriminated payload (crysystem/ScriptAnyValue.h)
struct ISerialize;
class  CScriptableBase;

namespace Offsets {

struct IScriptTable;     // Offsets/vtables/IScriptTable.h

struct IScriptSystem {
    // ---- lifecycle ----
    virtual void _vf0() = 0;                                                       // [0]  0x000  scalar-deleting dtor; frees 0x43D8                          VERIFIED
    virtual void Update() = 0;                                                     // [1]  0x008  per-frame update                                           tentative
    virtual void SetGCFrequency(float fRate) = 0;                                  // [2]  0x010  store m_fGCFreq @this+0xA0                                 VERIFIED

    // ---- environment ----
    virtual void SetEnvironment(HSCRIPTFUNCTION scriptFunction, IScriptTable* pEnv) = 0;   // [3]  0x018                                            tentative
    virtual IScriptTable* GetEnvironment(HSCRIPTFUNCTION scriptFunction) = 0;      // [4]  0x020                                                             tentative

    // ---- script execution / management ----
    virtual bool ExecuteFile(const char* sFileName, bool bRaiseError,
                             bool bForceReload, IScriptTable* pEnv) = 0;           // [5]  0x028                                                             tentative
    virtual bool ExecuteBuffer(const char* sBuffer, size_t nSize,
                               const char* sBufferDesc, IScriptTable* pEnv) = 0;   // [6]  0x030                                                             tentative
    virtual void UnloadScript(const char* sFileName) = 0;                          // [7]  0x038                                                             tentative
    virtual void UnloadScripts() = 0;                                              // [8]  0x040                                                             tentative
    virtual bool ReloadScript(const char* sFileName, bool bRaiseError) = 0;        // [9]  0x048                                                             tentative
    virtual bool ReloadScripts() = 0;                                              // [10] 0x050  walks m_pLoadedScripts @this+0x60                          tentative
    virtual void DumpLoadedScripts() = 0;                                          // [11] 0x058  walks m_pLoadedScripts @this+0x60                          tentative

    // NOTE: overload groups are DE-OVERLOADED (distinct names per slot). MSVC
    // allocates vtable slots for adjacent same-name overloads in REVERSE
    // declaration order, so keeping the engine's overloaded names here makes
    // OUR compiled calls land on the mirrored slot: BeginCall(HSCRIPTFUNCTION)
    // dispatched to [15] BeginCallTable and crashed in CScriptTable::PushRef
    // 0x180715F64 (ref int dereferenced as table). Same rule as ICryPak's
    // FOpen/FOpen_GamePath.

    // ---- tables ----
    virtual IScriptTable* CreateTable(int nArraySize, int nHashSize) = 0;          // [12] 0x060  lua_createtable(narr,nrec)                                 VERIFIED
    virtual IScriptTable* CreateTableEmpty(bool bEmpty) = 0;                       // [13] 0x068  SDK CreateTable(bool) overload                             VERIFIED
    virtual IScriptTable* AllocScriptTable() = 0;                                  // [14] 0x070  allocs 0x18 wh::scriptsystem::C_ScriptTableTemplate         tentative

    // ---- function calls ----
    virtual bool BeginCallTable(IScriptTable* pTable, const char* sFuncName) = 0;  // [15] 0x078  push table, index func (0x1808FE42C)                       VERIFIED
    virtual bool BeginCallTableName(const char* sTableName,
                                    const char* sFuncName) = 0;                    // [16] 0x080  push global table, index func                              VERIFIED
    virtual bool BeginCallGlobal(const char* sFuncName) = 0;                       // [17] 0x088  push global func                                           VERIFIED
    virtual bool BeginCall(HSCRIPTFUNCTION hFunc) = 0;                             // [18] 0x090  push registered func ref (0x180712B1C)                     VERIFIED
    virtual bool EndCall() = 0;                                                    // [19] 0x098  thunk -> EndCallAnyN(0,nullptr)                            VERIFIED
    virtual bool EndCallAny(ScriptAnyValue& any) = 0;                             // [20] 0x0A0  call, pop 1 result                                          VERIFIED
    virtual bool EndCallAnyN(int n, ScriptAnyValue* anys) = 0;                    // [21] 0x0A8  call, pop n results (stride 0x18)                          VERIFIED

    // ---- function references ----
    virtual HSCRIPTFUNCTION GetFunctionPtrTable(IScriptTable* pTable,
                                                const char* sFuncName) = 0;        // [22] 0x0B0  (de-overloaded, see note above)                            VERIFIED
    virtual HSCRIPTFUNCTION GetFunctionPtrTableName(const char* sTableName,
                                                    const char* sFuncName) = 0;    // [23] 0x0B8  (de-overloaded)                                            VERIFIED
    virtual HSCRIPTFUNCTION GetFunctionPtr(const char* sFuncName) = 0;             // [24] 0x0C0                                                             VERIFIED
    virtual HSCRIPTFUNCTION AddFuncRef(HSCRIPTFUNCTION f) = 0;                     // [25] 0x0C8  add reference (KCD2-inserted slot)                         VERIFIED
    virtual bool CompareFuncRef(HSCRIPTFUNCTION f1, HSCRIPTFUNCTION f2) = 0;       // [26] 0x0D0                                                             VERIFIED
    virtual void ReleaseFunc(HSCRIPTFUNCTION f) = 0;                              // [27] 0x0D8  luaL_unref                                                 VERIFIED

    // ---- any-value helpers ----
    virtual ScriptAnyValue CloneAny(const ScriptAnyValue& src) = 0;                          // [28] 0x0E0  deep-refs tables/funcs            VERIFIED
    virtual void ReleaseAny(const ScriptAnyValue& any) = 0;                        // [29] 0x0E8  releases ref if function/table                             VERIFIED
    virtual void PushFuncParamAny(const ScriptAnyValue& any) = 0;                 // [30] 0x0F0  push param; ++m_nParamCount @this+0x28                     VERIFIED

    // ---- globals ----
    virtual void SetGlobalAny(const char* sKey, const ScriptAnyValue& any) = 0;    // [31] 0x0F8                                                             VERIFIED
    virtual bool GetGlobalAny(const char* sKey, ScriptAnyValue& any) = 0;          // [32] 0x100  supports dotted 'a.b.c' paths                              VERIFIED
    virtual void SetGlobalToNull(const char* sKey) = 0;                            // [33] 0x108  -> SetGlobalAny(key, nil)                                  VERIFIED

    // ---- user data / GC ----
    virtual IScriptTable* CreateUserData(const void* pData, size_t nSize) = 0;     // [34] 0x110  lua userdata wrapped in CScriptTable                       VERIFIED
    virtual void ForceGarbageCollection() = 0;                                     // [35] 0x118                                                             tentative
    virtual int  GetCGCount() = 0;                                                 // [36] 0x120  lua GC total bytes >> 10 (KB)                              VERIFIED
    virtual void SetGCThreshhold(int nKb) = 0;                                     // [37] 0x128  nop stub in this build                                     tentative
    virtual void Release() = 0;                                                    // [38] 0x130                                                             tentative

    // ---- debug / diagnostics (interfuscated; names inferred) ----
    virtual void ShowDebugger(const char* pszSourceFile, int iLine, const char* pszReason) = 0; // [39] 0x138                                     tentative
    virtual void _vf40(int a, int b) = 0;                                          // [40] 0x140  reads m_pCallStackTable @this+0x43D0; -> 0                 VERIFIED(read)
    virtual void _vf41() = 0;                                                      // [41] 0x148  debug (408b)                                               tentative
    virtual void _vf42() = 0;                                                      // [42] 0x150  debug (350b)                                               tentative
    virtual void _vf43() = 0;                                                      // [43] 0x158  debug (71b)                                                tentative
    virtual void _vf44() = 0;                                                      // [44] 0x160  nop stub
    virtual void _vf45() = 0;                                                      // [45] 0x168  nop stub
    virtual void _vf46() = 0;                                                      // [46] 0x170  nop stub
    virtual void _vf47() = 0;                                                      // [47] 0x178  nop stub
    virtual int  _vf48() = 0;                                                      // [48] 0x180  returns const 4                                            tentative
    virtual void SerializeTimers(ISerialize* pSer) = 0;                            // [49] 0x188                                                             tentative
    virtual void _vf50() = 0;                                                      // [50] 0x190  nop stub
    virtual void RaiseError(const char* format, ...) = 0;                          // [51] 0x198  vararg error print                                         VERIFIED
    virtual void _vf52() = 0;                                                      // [52] 0x1A0  reads gEnv.pConsole (PostInit/RegisterConsoleVars?)        tentative
    virtual void _vf53() = 0;                                                      // [53] 0x1A8  thunk                                                      tentative
    virtual void RegisterScriptBind(::CScriptableBase* pBind) = 0;                 // [54] 0x1B0  reads m_pScriptBindManager @this+0xB0                      tentative
    virtual void _vf55() = 0;                                                      // [55] 0x1B8  reads m_pScriptBindManager @this+0xB0 (FlushBindings?)     tentative
    virtual int  GetStackSize() = 0;                                               // [56] 0x1C0  (lua top-base)>>4                                          VERIFIED
    virtual int  _vf57() = 0;                                                      // [57] 0x1C8  returns global dword_18547B6E0                             tentative
    virtual HSCRIPTFUNCTION CompileBuffer(const char* sBuffer, size_t nSize,
                                          const char* sBufferDesc) = 0;            // [58] 0x1D0                                                             tentative
    virtual void _vf59() = 0;                                                      // [59] 0x1D8                                                             tentative
    virtual void _vf60() = 0;                                                      // [60] 0x1E0                                                             tentative
    virtual void _vf61() = 0;                                                      // [61] 0x1E8  mov rbx,rcx; Reset?                                        tentative

    // ---- allocator ----
    virtual void*  Allocate(size_t sz) = 0;                                        // [62] 0x1F0  pooled if sz<=0x200                                        VERIFIED
    virtual size_t Deallocate(void* ptr) = 0;                                      // [63] 0x1F8  thunk to pool free                                         VERIFIED

    // ---- misc / string ----
    virtual void _vf64() = 0;                                                      // [64] 0x200                                                             tentative
    virtual void _vf65() = 0;                                                      // [65] 0x208  large (1161b); ValueToString?                             tentative
    virtual void _vf66() = 0;                                                      // [66] 0x210  large (643b); DumpStack?                                  tentative
    virtual void* _vf67() = 0;                                                     // [67] 0x218  returns 0 (KCD2-inserted slot)                            VERIFIED
    virtual void _vf68() = 0;                                                      // [68] 0x220  KCD1's final slot (GetFuncName?)                          tentative
};

}  // namespace Offsets
