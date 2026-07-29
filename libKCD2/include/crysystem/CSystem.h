#pragma once
#include <cstdint>
#include "../Offsets/vtables/ISystem.h"

// -----------------------------------------------
// CSystem — CrySystem main engine object (KCD2)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6, base 0x180000000, build "release_1_5_1308617_856"
// RTTI:   .?AVCSystem@@
// primary vtable:      0x183dc2858  (Offsets::ISystem, 239 slots)
// constructor:         0x180a52dba
// scalar-deleting dtor:0x182458548  -> operator delete(this, 0x2BF8)   => sizeof VERIFIED
// base dtor (~CSystem):0x182456ee0
//
// Multiple inheritance (vptr layout, from the dtor's vftable restores):
//   Offsets::ISystem                    +0x00   (primary, 239 slots)
//   Offsets::ILoadConfigurationEntrySink+0x08
//   Offsets::ISystemEventListener       +0x10
//   Offsets::IWindowMessageHandler      +0x18
//
// Layout is recovered from the constructor's member stores plus the vtable's
// getter/setter fingerprints. Named members are VERIFIED against disasm; opaque
// blocks (uint8_t[]) and _gapNN[] padding fill unidentified / folded regions.
// Per the analysis brief the long unnamed tail is intentionally left padded.

struct SSystemGlobalEnvironment;   // the +0x20 pointer targets the global gEnv (0x18492D800)
class CSystemEventDispatcher;      // 0x68-byte alloc, ctor sub_180A53908
class CXmlUtils;                   // 0x30-byte alloc, ctor sub_180A53D80 (embeds CReadWriteXMLSink)
namespace Serialization { class CArchiveHost; }  // ctor sub_180A53844
class CTestSystemLegacy;           // 0x38-byte alloc, ctor sub_180A53ACC
class CThreadTaskManager;          // 0x108-byte alloc, ctor sub_180A53EC0
class CResourceManager;            // 0x1F0-byte alloc, ctor sub_180A541AC (embeds CAsyncPakManager)

class CSystem : public Offsets::ISystem,                     // +0x00
                public Offsets::ILoadConfigurationEntrySink, // +0x08
                public Offsets::ISystemEventListener,        // +0x10
                public Offsets::IWindowMessageHandler        // +0x18
{
public:
    // ---- Global environment ------------------------------------------------
    SSystemGlobalEnvironment* m_env;                 // +0x020  = &gEnv (0x18492D800); GetGlobalEnvironment() slot 3
    uint8_t   _gap028[0x260];                        // +0x028  sub_180A53C48 sink object + init state

    // ---- Cameras (CCamera; boundary folds any trailing members) ------------
    uint8_t   m_ViewCamera[0x310];                   // +0x288  CCamera "CSystem::m_ViewCamera" (GetViewCamera slot 135)  /* size tentative */
    bool      m_bQuit;                               // +0x598  set by Quit() slot 32 ("Quitting is declared")
    uint8_t   _gap599[0x12F];                        // +0x599  flags/CryStrings/subsystem ptrs (0x5A0..0x6C0)
    uint8_t   m_PhysRendererCamera[0x320];           // +0x6C8  CCamera "CSystem::m_PhysRendererCamera"  /* size tentative */

    // ---- Subsystem/manager pointers (roles tentative) ----------------------
    CSystemEventDispatcher* m_pSystemEventDispatcher; // +0x9E8  ctor sub_180A53908 installs CSystemEventDispatcher vtable on 0x68-byte alloc; slot 85 getter
    uint8_t   _gap9F0[0x8];                          // +0x9F0
    void*     m_pUnk9F8;                             // +0x9F8  slot 59
    void*     m_pUnkA00;                             // +0xA00  slot 50
    void*     m_pUnkA08;                             // +0xA08  slot 51
    void*     m_pUnkA10;                             // +0xA10  slot 52
    uint8_t   _gapA18[0x8];                          // +0xA18
    CXmlUtils* m_pXmlUtils;                          // +0xA20  ctor sub_180A53D80 installs CXmlUtils vtable on 0x30-byte alloc (embeds CReadWriteXMLSink); slot 132
    Serialization::CArchiveHost* m_pArchiveHost;    // +0xA28  ctor sub_180A53844 returns Serialization::CArchiveHost; slot 133
    uint8_t   _gapA30[0x8];                          // +0xA30
    CryStringT<char> m_rootFolder;                   // +0xA38  GetRootFolder() slot 4; dtor decrefs via CryStringT dtor helper (a1+327)
    uint8_t   _gapA40[0x2A8];                         // +0xA40  m_env-forwarding accessors' backing (0xCC0/0xCC8 ...)

    // ---- Embedded subsystems (own vtables set in ctor) ---------------------
    uint8_t   m_FrameProfileSystem[0x70];            // +0xCE8  CFrameProfileSystem vtable (GetIProfileSystem slot 62; incl ptr@0xD48 alloc 0x9F8 slot 156)  /* size tentative */
    uint8_t   m_nameTable[0xC8];                     // +0xD58  CNameTable vtable (folds fields 0xDF0/0xE08/0xE10)  /* size tentative */
    uint8_t   m_ProfilingSystem[0x1C08];             // +0xE20  CProfilingSystem (GetIProfilingSystem slot 84) + folded state up to 0x2A28  /* size tentative */

    // ---- Tail state --------------------------------------------------------
    uint8_t   _gap2A28[0xD0];                         // +0x2A28  counters/flags/callbacks (0x2A28..0x2AF0)
    CTestSystemLegacy* m_pTestSystemLegacy;          // +0x2AF8  ctor sub_180A53ACC installs CTestSystemLegacy vtable on 0x38-byte alloc; slot 193
    uint8_t   _gap2B00[0x8];                          // +0x2B00
    CThreadTaskManager* m_pThreadTaskManager;        // +0x2B08  ctor sub_180A53EC0 installs CThreadTaskManager vtable on 0x108-byte alloc; slot 83
    CResourceManager* m_pResourceManager;            // +0x2B10  ctor sub_180A541AC installs CResourceManager vtable on 0x1F0-byte alloc (embeds CAsyncPakManager @+0x150); slot 82
    uint8_t   _gap2B18[0x50];                         // +0x2B18  ptrs 0x2B18..0x2B48 + CryStrings 0x2B50/2B58/2B60
    CryStringT<char> m_versionString;                // +0x2B68  build version "release_1_5_1308617_856"; dtor decrefs via CryStringT dtor helper (a1+1389); slot 152
    uint8_t   _gap2B70[0x60];                         // +0x2B70  0x2B78 ptr, 0x2B88 list, NeedDoWork flag@0x2BA1, 0x2BA4/2BA8/2BAC, 0x2BC8 ptr
    int32_t   m_refCount;                            // +0x2BD0  AddRef/DecRef slots 220/221 (lock inc/dec)
    uint8_t   _gap2BD4[0x24];                         // +0x2BD4  0x2BD8 ptr, 0x2BE1 flag, 0x2BE8 CryString  -> end
};
static_assert(sizeof(CSystem) == 0x2BF8);
