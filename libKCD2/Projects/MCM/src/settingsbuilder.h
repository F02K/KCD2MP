// MCM::ISettingsBuilder backend: appends into g_mods. An instance is passed
// as msg->data of the MCM::kMessage_BuildSettings broadcast (plugin.cpp
// RebuildSettings) — valid only for the duration of the dispatch.

#pragma once

#include "MCM_API.h"
#include "configmanager.h"

class SettingsBuilder : public MCM::ISettingsBuilder
{
public:
    void AddMod(const char* modId, const char* displayName) override
    {
        if (modId && *modId)
            GetOrAddMod(modId, displayName ? displayName : "");
    }

    void AddCategory(const char* modId, const char* label, const char* tooltip) override
    {
        if (!Valid(modId, label))
            return;
        Item it;
        it.kind  = Item::Cat;
        it.label = label;
        it.tip   = tooltip ? tooltip : "";
        ::AddItem(GetOrAddMod(modId, ""), std::move(it));
    }

    void AddToggle(const char* modId, const char* id, const char* label,
                   const char* tooltip, bool defaultOn) override
    {
        if (!Valid(modId, id, label))
            return;
        Item it;
        it.kind  = Item::Toggle;
        it.id    = id;
        it.label = label;
        it.tip   = tooltip ? tooltip : "";
        it.def   = it.val = defaultOn ? 1 : 0;
        ::AddItem(GetOrAddMod(modId, ""), std::move(it));
    }

    void AddDropdown(const char* modId, const char* id, const char* label,
                     const char* tooltip, const char* options, int defaultIndex) override
    {
        if (!Valid(modId, id, label) || !options || !*options)
            return;
        Item it;
        it.kind    = Item::Dropdown;
        it.id      = id;
        it.label   = label;
        it.tip     = tooltip ? tooltip : "";
        it.options = options;
        it.def     = it.val = defaultIndex;
        ::AddItem(GetOrAddMod(modId, ""), std::move(it));
    }

    void AddSlider(const char* modId, const char* id, const char* label,
                   const char* tooltip, double mn, double mx, double step,
                   double defaultValue, const char* suffix) override
    {
        if (!Valid(modId, id, label))
            return;
        Item it;
        it.kind   = Item::Slider;
        it.id     = id;
        it.label  = label;
        it.tip    = tooltip ? tooltip : "";
        it.mn     = mn;
        it.mx     = mx;
        it.step   = step;
        it.def    = it.val = defaultValue;
        it.suffix = suffix ? suffix : "";
        ::AddItem(GetOrAddMod(modId, ""), std::move(it));
    }

    void SetValue(const char* modId, const char* id, double value) override
    {
        if (!modId || !*modId || !id || !*id)
            return;
        if (!SetItemValue(GetOrAddMod(modId, ""), id, value))
            MCM_LOG("WARN: SetValue(%s.%s) - no such setting (call after its Add*)", modId, id);
    }

private:
    static bool Valid(const char* modId, const char* a, const char* b = "")
    {
        return modId && *modId && a && *a && b;
    }
};
