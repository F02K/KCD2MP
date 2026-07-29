// Events fired by the MCM element (names from MCM.xml; fscommand variants
// differ only by case, so one case-insensitive compare covers both).

#pragma once

#include <cstring>

#include "Offsets/vtables/IUIElement.h"
#include "Offsets/vtables/IUIElementEventListener.h"
#include "guimodule/SUIEventDesc.h"
#include "guimodule/SUITypes.h"

#include "../mcm.h"

class McmElementListener : public Offsets::IUIElementEventListener {
    void OnUIEvent(Offsets::IUIElement*, const SUIEventDesc& ev,
                   const SUIArguments& args, void*) override
    {
        const char* n = ev.sName ? ev.sName : ev.sDisplayName;
        if (!n)
            return;
        if (_stricmp(n, "onValueChanged") == 0) {
            if (args.GetArgCount() < 3)
                return;
            HandleValueChanged((int)args.GetArg(0).AsFloat(),
                               (int)args.GetArg(1).AsFloat(),
                               args.GetArg(2).AsFloat());
        } else if (_stricmp(n, "onBack") == 0) {
            CloseMCM();
        }
    }
    void OnUIEventEx(Offsets::IUIElement*, const char*, const SUIArguments&, void*) override {}
    void OnInit(Offsets::IUIElement*, Offsets::IFlashPlayer*) override {}
    void OnUnload(Offsets::IUIElement*) override {}
    void OnSetVisible(Offsets::IUIElement*, bool) override {}
    void OnInstanceCreated(Offsets::IUIElement*, Offsets::IUIElement*) override {}
    void OnInstanceDestroyed(Offsets::IUIElement*, Offsets::IUIElement*) override {}
    void Dtor(char) override {}
};

inline McmElementListener g_mcmListener;
