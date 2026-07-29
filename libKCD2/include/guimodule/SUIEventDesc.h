#pragma once
#include "guimodule/SUIParameterDesc.h"

// ---------------------------------------------------------------------------
// SUIEventDesc -- KCD2 WHGame.dll 1.5.6 (kd7u).  Native, binary-layout-matched.
// ---------------------------------------------------------------------------
// FlashUI event/function descriptor (CryEngine IFlashUI.h; GLOBAL namespace --
// see guimodule/SUIParameterDesc.h).  This is the desc handed to
// IUIElementEventListener::OnUIEvent and stored by the element (+0x160 events /
// +0x178 functions, stride-pointer arrays) and IUIEventSystem (+0x18).
//
// Layout VERIFIED against kd7u (sizeof 0x68):
//   IUIEventSystem::RegisterEvent [3] sub_1810C2DE8: pool-allocs 0x68, copies the
//   0x28 SUIParameterDesc head, then SEvtParams copies @+0x28 (InputParams) and
//   @+0x48 (OutputParams) via sub_1810C2E9C -- which copies OWORD @+0x00
//   (IsDynamic+pad, sDynamicName), QWORD @+0x10 (sDynamicDesc) and DynArray-copies
//   Params @+0x18 (sub_1810C2ECC).  0x28 + 2*0x20 = 0x68.

struct SUIEventDesc : public SUIParameterDesc
{
    struct SEvtParamsDesc
    {
        SEvtParamsDesc(bool isDyn = false, const char* dynName = "Array", const char* dynDesc = "")
            : IsDynamic(isDyn), sDynamicName(dynName), sDynamicDesc(dynDesc) {}

        bool IsDynamic;               // +0x00  (+0x01..+0x07 pad)
        const char* sDynamicName;     // +0x08
        const char* sDynamicDesc;     // +0x10
    };

    struct SEvtParams : public SEvtParamsDesc
    {
        SEvtParams() {}
        SEvtParams(const SEvtParamsDesc& desc) : SEvtParamsDesc(desc) {}

        TUIParams Params;             // +0x18  DynArray<SUIParameterDesc>

        inline bool operator==(const SEvtParams& other) const
        {
            bool res = IsDynamic == other.IsDynamic && Params.size() == other.Params.size();
            for (int i = 0; i < Params.size() && res; ++i)
                res &= Params[i] == other.Params[i];
            return res;
        }

        template <EUIParameterType T>
        inline void AddParam(const char* name, const char* desc)
        {
            Params.push_back(SUIParameterDesc(name, name, desc, T));
        }
    };

    SUIEventDesc() {}
    SUIEventDesc(const char* name, const char* desc) : SUIParameterDesc(name, name, desc) {}
    SUIEventDesc(const char* name, const char* displ, const char* desc,
                 SEvtParamsDesc inputs = SEvtParamsDesc(), SEvtParamsDesc outputs = SEvtParamsDesc(),
                 const SUIParameterDesc* parent = nullptr)
        : SUIParameterDesc(name, displ, desc, eUIPT_Any, parent)
        , InputParams(inputs)
        , OutputParams(outputs) {}

    template <EUIParameterType T>
    inline void AddParam(const char* name, const char* desc)
    {
        InputParams.AddParam<T>(name, desc);
    }

    inline bool operator==(const SUIEventDesc& other) const
    {
        return strcmp(sName, other.sName) == 0 && InputParams == other.InputParams && OutputParams == other.OutputParams;
    }

    SEvtParams InputParams;           // +0x28
    SEvtParams OutputParams;          // +0x48
};

static_assert(sizeof(SUIEventDesc) == 0x68, "SUIEventDesc size");
static_assert(offsetof(SUIEventDesc, InputParams) == 0x28, "InputParams offset");
static_assert(offsetof(SUIEventDesc, OutputParams) == 0x48, "OutputParams offset");
