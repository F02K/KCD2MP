#pragma once
#include <cstring>
#include "CryEngine/CryCommon/CryArray.h"   // DynArray (TUIParams)

// ---------------------------------------------------------------------------
// SUIParameterDesc -- KCD2 WHGame.dll 1.5.6 (kd7u).  Native, binary-layout-matched.
// ---------------------------------------------------------------------------
// FlashUI parameter/event/function descriptor head (CryEngine IFlashUI.h; GLOBAL
// namespace like the rest of the FlashUI value types -- see guimodule/SUITypes.h;
// the vendored SDK CryEngine/CryCommon/IFlashUI.h is never compiled).
//
// Layout VERIFIED against kd7u: IUIEventSystem::RegisterEvent [3] sub_1810C2DE8
// deep-copies the 0x28 head as OWORD @+0x00 (sName, sDisplayName), OWORD @+0x10
// (sDesc, pParent), QWORD @+0x20 (eType + pad) into its 0x68 pool block.
// Head strings additionally proven live in-game (listener-side name matching).

struct SUIParameterDesc
{
    enum EUIParameterType
    {
        eUIPT_Any = 0,
        eUIPT_Bool,
        eUIPT_Int,
        eUIPT_Float,
        eUIPT_Vec3,
        eUIPT_String,
        eUIPT_WString,
    };

    SUIParameterDesc() : sName("undefined"), sDisplayName("undefined"), sDesc("undefined"), pParent(nullptr), eType(eUIPT_Any) {}
    SUIParameterDesc(const char* name, const char* displ, const char* desc, EUIParameterType type = eUIPT_Any, const SUIParameterDesc* parent = nullptr)
        : sName(name), sDisplayName(displ), sDesc(desc), pParent(parent), eType(type) {}

    const char* sName;                // +0x00
    const char* sDisplayName;         // +0x08
    const char* sDesc;                // +0x10
    const SUIParameterDesc* pParent;  // +0x18
    EUIParameterType eType;           // +0x20  (int32; +0x24..+0x27 pad)

    inline bool operator==(const SUIParameterDesc& other) const
    {
        return strcmp(sName, other.sName) == 0;
    }
};
typedef DynArray<SUIParameterDesc> TUIParams;

static_assert(sizeof(SUIParameterDesc) == 0x28, "SUIParameterDesc size");
static_assert(offsetof(SUIParameterDesc, sDisplayName) == 0x08, "sDisplayName offset");
static_assert(offsetof(SUIParameterDesc, pParent) == 0x18, "pParent offset");
static_assert(offsetof(SUIParameterDesc, eType) == 0x20, "eType offset");
