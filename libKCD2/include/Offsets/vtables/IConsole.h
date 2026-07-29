#pragma once
#include <cstdint>
#include "IGame.h"   // reuse Offsets::IInputEventListener (2nd MI base of CXConsole)

// -----------------------------------------------
// IConsole - KCD2 binary vtable order  (CryEngine CXConsole primary vtable)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u), image base 0x180000000
// RTTI:   .?AVCXConsole@@   TypeDescriptor 0x184a4a298
// vtable: 0x183dcf850  (??_7CXConsole@@6B@, 76 slots)   COL 0x184192720
// ctor:   0x1817e0fdf   base dtor: 0x1824739db   scalar-deleting dtor: 0x182474444
//
// Slot identities recovered by decompiling the register/command thunks on kd7u and by
// matching simple-accessor fingerprints to the members recovered from the ctor (see
// ../../CryEngine/CryCommon/IConsole.h). The order is CryEngine's classic IConsole with
// a +1 slide from slot 6 on (one Warhorse insertion at [6]) and a second insertion at
// [64]; slots [68..75] are 8 appended Warhorse virtuals. 66 SDK + 10 extra = 76 (VERIFIED
// by slot count). Accessor slots carrying a VERIFIED tag are confirmed from disasm; other
// names are inferred from the classic order + fingerprint and marked "tentative".

struct ICVar;
struct IOutputPrintSink;
struct ICVarDumpSink;
struct IKeyBindDumpSink;
struct IConsoleVarSink;
struct IConsoleCmdArgs;
struct IConsoleArgumentAutoComplete;
struct ITexture;
struct ICrySizer;

typedef void (*ConsoleCommandFunc)(IConsoleCmdArgs*);
typedef void (*ConsoleVarFunc)(ICVar*);

