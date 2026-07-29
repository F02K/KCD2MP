#pragma once
#include <cstdint>

// -----------------------------------------------
// IRenderer - KCD2 binary vtable order (CryEngine 3 renderer interface)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (KCD2), image base 0x180000000, IDA instance kd7u.
//
// Concrete instance: CD3D9Renderer (RTTI .?AVCD3D9Renderer@@, TD name @0x184a52f68,
// primary vtable 0x183dfc008 / 381 slots, COL 0x18419eeb0; also owns a secondary
// vtable at 0x183dfcbf8, COL 0x18419ee10, NOT modeled here). It derives from
// CRenderer (RTTI .?AVCRenderer@@, TD name @0x184a4dbf0, vtable 0x183df0f68, same
// 381-slot shape) -- several slots (e.g. [97], [199]) are still the generic
// unimplemented stub 0x181d93e5d on CRenderer and only get real bodies on
// CD3D9Renderer, VERIFIED by direct decompile (see those slots' comments below).
// This header documents the vtable the runtime gEnv->pRenderer object actually
// carries (CD3D9Renderer's), not the less-derived CRenderer's.
//
// ACCESS: NOT a GetInstance()-style singleton. Reached exclusively through
// SSystemGlobalEnvironment::pRenderer (crysystem/SSystemGlobalEnvironment.h,
// already VERIFIED there, header offset +0x110 == abs 0x18492D908) -- already the
// idiomatic, zero-new-plumbing access path: SSystemGlobalEnvironment::GetInstance()->pRenderer.
// That gEnv slot is populated exactly once, at engine startup, by
// CEngineModule_CryRenderer::Initialize (RTTI-verified vtable 0x183dfbfc8 slot [6],
// sub_180FB1AE4): it calls sub_180FB1B08() and stores the result at gEnv CODE-offset
// +264 (== header +0x110 under the established +8 CSystem/gEnv skew documented in
// SSystemGlobalEnvironment.h's own header comment). sub_180FB1B08 in turn returns a
// SEPARATE renderer-module-internal global, qword_18547DB60 -- the classic CryEngine
// "gRenDev" pattern (the renderer module caches its own pointer to itself in
// addition to publishing through gEnv). gEnv->pRenderer is the correct access path
// for mod code; qword_18547DB60 is an internal implementation detail, not modeled.
//
// Slot count (381) established by scanning forward from the vtable start until
// hitting the next RTTI symbol boundary (CD3D9Renderer's own secondary-vtable COL,
// landing exactly at slot 381) -- a real, if unusually large, CryEngine 3 IRenderer
// surface. Only 4 slots below carry direct behavioral evidence (fully decompiled,
// not just observed from a caller); every other slot is a bare terse stub awaiting
// its own investigation, matching this codebase's existing convention for huge
// interfaces (see IGameFramework.h, IActor.h).

