#pragma once
#include <cstdarg>

// -----------------------------------------------
// ILog - KCD2 binary vtable order  (CryEngine CLog : IMiniLog)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u), image base 0x180000000.
// RTTI:   .?AVCLog@@   TypeDescriptor 0x184A42E78 (name @0x184A42E88)
// vtable: 0x183B27C30  (37 slots, 0..36; NULL qword terminator @0x183B27D58).
// ctor:   0x1811A6020 (stores &vftable)   dtor: 0x18242B6B4 (downgrades to IMiniLog vftable).
// Singleton reached as gEnv(SSystemGlobalEnvironment)->pLog(+0xE8)->vtable[..].
//
// The variadic loggers funnel: Log/LogWarning/LogError/LogAlways (5/6/7/12) -> [3]
// LogV(type,fmt,va) -> [0] LogV(type,flags,fmt,va) -> [1] LogV_WH_Impl (main dispatch,
// builds "$6[Warning]"/"$4[Error]" prefixes). Severity (ELogType) per wrapper is VERIFIED
// from the constant each passes: Log=0, LogWarning=1, LogError=2, LogAlways=3.
// [MODERATE vs KCD1] the interfuscator shuffled the order: dispatch 34->1, dtor 3->4, the
// Log* block +1, so LogAlways is slot 12 here (was 11 in KCD1). Offsets in comments are RVAs.

struct ILogCallback;
struct ICrySizer;

namespace Offsets {

// ELogType (from the dispatch @RVA 0x52F5F0).
enum class ELogType : int {
    eMessage = 0, eWarning = 1, eError = 2, eAlways = 3,
    eWarningAlways = 4, eErrorAlways = 5, eComment = 6, eAssert = 9,
};

struct ILog {
    virtual void LogV(ELogType type, int flags, const char* fmt, va_list args) = 0;               // [0]  0x52F5B8  src "Log.cpp":423 -> [1]   VERIFIED
    virtual void LogV_WH_Impl(const char* src, int line, ELogType type, int flags,
                              const char* fmt, va_list args) = 0;                                 // [1]  0x52F5F0  MAIN DISPATCH             VERIFIED
    virtual void LogV_WH(const char* src, int line, ELogType type, const char* fmt, va_list args) = 0; // [2]  0x4D4344  flags=0 -> [1]        VERIFIED
    virtual void LogV(ELogType type, const char* fmt, va_list args) = 0;                          // [3]  0x52F59C  flags=0 -> [0]            VERIFIED
    virtual void* Dtor(unsigned int flags) = 0;                                                  // [4]  0x242BF44 scalar-deleting dtor       VERIFIED
    virtual void Log(const char* fmt, ...) = 0;                                                  // [5]  0x5FB8E0  LogV(eMessage=0)          VERIFIED
    virtual void LogWarning(const char* fmt, ...) = 0;                                           // [6]  0x5FA1F4  LogV(eWarning=1)          VERIFIED
    virtual void LogError(const char* fmt, ...) = 0;                                             // [7]  0x1038260 LogV(eError=2)            VERIFIED
    virtual void Release() = 0;                                                                  // [8]  0x24301B0 calls [4] with flag 1      VERIFIED
    virtual bool SetFileName(const char* fileName) = 0;                                          // [9]  0x11A6524 opens log file ("wt")     VERIFIED
    virtual const char* GetFileName() = 0;                                                       // [10] 0x1A72C70 return this+0x14          VERIFIED
    virtual const char* GetBackupFileName() = 0;                                                 // [11] 0x1A72C50 return this+0x120         VERIFIED
    virtual void LogAlways(const char* fmt, ...) = 0;                                             // [12] 0x79B32C  LogV(eAlways=3)           VERIFIED
    virtual void _vf13(const char* fmt, ...) = 0;                                                // [13] 0x242F5E8 log variant, ELogType 10   tentative
    virtual void LogPlus(const char* fmt, ...) = 0;                                              // [14] 0x193AA9C -> [16]+[18]               VERIFIED
    virtual void LogToConsole(const char* fmt, ...) = 0;                                         // [15] 0x183B23C console write             VERIFIED
    virtual void LogToConsolePlus(const char* fmt, ...) = 0;                                     // [16] 0x242F6FC console append            VERIFIED
    virtual void LogToFile(const char* fmt, ...) = 0;                                            // [17] 0x19BAB60 file write                VERIFIED
    virtual void LogToFilePlus(const char* fmt, ...) = 0;                                        // [18] 0x242F614 file append               VERIFIED
    virtual void _vf19() = 0;                                                                    // [19] 0x19DBD5C timestamp helper           tentative
    virtual void UpdateLoadingScreen(const char* fmt, ...) = 0;                                  // [20] 0x9B7B14  LogV(eMessage), guarded    tentative
    virtual void RegisterConsoleVariables() = 0;                                                 // [21] 0x1940BD0 log_Verbosity/Filter/...  VERIFIED
    virtual void UnregisterConsoleVariables() = 0;                                               // [22] 0x1A72E10 nulls cvar ptrs           VERIFIED
    virtual int  GetVerbosityLevel() = 0;                                                        // [23] 0x2430CF0 reads log_Verbosity        tentative
    virtual void SetVerbosity(int verbosity) = 0;                                                // [24] 0x242E67C reads log_Verbosity        tentative
    virtual void AddCallback(ILogCallback* pCallback) = 0;                                       // [25] 0x242C390                           VERIFIED
    virtual void RemoveCallback(ILogCallback* pCallback) = 0;                                    // [26] 0x243062C                           VERIFIED
    virtual void Update() = 0;                                                                   // [27] 0x8D9884  tick/flush interval        VERIFIED
    virtual const char* GetModuleFilter() = 0;                                                   // [28] 0xA59FD8  log_Module cvar string     VERIFIED
    virtual void GetMemoryUsage(ICrySizer* pSizer) const = 0;                                    // [29] 0x242E218                           VERIFIED
    virtual void PushAssetScopeName(const char* sType, const char* sName) = 0;                   // [30] 0x3B6E80  nullsub                    tentative
    virtual void PopAssetScopeName() = 0;                                                        // [31] 0x3B6E80  nullsub                    tentative
    virtual const char* GetAssetScopeString() = 0;                                               // [32] 0x1A72B70 returns ""                VERIFIED
    virtual void Flush() = 0;                                                                    // [33] 0x74A8E4  Update() + fflush          VERIFIED
    virtual void FlushAndClose() = 0;                                                            // [34] 0x242D5DC Update() + close           VERIFIED
    virtual int  _vf35() = 0;                                                                    // [35] 0x1A72C90 return *(uint*)(this+0x268) tentative
    virtual void _vf36(void* outStr) = 0;                                                        // [36] 0x242E0E8 CryString getter @this+0x270 tentative
};
static_assert(sizeof(ILog) == sizeof(void*), "ILog is vtable-only");

}  // namespace Offsets
