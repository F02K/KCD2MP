#pragma once
#include "../../crysystem/ScriptAnyValue.h"

// -----------------------------------------------
// IFunctionHandler - KCD2 binary vtable order  (CryScriptSystem CFunctionHandler)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  CFunctionHandler vtable 0x183a4a7d0 (12 slots).
//
// Stack-constructed argument accessor handed to C functions registered through
// IScriptTable::AddFunction [22].  The Lua closure trampoline
// wh::scriptsystem::CScriptSystem::StdCFunction_0x1807137AC builds it as:
//   +0x00 vfptr            (0x183a4a7d0)
//   +0x08 lua_State* L
//   +0x10 CScriptSystem*   (global 0x18549CF98)
//   +0x18 const char* sFuncName   (from the closure blob)
//   +0x20 int nParamIdOffset      (blob byte; added to 1-based param indices)
// then invokes desc.pFunctor.thunk(&functorCopy, &handler); the returned int
// becomes the Lua C-function result count, so every path must end with one of
// the EndFunction* calls (they push the results and return the count).
//
// No interfuscator shuffle in this table (matches the SDK declaration order);
// the three EndFunctionAny overloads sit in the usual MSVC-reversed
// adjacent-overload order (3,2,1 args).  All 12 slots VERIFIED by body:
//   [1]  0x180638480  ret this+0x10                     [5] 0x18041c430  (L->top-L->base)>>4 - paramIdOffset
//   [2]  0x180711808  param-1 table's "__this" lightud  [6] 0x18082b688  lua tt -> ScriptVarType switch
//   [3]  0x180712920  ToAny(stack idx 1)                [7] 0x180718344  ToAny(idx+paramIdOffset); on fail
//   [4]  0x1809dd2d0  ret this+0x18                                      GetParamType + script error
//   [8]  0x18399e774  push 3 anys, ret 3                [10] 0x180711b98 push any, ret (type > ANY_TNIL)
//   [9]  0x1804d23ac  push 2 anys, ret 2                [11] 0x18066cd10 ret 0

namespace Offsets {

struct IScriptSystem;

struct IFunctionHandler {
    virtual void _vf0() = 0;                                             // [0]  0x00  0x1839ae81c  scalar deleting dtor (IFunctionHandler base)
    virtual IScriptSystem* GetIScriptSystem() = 0;                       // [1]  0x08  returns the CScriptSystem singleton              VERIFIED
    virtual void* GetThis() = 0;                                         // [2]  0x10  lightuserdata "__this" of the param-1 table      VERIFIED
    virtual bool GetSelfAny(ScriptAnyValue& any) = 0;                    // [3]  0x18  ToAny(stack index 1) - colon-call self           VERIFIED
    virtual const char* GetFuncName() = 0;                               // [4]  0x20  name string from the closure blob                VERIFIED
    virtual int GetParamCount() = 0;                                     // [5]  0x28  Lua arg count minus nParamIdOffset               VERIFIED
    virtual ScriptVarType GetParamType(int nIdx) = 0;                    // [6]  0x30  ScriptVarType of 1-based param (userdata -> svtNull in this build)  VERIFIED
    virtual bool GetParamAny(int nIdx, ScriptAnyValue& any) = 0;         // [7]  0x38  1-based; preset any.type = REQUIRED type (fails + raises script error on mismatch), ANY_ANY = accept any (returns actual)  VERIFIED
    // De-overloaded (EndFunctionAny3/2/Any): MSVC reverses vtable slots of
    // adjacent same-name overloads, which would mirror this group in OUR
    // compiled calls (same bug that crashed BeginCall - see IScriptSystem.h).
    virtual int EndFunctionAny3(const ScriptAnyValue& any1,
                                const ScriptAnyValue& any2,
                                const ScriptAnyValue& any3) = 0;         // [8]  0x40  push 3 results                                    VERIFIED
    virtual int EndFunctionAny2(const ScriptAnyValue& any1,
                                const ScriptAnyValue& any2) = 0;         // [9]  0x48  push 2 results                                    VERIFIED
    virtual int EndFunctionAny(const ScriptAnyValue& any) = 0;           // [10] 0x50  push 1 result (0 if nil)                          VERIFIED
    virtual int EndFunction() = 0;                                       // [11] 0x58  no results                                        VERIFIED

    // ---- SDK convenience templates (non-virtual - no ABI impact) ----

    template <class T>
    bool GetSelf(T& value)
    {
        ScriptAnyValue any(value, 0);
        return GetSelfAny(any) && any.CopyTo(value);
    }

    // 1-based; presets the required type, so a mismatched call raises a
    // proper script error against the registered debug name.
    template <typename T>
    bool GetParam(int nIdx, T& value)
    {
        ScriptAnyValue any(value, 0);
        return GetParamAny(nIdx, any) && any.CopyTo(value);
    }

    template <class T>
    int EndFunction(const T& value) { return EndFunctionAny(value); }
    template <class T1, class T2>
    int EndFunction(const T1& value1, const T2& value2) { return EndFunctionAny2(value1, value2); }
    template <class T1, class T2, class T3>
    int EndFunction(const T1& value1, const T2& value2, const T3& value3) { return EndFunctionAny3(value1, value2, value3); }
    int EndFunctionNull() { return EndFunction(); }

    template <class P1>
    bool GetParams(P1& p1)
    {
        return GetParam(1, p1);
    }
    template <class P1, class P2>
    bool GetParams(P1& p1, P2& p2)
    {
        return GetParam(1, p1) && GetParam(2, p2);
    }
    template <class P1, class P2, class P3>
    bool GetParams(P1& p1, P2& p2, P3& p3)
    {
        return GetParam(1, p1) && GetParam(2, p2) && GetParam(3, p3);
    }
    template <class P1, class P2, class P3, class P4>
    bool GetParams(P1& p1, P2& p2, P3& p3, P4& p4)
    {
        return GetParam(1, p1) && GetParam(2, p2) && GetParam(3, p3) && GetParam(4, p4);
    }
    template <class P1, class P2, class P3, class P4, class P5>
    bool GetParams(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5)
    {
        return GetParams(p1, p2, p3, p4) && GetParam(5, p5);
    }
    template <class P1, class P2, class P3, class P4, class P5, class P6>
    bool GetParams(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6)
    {
        return GetParams(p1, p2, p3, p4, p5) && GetParam(6, p6);
    }
    template <class P1, class P2, class P3, class P4, class P5, class P6, class P7>
    bool GetParams(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7)
    {
        return GetParams(p1, p2, p3, p4, p5, p6) && GetParam(7, p7);
    }
    template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
    bool GetParams(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7, P8& p8)
    {
        return GetParams(p1, p2, p3, p4, p5, p6, p7) && GetParam(8, p8);
    }
    template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
    bool GetParams(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7, P8& p8, P9& p9)
    {
        return GetParams(p1, p2, p3, p4, p5, p6, p7, p8) && GetParam(9, p9);
    }
    template <class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
    bool GetParams(P1& p1, P2& p2, P3& p3, P4& p4, P5& p5, P6& p6, P7& p7, P8& p8, P9& p9, P10& p10)
    {
        return GetParams(p1, p2, p3, p4, p5, p6, p7, p8, p9) && GetParam(10, p10);
    }
};

}  // namespace Offsets