namespace Offsets {

struct IRenderer {
    virtual void  _vf0() = 0;      // [0]   0x1825404F8
    virtual void  _vf1() = 0;      // [1]   0x1824DB3F4
    virtual void  _vf2() = 0;      // [2]   0x1824FE5A8
    virtual void  _vf3() = 0;      // [3]   0x181A74A40
    virtual void  _vf4() = 0;      // [4]   0x1805FB990
    virtual void  _vf5() = 0;      // [5]   0x1814477E4
    virtual void  _vf6() = 0;      // [6]   0x18254A274
    virtual void  _vf7() = 0;      // [7]   0x18252C898
    virtual void  _vf8() = 0;      // [8]   0x181A75540
    virtual void  _vf9() = 0;      // [9]   0x18052F280
    virtual void  _vf10() = 0;     // [10]  0x181843D88
    virtual void  _vf11() = 0;     // [11]  0x1807995BC
    virtual void  _vf12() = 0;     // [12]  0x1819B6BB0
    virtual void  _vf13() = 0;     // [13]  0x1824F8030
    virtual void  _vf14() = 0;     // [14]  0x180664064
    virtual void  _vf15() = 0;     // [15]  0x1824F2850
    virtual void  _vf16() = 0;     // [16]  0x181A74AE0
    virtual void  _vf17() = 0;     // [17]  0x180C18440
    virtual void  _vf18() = 0;     // [18]  0x1825295A0
    virtual void  _vf19() = 0;     // [19]  0x1825299F4
    virtual void  _vf20() = 0;     // [20]  0x18066CD10  (shared stub, retn 0 pattern)
    virtual void  _vf21() = 0;     // [21]  0x18066CD10  (shared stub)
    virtual void  _vf22() = 0;     // [22]  0x18066CD10  (shared stub)
    virtual void  _vf23() = 0;     // [23]  0x1803B6E80  (shared stub, distinct from 0x18066cd10)
    virtual void  _vf24() = 0;     // [24]  0x18066CD10  (shared stub)
    virtual void  _vf25() = 0;     // [25]  0x181A74F50
    virtual void  _vf26() = 0;     // [26]  0x1824F7F48
    virtual void  _vf27() = 0;     // [27]  0x180774614
    virtual void  _vf28() = 0;     // [28]  0x1824F7F8C
    virtual void  _vf29() = 0;     // [29]  0x1824E8A8C
    virtual void  _vf30() = 0;     // [30]  0x1824E8E74
    virtual void  _vf31() = 0;     // [31]  0x1824FECF8
    virtual void  _vf32() = 0;     // [32]  0x1824E9B8C
    virtual void  _vf33() = 0;     // [33]  0x181DEBF40
    virtual void  _vf34() = 0;     // [34]  0x181A749D0
    virtual void  _vf35() = 0;     // [35]  0x1824E8300
    virtual void  _vf36() = 0;     // [36]  0x1824FED3C
    virtual void  _vf37() = 0;     // [37]  0x1824FED8C
    virtual void  _vf38() = 0;     // [38]  0x1824E9CBC
    virtual void  _vf39() = 0;     // [39]  0x181A74E90
    virtual void  _vf40() = 0;     // [40]  0x1804EA50C
    virtual void  _vf41() = 0;     // [41]  0x1816832F0
    virtual void  _vf42() = 0;     // [42]  0x1824FA748
    virtual void  _vf43() = 0;     // [43]  0x1824F3C28
    virtual void  _vf44() = 0;     // [44]  0x181DF9460
    virtual void  _vf45() = 0;     // [45]  0x18252B854
    virtual void  _vf46() = 0;     // [46]  0x18252BB90
    virtual void  _vf47() = 0;     // [47]  0x180F6DB74
    virtual void  _vf48() = 0;     // [48]  0x181DF8010
    virtual void  _vf49() = 0;     // [49]  0x18052EF84
    virtual void  _vf50() = 0;     // [50]  0x18043D5F4
    virtual void  _vf51() = 0;     // [51]  0x182549DF8
    virtual void  _vf52() = 0;     // [52]  0x1807A35E8
    virtual void  _vf53() = 0;     // [53]  0x181447938
    virtual void  _vf54() = 0;     // [54]  0x18186D9AC
    virtual void  _vf55() = 0;     // [55]  0x1805F9E94
    virtual void  _vf56() = 0;     // [56]  0x182549774
    virtual void  _vf57() = 0;     // [57]  0x182548F3C
    virtual void  _vf58() = 0;     // [58]  0x1809A7C54
    virtual void  _vf59() = 0;     // [59]  0x1807A1E98
    virtual void  _vf60() = 0;     // [60]  0x182500DCC
    virtual void  _vf61() = 0;     // [61]  0x18043EB60
    virtual void  _vf62() = 0;     // [62]  0x180CA52E4
    virtual void  _vf63() = 0;     // [63]  0x1819A43F4
    virtual void  _vf64() = 0;     // [64]  0x181A75530
    virtual void  _vf65() = 0;     // [65]  0x1819DFA14
    virtual void  _vf66() = 0;     // [66]  0x1824FF1C8
    virtual void  _vf67() = 0;     // [67]  0x1824F6A14
    virtual void  _vf68() = 0;     // [68]  0x182544ACC
    virtual void  _vf69() = 0;     // [69]  0x182549EF8
    // GetHeight/GetWidth VERIFIED by full decompile (both trivial global-reads,
    // NOT computed from instance state; identical on CRenderer and CD3D9Renderer --
    // never overridden). Both are used by C_UIHudBubbles::UpdateBubbles
    // (sub_1805E0520) as the percent->pixel scale and the 1080.0/H stage factor
    // (see analysis/damage_numbers/worldtoscreen_hud_anchoring.md).
    virtual int   GetHeight() const = 0;   // [70]  0x1809A8730  return (int)dword_18547DB54
    virtual int   GetWidth() const = 0;    // [71]  0x1807F52A0  return (int)dword_18547DB50
    virtual void  _vf72() = 0;     // [72]  0x1806668C0
    virtual void  _vf73() = 0;     // [73]  0x1809A8980
    virtual void  _vf74() = 0;     // [74]  0x1809A8970
    virtual void  _vf75() = 0;     // [75]  0x181DEC0E0
    virtual void  _vf76() = 0;     // [76]  0x181DEC050
    virtual void  _vf77() = 0;     // [77]  0x181A74A60
    virtual void  _vf78() = 0;     // [78]  0x181DEC190
    virtual void  _vf79() = 0;     // [79]  0x181A74A50
    virtual void  _vf80() = 0;     // [80]  0x181DEC180
    virtual void  _vf81() = 0;     // [81]  0x181DEC750
    virtual void  _vf82() = 0;     // [82]  0x181DEC170
    virtual void  _vf83() = 0;     // [83]  0x182545BB4
    virtual void  _vf84() = 0;     // [84]  0x1803B6E80  (shared stub)
    virtual void  _vf85() = 0;     // [85]  0x1803B6E80  (shared stub)
    virtual void  _vf86() = 0;     // [86]  0x1824F803C
    virtual void  _vf87() = 0;     // [87]  0x1804E4A40
    virtual void  _vf88() = 0;     // [88]  0x1804E4658
    virtual void  _vf89() = 0;     // [89]  0x181A74A30
    virtual void  _vf90() = 0;     // [90]  0x18073C040
    virtual void  _vf91() = 0;     // [91]  0x180838AE0  (shared stub)
    virtual void  _vf92() = 0;     // [92]  0x180F36580
    virtual void  _vf93() = 0;     // [93]  0x181DEC820
    virtual void  _vf94() = 0;     // [94]  0x181DEC1A0
    virtual void  _vf95() = 0;     // [95]  0x181DEC400
    virtual void  _vf96() = 0;     // [96]  0x1804E5694
    // ProjectToScreen VERIFIED by full decompile: CD3D9Renderer OVERRIDE (CRenderer's
    // slot [97] is the generic unimplemented stub 0x181d93e5d -- proves the runtime
    // gEnv->pRenderer object carries CD3D9Renderer's vtable, not CRenderer's). Reads
    // the view/proj matrix off *(qword_18492D8C8+1080), transforms (wx,wy,wz) through
    // it, perspective-divides by w, clip-tests the divided x/y against 1.0 (fabs mask
    // xmmword_18409A220) and w>=0 for the behind-camera case, then writes:
    //   *sx = ((ndcX+1)*0.5) * 100.0 / GetWidth()     -- PERCENT of viewport, 0..100
    //   *sy = ((1-ndcY)*0.5) * 100.0 / GetHeight()    -- PERCENT of viewport, 0..100
    //   *sz = ndcZ (post-divide depth; >1.0 == behind camera / clipped)
    // return = on-screen bool (false when behind camera or x/y clipped outside [-1,1]).
    // Full end-to-end usage trace + percent->pixel->stage recipe:
    // analysis/damage_numbers/worldtoscreen_hud_anchoring.md
    virtual bool  ProjectToScreen(float wx, float wy, float wz,
                                   float* sx, float* sy, float* sz) const = 0;  // [97] 0x1809356D8
    virtual void  _vf98() = 0;     // [98]  0x182549E94
    virtual void  _vf99() = 0;     // [99]  0x181045F78
    virtual void  _vf100() = 0;    // [100] 0x181A74B50
    virtual void  _vf101() = 0;    // [101] 0x182528BC4
    virtual void  _vf102() = 0;    // [102] 0x1819272A8
    virtual void  _vf103() = 0;    // [103] 0x1819D0668
    virtual void  _vf104() = 0;    // [104] 0x182528BA8
    virtual void  _vf105() = 0;    // [105] 0x18052FB9C
    virtual void  _vf106() = 0;    // [106] 0x1824F33FC
    virtual void  _vf107() = 0;    // [107] 0x1824E8FD4
    virtual void  _vf108() = 0;    // [108] 0x1803FAB00
    virtual void  _vf109() = 0;    // [109] 0x182515554
    virtual void  _vf110() = 0;    // [110] 0x1803FA768
    virtual void  _vf111() = 0;    // [111] 0x18080B7D0
    virtual void  _vf112() = 0;    // [112] 0x181224B24
    virtual void  _vf113() = 0;    // [113] 0x180860C38
    virtual void  _vf114() = 0;    // [114] 0x1824F1B24
    virtual void  _vf115() = 0;    // [115] 0x18041A6A0  (shared stub)
    virtual void  _vf116() = 0;    // [116] 0x180B4C380
    virtual void  _vf117() = 0;    // [117] 0x1824F1F44
    virtual void  _vf118() = 0;    // [118] 0x1824F1BDC
    virtual void  _vf119() = 0;    // [119] 0x180B4C3BC
    virtual void  _vf120() = 0;    // [120] 0x1824F1DB8
    virtual void  _vf121() = 0;    // [121] 0x1824F1F80
    virtual void  _vf122() = 0;    // [122] 0x180BAB9C8
    virtual void  _vf123() = 0;    // [123] 0x1819DF51C
    virtual void  _vf124() = 0;    // [124] 0x1805F88A8
    virtual void  _vf125() = 0;    // [125] 0x180BAB9D0
    virtual void  _vf126() = 0;    // [126] 0x1824F9B90
    virtual void  _vf127() = 0;    // [127] 0x1805F9AF4
    virtual void  _vf128() = 0;    // [128] 0x1817C8FD4
    virtual void  _vf129() = 0;    // [129] 0x1809B1718
    virtual void  _vf130() = 0;    // [130] 0x1806B8FB4
    virtual void  _vf131() = 0;    // [131] 0x1806B9370
    virtual void  _vf132() = 0;    // [132] 0x1803B6E80  (shared stub)
    virtual void  _vf133() = 0;    // [133] 0x181A749C0
    virtual void  _vf134() = 0;    // [134] 0x181A749A0
    virtual void  _vf135() = 0;    // [135] 0x1824F1984
    virtual void  _vf136() = 0;    // [136] 0x1824F1FA8
    virtual void  _vf137() = 0;    // [137] 0x1824F21B0
    virtual void  _vf138() = 0;    // [138] 0x180C1D308
    virtual void  _vf139() = 0;    // [139] 0x180BAAE20
    virtual void  _vf140() = 0;    // [140] 0x18076C840
    virtual void  _vf141() = 0;    // [141] 0x1824F19AC
    virtual void  _vf142() = 0;    // [142] 0x1807BD160
    virtual void  _vf143() = 0;    // [143] 0x1824F2370
    virtual void  _vf144() = 0;    // [144] 0x1804E2A2C
    virtual void  _vf145() = 0;    // [145] 0x181A74990
    virtual void  _vf146() = 0;    // [146] 0x1806667DC
    virtual void  _vf147() = 0;    // [147] 0x1807E8C80
    virtual void  _vf148() = 0;    // [148] 0x180C169A0
    virtual void  _vf149() = 0;    // [149] 0x1803B6E80  (shared stub)
    virtual void  _vf150() = 0;    // [150] 0x181A749B0
    virtual void  _vf151() = 0;    // [151] 0x1824F230C
    virtual void  _vf152() = 0;    // [152] 0x1804D80D0
    virtual void  _vf153() = 0;    // [153] 0x1807724F4
    virtual void  _vf154() = 0;    // [154] 0x18045C620
    virtual void  _vf155() = 0;    // [155] 0x1809B8E1C
    virtual void  _vf156() = 0;    // [156] 0x1824F1A38
    virtual void  _vf157() = 0;    // [157] 0x1824F1AF8
    virtual void  _vf158() = 0;    // [158] 0x1824E8294
    virtual void  _vf159() = 0;    // [159] 0x18252BE64
    virtual void  _vf160() = 0;    // [160] 0x180581868
    virtual void  _vf161() = 0;    // [161] 0x180749558
    virtual void  _vf162() = 0;    // [162] 0x18049C4DC
    virtual void  _vf163() = 0;    // [163] 0x180808F34
    virtual void  _vf164() = 0;    // [164] 0x18049DA64
    virtual void  _vf165() = 0;    // [165] 0x180A5C3D4
    virtual void  _vf166() = 0;    // [166] 0x1804E7058
    virtual void  _vf167() = 0;    // [167] 0x1804E6ED4
    virtual void  _vf168() = 0;    // [168] 0x1804E6FB4
    virtual void  _vf169() = 0;    // [169] 0x180E973BC
    virtual void  _vf170() = 0;    // [170] 0x1824F1A08
    virtual void  _vf171() = 0;    // [171] 0x1804E4454
    virtual void  _vf172() = 0;    // [172] 0x180656AB0
    virtual void  _vf173() = 0;    // [173] 0x180659864
    virtual void  _vf174() = 0;    // [174] 0x180656B14
    virtual void  _vf175() = 0;    // [175] 0x180E972B8
    virtual void  _vf176() = 0;    // [176] 0x18076D714
    virtual void  _vf177() = 0;    // [177] 0x18066429C
    virtual void  _vf178() = 0;    // [178] 0x18074939C
    virtual void  _vf179() = 0;    // [179] 0x1824F2338
    virtual void  _vf180() = 0;    // [180] 0x180C043F4
    virtual void  _vf181() = 0;    // [181] 0x1824F1944
    virtual void  _vf182() = 0;    // [182] 0x1824F18FC
    virtual void  _vf183() = 0;    // [183] 0x1824F18E0
    virtual void  _vf184() = 0;    // [184] 0x1819BA908
    virtual void  _vf185() = 0;    // [185] 0x1808BB50C
    virtual void  _vf186() = 0;    // [186] 0x1803B6E80  (shared stub)
    virtual void  _vf187() = 0;    // [187] 0x1809A7BE4
    virtual void  _vf188() = 0;    // [188] 0x1803B6E80  (shared stub)
    virtual void  _vf189() = 0;    // [189] 0x18066CD10  (shared stub)
    virtual void  _vf190() = 0;    // [190] 0x18051FA90
    virtual void  _vf191() = 0;    // [191] 0x18078BEB4
    virtual void  _vf192() = 0;    // [192] 0x1805D7EB0
    virtual void  _vf193() = 0;    // [193] 0x1803B6E80  (shared stub)
    virtual void  _vf194() = 0;    // [194] 0x1824FF5D4
    virtual void  _vf195() = 0;    // [195] 0x1824FF5EC
    virtual void  _vf196() = 0;    // [196] 0x1811525A8
    virtual void  _vf197() = 0;    // [197] 0x181A74E80
    virtual void  _vf198() = 0;    // [198] 0x181A750D0
    // Debug aux-geom/DrawLabel interface getter -- VERIFIED by full decompile:
    // CD3D9Renderer OVERRIDE (CRenderer's slot [199] is the SAME generic stub
    // 0x181d93e5d as slot [97] above -- second independent confirmation the runtime
    // object is CD3D9Renderer, not CRenderer). Thread-aware: compares
    // GetCurrentThreadId() against two cached ids at *(this+248)+240/+244; on a
    // match returns an embedded sub-object (this+12736), else forwards to
    // sub_1803D4388 for cross-thread (render-thread) access. Only used under debug
    // CVars by callers (e.g. C_UIHudBubbles's bubble-count/position dump); not
    // needed for the ProjectToScreen recipe. Return type left as void* pending its
    // own interface RE.
    virtual void* _vf199() = 0;    // [199] 0x1803D433C
    virtual void  _vf200() = 0;    // [200] 0x1803D2780
    virtual void  _vf201() = 0;    // [201] 0x180B9DA64
    virtual void  _vf202() = 0;    // [202] 0x182549E54
    virtual void  _vf203() = 0;    // [203] 0x18043E3A0
    virtual void  _vf204() = 0;    // [204] 0x1825462B0
    virtual void  _vf205() = 0;    // [205] 0x1803B6E80  (shared stub)
    virtual void  _vf206() = 0;    // [206] 0x18073B880
    virtual void  _vf207() = 0;    // [207] 0x1818178C4
    virtual void  _vf208() = 0;    // [208] 0x18254A2D0
    virtual void  _vf209() = 0;    // [209] 0x180476900
    virtual void  _vf210() = 0;    // [210] 0x1824E8FE0
    virtual void  _vf211() = 0;    // [211] 0x1814F5B9C
    virtual void  _vf212() = 0;    // [212] 0x1803DE070
    virtual void  _vf213() = 0;    // [213] 0x1824FADD4
    virtual void  _vf214() = 0;    // [214] 0x1824FAD04
    virtual void  _vf215() = 0;    // [215] 0x182522ED4
    virtual void  _vf216() = 0;    // [216] 0x180838AE0  (shared stub)
    virtual void  _vf217() = 0;    // [217] 0x180838AE0  (shared stub)
    virtual void  _vf218() = 0;    // [218] 0x180838AE0  (shared stub)
    virtual void  _vf219() = 0;    // [219] 0x180838AE0  (shared stub)
    virtual void  _vf220() = 0;    // [220] 0x18041A6A0  (shared stub)
    virtual void  _vf221() = 0;    // [221] 0x1803B6E80  (shared stub)
    virtual void  _vf222() = 0;    // [222] 0x1803B6E80  (shared stub)
    virtual void  _vf223() = 0;    // [223] 0x18254911C
    virtual void  _vf224() = 0;    // [224] 0x182548EC4
    virtual void  _vf225() = 0;    // [225] 0x180749DFC
    virtual void  _vf226() = 0;    // [226] 0x1824FE850
    virtual void  _vf227() = 0;    // [227] 0x1803B6E80  (shared stub)
    virtual void  _vf228() = 0;    // [228] 0x180A2C3E0
    virtual void  _vf229() = 0;    // [229] 0x182501F58
    virtual void  _vf230() = 0;    // [230] 0x1824FEDDC
    virtual void  _vf231() = 0;    // [231] 0x18085E8B0
    virtual void  _vf232() = 0;    // [232] 0x18085E8B0
    virtual void  _vf233() = 0;    // [233] 0x18252B6F4
    virtual void  _vf234() = 0;    // [234] 0x181E03270
    virtual void  _vf235() = 0;    // [235] 0x181446E70
    virtual void  _vf236() = 0;    // [236] 0x1824F19EC
    virtual void  _vf237() = 0;    // [237] 0x180F9C054
    virtual void  _vf238() = 0;    // [238] 0x180A558D0
    virtual void  _vf239() = 0;    // [239] 0x18146E350
    virtual void  _vf240() = 0;    // [240] 0x181A75000
    virtual void  _vf241() = 0;    // [241] 0x1803D2718
    virtual void  _vf242() = 0;    // [242] 0x180F35DC8
    virtual void  _vf243() = 0;    // [243] 0x1808911F4
    virtual void  _vf244() = 0;    // [244] 0x180892330
    virtual void  _vf245() = 0;    // [245] 0x18252A6F4
    virtual void  _vf246() = 0;    // [246] 0x18252ACE8
    virtual void  _vf247() = 0;    // [247] 0x180891C1C
    virtual void  _vf248() = 0;    // [248] 0x180F36590
    virtual void  _vf249() = 0;    // [249] 0x1824E8C80
    virtual void  _vf250() = 0;    // [250] 0x1824E8DB4
    virtual void  _vf251() = 0;    // [251] 0x18066CD10  (shared stub)
    virtual void  _vf252() = 0;    // [252] 0x18066CD10  (shared stub)
    virtual void  _vf253() = 0;    // [253] 0x18066CD10  (shared stub)
    virtual void  _vf254() = 0;    // [254] 0x18066CD10  (shared stub)
    virtual void  _vf255() = 0;    // [255] 0x1807A1F98
    virtual void  _vf256() = 0;    // [256] 0x1807A201C
    virtual void  _vf257() = 0;    // [257] 0x1807AB140
    virtual void  _vf258() = 0;    // [258] 0x182546250
    virtual void  _vf259() = 0;    // [259] 0x180C70250
    virtual void  _vf260() = 0;    // [260] 0x18250019C
    virtual void  _vf261() = 0;    // [261] 0x180C70214
    virtual void  _vf262() = 0;    // [262] 0x180C70354
    virtual void  _vf263() = 0;    // [263] 0x181A74B80
    virtual void  _vf264() = 0;    // [264] 0x1803DD908
    virtual void  _vf265() = 0;    // [265] 0x180C1D37C
    virtual void  _vf266() = 0;    // [266] 0x1819DF530
    virtual void  _vf267() = 0;    // [267] 0x18043FBA0
    virtual void  _vf268() = 0;    // [268] 0x1806525EC
    virtual void  _vf269() = 0;    // [269] 0x1813970D0
    virtual void  _vf270() = 0;    // [270] 0x18056A7A0
    virtual void  _vf271() = 0;    // [271] 0x181A74EF0
    virtual void  _vf272() = 0;    // [272] 0x181396F64
    virtual void  _vf273() = 0;    // [273] 0x1804BF8D0
    virtual void  _vf274() = 0;    // [274] 0x1804BF88C
    virtual void  _vf275() = 0;    // [275] 0x182500DB0
    virtual void  _vf276() = 0;    // [276] 0x1824F9B98
    virtual void  _vf277() = 0;    // [277] 0x1803B6E80  (shared stub)
    virtual void  _vf278() = 0;    // [278] 0x1803B6E80  (shared stub)
    virtual void  _vf279() = 0;    // [279] 0x1824FA54C
    virtual void  _vf280() = 0;    // [280] 0x181A74C40
    virtual void  _vf281() = 0;    // [281] 0x181A74B10
    virtual void  _vf282() = 0;    // [282] 0x18196F660
    virtual void  _vf283() = 0;    // [283] 0x1819DCF60
    virtual void  _vf284() = 0;    // [284] 0x181A74970
    virtual void  _vf285() = 0;    // [285] 0x181A74980
    virtual void  _vf286() = 0;    // [286] 0x1803D5BF0
    virtual void  _vf287() = 0;    // [287] 0x1803D5EC0
    virtual void  _vf288() = 0;    // [288] 0x180FC2B60
    virtual void  _vf289() = 0;    // [289] 0x1804A48F0
    virtual void  _vf290() = 0;    // [290] 0x1824F23D4
    virtual void  _vf291() = 0;    // [291] 0x1805F97D8
    virtual void  _vf292() = 0;    // [292] 0x180C16CD8
    virtual void  _vf293() = 0;    // [293] 0x180C16D64
    virtual void  _vf294() = 0;    // [294] 0x181257E9C
    virtual void  _vf295() = 0;    // [295] 0x1825035C8
    virtual void  _vf296() = 0;    // [296] 0x181A74FA0
    virtual void  _vf297() = 0;    // [297] 0x181A75060
    virtual void  _vf298() = 0;    // [298] 0x181A74FD0
    virtual void  _vf299() = 0;    // [299] 0x180838AE0  (shared stub)
    virtual void  _vf300() = 0;    // [300] 0x181294530
    virtual void  _vf301() = 0;    // [301] 0x180862C28
    virtual void  _vf302() = 0;    // [302] 0x182549E88
    virtual void  _vf303() = 0;    // [303] 0x181354340
    virtual void  _vf304() = 0;    // [304] 0x1819DFBA4
    virtual void  _vf305() = 0;    // [305] 0x1803B6E80  (shared stub)
    virtual void  _vf306() = 0;    // [306] 0x1805F8970
    virtual void  _vf307() = 0;    // [307] 0x18041A6A0  (shared stub)
    virtual void  _vf308() = 0;    // [308] 0x1803B6E80  (shared stub)
    virtual void  _vf309() = 0;    // [309] 0x1815EAD34
    virtual void  _vf310() = 0;    // [310] 0x180838AE0  (shared stub)
    virtual void  _vf311() = 0;    // [311] 0x1803B6E80  (shared stub)
    virtual void  _vf312() = 0;    // [312] 0x1819C939C
    virtual void  _vf313() = 0;    // [313] 0x181DF9970
    virtual void  _vf314() = 0;    // [314] 0x181DF9930
    virtual void  _vf315() = 0;    // [315] 0x1811EEB60
    virtual void  _vf316() = 0;    // [316] 0x180977508
    virtual void  _vf317() = 0;    // [317] 0x181DEC3A0
    virtual void  _vf318() = 0;    // [318] 0x181DEC320
    virtual void  _vf319() = 0;    // [319] 0x181DEC210
    virtual void  _vf320() = 0;    // [320] 0x181DEC7E0
    virtual void  _vf321() = 0;    // [321] 0x1806BDE78
    virtual void  _vf322() = 0;    // [322] 0x1809A6440
    virtual void  _vf323() = 0;    // [323] 0x180654F40
    virtual void  _vf324() = 0;    // [324] 0x180654F70
    virtual void  _vf325() = 0;    // [325] 0x18066CD10  (shared stub)
    virtual void  _vf326() = 0;    // [326] 0x18066CD10  (shared stub)
    virtual void  _vf327() = 0;    // [327] 0x1812C2E70
    virtual void  _vf328() = 0;    // [328] 0x1817AA4A0
    virtual void  _vf329() = 0;    // [329] 0x1808649FC
    virtual void  _vf330() = 0;    // [330] 0x181DEC8D0
    virtual void  _vf331() = 0;    // [331] 0x181E03AD0
    virtual void  _vf332() = 0;    // [332] 0x181DEC950
    virtual void  _vf333() = 0;    // [333] 0x1804FE2DC
    virtual void  _vf334() = 0;    // [334] 0x18250B3A4
    virtual void  _vf335() = 0;    // [335] 0x1805F5DA0
    virtual void  _vf336() = 0;    // [336] 0x18250B2C8
    virtual void  _vf337() = 0;    // [337] 0x180FB0798
    virtual void  _vf338() = 0;    // [338] 0x1812038FC
    virtual void  _vf339() = 0;    // [339] 0x1825488E8
    virtual void  _vf340() = 0;    // [340] 0x180861260
    virtual void  _vf341() = 0;    // [341] 0x180C17A1C
    virtual void  _vf342() = 0;    // [342] 0x18086264C
    virtual void  _vf343() = 0;    // [343] 0x18252A060
    virtual void  _vf344() = 0;    // [344] 0x18252A0A0
    virtual void  _vf345() = 0;    // [345] 0x1823DCFD8
    virtual void  _vf346() = 0;    // [346] 0x1803B6E80  (shared stub)
    virtual void  _vf347() = 0;    // [347] 0x180864728
    virtual void  _vf348() = 0;    // [348] 0x180DE4EC8
    virtual void  _vf349() = 0;    // [349] 0x180FD21B4
    virtual void  _vf350() = 0;    // [350] 0x181683268
    virtual void  _vf351() = 0;    // [351] 0x18197AB8C
    virtual void  _vf352() = 0;    // [352] 0x181E04610
    virtual void  _vf353() = 0;    // [353] 0x181E04BC0
    virtual void  _vf354() = 0;    // [354] 0x181E04880
    virtual void  _vf355() = 0;    // [355] 0x180860D2C
    virtual void  _vf356() = 0;    // [356] 0x1803B6E80  (shared stub)
    virtual void  _vf357() = 0;    // [357] 0x1803FA7A0
    virtual void  _vf358() = 0;    // [358] 0x1803FA774
    virtual void  _vf359() = 0;    // [359] 0x18252A01C
    virtual void  _vf360() = 0;    // [360] 0x1819DE1E0
    virtual void  _vf361() = 0;    // [361] 0x1806B7964
    virtual void  _vf362() = 0;    // [362] 0x1803B6E80  (shared stub)
    virtual void  _vf363() = 0;    // [363] 0x1803B6E80  (shared stub)
    virtual void  _vf364() = 0;    // [364] 0x181A74DC0
    virtual void  _vf365() = 0;    // [365] 0x182545A70
    virtual void  _vf366() = 0;    // [366] 0x1805F987C
    virtual void  _vf367() = 0;    // [367] 0x1803D5D34
    virtual void  _vf368() = 0;    // [368] 0x180749808
    virtual void  _vf369() = 0;    // [369] 0x1803B6E80  (shared stub)
    virtual void  _vf370() = 0;    // [370] 0x18066CD10  (shared stub)
    virtual void  _vf371() = 0;    // [371] 0x18066CD10  (shared stub)
    virtual void  _vf372() = 0;    // [372] 0x181A75850
    virtual void  _vf373() = 0;    // [373] 0x1803B6E80  (shared stub)
    virtual void  _vf374() = 0;    // [374] 0x181A75730
    virtual void  _vf375() = 0;    // [375] 0x181A740C0
    virtual void  _vf376() = 0;    // [376] 0x1808630D8
    virtual void  _vf377() = 0;    // [377] 0x1807C2D40
    virtual void  _vf378() = 0;    // [378] 0x18086466C
    virtual void  _vf379() = 0;    // [379] 0x181203928
    virtual void  _vf380() = 0;    // [380] 0x182548834
};

}  // namespace Offsets
