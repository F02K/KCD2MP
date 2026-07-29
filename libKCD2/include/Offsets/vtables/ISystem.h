#pragma once
#include <cstdint>

// -----------------------------------------------
// ISystem — KCD2 binary vtable order (CrySystem CSystem primary interface)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (build "release_1_5_1308617_856"), base 0x180000000
// RTTI:   .?AVCSystem@@
// primary vtable: 0x183dc2858, 239 slots
// ctor: 0x180a52dba   scalar-deleting dtor: 0x182458548 (operator delete this,0x2BF8)
//
// This is the LEAN vtable base for the concrete CSystem (see CryCommon/ISystem.h).
// Slots 0..15 map cleanly onto the CryEngine ISystem order (confirmed against the
// KCD1 ISystem.h declaration + fingerprints). Past slot 16 the KCD2 ordering drifts,
// so most slots stay terse `_vfN` with a one-line fingerprint note. Accessor notes:
//   [+0xNN]      = returns/uses CSystem member at that offset (VERIFIED from disasm)
//   gEnv->+0xNN  = returns *(m_env + 0xNN)  (m_env is the +0x20 SSystemGlobalEnvironment*)
// Non-accessor names are marked "tentative".

struct IXmlUtils;
struct SSystemGlobalEnvironment;

namespace Offsets {

struct IXmlNode;

struct ISystem {
    virtual void _vf0(char flags) = 0;                              // [0]   0x000  scalar-deleting dtor 0x182458548
    virtual void Release() = 0;                                     // [1]   0x008  0x1803a5684
    virtual void* GetCVarsWhiteListConfigSink() const = 0;          // [2]   0x010  ret NULL
    virtual SSystemGlobalEnvironment* GetGlobalEnvironment() = 0;   // [3]   0x018  ret m_env [+0x20]                 VERIFIED
    virtual const char* GetRootFolder() const = 0;                  // [4]   0x020  ret [+0xA38] (m_rootFolder)       VERIFIED
    virtual bool Update(int updateFlags, int nPauseMode) = 0;       // [5]   0x028  0x1806678c4 (main update, 5249b)
    virtual bool UpdateLoadtime() = 0;                              // [6]   0x030  581b (tentative)
    virtual void DoWorkDuringOcclusionChecks() = 0;                 // [7]   0x038  45b (tentative)
    virtual bool NeedDoWorkDuringOcclusionChecks() = 0;             // [8]   0x040  ret byte [+0x2BA1]                VERIFIED
    virtual void Render() = 0;                                      // [9]   0x048  537b
    virtual void RenderBegin() = 0;                                 // [10]  0x050  reads byte [+0x5C2]
    virtual void RenderEnd(bool bRenderStats) = 0;                  // [11]  0x058  219b
    virtual void SynchronousLoadingTick(const char*, int) = 0;      // [12]  0x060  thunk -> slot 6 (tentative)
    virtual void RenderStatistics() = 0;                            // [13]  0x068  78b (tentative)
    virtual void RenderPhysicsStatistics(void* pWorld) = 0;         // [14]  0x070  stub retn 0
    virtual uint32_t GetUsedMemory() = 0;                           // [15]  0x078  stub retn 0 (tentative)
    virtual uint32_t _vf16() = 0;                                   // [16]  0x080  ret global dword_18547D8E8 (drift starts)
    virtual void _vf17() = 0;                                       // [17]  0x088  174b
    virtual void _vf18() = 0;                                       // [18]  0x090  reads TlsIndex
    virtual void _vf19() = 0;                                       // [19]  0x098  uses qword_18492D978
    virtual void _vf20() = 0;                                       // [20]  0x0A0  uses qword_18492D978
    virtual void _vf21() = 0;                                       // [21]  0x0A8  virtual-forward [rax+0xA0]
    virtual void _vf22() = 0;                                       // [22]  0x0B0  uses qword_18492D978
    virtual void* _vf23() = 0;                                      // [23]  0x0B8  ret [+0x2B58] (CryString buf)
    virtual void* _vf24() = 0;                                      // [24]  0x0C0  reads ptr [+0x5F0]
    virtual void* _vf25() = 0;                                      // [25]  0x0C8  ret [+0x5F0]
    virtual void _vf26() = 0;                                       // [26]  0x0D0
    virtual void _vf27() = 0;                                       // [27]  0x0D8
    virtual void _vf28() = 0;                                       // [28]  0x0E0
    virtual void _vf29() = 0;                                       // [29]  0x0E8  904b
    virtual void _vf30() = 0;                                       // [30]  0x0F0
    virtual bool _vf31() const = 0;                                 // [31]  0x0F8  ret byte [+0x598] (m_bQuit)
    virtual void Quit() = 0;                                        // [32]  0x100  sets byte [+0x598]=1 "Quitting is declared"
    virtual bool _vf33() const = 0;                                 // [33]  0x108  ret byte [+0x5A8]
    virtual bool _vf34() const = 0;                                 // [34]  0x110  ret byte [+0x5A9]
    virtual void _vf35(uint32_t) = 0;                               // [35]  0x118  set dword [+0x5AC]
    virtual uint32_t _vf36() const = 0;                             // [36]  0x120  ret dword [+0x5AC]
    virtual uint32_t _vf37() const = 0;                             // [37]  0x128  ret dword [+0x2BA4] (cmp 0x13)
    virtual bool _vf38() const = 0;                                 // [38]  0x130  ret byte [+0x5AA]
    virtual void _vf39() = 0;                                       // [39]  0x138  GetSystemMetrics(0x1000)
    virtual void _vf40() = 0;                                       // [40]  0x140
    virtual void _vf41() = 0;                                       // [41]  0x148  varargs (Format,r9)
    virtual void _vf42() = 0;                                       // [42]  0x150  stores r9
    virtual uint32_t _vf43() = 0;                                   // [43]  0x158  ret global dword_18547DA9C
    virtual void _vf44() = 0;                                       // [44]  0x160  705b
    virtual void _vf45() = 0;                                       // [45]  0x168
    virtual void _vf46() = 0;                                       // [46]  0x170  reads ptr [+0xCC0]
    virtual void _vf47() = 0;                                       // [47]  0x178
    virtual void _vf48() = 0;                                       // [48]  0x180  reads m_env [+0x20]
    virtual bool _vf49() const = 0;                                 // [49]  0x188  ret byte [+0x5C0]
    virtual void* GetIRenderer_unk() = 0;                           // [50]  0x190  ret [+0xA00] (tentative subsystem)
    virtual void* _vf51() = 0;                                      // [51]  0x198  ret [+0xA08]
    virtual void* _vf52() = 0;                                      // [52]  0x1A0  ret [+0xA10]
    virtual void* _vf53() = 0;                                      // [53]  0x1A8  ret [+0xD08]
    virtual void* _vf54() = 0;                                      // [54]  0x1B0  ret [+0xD00]
    virtual void* _vf55() = 0;                                      // [55]  0x1B8  ret [+0x2B48]
    virtual void* _vf56() = 0;                                      // [56]  0x1C0  ret gEnv->+0x118
    virtual void* _vf57() = 0;                                      // [57]  0x1C8  ret gEnv->+0x00
    virtual void* _vf58() = 0;                                      // [58]  0x1D0  ret gEnv->+0x38
    virtual void* _vf59() = 0;                                      // [59]  0x1D8  ret [+0x9F8]
    virtual void* _vf60() = 0;                                      // [60]  0x1E0  ret gEnv->+0xF8
    virtual void* _vf61() = 0;                                      // [61]  0x1E8  ret [+0xCF8]
    virtual void* GetIProfileSystem() = 0;                         // [62]  0x1F0  lea &m_FrameProfileSystem [+0xCE8]   VERIFIED
    virtual void* _vf63() = 0;                                      // [63]  0x1F8  ret [+0x5C8]
    virtual void* _vf64() = 0;                                      // [64]  0x200  ret [+0x6C0]
    virtual void* _vf65() = 0;                                      // [65]  0x208  ret [+0x6C0]+8
    virtual void* _vf66() = 0;                                      // [66]  0x210  ret gEnv->+0xD0
    virtual void* _vf67() = 0;                                      // [67]  0x218  ret [+0x6A8]
    virtual void* _vf68() = 0;                                      // [68]  0x220  ret [+0x2AF0]
    virtual void* _vf69() = 0;                                      // [69]  0x228  ret gEnv->+0xE0
    virtual void* _vf70() = 0;                                      // [70]  0x230  ret gEnv->+0x50
    virtual void* _vf71() = 0;                                      // [71]  0x238  ret gEnv->+0x88
    virtual void* _vf72() = 0;                                      // [72]  0x240  ret gEnv->+0xA0
    virtual void* _vf73() = 0;                                      // [73]  0x248  ret [+0x6B8]
    virtual void* _vf74() = 0;                                      // [74]  0x250  ret gEnv->+0xD8
    virtual void* _vf75() = 0;                                      // [75]  0x258  ret gEnv->+0xF0
    virtual void* _vf76() = 0;                                      // [76]  0x260  ret gEnv->+0x30
    virtual void* _vf77() = 0;                                      // [77]  0x268  ret gEnv->+0xB8
    virtual void* _vf78() = 0;                                      // [78]  0x270  ret gEnv->+0x08
    virtual void* _vf79() = 0;                                      // [79]  0x278  ret gEnv->+0x28
    virtual void* _vf80() = 0;                                      // [80]  0x280  ret gEnv->+0xA8
    virtual void _vf81() = 0;                                       // [81]  0x288  reads TlsIndex
    virtual void* _vf82() = 0;                                      // [82]  0x290  ret [+0x2B10] (subsystem, alloc 0x1F0)
    virtual void* _vf83() = 0;                                      // [83]  0x298  ret [+0x2B08] (subsystem, alloc 0x108)
    virtual void* GetIProfilingSystem() = 0;                       // [84]  0x2A0  lea &m_ProfilingSystem [+0xE20]      VERIFIED
    virtual void* _vf85() = 0;                                      // [85]  0x2A8  ret [+0x9E8] (subsystem, alloc 0x68)
    virtual void* _vf86() = 0;                                      // [86]  0x2B0  ret gEnv->+0x100
    virtual void* _vf87() = 0;                                      // [87]  0x2B8  ret gEnv->+0x58
    virtual void* _vf88() = 0;                                      // [88]  0x2C0  ret [+0xCC8]
    virtual void* _vf89() = 0;                                      // [89]  0x2C8  ret gEnv->+0x90
    virtual void* _vf90() = 0;                                      // [90]  0x2D0  ret gEnv->+0x10
    virtual void* _vf91() = 0;                                      // [91]  0x2D8  ret gEnv->+0x108
    virtual void* _vf92() = 0;                                      // [92]  0x2E0  ret gEnv->+0x40
    virtual void* _vf93() = 0;                                      // [93]  0x2E8  ret gEnv->+0x80
    virtual void* _vf94() = 0;                                      // [94]  0x2F0  ret gEnv->+0x160
    virtual void _vf95() = 0;                                       // [95]  0x2F8  jmp sub_1808CF0F0
    virtual void* _vf96() = 0;                                      // [96]  0x300  ret gEnv->+0x180 -> +0x98
    virtual void* _vf97() = 0;                                      // [97]  0x308  ret gEnv->+0x180 -> +0xA0
    virtual void* _vf98() = 0;                                      // [98]  0x310  ret [+0x2B18]
    virtual void* _vf99() = 0;                                      // [99]  0x318  ret [+0x2B20]
    virtual void* _vf100() = 0;                                     // [100] 0x320  ret [+0x2B28]
    virtual void* _vf101() = 0;                                     // [101] 0x328  ret [+0x2B30]
    virtual void _vf102() = 0;                                      // [102] 0x330  jmp sub_180EA38D8
    virtual void* _vf103() = 0;                                     // [103] 0x338  ret [+0x2B38]
    virtual bool _vf104() const = 0;                               // [104] 0x340  ret byte [+0x2A2F]
    virtual void _vf105(void*) = 0;                                // [105] 0x348  set ptr [+0x2AE8]
    virtual void* _vf106() = 0;                                     // [106] 0x350  ret [+0x2AE8]
    virtual void* _vf107() = 0;                                     // [107] 0x358  ret &qword_1851DFE80
    virtual void _vf108(void*) = 0;                                // [108] 0x360  set gEnv->+0x90
    virtual void _vf109(void*) = 0;                                // [109] 0x368  set gEnv->+0x38
    virtual void _vf110(void*) = 0;                                // [110] 0x370  set gEnv->+0x00
    virtual void _vf111(void*) = 0;                                // [111] 0x378  set gEnv->+0x120
    virtual void _vf112(void*) = 0;                                // [112] 0x380  set gEnv->+0x68
    virtual void _vf113(void*) = 0;                                // [113] 0x388  set gEnv->+0x70
    virtual void _vf114(void*) = 0;                                // [114] 0x390  set gEnv->+0x58
    virtual void _vf115(void*) = 0;                                // [115] 0x398  set gEnv->+0x100
    virtual void _vf116(void*) = 0;                                // [116] 0x3A0  set gEnv->+0x138
    virtual void _vf117(void*) = 0;                                // [117] 0x3A8  set gEnv->+0x190
    virtual void _vf118(void*) = 0;                                // [118] 0x3B0  set gEnv->+0x140
    virtual void _vf119() = 0;                                      // [119] 0x3B8  421b
    virtual void _vf120() = 0;                                      // [120] 0x3C0  1291b
    virtual void _vf121() = 0;                                      // [121] 0x3C8  1019b
    virtual void _vf122() = 0;                                      // [122] 0x3D0  82b
    virtual void* _vf123() = 0;                                     // [123] 0x3D8  ret [+0xCC0]
    virtual void _vf124(bool) = 0;                                 // [124] 0x3E0  set byte [+0x5D0]
    virtual bool _vf125() const = 0;                               // [125] 0x3E8  ret byte [+0x5D0]
    virtual bool _vf126() const = 0;                               // [126] 0x3F0  ret byte [+0x5D1]
    virtual void _vf127() = 0;                                      // [127] 0x3F8
    virtual void _vf128() = 0;                                      // [128] 0x400  141b
    virtual void _vf129() = 0;                                      // [129] 0x408  101b
    virtual void LoadXmlFromBuffer(IXmlNode** out, const char* buffer,
                                   uint64_t size, char a5, char a6) = 0;  // [130] 0x410  forwards to CXmlUtils [+0xA20]; consumer mod.manifest reader 0x18243F278: (this,&out,buf,size,0,1) -- arity VERIFIED, a5/a6 semantics UNVERIFIED
    virtual void LoadXmlFromFile(IXmlNode** out, const char* path,
                                 char a4, char a5, char a6) = 0;          // [131] 0x418  impl 0x180D1B0EC forwards to CXmlUtils vfunc[1] 0x1806E63CC (injects const 1 as its arg4, demotes a4 to tail); consumers 0x180C3D5B4 / C_Keybinds::LoadConfig / C_Game::LoadConfigXml_18173CB3C all pass (this,&out,path,0,1,1) -- arity VERIFIED, flag semantics UNVERIFIED, pass (0,1,1) like stock
    virtual IXmlUtils* GetXmlUtils() = 0;                         // [132] 0x420  ret [+0xA20] (CXmlUtils, alloc 0x30)   VERIFIED
    virtual void* _vf133() = 0;                                     // [133] 0x428  ret [+0xA28]
    virtual void* _vf134() = 0;                                     // [134] 0x430  &m_ViewCamera [+0x288] (add rcx,0x288)
    virtual void* GetViewCamera() = 0;                             // [135] 0x438  lea &m_ViewCamera [+0x288]           VERIFIED
    virtual void _vf136(bool) = 0;                                 // [136] 0x440  set byte [+0x5C2]
    virtual void _vf137(void*) = 0;                                // [137] 0x448  set ptr [+0x6B0]
    virtual void* _vf138() = 0;                                     // [138] 0x450  ret [+0x6B0]
    virtual bool _vf139() const = 0;                               // [139] 0x458  ret byte [+0x5B0]
    virtual void _vf140() = 0;                                      // [140] 0x460  stub retn 0
    virtual void* _vf141() = 0;                                     // [141] 0x468  stub ret NULL
    virtual void _vf142() = 0;                                      // [142] 0x470  stub retn 0
    virtual void* _vf143() = 0;                                     // [143] 0x478  stub ret NULL
    virtual void _vf144() = 0;                                      // [144] 0x480  stub retn 0
    virtual void _vf145() = 0;                                      // [145] 0x488  stub retn 0
    virtual void _vf146() = 0;                                      // [146] 0x490  stub retn 0
    virtual void _vf147() = 0;                                      // [147] 0x498  stub retn 0
    virtual void* _vf148() = 0;                                     // [148] 0x4A0  stub ret NULL
    virtual void* _vf149() = 0;                                     // [149] 0x4A8  lea &[+0xD18] (sub-object)
    virtual void* _vf150() = 0;                                     // [150] 0x4B0  lea &[+0xD28] (sub-object)
    virtual void* _vf151() = 0;                                     // [151] 0x4B8  lea &[+0xD38] (sub-object)
    virtual void* _vf152() = 0;                                     // [152] 0x4C0  ret [+0x2B68] (m_versionString buf)
    virtual void _vf153() = 0;                                      // [153] 0x4C8  hash (shr 0xE)
    virtual void _vf154() = 0;                                      // [154] 0x4D0  90b
    virtual void _vf155() = 0;                                      // [155] 0x4D8  82b
    virtual void* _vf156() = 0;                                     // [156] 0x4E0  ret [+0xD48] (subsystem, alloc 0x9F8)
    virtual void _vf157() = 0;                                      // [157] 0x4E8  253b
    virtual void _vf158(bool) = 0;                                 // [158] 0x4F0  uses ptr [+0xC58]
    virtual void _vf159() = 0;                                      // [159] 0x4F8  uses qword_18492D8A8
    virtual uint32_t _vf160() const = 0;                           // [160] 0x500  ret dword [+0xE0C]
    virtual void _vf161(bool) = 0;                                 // [161] 0x508  set byte [+0xE10]
    virtual bool _vf162() const = 0;                               // [162] 0x510  ret byte [+0xE10]
    virtual void _vf163(uint32_t, bool) = 0;                       // [163] 0x518  set dword [+0xE08]
    virtual void _vf164() = 0;                                      // [164] 0x520  1288b
    virtual void _vf165() = 0;                                      // [165] 0x528  253b
    virtual void _vf166() = 0;                                      // [166] 0x530  968b
    virtual void _vf167() = 0;                                      // [167] 0x538  113b
    virtual void _vf168() = 0;                                      // [168] 0x540
    virtual void _vf169() = 0;                                      // [169] 0x548  294b
    virtual void _vf170(int) = 0;                                  // [170] 0x550  uses ptr [+0xDF0]
    virtual void _vf171() = 0;                                      // [171] 0x558  97b
    virtual bool _vf172() const = 0;                               // [172] 0x560  ret byte [+0x2A2C]
    virtual void* _vf173() = 0;                                     // [173] 0x568  ret [+0xD50]
    virtual void _vf174() = 0;                                      // [174] 0x570  checks dword_18549D370
    virtual void _vf175() = 0;                                      // [175] 0x578
    virtual void _vf176() = 0;                                      // [176] 0x580  checks byte [+0x5C1]
    virtual void _vf177() = 0;                                      // [177] 0x588  reads TlsIndex
    virtual void* _vf178() = 0;                                     // [178] 0x590  ret [+0x2A30]
    virtual void* _vf179() = 0;                                     // [179] 0x598  ret &qword_1851DFC20
    virtual void _vf180() = 0;                                      // [180] 0x5A0  84b
    virtual void _vf181() = 0;                                      // [181] 0x5A8  78b
    virtual void _vf182() = 0;                                      // [182] 0x5B0  159b
    virtual bool _vf183() const = 0;                               // [183] 0x5B8  ret byte [+0x2BA8]
    virtual bool _vf184() const = 0;                               // [184] 0x5C0  test byte gEnv->+0x250
    virtual void _vf185(bool) = 0;                                 // [185] 0x5C8  set byte [+0x2BA8]
    virtual void _vf186() = 0;                                      // [186] 0x5D0  206b
    virtual uint32_t _vf187() const = 0;                           // [187] 0x5D8  ret dword [+0x2A28]*0x38
    virtual void _vf188(void*) = 0;                                // [188] 0x5E0  ret dword [+0x2A28]
    virtual void _vf189() = 0;                                      // [189] 0x5E8  59b
    virtual void _vf190() = 0;                                      // [190] 0x5F0  340b
    virtual void _vf191() = 0;                                      // [191] 0x5F8  224b
    virtual void _vf192() = 0;                                      // [192] 0x600  135b
    virtual void* _vf193() = 0;                                     // [193] 0x608  ret [+0x2AF8] (subsystem, alloc 0x38)
    virtual void _vf194() = 0;                                      // [194] 0x610  504b
    virtual void* _vf195() = 0;                                     // [195] 0x618  ret [+0x2B78]
    virtual void _vf196() = 0;                                      // [196] 0x620  &[+0x2B88]; jmp sub_182459F54
    virtual uint32_t _vf197() const = 0;                           // [197] 0x628  ret dword [+0x2AE0]
    virtual void _vf198() = 0;                                      // [198] 0x630  293b
    virtual void _vf199(bool) = 0;                                 // [199] 0x638  or byte [+0x2A2D]
    virtual void _vf200() = 0;                                      // [200] 0x640
    virtual void* _vf201() = 0;                                     // [201] 0x648  ret NULL
    virtual uint32_t _vf202() const = 0;                           // [202] 0x650  ret dword [+0x2BAC]
    virtual void _vf203(uint32_t) = 0;                             // [203] 0x658  set dword [+0x2BAC]
    virtual void _vf204() = 0;                                      // [204] 0x660  583b
    virtual bool _vf205() const = 0;                               // [205] 0x668  ret byte [+0x5B4]
    virtual bool _vf206() const = 0;                               // [206] 0x670  ret byte [+0x5B5]
    virtual bool _vf207() const = 0;                               // [207] 0x678  ret byte [+0x5B6]
    virtual bool _vf208() const = 0;                               // [208] 0x680  ret byte [+0x5B7]
    virtual bool _vf209() const = 0;                               // [209] 0x688  ret byte [+0x5B8]
    virtual bool _vf210() const = 0;                               // [210] 0x690  ret byte [+0x5B9]
    virtual bool _vf211() const = 0;                               // [211] 0x698  ret byte [+0x5BA]
    virtual bool _vf212() const = 0;                               // [212] 0x6A0  ret byte [+0x5BB]
    virtual bool _vf213() const = 0;                               // [213] 0x6A8  ret byte [+0x5BC]
    virtual bool _vf214() const = 0;                               // [214] 0x6B0  ret byte [+0x5BD]
    virtual void _vf215() = 0;                                      // [215] 0x6B8  76b
    virtual void _vf216(bool) = 0;                                 // [216] 0x6C0  set byte [+0x2BE1]
    virtual bool _vf217() const = 0;                               // [217] 0x6C8  ret byte [+0x2BE1]
    virtual bool _vf218() const = 0;                               // [218] 0x6D0  ret byte [+0x5BE]
    virtual void _vf219() = 0;                                      // [219] 0x6D8  97b
    virtual void AddRef() = 0;                                      // [220] 0x6E0  lock inc dword [+0x2BD0] (m_refCount)
    virtual void DecRef() = 0;                                      // [221] 0x6E8  lock dec dword [+0x2BD0] (m_refCount)
    virtual void _vf222() = 0;                                      // [222] 0x6F0  uses ptr [+0x2BD8]
    virtual void* _vf223() = 0;                                     // [223] 0x6F8  ret [+0x2BD8]
    virtual void _vf224() = 0;                                      // [224] 0x700  352b
    virtual void* _vf225() = 0;                                     // [225] 0x708  ret &sub_180A57EAC
    virtual void _vf226() = 0;                                      // [226] 0x710  53b
    virtual void _vf227() = 0;                                      // [227] 0x718  thunk -> [rax+0x720]
    virtual void _vf228() = 0;                                      // [228] 0x720  78b
    virtual void _vf229() = 0;                                      // [229] 0x728  307b
    virtual void _vf230() = 0;                                      // [230] 0x730  uses ptr [+0x2BC8]
    virtual void _vf231() = 0;                                      // [231] 0x738  thunk sub_1804DA254
    virtual void _vf232() = 0;                                      // [232] 0x740  57b
    virtual void _vf233() = 0;                                      // [233] 0x748  118b
    virtual void _vf234() = 0;                                      // [234] 0x750  26b
    virtual void* _vf235() = 0;                                     // [235] 0x758  ret this (mov rax,rcx)
    virtual void _vf236() = 0;                                      // [236] 0x760  30b
    virtual void _vf237() = 0;                                      // [237] 0x768  7642b (huge)
    virtual void _vf238() = 0;                                      // [238] 0x770  __security_cookie frame
};
static_assert(sizeof(ISystem) == 8);

// --- Secondary bases of CSystem (each contributes one vptr; no data). ---
// Layout order in CSystem: ISystem@+0x00, ILoadConfigurationEntrySink@+0x08,
// ISystemEventListener@+0x10, IWindowMessageHandler@+0x18  (from dtor 0x182456ee0).

struct ILoadConfigurationEntrySink {
    virtual void _vf0() = 0;                                                    // ~dtor region
    virtual void OnLoadConfigurationEntry(const char* key, const char* value, const char* group) = 0;
    virtual void OnLoadConfigurationEntry_End() = 0;
};
static_assert(sizeof(ILoadConfigurationEntrySink) == 8);

struct ISystemEventListener {
    virtual void _vf0() = 0;                                                    // ~dtor region
    virtual void OnSystemEventAnyThread(uint32_t event, uint64_t wparam, uint64_t lparam) = 0;
    virtual void OnSystemEvent(uint32_t event, uint64_t wparam, uint64_t lparam) = 0;
};
static_assert(sizeof(ISystemEventListener) == 8);

}  // namespace Offsets

// IWindowMessageHandler (CSystem secondary base @ +0x18) is defined canonically in its own header.
#include "IWindowMessageHandler.h"
