#pragma once

#include <cstdint>
#include "../Offsets/vtables/IConsole.h"   // Offsets::IConsole (76-slot vtable) + Offsets::IRemoteConsoleListener
                                               // (also pulls Offsets::IInputEventListener via IGame.h) + SDK typedefs

// -----------------------------------------------
// CXConsole - CryEngine 3 engine console (CryEngine + Warhorse, KCD2)
// -----------------------------------------------
// RTTI:   .?AVCXConsole@@              TypeDescriptor 0x184a4a298
// vtable: 0x183dcf850  (??_7CXConsole@@6B@, 76 slots)   COL 0x184192720
// ctor:   0x1817e0fdf  (body sub_1817e0fd0)
// base dtor:            0x1824739db  (sub_1824739dc; frees sub-objects, does NOT free `this`)
// scalar deleting dtor: 0x182474444  (vtable slot 0) -> operator delete(this, 0x240)
// Total size: 0x240     (recovered from the deleting dtor's sub_181AB5160(this, 576))
// Target: WHGame.dll 1.5.6 (kd7u), image base 0x180000000
//
// Three polymorphic bases, one vptr each (no data), laid out at:
//   +0x00 IConsole                (primary, 76-slot vtable)
//   +0x08 IInputEventListener     (raw input listener; 4-slot secondary vtable)
//   +0x10 IRemoteConsoleListener  (3-slot secondary vtable; VERIFIED by dtor vptr rebind)
// CXConsole's own members begin at +0x18.
//
// Layout recovered by decompiling the ctor (member stores -> offsets/types) and confirming
// the simple-accessor slots against the vtable fingerprints (GetImage@0x190, StaticBackground@0x68,
// GetLineCount@0x38, GetStatus@0x1DC, GetNumVars@0xC8, cheat-hash triad@0x1E0/0x1E8/0x1F0/0x1F8,
// scroll@0x170/0x174/0x178, EnableActivationKey@0x1DD).
//
// Confidence: base layout and sizeof are VERIFIED from disasm. Members carrying a VERIFIED tag
// are confirmed by an accessor slot or ctor/dtor use; members marked /* tentative */ are inferred
// (role only); m_unk*/_pad*/_blk* are ctor-zeroed but otherwise unreferenced - treat as provisional.
// The string members are CryStringT<char> (a single char* each) - kept as void* here, exactly like
// CEntity.h / C_Actor.h, so this header stands alone before the KCD2 kcd.h umbrella exists.

struct ISystem;   // engine root (m_pSystem +0x200)

// ---- SDK support types (param interfaces for Offsets::IConsole; global namespace) ----

// Flags used by ICVar registration (EVarFlags); the common ones.
enum EVarFlags {
    VF_NULL                = 0x00000000,
    VF_CHEAT               = 0x00000002,   // stays default when cheats disabled
    VF_DEV_ONLY            = 0x00000004,
    VF_DEDI_ONLY           = 0x00000008,
    VF_NET_SYNCED          = 0x00000080,
    VF_DUMPTODISK          = 0x00000100,
    VF_READONLY            = 0x00000800,
    VF_REQUIRE_LEVEL_RELOAD= 0x00001000,
    VF_REQUIRE_APP_RESTART = 0x00002000,
    VF_COPYNAME            = 0x00008000,
    VF_MODIFIED            = 0x00010000,
    VF_WASINCONFIG         = 0x00020000,
    VF_BITFIELD            = 0x00040000,
    VF_RESTRICTEDMODE      = 0x00080000,
    VF_INVISIBLE           = 0x00100000,
    VF_CONST_CVAR          = 0x00800000,   // const cvar; Register(src) sets this unless allowModify
    VF_CHEAT_ALWAYS_CHECK  = 0x01000000,
    VF_CHEAT_NOCHECK       = 0x02000000,
};

struct IOutputPrintSink {
    virtual ~IOutputPrintSink() {}
    virtual void Print(const char* inszText) = 0;
};

struct ICVarDumpSink {
    virtual ~ICVarDumpSink() {}
    virtual void OnElementFound(ICVar* pCVar) = 0;
};

struct IKeyBindDumpSink {
    virtual ~IKeyBindDumpSink() {}
    virtual void OnKeyBindFound(const char* sBind, const char* sCommand) = 0;
};

struct IConsoleVarSink {
    virtual ~IConsoleVarSink() {}
    virtual bool OnBeforeVarChange(ICVar* pVar, const char* sNewValue) = 0;
    virtual void OnAfterVarChange(ICVar* pVar) = 0;
};

struct IConsoleCmdArgs {
    virtual ~IConsoleCmdArgs() {}
    virtual int         GetArgCount() const = 0;
    virtual const char* GetArg(int nIndex) const = 0;
    virtual const char* GetCommandLine() const = 0;
};

struct IConsoleArgumentAutoComplete {
    virtual ~IConsoleArgumentAutoComplete() {}
    virtual int         GetCount() const = 0;
    virtual const char* GetValue(int nIndex) const = 0;
};

