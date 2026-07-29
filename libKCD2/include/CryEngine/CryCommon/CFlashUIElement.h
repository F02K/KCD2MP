#pragma once
#include <cstdint>
#include <memory>
#include "Offsets/vtables/IUIElement.h"
#include "Offsets/vtables/IFlashPlayer.h"
#include "Offsets/vtables/IFSCommandHandler.h"
#include "Offsets/vtables/IExternalInterfaceHandler.h"
#include "Offsets/vtables/IVirtualKeyboardEvents.h"

// -----------------------------------------------
// CFlashUIElement -- CryEngine's concrete IUIElement (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// The single engine class behind EVERY named FlashUI element ("HUD", "Menu",
// "ApseMap", ...). There is NO per-element subclass -- the wh::guimodule C_UIHud*
// / C_UICompass screens are separate C_UIFlashBase objects that merely BIND to one
// of these by name (g_pFlashUI->GetUIElement(name), stored at C_UIFlashBase+0x48).
// So the pointer a HUD screen holds at +0x48 is a CFlashUIElement*.
//
// Global-namespace CryEngine class (RTTI .?AVCFlashUIElement@@), sizeof 0x250. ctor
// sub_18088DF88(elem, IFlashUI* pFlashUI, IUIElement* pBaseElem, unsigned instanceId).
// Four bases (verified via ctor vtable writes + CHD RTTI): IUIElement (+0x00, primary
// vtable 0x184049BD0, 113 slots), IFSCommandHandler (+0x08, 0x184049BB8),
// IExternalInterfaceHandler (+0x10, 0x184049BA0), IVirtualKeyboardEvents (+0x18,
// 0x184049B80) -- the interface replicas stay in Offsets:: (that is how the engine
// vtables are called project-wide); only this concrete class is global-namespace.
//
// Init [15] (0x18088EF1C) loads the .gfx asset from "Libs/UI/<m_sFlashFile>" and
// creates the movie player into m_pFlashPlayer (+0x58); Reload re-runs it. That is the
// natural moment to inject overlays (fresh player, _root present, re-fires per reload).
//
// Field layout below is from the ctor + the IUIElement.h slot-behaviour banner. The
// +0x60..+0xD0 and +0xE8..+0x250 spans (bootstrapper, flags, SUIConstraints block, the
// var/array/movieclip/event/function desc arrays, listeners, ext-texture set, world-host
// tail, string cache) are kept opaque -- see IUIElement.h for their roles; only the
// offsets asserted below are load-bearing here.

namespace Offsets { struct IFlashUI; }

struct CFlashUIElement : Offsets::IUIElement,
                         Offsets::IFSCommandHandler,
                         Offsets::IExternalInterfaceHandler,
                         Offsets::IVirtualKeyboardEvents {
    uint32_t          m_refCount;              // +0x20  intrusive refcount (AddRef/Release)
    uint32_t          _pad24;                  // +0x24
    Offsets::IFlashUI* m_pFlashUI;             // +0x28  owning FlashUI (ctor arg)
    CryStringT<char>  m_sName;                 // +0x30  element name (e.g. "HUD"); GetName [10]
    CryStringT<char>  m_sGroupName;            // +0x38  file-level group name
    CryStringT<char>  m_sFlashFile;            // +0x40  gfx file, resolved as Libs/UI/<this>
    float             m_fAlpha;                // +0x48
    int32_t           m_iLayer;                // +0x4C
    bool              m_bValid;                // +0x50  Serialize succeeded
    bool              m_bHiding;               // +0x51
    bool              m_bUnloadRequested;      // +0x52
    bool              m_bUnloadAllInstances;   // +0x53
    uint32_t          _pad54;                  // +0x54
    std::shared_ptr<Offsets::IFlashPlayer> m_pFlashPlayer;  // +0x58  movie player (Init creates it; null until loaded)
    uint8_t           _mid[0x68];              // +0x68  bootstrapper, m_bVisible(+0x78), m_flags(+0x80),
                                               //        lazy bytes, SUIConstraints(+0x8C, 40B) -- see IUIElement.h
    Offsets::IUIElement* m_pBaseElement;       // +0xD0  root element (== this for the root instance)
    uint32_t          m_iInstanceID;           // +0xD8  0 for the root
    uint32_t          _padDC;                  // +0xDC
    void*             m_lock;                  // +0xE0  SRWLOCK guarding the instances vector
    uint8_t           _tail[0x250 - 0xE8];     // +0xE8  instances vector + desc arrays + listeners +
                                               //        ext textures + world-host tail + string cache
};

static_assert(sizeof(CFlashUIElement) == 0x250, "CFlashUIElement must be 0x250");
static_assert(offsetof(CFlashUIElement, m_pFlashUI) == 0x28, "m_pFlashUI at 0x28");
static_assert(offsetof(CFlashUIElement, m_sFlashFile) == 0x40, "m_sFlashFile at 0x40");
static_assert(offsetof(CFlashUIElement, m_pFlashPlayer) == 0x58, "m_pFlashPlayer at 0x58");
static_assert(offsetof(CFlashUIElement, m_pBaseElement) == 0xD0, "m_pBaseElement at 0xD0");
static_assert(offsetof(CFlashUIElement, m_iInstanceID) == 0xD8, "m_iInstanceID at 0xD8");
