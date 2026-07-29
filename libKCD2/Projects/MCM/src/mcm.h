// Internal surface shared between plugin.cpp, configmanager.h and the
// element listeners.

#pragma once

#include <string>

#include "crysystem/SSystemGlobalEnvironment.h"

namespace Offsets { struct IUIElement; }

#define MCM_LOG(fmt, ...) do { \
    if (auto* _env = SSystemGlobalEnvironment::GetInstance(); _env && _env->pLog) \
        _env->pLog->LogAlways("[MCM] " fmt, ##__VA_ARGS__); \
} while (0)

extern bool g_mcmOpen;

// Name-resolved on demand (session-cached _smart_ptr inside — holds a ref, so
// never dangles across UI reloads).
Offsets::IUIElement* GetMenuElement();
Offsets::IUIElement* GetMcmElement();

void OpenMCM();
void CloseMCM();

// Every mutation arrives here — manual edits AND flash-local X resets both
// come in as OnValueChanged(modId, settingId, value).
void HandleValueChanged(int modIdx, int itemIdx, double v);
