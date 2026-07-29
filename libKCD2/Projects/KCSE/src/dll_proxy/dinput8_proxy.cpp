#include "dinput8_proxy.h"
#include <Windows.h>
#include <objbase.h>

static HMODULE s_realDinput8 = nullptr;

using DirectInput8Create_t = HRESULT(WINAPI*)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);
static DirectInput8Create_t s_realDirectInput8Create = nullptr;

extern "C" __declspec(dllexport) HRESULT WINAPI DirectInput8Create(
    HINSTANCE hinst, DWORD dwVersion, REFIID riidltf, LPVOID* ppvOut, LPUNKNOWN punkOuter)
{
    if (!s_realDirectInput8Create)
        return E_FAIL;
    return s_realDirectInput8Create(hinst, dwVersion, riidltf, ppvOut, punkOuter);
}

namespace proxy {

bool Init()
{
    char sysDir[MAX_PATH];
    GetSystemDirectoryA(sysDir, MAX_PATH);
    strcat_s(sysDir, "\\dinput8.dll");

    s_realDinput8 = LoadLibraryA(sysDir);
    if (!s_realDinput8)
        return false;

    s_realDirectInput8Create = reinterpret_cast<DirectInput8Create_t>(
        GetProcAddress(s_realDinput8, "DirectInput8Create"));

    return s_realDirectInput8Create != nullptr;
}

void Shutdown()
{
    if (s_realDinput8) {
        FreeLibrary(s_realDinput8);
        s_realDinput8 = nullptr;
    }
}

}  // namespace proxy