namespace Offsets {

// ---------------------------------------------------------------------------
// IConsole - CXConsole primary vtable (76 slots). This is the LEAN dispatch base
// that wh's concrete CXConsole (global namespace) inherits at +0x00.
// ---------------------------------------------------------------------------
struct IConsole {
    virtual void   Dtor(char flags) = 0;                                                                 // [0]  0x182474444  scalar-deleting dtor -> operator delete(this,0x240)
    virtual void   Release() = 0;                                                                        // [1]  0x1803a5684
    virtual ICVar* RegisterString(const char* name, const char* value, int flags,
                                  const char* help = "", ConsoleVarFunc cb = nullptr) = 0;               // [2]  0x180b9516c  new CXConsoleVariableString   VERIFIED
    virtual ICVar* RegisterInt(const char* name, int value, int flags,
                               const char* help = "", ConsoleVarFunc cb = nullptr) = 0;                  // [3]  0x180b95054  new CXConsoleVariableInt      VERIFIED
    virtual ICVar* RegisterInt64(const char* name, int64_t value, int flags,
                                 const char* help = "", ConsoleVarFunc cb = nullptr) = 0;                // [4]  0x18247ee74  new CXConsoleVariableInt64    VERIFIED
    virtual ICVar* RegisterFloat(const char* name, float value, int flags,
                                 const char* help = "", ConsoleVarFunc cb = nullptr) = 0;                // [5]  0x180e3e59c  new CXConsoleVariableFloat    VERIFIED
    virtual ICVar* RegisterVariable(ICVar* pVar) = 0;                                                    // [6]  0x18247ee58  inserts an already-built ICVar (Warhorse extra)  /* tentative */
    // Ref-binding registers. DISTINCT names (not overloads of Register) on purpose: MSVC
    // reverses adjacent same-name virtual overloads in the vtable, which would swap the
    // [7]/[9] slots vs the binary. Distinct names keep declaration order == slot order.
    virtual ICVar* RegisterCVarStr(const char* name, const char** src, const char* defaultValue,
                            int flags = 0, const char* help = "", ConsoleVarFunc cb = nullptr,
                            bool allowModify = true) = 0;                                                 // [7]  0x180600178  CXConsoleVariableStringRef    VERIFIED
    virtual ICVar* RegisterCVarInt(const char* name, int* src, int defaultValue,
                            int flags = 0, const char* help = "", ConsoleVarFunc cb = nullptr,
                            bool allowModify = true) = 0;                                                 // [8]  0x180b94300  CXConsoleVariableIntRef       VERIFIED
    virtual ICVar* RegisterCVarFloat(const char* name, float* src, float defaultValue,
                            int flags = 0, const char* help = "", ConsoleVarFunc cb = nullptr,
                            bool allowModify = true) = 0;                                                 // [9]  0x180b94208  CXConsoleVariableFloatRef     VERIFIED
    virtual void   UnregisterVariable(const char* name, bool bDelete = false) = 0;                       // [10] 0x18158fc44  erase from registry@0xC0     VERIFIED
    virtual void   SetScrollMax(int value) = 0;                                                          // [11] 0x1809b17d0  m_scrollHeight/m_scrollMax   VERIFIED
    virtual void   AddOutputPrintSink(IOutputPrintSink* sink) = 0;                                       // [12] 0x182474f20  push sinks vector@0x100      VERIFIED
    virtual void   RemoveOutputPrintSink(IOutputPrintSink* sink) = 0;                                    // [13] 0x18247f790
    virtual void   ShowConsole(bool show, int requestScrollMax = -1) = 0;                                // [14] 0x1808ea368  tentative
    virtual void   DumpCVars(ICVarDumpSink* callback, unsigned int flagsFilter = 0) = 0;                 // [15] 0x18247797c  tentative
    virtual void   CreateKeyBind(const char* cmd, const char* key) = 0;                                  // [16] 0x180b908a4  tentative
    virtual void   SetImage(ITexture* pImage, bool bDeleteCurrent) = 0;                                  // [17] 0x182482e34  tentative
    virtual ITexture* GetImage() = 0;                                                                    // [18] 0x181a73a30  return m_pImage@0x190        VERIFIED
    virtual void   StaticBackground(bool bStatic) = 0;                                                   // [19] 0x181a73b30  m_bStaticBackground@0x68     VERIFIED
    virtual void   SetLoadingImage(const char* szFilename) = 0;                                          // [20] 0x182482e64  tentative
    virtual bool   GetLineNo(int lineNo, char* outBuffer, int bufferSize) const = 0;                     // [21] 0x1824787a0  iterates buffer@0x18         tentative
    virtual int    GetLineCount() const = 0;                                                             // [22] 0x181a73a40  return count@0x38            VERIFIED
    virtual ICVar* GetCVar(const char* name) = 0;                                                        // [23] 0x1809d8d38  lookup registry@0xC0         VERIFIED
    virtual float  GetVariable(const char* varName, const char* fileName, float def) = 0;                // [24] 0x181a730f0  stub -> 0.0f                 tentative
    virtual char*  GetVariable(const char* varName, const char* fileName, const char* def) = 0;          // [25] 0x18066cd10  stub -> null                 tentative
    virtual void   PrintLine(const char* s) = 0;                                                         // [26] 0x1808d9348  thunk                        tentative
    virtual void   PrintLinePlus(const char* s) = 0;                                                     // [27] 0x18247d338  thunk                        tentative
    virtual bool   GetStatus() = 0;                                                                      // [28] 0x18085d1e0  return m_bConsoleActive@0x1DC VERIFIED
    virtual void   Clear() = 0;                                                                          // [29] 0x182475f64  clears buffer@0x18           VERIFIED
    virtual void   Update() = 0;                                                                         // [30] 0x18052f3fc  tentative
    virtual void   Draw() = 0;                                                                           // [31] 0x1809a8134  (1086 bytes)                 tentative
    virtual void   AddCommand(const char* name, ConsoleCommandFunc func, int flags = 0,
                              const char* help = nullptr) = 0;                                           // [32] 0x18100f1d4  tentative
    virtual void   AddCommand(const char* name, const char* scriptFunc, int flags = 0,
                              const char* help = nullptr) = 0;                                           // [33] 0x180b937d0  script variant              tentative
    virtual void   RemoveCommand(const char* name) = 0;                                                  // [34] 0x18100e35c  tentative
    virtual void   ExecuteString(const char* command, bool bSilent = false,
                                 bool bDeferred = false) = 0;                                            // [35] 0x18079eb88  tentative
    virtual void   Exit(const char* format, ...) = 0;                                                    // [36] 0x182477b78  tentative
    virtual bool   IsOpened() = 0;                                                                       // [37] 0x1808e50d8  (scrollCur@0x170==scrollH@0x174) VERIFIED
    virtual int    GetNumVars() = 0;                                                                     // [38] 0x181a73a70  return m_nCVarCount@0xC8     VERIFIED
    virtual size_t GetSortedVars(const char** outArray, size_t numItems,
                                 const char* prefix = nullptr) = 0;                                      // [39] 0x180d8e0a0  tentative
    virtual const char* AutoComplete(const char* substr) = 0;                                            // [40] 0x180d8ded0  tentative
    virtual const char* AutoCompletePrev(const char* substr) = 0;                                        // [41] 0x182475968  tentative
    virtual const char* ProcessCompletion(const char* inputBuffer) = 0;                                  // [42] 0x18247d3d4  (1675 bytes)                 tentative
    virtual void   RegisterAutoComplete(const char* name, IConsoleArgumentAutoComplete* pAC) = 0;        // [43] 0x18100f524  tentative
    virtual void   UnRegisterAutoComplete(const char* name) = 0;                                         // [44] 0x18100e72c  tentative
    virtual void   ResetAutoCompletion() = 0;                                                            // [45] 0x180b91f44  clears m_sPrevTab@0x90/idx@0x98 VERIFIED
    virtual void   GetMemoryUsage(ICrySizer* pSizer) const = 0;                                          // [46] 0x182478b0c  tentative
    virtual void   ResetProgressBar(int nProgressRange) = 0;                                             // [47] 0x18247f9dc  sets m_progressRange@0x78    VERIFIED
    virtual void   TickProgressBar() = 0;                                                                // [48] 0x18052fb38  checks m_progressRange@0x78  VERIFIED
    virtual void   SetInputLine(const char* line) = 0;                                                   // [49] 0x1819dd91c  tentative
    virtual void   DumpKeyBinds(IKeyBindDumpSink* callback) = 0;                                         // [50] 0x1824779f8  tentative
    virtual const char* FindKeyBind(const char* cmd) const = 0;                                          // [51] 0x180b933ac  tentative
    virtual int    GetNumCheatVars() = 0;                                                                // [52] 0x181a73a50  (end-begin)/16 @0xD0         VERIFIED
    virtual void   SetCheatVarHashRange(size_t firstVar, size_t lastVar) = 0;                            // [53] 0x181a73ae0  0x1E0/0x1E8, flag@0x1F0=1    VERIFIED
    virtual void   CalcCheatVarHash() = 0;                                                               // [54] 0x182475d9c  reads flag@0x1F0             VERIFIED
    virtual bool   IsHashCalculated() = 0;                                                               // [55] 0x1809a7bd8  return !flag@0x1F0           VERIFIED
    virtual uint64_t GetCheatVarHash() = 0;                                                              // [56] 0x181a739b0  return m_cheatVarHash@0x1F8  VERIFIED
    virtual void   PrintCheatVars(bool bUseLastHashRange) = 0;                                           // [57] 0x1803b6e80  stub (retn)                  VERIFIED
    virtual char*  GetCheatVarAt(uint32_t nOffset) = 0;                                                  // [58] 0x182478260  tentative
    virtual void   AddConsoleVarSink(IConsoleVarSink* sink) = 0;                                         // [59] 0x181684790  push_back std::list<IConsoleVarSink*>@0x150  VERIFIED (body)
    virtual void   RemoveConsoleVarSink(IConsoleVarSink* sink) = 0;                                      // [60] 0x180da7464  sinks container@0x150        VERIFIED (offset)
    virtual const char* GetHistoryElement(bool bUpOrDown) = 0;                                           // [61] 0x18247867c  tentative
    virtual void   AddCommandToHistory(const char* command) = 0;                                         // [62] 0x181007488  tentative
    virtual void   LoadConfigVar(const char* variable, const char* value) = 0;                           // [63] 0x180b939b0  tentative
    virtual int    _vf64(int value) = 0;                                                                 // [64] 0x180b922a0  get-set m_state238@0x238 (Warhorse extra)  /* tentative */
    virtual void   EnableActivationKey(bool bEnable) = 0;                                                // [65] 0x181a73980  m_bActivationKeyEnabled@0x1DD VERIFIED
    virtual void   SaveInternalState(void* writer) const = 0;                                            // [66] 0x182481e6c  IDataWriteStream&            tentative
    virtual void   LoadInternalState(void* reader) = 0;                                                  // [67] 0x18247b428  IDataReadStream&             tentative
    // ---- Warhorse-appended virtuals [68..75] (not in SDK IConsole) ----
    virtual void   _vf68() = 0;                                                                          // [68] 0x18248794c
    virtual void*  _vf69() = 0;                                                                          // [69] 0x1805f5da0  return this (mov rax,rcx)
    virtual void   _vf70(void** out) = 0;                                                                // [70] 0x1824878f0  *out = this
    virtual void   _vf71() = 0;                                                                          // [71] 0x182478214
    virtual void   _vf72() = 0;                                                                          // [72] 0x1824781c8
    virtual void   _vf73() = 0;                                                                          // [73] 0x182477fc0
    // Sink broadcast pair (plain methods in stock CryEngine, appended virtuals here). Called from
    // INSIDE every CXConsoleVariable*::Set* body (e.g. IntRef SetInt@0x180C05DB8 / SetFloat@0x18197D6B0
    // / SetString@0x180B924B4: vtbl+592 gate -> store -> change cb -> vtbl+600), so sinks observe ALL
    // value changes - console lines, config eval, Lua SetCVar, C++ ICVar::Set - not just console input.
    // Set* early-outs when the value is unchanged (unless flag 0x200000), so no-op sets never reach sinks.
    virtual bool   OnBeforeVarChange(ICVar* pVar, const char* sNewValue) = 0;                            // [74] 0x180b9409c  flag gate (0x3000002/0x800/0x40000000, bypass this+0x1DE) then sinks vf[1]; any false blocks  VERIFIED (body)
    virtual void   OnAfterVarChange(ICVar* pVar) = 0;                                                    // [75] 0x180b93d88  sinks vf[2] over list@0x150  VERIFIED (body)
};

// ---------------------------------------------------------------------------
// IRemoteConsoleListener - CXConsole 3rd MI base at +0x10 (VERIFIED: the base dtor
// rebinds *(this+0x10) to &IRemoteConsoleListener::`vftable'). 3-slot secondary vtable.
// ---------------------------------------------------------------------------
struct IRemoteConsoleListener {
    virtual ~IRemoteConsoleListener() = default;                 // [0] thunk (sub rcx,0x10)
    virtual void OnConsoleCommand(const char* cmd) = 0;          // [1]
    virtual void OnGameplayCommand(const char* cmd) = 0;         // [2] ret0 default
};

}  // namespace Offsets
