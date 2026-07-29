#pragma once
#include <cstdint>

// -----------------------------------------------
// ICVar - KCD2 binary vtable order (CXConsoleVariableBase family)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u), image base 0x180000000
// Reference vtable: ??_7CXConsoleVariableIntRef@@6B@ @ 0x183dd0020 (28 slots);
// all CXConsoleVariable* variants share this layout (base CXConsoleVariableBase).
// Slot addrs below are the IntRef implementations.
//
// Shared base data layout (from the verified accessors / Set bodies):
//   +0x08 const char* name          (GetName[15] @0x180947210)
//   +0x18 int         flags         (GetFlags[12] @0x181A72850; VF_MODIFIED |= on set)
//   +0x40 CXConsole*  m_pConsole    (Set* call its OnBefore/OnAfterVarChange[74/75])
//   +0x48 T*          value store   (Ref variants point at external storage)
//
// SDK declares the Set overloads as Set(const char*), Set(float), Set(int) -
// an adjacent same-name group, which MSVC allocates in REVERSE, so the binary
// order is [7]=int, [8]=float, [9]=string. De-overloaded names below keep
// declaration order == slot order (same policy as IScriptSystem.h).
//
// Slots without a VERIFIED tag follow the classic CryEngine 3.8 ICVar order
// between the verified anchors ([2],[5],[7],[8],[9],[12],[15]) - tentative.

typedef void (*ConsoleVarFunc)(struct ICVar*);
class ICrySizer;

struct ICVar {
    virtual void        Dtor(char flags) = 0;                        // [0]  0x1824744e0  scalar-deleting dtor
    virtual void        Release() = 0;                               // [1]  0x18247ef98  tentative
    virtual int         GetIVal() const = 0;                         // [2]  0x1804a48e0  return *(int*)store   VERIFIED
    virtual int64_t     GetI64Val() const = 0;                       // [3]  0x181a73a10  tentative
    virtual float       GetFVal() const = 0;                         // [4]  0x18041c370  tentative
    virtual const char* GetString() const = 0;                       // [5]  0x180b953cc  sprintf("%d") into SHARED static buffer 0x18549E110 (copy immediately, not reentrant)  VERIFIED
    virtual const char* GetDataProbeString() const = 0;              // [6]  0x1824782ec  tentative
    virtual void        SetInt(int i) = 0;                           // [7]  0x180c05db8  gate[74] -> store -> cb -> [75]  VERIFIED
    virtual void        SetFloat(float f) = 0;                       // [8]  0x18197d6b0  same shape                       VERIFIED
    virtual void        SetString(const char* s) = 0;                // [9]  0x180b924b4  parses (0x1816084E4), same shape VERIFIED
    virtual void        ForceSet(const char* s) = 0;                 // [10] 0x1819ec608  tentative
    virtual void        ClearFlags(int flags) = 0;                   // [11] 0x180b94840  tentative
    virtual int         GetFlags() const = 0;                        // [12] 0x181a72850  return flags@+0x18    VERIFIED
    virtual int         SetFlags(int flags) = 0;                     // [13] 0x180b93c90  tentative
    virtual int         GetType() = 0;                               // [14] 0x181a72470  tentative
    virtual const char* GetName() const = 0;                         // [15] 0x180947210  return name@+0x08     VERIFIED
    virtual const char* GetHelp() = 0;                               // [16] 0x180638480  tentative
    virtual bool        IsConstCVar() const = 0;                     // [17] 0x180b94828  queried by OnBeforeVarChange[74] gate  tentative
    virtual void        SetOnChangeCallback(ConsoleVarFunc cb) = 0;  // [18] 0x181a737a0  tentative
    virtual ConsoleVarFunc GetOnChangeCallback() const = 0;          // [19] 0x180601c84  tentative
    virtual void        GetMemoryUsage(ICrySizer* pSizer) const = 0; // [20] 0x180602410  tentative
    virtual int         GetRealIVal() const = 0;                     // [21] 0x182478f2c  tentative
    // ---- tail unresolved (roles unknown; declared to keep the vtable span honest) ----
    virtual void        _vf22() = 0;                                 // [22] 0x18247f75c
    virtual void        _vf23() = 0;                                 // [23] 0x180602360
    virtual void        _vf24() = 0;                                 // [24] 0x182478bfc
    virtual void        _vf25() = 0;                                 // [25] 0x180689268
    virtual void        _vf26() = 0;                                 // [26] 0x1803b6e80  stub (retn)
    virtual void        _vf27() = 0;                                 // [27] 0x181a71830
};
