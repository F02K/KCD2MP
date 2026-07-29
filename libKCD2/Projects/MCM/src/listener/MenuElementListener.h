// Menu element: our raw-tier button click + menu closing while MCM is open.

#pragma once

#include <cstring>

#include "Offsets/vtables/IUIElement.h"
#include "Offsets/vtables/IUIElementEventListener.h"
#include "guimodule/SUIEventDesc.h"
#include "guimodule/SUITypes.h"

#include "../mcm.h"

class MenuElementListener : public Offsets::IUIElementEventListener {
    void OnUIEvent(Offsets::IUIElement*, const SUIEventDesc& ev,
                   const SUIArguments& args, void*) override
    {
        const char* n = ev.sName ? ev.sName : ev.sDisplayName;
        if (!n || (_stricmp(n, "onBasicButton") != 0 && _stricmp(n, "OnButton") != 0))
            return;
        if (args.GetArgCount() < 1)
            return;
        if (args.GetArg(0).AsString() == "mcm_open")
            OpenMCM();
    }
    void OnUIEventEx(Offsets::IUIElement*, const char*, const SUIArguments&, void*) override {}
    void OnInit(Offsets::IUIElement*, Offsets::IFlashPlayer*) override {}
    void OnUnload(Offsets::IUIElement*) override {}
    void OnSetVisible(Offsets::IUIElement*, bool bVisible) override
    {
        if (!bVisible && g_mcmOpen)                       // menu closed under us (Esc)
            CloseMCM();
    }
    void OnInstanceCreated(Offsets::IUIElement*, Offsets::IUIElement*) override {}
    void OnInstanceDestroyed(Offsets::IUIElement*, Offsets::IUIElement*) override {}
    void Dtor(char) override {}
};

inline MenuElementListener g_menuListener;