// ---------------------------------------------------------------------------
// CXConsole - concrete engine console.
// ---------------------------------------------------------------------------
class CXConsole : public Offsets::IConsole,                // +0x00
                  public Offsets::IInputEventListener,     // +0x08
                  public Offsets::IRemoteConsoleListener   // +0x10
{
public:
    // The engine keeps the live console in gEnv->pConsole; not a fixed CXConsole global.

    // ===================================================================
    // Data members (+0x18 onwards, after the three interface vptrs)
    // ===================================================================

    uint8_t   m_dqConsoleBuffer[0x28];   // +0x18  console text lines (deque<CryStringT<char>>); Clear()[29]; GetLineCount()=size@0x38 [22] VERIFIED
    uint8_t   m_dqHistory[0x28];         // +0x40  command/scroll history deque (ctor sub_1803DF440) /* tentative */

    uint8_t   m_bStaticBackground;       // +0x68  StaticBackground(bool)[19]                       VERIFIED
    uint8_t   _pad69[3];                 // +0x69
    int32_t   m_nLoadingBackTexID;       // +0x6C  loading-screen tex id (SetLoadingImage[20]@0x182482e64 stores GetIRenderer()->LoadTexture id here)
    uint8_t   _blk70[8];                 // +0x70  ctor-zeroed (likely white/font tex ids)
    int32_t   m_progressRange;           // +0x78  ResetProgressBar[47]@0x18247f9dc sets / TickProgressBar[48] checks   VERIFIED
    uint8_t   _pad7C[4];                 // +0x7C

    void*     m_sInputBuffer;            // +0x80  CryStringT<char> current input line
    void*     m_sReturnString;           // +0x88  CryStringT<char> ExecuteString return string
    void*     m_sPrevTab;                // +0x90  CryStringT<char> autocomplete buffer; ResetAutoCompletion()[45] clears VERIFIED
    int32_t   m_nTabCount;               // +0x98  autocomplete match index; reset by [45]          VERIFIED
    uint8_t   _pad9C[4];                 // +0x9C

    // std::map<CryStringT<char>, CConsoleCommand> - console COMMAND registry (AddCommand[32/33]@0x18100f1d4 -> this+0xA0).
    //   80-byte _Tree node: key CryStringT<char>@+32, value CConsoleCommand(~40B: CryString name/exec + CryString help + int flags)@+40.
    void*     m_mapCommands_head;        // +0xA0  _Tree sentinel (ctor 3-way self-ref + isnil word 0x0101; node=80 via sub_1807B46D8)
    size_t    m_mapCommands_size;        // +0xA8  _Mysize
    // std::map<CryStringT<char>, CryStringT<char>> - KEYBIND registry (CreateKeyBind[16]@0x180b908a4 -> this+0xB0).
    //   48-byte _Tree node: key CryStringT<char>@+32, value CryStringT<char>@+40.
    void*     m_mapBinds_head;           // +0xB0  _Tree sentinel (ctor sub_180452BA4: node=48, 3-way self-ref + isnil 0x0101)
    size_t    m_mapBinds_size;           // +0xB8  _Mysize

    void*     m_pCVarRegistry;           // +0xC0  variable registry root; a1+0xC0 used by all Register*/GetCVar/Unregister VERIFIED (use)
    uint32_t  m_nCVarCount;              // +0xC8  GetNumVars()[38] = *(u32)0xC8                     VERIFIED
    uint8_t   _padCC[4];                 // +0xCC

    // std::vector<std::pair<const char*, ICVar*>> - cheat-checkable cvars; 16B elem {const char* name@0, ICVar* pVar@8}.
    // GetNumCheatVars[52]=(end-begin)/16; CalcCheatVarHash[54]@0x182475d9c -> sub_182474C34 walks this+0xD0 stride 16.
    std::vector<std::pair<const char*, ICVar*>> m_cheatVars;   // +0xD0  {first,last,end}

    // std::vector<std::pair<const char*, ICVar*>> - VF_CHEAT_ALWAYS_CHECK cvars; same 16B elem as m_cheatVars.
    // CalcCheatVarHash[54] hashes this+0xE8..(end-16) stride 16; UnregisterVariable uses this+0xE8.
    std::vector<std::pair<const char*, ICVar*>> m_alwaysCheckVars;  // +0xE8  {first,last,end}

    std::vector<IOutputPrintSink*> m_OutputSinks;  // +0x100  {first,last,end}; Add/RemoveOutputPrintSink[12/13] push/erase at this+0x100

    // std::list<T> (T = 16-byte element) - role unresolved. 32-byte list node {_Next,_Prev,T(16)}.
    void*     m_list118_head;            // +0x118  std::list sentinel (ctor sub_1804F75C0(32): _Next=_Prev=self, no color/isnil word)
    size_t    m_list118_size;            // +0x120  _Mysize
    uint8_t   m_flag128;                 // +0x128  (ctor=0)
    uint8_t   _pad129[3];                // +0x129
    uint32_t  m_unk12C;                  // +0x12C  (ctor=0)
    void*     m_unk130;                  // +0x130  (ctor=0)
    uint32_t  m_unk138;                  // +0x138  (ctor=0)
    uint8_t   _pad13C[4];                // +0x13C

    // std::map<CryStringT<char>, IConsoleArgumentAutoComplete*> - AUTOCOMPLETE registry (RegisterAutoComplete[43]@0x18100f524 -> this+0x140).
    //   48-byte _Tree node: key CryStringT<char>@+32, value IConsoleArgumentAutoComplete*@+40.
    void*     m_mapArgAutoComplete_head; // +0x140  _Tree sentinel (ctor sub_181AB55C0(sub_180451E4C(1)=48) + 3-way self-ref + 0x0101)
    size_t    m_mapArgAutoComplete_size; // +0x148  _Mysize

    uint8_t   m_conVarSinks[0x10];       // +0x150  std::list<IConsoleVarSink*> {sentinel,size} (Add[59]@0x181684790 push_back; dispatched by OnBefore/OnAfterVarChange[74/75]) VERIFIED (body)
    void*     m_pListHead160;            // +0x160  56-byte intrusive list head (ctor sub_181AB55C0(56)) /* tentative */
    void*     m_unk168;                  // +0x168  (ctor=0)

    int32_t   m_scrollCurrent;           // +0x170  current drop position; IsOpened()[37] cmp 0x170==0x174 VERIFIED
    int32_t   m_scrollHeight;            // +0x174  SetScrollMax()[11] target (init 300)             VERIFIED
    int32_t   m_scrollMax;               // +0x178  SetScrollMax()[11] max    (init 300)             VERIFIED
    uint32_t  m_unk17C;                  // +0x17C  (ctor=0)
    int32_t   m_unk180;                  // +0x180  (ctor=-1)
    uint8_t   _pad184[4];                // +0x184
    void*     m_unk188;                  // +0x188  (ctor=0)
    ITexture* m_pImage;                  // +0x190  background image (ctor=0); GetImage[18]@0x181a73a30 returns it, SetImage[17]@0x182482e34 stores  VERIFIED
    uint32_t  m_unk198;                  // +0x198  (ctor=0)
    uint8_t   _pad19C[4];                // +0x19C

    uint8_t   m_flag1A0;                 // +0x1A0  (ctor=0xFF)
    uint8_t   _pad1A1[7];                // +0x1A1
    const char* m_pLoadingImageName;     // +0x1A8  non-owning const char* (ctor lea &byte_183A3D1E0 = empty ""); NOT an owned CryStringT (base dtor never decrefs +0x1A8)
    int32_t   m_unk1B0;                  // +0x1B0  (ctor=-1)
    uint8_t   _blk1B4[0x1C];             // +0x1B4  ctor-zeroed block (0x1B4,0x1C0,0x1C8 stores)
    uint32_t  m_unk1D0;                  // +0x1D0  (ctor=0)
    uint8_t   m_flag1D4;                 // +0x1D4  (ctor=1)
    uint8_t   _pad1D5[3];                // +0x1D5
    int32_t   m_unk1D8;                  // +0x1D8  (ctor=2)

    uint8_t   m_bConsoleActive;          // +0x1DC  GetStatus()[28]                                  VERIFIED
    uint8_t   m_bActivationKeyEnabled;   // +0x1DD  EnableActivationKey(bool)[65] (ctor=1)           VERIFIED
    uint8_t   m_flag1DE;                 // +0x1DE  (ctor=0)
    uint8_t   _pad1DF;                   // +0x1DF

    // Cheat-var hashing (anti-cheat). SetCheatVarHashRange[53] writes first/last + sets the flag;
    // IsHashCalculated[55]=!flag; GetCheatVarHash[56] returns the 64-bit hash.
    uint64_t  m_cheatHashFirstVar;       // +0x1E0                                                   VERIFIED
    uint64_t  m_cheatHashLastVar;        // +0x1E8                                                   VERIFIED
    uint8_t   m_bCheatHashRangeSet;      // +0x1F0                                                   VERIFIED
    uint8_t   _pad1F1[7];                // +0x1F1
    uint64_t  m_cheatVarHash;            // +0x1F8  GetCheatVarHash()[56]                            VERIFIED

    ISystem*  m_pSystem;                 // +0x200  engine ISystem* (ctor=0, set post-init); GetIRenderer via vtbl+0x2D8 in SetLoadingImage@0x182482e64; used by GetCVar@0x1809d8d38 (vtbl+0x5F8)
    uint8_t   _blk208[0x30];             // +0x208  ctor-zeroed block (0x208,0x210,0x218,0x228,0x230 stores)
    int32_t   m_state238;                // +0x238  get-set by vtable slot [64] (ctor=0)            VERIFIED (accessor)
    uint8_t   _pad23C[4];                // +0x23C
};
static_assert(sizeof(CXConsole) == 0x240, "CXConsole size must be 0x240 (operator delete 576)");
