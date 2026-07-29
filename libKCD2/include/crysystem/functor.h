#pragma once
#include <cstring>

// -----------------------------------------------
// functor.h - Rich Hickey callback functors as compiled into KCD2  (WHGame.dll 1.5.6, kd7u)
// -----------------------------------------------
// Binary-pinned subset of CryCommon/functor.h - only the shape the script
// system needs: Functor1wRet, used as FunctionFunctor by
// IScriptTable::AddFunction (see SUserFunctionDesc.h).
//
// Layout evidence:
//  - CScriptTable::AddFunction 0x180b963d4 copies exactly 32 bytes of functor
//    (desc+0x18..0x37) into the Lua closure blob;
//  - the "functor set" test 0x180531E1C is `callee || first-union-qword`
//    => union {func, memFunc[16]} at +0x00, callee at +0x10;
//  - the trampoline CScriptSystem::StdCFunction 0x1807137AC calls
//    *(blob+0x18)(blob, &handler) => thunk at +0x18, signature
//    RT(const CFunctorBase&, P1).
// The engine never touches func/memFunc itself - dispatch goes entirely
// through the translator thunk, which is why plugin-built functors
// interoperate with the game's.

#define MEM_FUNC_SIZE 16

class CFunctorBase
{
public:
    typedef void (*PFunc)();

    CFunctorBase() : func(nullptr), callee(nullptr) {}

    const void* getFunc() const    { return func; }
    void*       getCallee() const  { return callee; }
    const char* getMemFunc() const { return memFunc; }
    operator bool() const { return callee || func; }   // == binary check 0x180531E1C

protected:
    CFunctorBase(const void* c, const void* f, size_t sz)
        : func(nullptr), callee(const_cast<void*>(c))
    {
        if (c)
            memcpy(memFunc, f, sz);          // member-function functor
        else
            func = f;                        // free-function functor
    }

    union {                                  // +0x00
        const void* func;
        char        memFunc[MEM_FUNC_SIZE];
    };
    void* callee;                            // +0x10
};
static_assert(sizeof(CFunctorBase) == 0x18, "union@0 + callee@0x10 (0x180531E1C)");

template <class P1, class RT>
class Functor1wRet : public CFunctorBase
{
public:
    Functor1wRet() : thunk(nullptr) {}
    RT operator()(P1 p1) const { return thunk(*this, p1); }

protected:
    typedef RT (*Thunk)(const CFunctorBase&, P1);
    Functor1wRet(Thunk t, const void* c, const void* f, size_t sz)
        : CFunctorBase(c, f, sz), thunk(t) {}

    Thunk thunk;                             // +0x18  the only field the engine calls
};

template <class P1, class RT, class TRT, class CallType>
class FunctionTranslator1wRet : public Functor1wRet<P1, RT>
{
public:
    FunctionTranslator1wRet(TRT (*f)(P1))
        : Functor1wRet<P1, RT>(thunk, nullptr, reinterpret_cast<const void*>(f), 0) {}

    static RT thunk(const CFunctorBase& ftor, P1 p1)
    {
        return (RT)((TRT (*)(P1))ftor.getFunc())(p1);
    }
};

template <class P1, class RT, class TRT, class CallType, class Callee>
class MemberTranslator1wRet : public Functor1wRet<P1, RT>
{
public:
    typedef TRT (CallType::*MemFunc)(P1);
    static_assert(sizeof(MemFunc) <= MEM_FUNC_SIZE, "member pointer exceeds memFunc storage");

    MemberTranslator1wRet(Callee& c, MemFunc f)
        : Functor1wRet<P1, RT>(thunk, &c, &f, sizeof(MemFunc)) {}

    static RT thunk(const CFunctorBase& ftor, P1 p1)
    {
        Callee* callee = (Callee*)ftor.getCallee();
        MemFunc& memFunc = *(MemFunc*)const_cast<char*>(ftor.getMemFunc());
        return (RT)(callee->*memFunc)(p1);
    }
};

// functor() helpers (CryEngine naming) - construct via the matching translator;
// the return slices to the plain Functor1wRet, keeping union/callee/thunk.
template <class RT, class P1, class Callee, class CallType>
inline Functor1wRet<P1, RT> functor(Callee& c, RT (CallType::*f)(P1))
{
    return MemberTranslator1wRet<P1, RT, RT, CallType, Callee>(c, f);
}

template <class RT, class P1>
inline Functor1wRet<P1, RT> functor(RT (*f)(P1))
{
    return FunctionTranslator1wRet<P1, RT, RT, void>(f);
}

static_assert(sizeof(Functor1wRet<void*, int>) == 0x20, "thunk@0x18 (trampoline 0x1807137AC)");
