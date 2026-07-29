#pragma once

// -----------------------------------------------
// ActionId (CCryName) + EActionActivationMode  — Offsets namespace
// -----------------------------------------------
// VERIFIED against CryName.h: CCryName's only data member is `const char* m_str`
// (interned via INameTable, so equality is a pointer compare). ActionId is a
// typedef of CCryName (IActionMapManager.h:29).

namespace Offsets {

class CCryName {
public:
    const char* c_str() const { return m_str ? m_str : ""; }
    bool        empty() const { return !m_str || !m_str[0]; }
    bool operator==(const CCryName& o) const { return m_str == o.m_str; }
    bool operator<(const CCryName& o) const { return m_str < o.m_str; }   // key order = interned-ptr compare (matches the binary's std::map)
private:
    const char* m_str;   // 0x00 — into the interned INameTable buffer
};
static_assert(sizeof(CCryName) == sizeof(void*), "CCryName must be one pointer");

using ActionId = CCryName;

// COPIED-FROM-SDK (IActionMapManager.h). Low 3 bits match EInputState.
// This is the `activationMode` passed to IActionListener::OnAction. (Lua
// receives it pre-translated to the strings "press"/"hold"/"release".)
enum EActionActivationMode : int {
    eAAM_Invalid       = 0,
    eAAM_OnPress       = 1 << 0,   // 1
    eAAM_OnRelease     = 1 << 1,   // 2
    eAAM_OnHold        = 1 << 2,   // 4
    eAAM_Always        = 1 << 3,   // 8
    eAAM_Retriggerable = 1 << 4,   // 16
    eAAM_NoModifiers   = 1 << 5,   // 32
    eAAM_ConsoleCmd    = 1 << 6,   // 64
    eAAM_AnalogCompare = 1 << 7,   // 128
};

}  // namespace Offsets
