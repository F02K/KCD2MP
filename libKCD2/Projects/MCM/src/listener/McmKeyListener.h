// Native input route for ALL MCM keybinds, mirroring the game's own pattern
// (C++ forwards into the movie via CallFunction("SetInput", ...)). PROVEN
// NECESSARY: the engine delivers only Esc to the movie as a GFx key event —
// menu keys are action-driven C++-side, and search typing rides char events.
// Inbound C++→flash is reliable; flash keeps the search-focus gate since only
// it knows that state.

#pragma once

#include "CryEngine/CryCommon/SInputEvent.h"
#include "Offsets/vtables/IInputEventListener.h"
#include "Offsets/vtables/IUIElement.h"
#include "guimodule/SUITypes.h"

#include "../mcm.h"

class McmKeyListener : public Offsets::IInputEventListener {
    static void SendInput(const char* action)
    {
        if (Offsets::IUIElement* el = GetMcmElement()) {
            SUIArguments a;
            a.AddArgument(action);
            el->CallFunction("SetInput", a, nullptr, nullptr);
        }
    }

    bool OnInputEvent(const Offsets::SInputEvent& ev) override
    {
        if (!g_mcmOpen)
            return false;
        // arrows feed down AND up: flash runs the hold-repeat between the two
        // and emits the value change on the up edge (fc_setInput)
        if (ev.keyId == Offsets::eKI_Left || ev.keyId == Offsets::eKI_Right) {
            const bool left = (ev.keyId == Offsets::eKI_Left);
            if (ev.state & Offsets::eIS_Pressed)
                SendInput(left ? "left_down" : "right_down");
            else if (ev.state & Offsets::eIS_Released)
                SendInput(left ? "left_up" : "right_up");
            return false;
        }
        if (!(ev.state & Offsets::eIS_Pressed))
            return false;
        if (ev.keyId == Offsets::eKI_X)
            SendInput("reset");
        else if (ev.keyId == Offsets::eKI_Escape)
            SendInput("back");   // flash decides: leave search box OR emit onBack
        return false;   // observe only — never consume
    }
    bool OnInputEventUI(const void*) override { return false; }
    int  GetPriority() const override { return 0; }
    bool _vf3(const void*) override { return false; }
};

inline McmKeyListener g_mcmKeyListener;
