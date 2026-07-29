#pragma once
#include <cstdint>

// -----------------------------------------------
// IUIEventSystem -- KCD2 binary vtable order (NOT interfuscated)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashUI.h
// (UNIQUE_IFACE struct IUIEventSystem, no <interfuscator:shuffle> tag).
// RTTI: .?AUIUIEventSystem@@ (global-ns struct).
//
// Impl: CFlashUIEventSystem (.?AVCFlashUIEventSystem@@ : IUIEventSystem @ +0).
//   Single vtable @ 0x183A40F98, 12 slots, this_off 0.
//   Created by CFlashUI::CreateEventSystem (this-slot [30]) via qword_18549D378
//   pool alloc of 0x58 bytes.
//
// [SAME AS KCD1] All 12 slots map 1:1 to the KCD1 IUIEventSystem order and to the
// SDK declaration order -- this interface did NOT gain/lose a slot in KCD2. Slots
// re-verified behaviorally against the KCD2 binary (member offsets differ from KCD1
// only in that the events array lives at this+0x18 and the listeners vector at
// this+0x30, same as KCD1). String-keyed debug logs are stripped in KCD2, so
// identification is by structure (alloc size, member offsets, arg types).
//
// Members recovered: +0x08 m_sName (CryStringT), +0x10 m_eType (EEventSystemType),
// +0x18 events array (SUIEventDesc*, element alloc 0x68), +0x30 listeners vector
// (IUIEventListener*), +0x48 reentrancy guard, +0x50/+0x51 deferred-removal flags.
//
// OVERLOAD PAIR: GetEventDesc(const char*) [7] / GetEventDesc(int) [8].
//   Binary: const char* = LOWER slot [7], int = HIGHER slot [8].
//   MSVC emits adjacent same-name overloads REVERSED (2nd-declared -> lower slot),
//   so to land each on its binary slot the HIGHER-slot overload must be declared
//   FIRST. Here GetEventDesc(int) [8] is declared before GetEventDesc(const char*)
//   [7]. (Note: this is the OPPOSITE within-pair binary order from CFlashUI's
//   GetUIElement/GetUIAction, where const char* is the higher slot -- every pair
//   must be checked individually.)

struct SUIArguments;   // FlashUI value type -- GLOBAL namespace (defined in guimodule/SUITypes.h)
struct SUIEventDesc;   // GLOBAL namespace (defined in guimodule/SUIEventDesc.h)

namespace Offsets {

struct IUIEventListener;
struct SUIEvent;
struct ICrySizer;

struct IUIEventSystem {
    enum EEventSystemType {
        eEST_UI_TO_SYSTEM = 0,
        eEST_SYSTEM_TO_UI = 1,
    };

    virtual void Dtor(char flags) = 0;                                          // [0]  0x00  0x183595618  scalar deleting dtor (sub_183593ED0 + free)
    virtual const char* GetName() const = 0;                                    // [1]  0x08  0x180947210  returns m_sName (this+0x08)
    virtual EEventSystemType GetType() const = 0;                               // [2]  0x10  0x181A72C80  returns m_eType (dword this+0x10)

    virtual unsigned int RegisterEvent(const SUIEventDesc& eventDesc) = 0;       // [3]  0x18  0x1810C2DE8  VERIFIED: pool-allocs 0x68-byte desc, deep-copies eventDesc, appends to events array this+0x18, returns (count-1) as event id
    virtual void RegisterListener(IUIEventListener* pListener, const char* name) = 0; // [4]  0x20  0x180606EF0  VERIFIED: dedup-append pListener into listeners vector this+0x30
    virtual void UnregisterListener(IUIEventListener* pListener) = 0;           // [5]  0x28  0x18357FB9C  VERIFIED: remove from this+0x30 (deferred if iterating: guard this+0x48)

    virtual SUIArguments SendEvent(const SUIEvent& event) = 0;                       // [6]  0x30  0x180557468  VERIFIED: walks listeners this+0x30, dispatches event, aggregates and returns the SUIArguments by value

    // OVERLOAD PAIR (declare HIGHER-slot int overload FIRST; see banner)
    virtual const SUIEventDesc* GetEventDesc(int index) const = 0;              // [8]  0x40  0x1806079B4  VERIFIED: bounds-checked index into the events array this+0x18
    virtual const SUIEventDesc* GetEventDesc(const char* eventName) const = 0;  // [7]  0x38  0x1835AD6F4  VERIFIED: name-keyed lookup on the events map (helper sub_18088B3EC(this+0x18, name)), returns the matching SUIEventDesc*
    virtual int GetEventCount() const = 0;                                      // [9]  0x48  0x181AAEFD0  returns count of events array this+0x18

    virtual unsigned int GetEventId(const char* sEventName) = 0;                // [10] 0x50  0x180606F40  VERIFIED: linear strcmp scan of events array, returns index or 0xFFFFFFFF on miss
    virtual void GetMemoryUsage(ICrySizer* s) const = 0;                        // [11] 0x58  0x1835AEC1C  VERIFIED: Add("FlashUIEventSystem") + AddObject of events/listeners
};

}  // namespace Offsets
