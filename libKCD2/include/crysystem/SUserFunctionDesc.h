#pragma once
#include <cstddef>
#include "functor.h"

// -----------------------------------------------
// SUserFunctionDesc - IScriptTable::AddFunction descriptor (KCD2 layout)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  Field-for-field the SDK struct (nested in
// IScriptTable there; global here to match the Offsets/vtables/IScriptTable.h
// forward declaration).
//
// Layout recovered from wh::scriptsystem::CScriptTable::AddFunction_0x180b963d4
// and the Lua closure trampolines it installs:
//   functor path  -> CScriptSystem::StdCFunction_0x1807137AC
//   userdata path -> CScriptSystem::StdCUserDataFunction_0x180719A48
//
// AddFunction copies the descriptor into a Lua userdata blob and registers a
// 3-upvalue C closure under table[sFunctionName]:
//   functor path (taken when pFunctor is set - `callee || func`, 0x180531E1C):
//     blob = { functor copy (32 bytes), uint8 nParamIdOffset, char debugName[] }
//     trampoline: ret = pFunctor.thunk(&blob, &CFunctionHandler-on-stack)
//   userdata path (functor empty):
//     blob = { pUserDataFunc, int nDataSize, data[nDataSize], nParamIdOffset, debugName }
// debugName = "sGlobalName.sFunctionName(sFunctionParams)" (shown in script errors).

namespace Offsets { struct IFunctionHandler; }

// SDK: these typedefs are IScriptTable::FunctionFunctor / ::UserDataFunction.
typedef Functor1wRet<Offsets::IFunctionHandler*, int> FunctionFunctor;
typedef int (*UserDataFunction)(Offsets::IFunctionHandler* pH, void* pBuffer, int nSize);

struct SUserFunctionDesc
{
    const char* sFunctionName;       // +0x00  Lua-visible key in the target table (also closure upvalue 2)
    const char* sFunctionParams;     // +0x08  parameter list, debug name only (ex "nSlot,vDirection")
    const char* sGlobalName;         // +0x10  global table name, debug name only (ex "System")
    FunctionFunctor pFunctor;        // +0x18  handler functor (see functor.h for the invoke contract)
    int nParamIdOffset;              // +0x38  offset added to 1-based param indices (binary stores/reads the low byte, 0x180b96437)
    UserDataFunction pUserDataFunc;  // +0x40  fallback path handler (when pFunctor is empty)
    void* pDataBuffer;               // +0x48  data copied into the closure blob for pUserDataFunc
    int   nDataSize;                 // +0x50  size of pDataBuffer data

    SUserFunctionDesc()
        : sFunctionName(""), sFunctionParams(""), sGlobalName(""),
          nParamIdOffset(0), pUserDataFunc(nullptr), pDataBuffer(nullptr), nDataSize(0) {}
};
static_assert(offsetof(SUserFunctionDesc, pFunctor) == 0x18, "functor blob copied from +0x18 (0x180b96480)");
static_assert(offsetof(SUserFunctionDesc, nParamIdOffset) == 0x38, "byte read @+0x38 (0x180b96437)");
static_assert(offsetof(SUserFunctionDesc, pUserDataFunc) == 0x40, "userdata path fields (0x180b964e8)");
static_assert(sizeof(SUserFunctionDesc) == 0x58, "KCD2 SUserFunctionDesc size");
