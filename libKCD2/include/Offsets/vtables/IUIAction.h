#pragma once
#include <cstdint>

// -----------------------------------------------
// IUIAction -- KCD2 binary vtable order (NOT interfuscated)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashUI.h
// (UNIQUE_IFACE struct IUIAction, no <interfuscator:shuffle> tag -> slot order ==
// SDK declaration order). RTTI: .?AUIUIAction@@.
//
// Impl: CFlashUIAction (.?AVCFlashUIAction@@ : IUIAction @ +0).
//   Single vtable @ 0x184049F60, 13 slots, this_off 0.
//   Created/owned by CFlashUI (actions vector this+0x80; CFlashUI::LoadActionFromFile
//   slot [12] dispatches FlowGraph vs LuaScript actions here).
//
// [SAME AS KCD1/SDK] All 13 slots map 1:1 to the SDK IUIAction declaration order --
// this interface was NOT reordered in KCD2. Verified behaviorally (KCD2 strips debug
// strings; matched by member offsets + child vtable dispatch).
//
// Members recovered: +0x08 m_sName (CryStringT), +0x10 m_sScriptFile (CryStringT),
// +0x18 m_pFlowGraph (IFlowGraphPtr cell), +0x20 m_pScript (SmartScriptTable cell),
// +0x28 m_bValid (byte @0x28), +0x29 m_bEnabled (byte @0x29), +0x2C m_eType
// (EUIActionType dword). Slot bodies confirm: [1]->+0x2C, [2]/[3]->+0x08, [5]->byte
// +0x28, [6]/[7]->byte +0x29, [8]->+0x18, [9]->+0x20.
//
// OVERLOAD PAIR: Serialize(XmlNodeRef&, bool) [11] / Serialize(const char*, bool) [10].
//   Binary: const char* scriptFile = LOWER slot [10], XmlNodeRef& = HIGHER slot [11].
//   MSVC reverses adjacent overloads (2nd-declared -> lower slot); SDK declares the
//   XmlNodeRef& overload FIRST, which correctly lands it at the HIGHER binary slot
//   [11] and the const char* overload at the LOWER slot [10]. Kept in SDK order.
//   [UNVERIFIED direction: the two Serialize bodies are thin in KCD2 -- [10]
//   sub_1835BEF74 writes a CryString to this+0x10 (fits scriptFile), [11]
//   sub_18173E9C0 re-inits via the owning element. Do not rely on the exact
//   overload split without a call-site check.]

namespace Offsets {

struct ICrySizer;
struct IFlowGraph;
struct IScriptTable;
struct XmlNodeRef;

struct IUIAction {
    enum EUIActionType {
        eUIAT_FlowGraph = 0,
        eUIAT_LuaScript = 1,
    };

    virtual void Dtor(char flags) = 0;                                          // [0]  0x00  0x1835953A8  scalar deleting dtor (sub_183593ADC + free)
    virtual EUIActionType GetType() const = 0;                                  // [1]  0x08  0x181A73F90  VERIFIED: returns m_eType (dword this+0x2C)
    virtual const char* GetName() const = 0;                                    // [2]  0x10  0x180947210  VERIFIED: returns m_sName (this+0x08)
    virtual void SetName(const char* name) = 0;                                 // [3]  0x18  0x1823CD060  VERIFIED: CryString assign into this+0x08
    virtual bool Init() = 0;                                                    // [4]  0x20  0x1817EBEF8  VERIFIED: guarded by m_bValid (+0x28); drives the owning element (child vf+0x90/+0xF8)
    virtual bool IsValid() const = 0;                                           // [5]  0x28  0x181A731D0  VERIFIED: returns m_bValid (byte this+0x28)
    virtual void SetEnabled(bool bEnabled) = 0;                                 // [6]  0x30  0x1835C00E8  VERIFIED: sets m_bEnabled (byte +0x29), dispatches "OnInit"/"enabled" to the flow/script
    virtual bool IsEnabled() const = 0;                                         // [7]  0x38  0x1835B1974  VERIFIED: returns (m_bEnabled(+0x29) && m_bValid(+0x28))
    virtual void* GetFlowGraph() const = 0;                                     // [8]  0x40  0x18173E7F4  VERIFIED: returns IFlowGraphPtr copied from this+0x18 (SDK returns IFlowGraphPtr by value)
    virtual void* GetScript() const = 0;                                        // [9]  0x48  0x1835B0170  VERIFIED: returns SmartScriptTable copied from this+0x20 (SDK returns SmartScriptTable by value)

    // OVERLOAD PAIR (SDK order already correct: XmlNodeRef& overload declared FIRST -> higher slot [11]; see banner)
    virtual bool Serialize(XmlNodeRef& xmlNode, bool bIsLoading) = 0;           // [11] 0x58  0x18173E9C0  Serialize from/to XML node (re-inits via owning element); [UNVERIFIED direction]
    virtual bool Serialize(const char* scriptFile, bool bIsLoading) = 0;        // [10] 0x50  0x1835BEF74  Serialize a script path (writes CryString this+0x10 then reload sub_1835BD5B8); [UNVERIFIED direction]
    virtual void GetMemoryUsage(ICrySizer* s) const = 0;                        // [12] 0x60  0x1835AE830  VERIFIED: Add("UIAction") + AddObject(this, 64)
};

}  // namespace Offsets
