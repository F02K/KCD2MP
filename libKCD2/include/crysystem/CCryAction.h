#pragma once

#include <cstdint>
#include "../Offsets/vtables/IGameFramework.h"

// -----------------------------------------------------------------------------
// CCryAction - CryAction game-framework implementation (KCD2)
// -----------------------------------------------------------------------------
// RTTI:  .?AVCCryAction@@   (global namespace; not a wh:: class)
// Target: WHGame.dll 1.5.6, image base 0x180000000 (IDA instance "kd7u").
//
// vtable:              0x1840472d0  (143 slots, see Offsets::IGameFramework)
// constructor:         0x181865c3a  (zero-init ctor; subsystem ptrs are filled in Init())
// base destructor:     0x18355fce8
// scalar deleting dtor:vtable slot [5] (0x183560d0c) -> operator delete(this, 1984)
//
// SIZE: sizeof == 0x7C0 (1984), recovered two independent ways:
//   (1) scalar-deleting dtor passes 1984 to operator delete (sub_181AB5160(this,1984));
//   (2) CCryAction is a STATIC global at 0x18562EF50 and the next data symbol
//       (dword_18562F710) sits exactly 0x7C0 bytes later; the ctor's last store is
//       a qword at +0x7B8 (ends 0x7C0).
//
// Singleton access (KCD2 globals):
//   IGameFramework*  qword_18549D328   (returned by the framework accessor)
//   CCryAction*      qword_185497F70   (set at the tail of the ctor)
//
// Data members below are recovered from the ctor stores and from the vtable accessor
// fingerprints (getter `mov rax,[rcx+OFF]` / setter `mov [rcx+OFF],dl`). Members marked
// VERIFIED are pinned by a dedicated accessor; those marked /* tentative */ are inferred
// from KCD1 accessor semantics + the drifted offset and want a decompile pass. Everything
// else is an owned pointer/qword whose pointee type is not yet identified (void*), padded
// with _padXX where the ctor left a gap. Offsets are absolute within CCryAction.
// -----------------------------------------------------------------------------

// CryEngine SDK subsystem interfaces (SDK declares these as struct; pointees live
// behind their Get* accessors — full vtable replicas not yet RE'd).
struct ILevelSystem;
struct IActorSystem;
struct IActionMapManager;
struct IViewSystem;
struct IGameObjectSystem;
struct IUIDraw;
struct IGameStatistics;
struct IGameSessionHandler;

// Concrete CryAction singleton. Single inheritance: the only base is IGameFramework,
// so the whole object is [vtable][members]. Offsets::IGameFramework supplies the 143-slot
// vtable base; the members here start at +0x08 (right after the vtable pointer).
class CCryAction : public Offsets::IGameFramework
{
public:
    // The engine-owned singleton: cached framework-pointer global qword_18549D328 (REL::ID
    // 2356); the object itself is the static CCryAction @0x18562EF50.  Single inheritance,
    // so the cached IGameFramework* IS the CCryAction*.  (src/crysystem/CCryAction.cpp)
    static CCryAction* GetInstance();

    // ---- +0x08 .. +0x43: uninitialised-by-ctor head (listener/factory containers) ----
    uint8_t     _pad08[0x3C];                    // +0x08

    // ---- Save/load + engine handle ----
    bool        m_bLoadingSaveGame;              // +0x44  IsLoadingSaveGame / SetLoadingSaveGame   VERIFIED
    uint8_t     _pad45[3];                        // +0x45
    ISystem*    m_pSystem;                        // +0x48  (GetISystem 0x180856CF0 = return *(this+0x48))  VERIFIED
    void*       m_unk50;                          // +0x50
    void*       m_unk58;                          // +0x58
    void*       m_unk60;                          // +0x60
    void*       m_unk68;                          // +0x68
    void*       m_unk70;                          // +0x70
    void*       m_unk78;                          // +0x78
    void*       m_unk80;                          // +0x80
    void*       m_pGameContext;                   // +0x88  game/network context; heavily used     /* tentative (CActionGame*) */
    bool        m_flag90;                         // +0x90  cleared in ctor
    uint8_t     _pad91[0x1FF];                     // +0x91

    bool        m_flag290;                        // +0x290 cleared in ctor
    uint8_t     _pad291[0x1FF];                     // +0x291

    // ---- Game GUID string buffer (GetGameGUID; ctor seeds "{000...-000}") ----
    char        m_gameGUID[0x80];                 // +0x490                                          VERIFIED

