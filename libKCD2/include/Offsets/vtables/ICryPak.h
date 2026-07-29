#pragma once
#include <cstddef>
#include <cstdint>

// -----------------------------------------------
// Offsets::ICryPak -- KCD2 CCryPak (gEnv+0x58), PARTIAL interface.
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). CCryPak RTTI 0x184a42130, primary vtable
// 0x183a97328 (COL 0x184176b58, subobject +0).
//
// The SDK ICryPak declaration order SURVIVES with a uniform +2 slot shift
// (two WH-inserted slots ahead of the file API); MSVC reverses each pair of
// adjacent overloads (FOpen, FGetSize, OpenPack, OpenPacks), same rule as
// IFlashPlayer::Set/GetVariable.
//
// VERIFIED anchors (decompiled bodies / in-binary consumer):
//   [36] FOpen(name,mode,nFlags)      0x180461240  full CryEngine mode-char parser
//                                                  ('r','w','b','t','x','+','S','R','D','N','C','T')
//   [39] FReadRawAll(data,size,f)     0x18051DF98  fseek(f,0,SEEK_SET) + whole-file read
//                                                  (in-memory pak entry copy short-circuit)
//   [46] FGetSize(FILE*)              0x1804609A8  handle-table entry size, fstat64 fallback
//   [55] FClose(FILE*)                0x180460770  1-based handle-slot release / real fclose
//   [63] FindFirst(wild,fd,flags,fs)  0x18096C4E8  consumer: IActorSystem::Scan 0x180E46CE8
//                                                  ("<dir>/*.*" loop on gEnv+0x58); ret -1 = none
//   [64] FindNext(handle,fd)          0x18041D3E0  same consumer; ret <0 = done
//   [65] FindClose(handle)            0x18096CCCC  find-handle refcount release
//
// FILE* handles are CryPak pseudo-handles (1-based indices into an internal
// table, real CRT FILE* for loose files) -- only pass them back into CryPak.
// Unnamed _vfN slots are real functions whose role is UNVERIFIED -- do not call.

namespace Offsets {

// CryPak find record. Filename offset +0x24 proven by the Scan consumer
// (reads fd+0x24 as the name); layout = CryCommon __finddata64_t with 32-bit
// size field.
struct SCryPakFindData {
    uint32_t attrib;         // 0x00
    int64_t  time_create;    // 0x08
    int64_t  time_access;    // 0x10
    int64_t  time_write;     // 0x18
    uint32_t size;           // 0x20
    char     name[260];      // 0x24
};
static_assert(offsetof(SCryPakFindData, name) == 0x24, "CryPak fd name @+0x24 (consumer 0x180E46CE8)");

struct ICryPak {
    virtual void _vf0() = 0;   // [0]  scalar deleting dtor
    virtual void _vf1() = 0;   // [1]
    virtual void _vf2() = 0;   // [2]
    virtual void _vf3() = 0;   // [3]
    virtual void _vf4() = 0;   // [4]
    virtual void _vf5() = 0;   // [5]
    virtual void _vf6() = 0;   // [6]
    virtual void _vf7() = 0;   // [7]
    virtual void _vf8() = 0;   // [8]
    virtual void _vf9() = 0;   // [9]
    virtual void _vf10() = 0;  // [10]
    virtual void _vf11() = 0;  // [11]
    virtual void _vf12() = 0;  // [12]
    virtual void _vf13() = 0;  // [13]
    virtual void _vf14() = 0;  // [14]
    virtual void _vf15() = 0;  // [15]
    virtual void _vf16() = 0;  // [16]
    virtual void _vf17() = 0;  // [17]
    virtual void _vf18() = 0;  // [18]
    virtual void _vf19() = 0;  // [19]
    virtual void _vf20() = 0;  // [20]
    virtual void _vf21() = 0;  // [21]
    virtual void _vf22() = 0;  // [22]
    virtual void _vf23() = 0;  // [23]
    virtual void _vf24() = 0;  // [24]
    virtual void _vf25() = 0;  // [25]
    virtual void _vf26() = 0;  // [26]
    virtual void _vf27() = 0;  // [27]
    virtual void _vf28() = 0;  // [28]
    virtual void _vf29() = 0;  // [29]
    virtual void _vf30() = 0;  // [30]
    virtual void _vf31() = 0;  // [31]
    virtual void _vf32() = 0;  // [32]
    virtual void _vf33() = 0;  // [33]
    virtual void _vf34() = 0;  // [34]
    virtual void* FOpen_GamePath(const char* pName, const char* mode,
                                 char* szFileGamePath, int nLen) = 0;         // [35] 0x1824198A4  overload pair partner (SDK order reversed) -- tentative, prefer [36]
    virtual void* FOpen(const char* pName, const char* mode,
                        unsigned nFlags = 0) = 0;                             // [36] 0x180461240  VERIFIED (mode-char parser)
    virtual void* FOpenRaw(const char* pName, const char* mode) = 0;          // [37] 0x182419988  by SDK order -- tentative
    virtual size_t FReadRaw(void* data, size_t length, size_t elems,
                            void* handle) = 0;                                // [38] 0x1804610A4  by SDK order (sandwiched by verified [36]/[39])
    virtual size_t FReadRawAll(void* data, size_t nFileSize,
                               void* handle) = 0;                             // [39] 0x18051DF98  VERIFIED
    virtual void _vf40() = 0;  // [40] FGetCachedFileData by SDK order
    virtual void _vf41() = 0;  // [41] FWrite
    virtual void _vf42() = 0;  // [42] FPrintf
    virtual void _vf43() = 0;  // [43] FGets
    virtual void _vf44() = 0;  // [44] Getc
    virtual size_t FGetSizeByName(const char* pName,
                                  bool bAllowUseFileSystem = false) = 0;      // [45] 0x182419608  overload pair partner -- tentative
    virtual size_t FGetSize(void* handle) = 0;                                // [46] 0x1804609A8  VERIFIED (handle table / fstat64)
    virtual void _vf47() = 0;  // [47] Ungetc
    virtual void _vf48() = 0;  // [48] IsInPak
    virtual void _vf49() = 0;  // [49] RemoveFile
    virtual void _vf50() = 0;  // [50] RemoveDir
    virtual void _vf51() = 0;  // [51] IsAbsPath
    virtual void _vf52() = 0;  // [52] CopyFileOnDisk
    virtual void _vf53() = 0;  // [53] FSeek
    virtual void _vf54() = 0;  // [54] FTell
    virtual int FClose(void* handle) = 0;                                     // [55] 0x180460770  VERIFIED
    virtual void _vf56() = 0;  // [56] FEof
    virtual void _vf57() = 0;  // [57] FError
    virtual void _vf58() = 0;  // [58] FGetErrno
    virtual void _vf59() = 0;  // [59] FFlush
    virtual void _vf60() = 0;  // [60] PoolMalloc
    virtual void _vf61() = 0;  // [61] PoolFree
    virtual void _vf62() = 0;  // [62] PoolAllocMemoryBlock
    virtual intptr_t FindFirst(const char* pDir, SCryPakFindData* fd,
                               unsigned nFlags = 0,
                               bool bAllowUseFileSystem = false) = 0;         // [63] 0x18096C4E8  VERIFIED (consumer)
    virtual int FindNext(intptr_t handle, SCryPakFindData* fd) = 0;           // [64] 0x18041D3E0  VERIFIED (consumer)
    virtual int FindClose(intptr_t handle) = 0;                               // [65] 0x18096CCCC  VERIFIED
};

}  // namespace Offsets