    // ===================================================================
    // Subsystem pointer block (+0x510 .. +0x638). Filled during Init().
    // ===================================================================
    ILevelSystem* m_pLevelSystem;                // +0x510 CLevelSystem (GetILevelSystem 0x180B8F740 = return *(this+0x510)) VERIFIED
    IActorSystem* m_pActorSystem;                // +0x518 CActorSystem (GetIActorSystem 0x180641150 = return *(this+0x518)) VERIFIED
    void*       m_pItemSystem;                    // +0x520 CItemSystem         (GetIItemSystem)     VERIFIED
    void*       m_unk528;                         // +0x528 subsystem ptr (accessor slot [48])
    IActionMapManager* m_pActionMapManager;      // +0x530 CActionMapManager (GetIActionMapManager slot[28] = return *(this+0x530)) VERIFIED
    IViewSystem* m_pViewSystem;                  // +0x538 CViewSystem (GetIViewSystem slot[29] = return *(this+0x538)) VERIFIED
    void*       m_unk540;                         // +0x540 subsystem ptr (accessor slot [30])
    void*       m_pVehicleSystem;                 // +0x548 (accessor slot [31])                     /* tentative */
    void*       m_pGameRulesSystem;               // +0x550 (accessor slot [32])                     /* tentative */
    IGameObjectSystem* m_pGameObjectSystem;      // +0x558 CGameObjectSystem (GetIGameObjectSystem slot[23] = return *(this+0x558)) VERIFIED
    IUIDraw*    m_pUIDraw;                        // +0x560 CUIDraw (GetIUIDraw slot[21] = return *(this+0x560))         VERIFIED
    void*       m_unk568;                         // +0x568
    void*       m_unk570;                         // +0x570
    void*       m_pMannequin;                     // +0x578 CMannequinInterface (GetIMannequin)      VERIFIED
    void*       m_pEffectSystem;                  // +0x580 (accessor slot [35])                     /* tentative */
    void*       m_pPlayerProfileManager;          // +0x588 (accessor slot [139])                    /* tentative */
    void*       m_unk590;                         // +0x590 subsystem ptr (accessor slot [37])
    void*       m_pMaterialEffects;               // +0x598 (accessor slot [36])                     /* tentative */
    void*       m_unk5A0;                         // +0x5A0 subsystem ptr (accessor slot [38])
    void*       m_unk5A8;                         // +0x5A8 subsystem ptr (accessor slot [33])
    void*       m_pGameTokenSystem;               // +0x5B0 (accessor slot [34])                     /* tentative */
    void*       m_unk5B8;                         // +0x5B8 (used by slots [3]/[4])
    void*       m_unk5C0;                         // +0x5C0
    void*       m_unk5C8;                         // +0x5C8
    void*       m_unk5D0;                         // +0x5D0 subsystem ptr (accessor slot [41])
    void*       m_unk5D8;                         // +0x5D8 subsystem ptr (accessor slot [44])
    void*       m_unk5E0;                         // +0x5E0 subsystem ptr (accessor slot [20])
    void*       m_unk5E8;                         // +0x5E8 subsystem ptr (accessor slot [45])
    void*       m_unk5F0;                         // +0x5F0 subsystem ptr (accessor slot [46])
    IGameStatistics* m_pGameStatistics;          // +0x5F8 CGameStatistics (GetIGameStatistics slot[142] = return *(this+0x5F8)) VERIFIED
    void*       m_unk600;                         // +0x600 subsystem ptr (accessor slot [40])
    void*       m_unk608;                         // +0x608 subsystem ptr (accessor slot [42])
    IGameSessionHandler* m_pGameSessionHandler;  // +0x610 (SetGameSessionHandler slot[55] = *(this+0x610)=arg) VERIFIED
    void*       m_unk618;                         // +0x618
    void*       m_unk620;                         // +0x620 (accessor slot [132])
    void*       m_unk628;                         // +0x628
    void*       m_unk630;                         // +0x630 (accessor slot [78]; used by many slots)
    void*       m_unk638;                         // +0x638 owned polymorphic obj (released in dtor via slot 0)
    uint8_t     _pad640[0x68];                     // +0x640

    void*       m_unk6A8;                         // +0x6A8 (accessor slot [97])
    uint8_t     _pad6B0[0x40];                     // +0x6B0

    // ---- std::vector<T> (begin / end / capacity-end); freed in the dtor ----
    std::vector<void*> m_vec;   // +0x6F0  begin/end/capacity (0x18); element type unidentified
    uint8_t     _pad708[0x38];                     // +0x708

    uint8_t     m_unk740;                         // +0x740
    bool        m_flag741;                        // +0x741 (getter slot [84])
    bool        m_flag742;                        // +0x742 (setter slot [82])
    uint8_t     _pad743;                          // +0x743
    uint32_t    m_unk744;                         // +0x744
    uint32_t    _pad748;                          // +0x748
    uint32_t    m_unk74C;                         // +0x74C
    void*       m_unk750;                         // +0x750 (read by slot [82])
    void*       m_unk758;                         // +0x758 (used by slot [101])
    void*       m_unk760;                         // +0x760 (set/clear by slots [98]/[99])
    std::vector<uint32_t> m_vecUnk768;            // +0x768 std::vector<T4> {first,last,end}; dtor sub_180A608D4 frees, elem size 4 (&~3 mask). T4 4-byte (uint32/EntityId/int/float) unconfirmed
    void*       m_unk780;                         // +0x780
    uint32_t    m_unk788;                         // +0x788 (set slot [108] / tested slot [112])
    bool        m_bAllowSave;                     // +0x78C AllowSave  (ctor = true)                 VERIFIED
    bool        m_bAllowLoad;                     // +0x78D AllowLoad  (ctor = true)                 VERIFIED
    uint8_t     _pad78E[2];                        // +0x78E
    void*       m_unk790;                         // +0x790 owned obj (accessed by slots [114]-[116])
    uint64_t    _pad798;                          // +0x798
    double      m_loadSaveDelay;                  // +0x7A0 GetLoadSaveDelay returns (float)this     /* tentative */
    void*       m_unk7A8;                          // +0x7A8
    uint16_t    m_unk7B0;                          // +0x7B0
    uint8_t     _pad7B2[2];                        // +0x7B2
    uint32_t    m_preUpdateTicks;                  // +0x7B4 GetPreUpdateTicks (read + reset)         /* tentative */
    void*       m_unk7B8;                          // +0x7B8  (ctor's final store; ends at 0x7C0)
};
static_assert(sizeof(CCryAction) == 0x7C0, "CCryAction must be 0x7C0 (static object extent / operator delete 1984)");
