#pragma once
#include <cstdint>
#include "IFlashVariableObject.h"
#include "CryEngine/CryCommon/SFlashVarValue.h"    // global SFlashVarValue (NOT the SDK IFlashPlayer.h — Cry_Math swamp + stale structs)
#include "CryEngine/CryCommon/SFlashCxform.h"      // KCD2-binary-layout replica (stock SDK shape is stale)
#include "CryEngine/CryCommon/SFlashDisplayInfo.h" // KCD2-binary-layout replica (stock SDK shape is stale)

// -----------------------------------------------
// IFlashPlayer — KCD2 binary vtable order (INTERFUSCATED)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u). SDK: CryEngine CryCommon/IFlashPlayer.h
// (<interfuscator:shuffle> block => binary slot order != SDK declaration order;
// this replica is BINARY order, matched slot-by-slot against the KCD1 named map
// by GFx vfunc offsets / strings / field writes — NOT by position).
// RTTI: .?AUIFlashPlayer@@ (global namespace, struct)
// Impl: CFlashPlayer (.?AVCFlashPlayer@@), sizeof == 0x288
//   - primary base (+0x00) IFlashPlayer             -> vtable 0x183DC07A8 (this replica, 65 slots)
//   - second  base (+0x08) IFlashPlayer_RenderProxy -> vtable 0x183DC09B8 (see IFlashPlayer_RenderProxy.h)
//   (base offsets verified via RTTI COLs @ 0x18418C020/+0x0 and 0x18418C048/+0x8)
//
// [FUNDAMENTAL vs KCD1] CFlashPlayer is now std::shared_ptr-managed
// (std::_Ref_count_obj2<CFlashPlayer>, enable_shared_from_this weak ref at
// +0x10/+0x18; factory make_shared 0x18088FAB8, alloc 0x298 = 0x10 hdr + 0x288).
// AddRef/Release (KCD1 slots [0]/[1]) are GONE from the vtable. Never delete or
// Release a KCD2 IFlashPlayer — drop the shared_ptr instead.
// 4 slots are NEW vs KCD1: SetClearFlags [14], RenderToTexture [21],
// HitTest [34], GetPlayback [62]. Render() [13] LOST its bool-stereo param.
// KCD1's 63 slots − AddRef − Release + 4 new == 65.
//
// CFlashPlayer remains the thread-safe wrapper around statically linked
// Scaleform GFx: nearly every slot takes the shared SRWLOCK at this+0xF8
// (KCD1: CRITICAL_SECTION) and forwards to GFxMovieView* at this+0xD0
// (movie-def queries use GFxMovieDef* at this+0xC8). The GFx-side vfunc
// offsets are IDENTICAL to KCD1 (GetCurrentFrame +0x10 ... HandleEvent +0x168),
// which is what makes each slot's identification exact; the CFlashPlayer field
// block simply shifted +0x18 vs KCD1 (frameCount 0x80->0x98, movie 0xB8->0xD0,
// filepath cell 0xD0->0xE8, lock 0xF8 stays, string cache 0x108 stays).
// Render dispatch changed: the ctor (0x18088FD78) acquires the process-global
// CScaleformRecording at +0xE0 (vtable 0x183DC0338) whose GetPlayback (+0x1A8)
// returns the CScaleformPlayback (vtable 0x183DF4030) used by the render path.
//
// UNVERIFIED (flagged inline): slots [18][19][20] are three bool setters writing
// bit2/bit3 of +0x3C and bit5 of +0x3D (pushed to the recording renderer by the
// render proxy, vf+472/+480/+488). They are AvoidStencilClear /
// EnableMaskedRendering / ExtendCanvasToViewport; the 1:1 assignment among the
// three rests only on SDK member-declaration-order correlation (same caveat as
// KCD1, same bit positions). Slot [21]'s NAME is coined from behavior (renders
// the movie into a caller-supplied texture); slots [14][34][62] carry
// CRYENGINE-V API names whose behavior matches this binary exactly.

struct SFlashCursorEvent;
struct SFlashKeyEvent;
struct SFlashCharEvent;
struct IFSCommandHandler;
struct IExternalInterfaceHandler;
struct IActionScriptFunction;
struct IScaleformPlayback;   // renderer-side command playback (CScaleformPlayback vtable 0x183DF4030)
struct ITexture;
template<typename T> struct Color_tpl;
typedef Color_tpl<unsigned char> ColorB;   // matches CryCommon Cry_Color.h typedef
typedef Color_tpl<float>         ColorF;

namespace Offsets {

struct IFlashPlayer {
    enum EScaleModeType { eSM_NoScale, eSM_ShowAll, eSM_ExactFit, eSM_NoBorder };
    enum EAlignType {
        eAT_Center, eAT_TopCenter, eAT_BottomCenter, eAT_CenterLeft, eAT_CenterRight,
        eAT_TopLeft, eAT_TopRight, eAT_BottomLeft, eAT_BottomRight
    };
    enum EFlashVariableArrayType { FVAT_Int, FVAT_Double, FVAT_Float, FVAT_ConstStrPtr, FVAT_ConstWstrPtr };

    // Binary vtable 0x183DC07A8 (65 slots)
    virtual bool Load(const char* pFilePath, unsigned int options = 0x1C,
                      unsigned int cat = 0) = 0;                            // [0]  0x000 0x18244E308  "Trying to load non-stripped flash movie..."; impl 0x180890458 (options bits 4/8/0x10 = EdgeAA/InitFirstFrame/Mouse verified)
    virtual void SetBackgroundColor(const ColorB& color) = 0;               // [1]  0x008 0x182450FE8  repacks 4 colour bytes, GFxMovieView vf+0x150
    virtual void SetBackgroundAlpha(float alpha) = 0;                       // [2]  0x010 0x18056B84C  GFxMovieView vf+0x158 (float stays in xmm1)
    virtual float GetBackgroundAlpha() const = 0;                           // [3]  0x018 0x18244C600  GFxMovieView vf+0x160 (double->float), 1.0f when no movie
    virtual void SetViewport(int x0, int y0, int width, int height,
                             float aspectRatio = 1.0f) = 0;                 // [4]  0x020 0x18056B8A0  builds GViewport, aspect<=0 -> 1.0f, GFxMovieView vf+0xC8
    virtual void GetViewport(int& x0, int& y0, int& width, int& height,
                             float& aspectRatio) const = 0;                 // [5]  0x028 0x18056BA1C  GFxMovieView vf+0xD0
    virtual void SetViewScaleMode(EScaleModeType scaleMode) = 0;            // [6]  0x030 0x18088E624  enum remap 0..3, GFxMovieView vf+0xD8
    virtual EScaleModeType GetViewScaleMode() const = 0;                    // [7]  0x038 0x18244D64C  GFxMovieView vf+0xE0 (inverse remap)
    virtual void SetViewAlignment(EAlignType viewAlignment) = 0;            // [8]  0x040 0x18173FC80  enum remap 0..8, GFxMovieView vf+0xE8
    virtual EAlignType GetViewAlignment() const = 0;                        // [9]  0x048 0x18244D598  GFxMovieView vf+0xF0
    virtual void SetScissorRect(int x0, int y0, int width, int height) = 0; // [10] 0x050 0x18056B958  GetViewport vf+0xD0, OR flag 0x4 (scissor), SetViewport vf+0xC8
    virtual void GetScissorRect(int& x0, int& y0, int& width,
                                int& height) const = 0;                     // [11] 0x058 0x18244CEF8  vf+0xD0, checks scissor flag bit 0x4
    virtual void Advance(float deltaTime) = 0;                              // [12] 0x060 0x18085F0A8  GFxMovieView vf+0x128 = GFxMovie::Advance(dt, catchUp cvar dword_18492DD68); also +0x90 = (dword_18557028C>>1)&1
    virtual void Render() = 0;                                              // [13] 0x068 0x1803FAB84  KCD1 Render(bool stereo) -> no args (rdx unused, verified in disasm); locks weak self (+0x18), renderer qword_18492D908 vf+0x360(&shared_ptr<RenderProxy>(this+8))
    virtual void SetClearFlags(uint32_t clearFlags,
                               const ColorF& clearColor) = 0;               // [14] 0x070 0x18073EF90  KCD2-NEW (CE5 API name): this+0x20 = flags, +0x24..0x33 = ColorF; RenderCallback pushes them via recording vf+432
    virtual void SetCompositingDepth(float depth) = 0;                      // [15] 0x078 0x181A73250  this+0x34 = depth; ctor default 0.0f (ICF-folded body)
    virtual void StereoEnforceFixedProjectionDepth(bool enforce) = 0;       // [16] 0x080 0x18056ADA0  bit1 of +0x3C; render proxy pushes via recording vf+456
    virtual void StereoSetCustomMaxParallax(float maxParallax = -1.0f) = 0; // [17] 0x088 0x181A728D0  this+0x38 = v; ctor default 0xBF800000 = -1.0f == SDK default
    virtual void AvoidStencilClear(bool avoid) = 0;                         // [18] 0x090 0x181A73020  bit2 of +0x3C -> recording vf+472   [UNVERIFIED name: one of the 3 bool render flags, see banner]
    virtual void EnableMaskedRendering(bool enable) = 0;                    // [19] 0x098 0x181A73080  bit3 of +0x3C -> recording vf+480   [UNVERIFIED name: one of the 3 bool render flags, see banner]
    virtual void ExtendCanvasToViewport(bool extend) = 0;                   // [20] 0x0A0 0x181A73090  bit5 of +0x3D -> recording vf+488   [UNVERIFIED name: one of the 3 bool render flags, see banner]
    virtual void RenderToTexture(ITexture* pTexture) = 0;                   // [21] 0x0A8 0x18056B800  KCD2-NEW, NAME COINED: playback vf+0x120 -> make_shared<CRenderOutput>(pTexture, viewport from GetViewport), swap into playback+0x418, self Render() vf+0x68, restore (sub_18056B09C)
    virtual void Restart() = 0;                                             // [22] 0x0B0 0x18244FFC0  GFxMovieView vf+0x120
    virtual bool IsPaused() const = 0;                                      // [23] 0x0B8 0x18056B70C  GFxMovieView vf+0x148
    virtual void Pause(bool pause) = 0;                                     // [24] 0x0C0 0x18244EEF4  GFxMovieView vf+0x140 (SetPause)
    virtual void GotoFrame(unsigned int frameNumber) = 0;                   // [25] 0x0C8 0x18244D780  GFxMovie vf+0x20
    virtual bool GotoLabeledFrame(const char* pLabel, int offset = 0) = 0;  // [26] 0x0D0 0x18244D7D0  GFxMovie vf+0x28, null label rejected
    virtual unsigned int GetCurrentFrame() const = 0;                       // [27] 0x0D8 0x18244C70C  GFxMovie vf+0x10
    virtual bool HasLooped() const = 0;                                     // [28] 0x0E0 0x18244D93C  GFxMovie vf+0x18
    virtual void SetFSCommandHandler(IFSCommandHandler* pHandler,
                                     void* pUserData = 0) = 0;              // [29] 0x0E8 0x180890150  stores this+0xA8 / this+0xB0
    virtual void SetExternalInterfaceHandler(IExternalInterfaceHandler* pHandler,
                                             void* pUserData = 0) = 0;      // [30] 0x0F0 0x180890140  stores this+0xB8 / this+0xC0
    virtual void SendCursorEvent(const SFlashCursorEvent& cursorEvent) = 0; // [31] 0x0F8 0x18056A970  state {0..3}->GFxEvent{1..4}, m_cursorX@+4/m_cursorY@+8 (low 16 bits -> float), m_button@+0xC, m_wheelScrollVal@+0x10; vf+0x168 HandleEvent
    virtual void SendKeyEvent(const SFlashKeyEvent& keyEvent) = 0;          // [32] 0x100 0x182450CB8  state 0/1 -> GFxEvent 5/6 (KeyDown/KeyUp), remaps m_specialKeyState bits, m_keyCode@+4 m_asciiCode@+9; vf+0x168
    virtual void SendCharEvent(const SFlashCharEvent& charEvent) = 0;       // [33] 0x108 0x182450B1C  GFxEvent type 0xD; gamepad/IME path (qword_18492D840 vf+344 & 0x11) synthesizes a KeyEvent via self vf+0x100 instead
    virtual bool HitTest(float x, float y) const = 0;                       // [34] 0x110 0x18244DB08  KCD2-NEW (CE5 API name): GFxMovieView vf+0x180 HitTest(x, y, testType=3, controllerIdx=0); x/y stay in xmm1/xmm2 (disasm-verified)
    virtual void SetVisible(bool visible) = 0;                              // [35] 0x118 0x1824519D8  GFxMovie vf+0x40
    virtual bool GetVisible() const = 0;                                    // [36] 0x120 0x18244D6C4  GFxMovie vf+0x48 (IsVisible)
    virtual void _vf37() = 0;                                               // [37] 0x128 0x182451280  NO-OP (SRWLOCK acquire/release only). Same stub as KCD1 [36]: likely RenderRecordLockless or RenderPlaybackLockless — UNVERIFIED, do not call
    // SetVariable/GetVariable overload pairs: SFlashVarValue& declared FIRST so
    // MSVC's overload-reversal lands it at the HIGHER slot the binary uses
    // (same within-pair order as KCD1).
    virtual bool SetVariable(const char* pPathToVar,
                             const SFlashVarValue& value) = 0;              // [39] 0x138 0x1824517A4  SFlashVarValue->GFxValue (sub_1807F6490); GFxMovie vf+0x80
    virtual bool SetVariable(const char* pPathToVar,
                             const IFlashVariableObject* pVarObj) = 0;      // [38] 0x130 0x182451844  GFxValue copied from pVarObj+8 (sub_1807F5ED0), nullptr -> undefined; GFxMovie vf+0x80
    virtual bool GetVariable(const char* pPathToVar,
                             SFlashVarValue& value) const = 0;              // [41] 0x148 0x18244D384  vf+0x88; GFxValue->SFlashVarValue sub_1807F659C; (w)string results cached at this+0x108
    virtual bool GetVariable(const char* pPathToVar,
                             IFlashVariableObject*& pVarObj) const = 0;     // [40] 0x140 0x180890324  GFxMovie vf+0x88; wraps result: operator new(0x70) + CFlashVariableObject::Ctor_1807F5E50
    virtual bool IsAvailable(const char* pPathToVar) const = 0;             // [42] 0x150 0x1805686C8  GFxMovie vf+0x50 (VariableExists)
    virtual bool SetVariableArray(EFlashVariableArrayType type, const char* pPathToVar,
                                  unsigned int index, const void* pData,
                                  unsigned int count) = 0;                  // [43] 0x158 0x182451924  type remap sub_18244C5BC; GFxMovie vf+0x90 (..., setType=1)
    virtual unsigned int GetVariableArraySize(const char* pPathToVar) const = 0; // [44] 0x160 0x18244D534  GFxMovie vf+0xA0
    virtual bool GetVariableArray(EFlashVariableArrayType type, const char* pPathToVar,
                                  unsigned int index, void* pData,
                                  unsigned int count) const = 0;            // [45] 0x168 0x18244D48C  GFxMovie vf+0xA8; pData==null rejected
    virtual bool Invoke(const char* pMethodName, const SFlashVarValue* pArgs,
                        unsigned int numArgs, SFlashVarValue* pResult = 0) = 0; // [46] 0x170 0x1814914E0  alloca GFxValue[numArgs] from 0x10-stride SFlashVarValue array (sub_1807F6490); GFxMovie vf+0xB8; result via sub_1807F659C
    virtual bool CreateString(const char* pString,
                              IFlashVariableObject*& pVarObj) = 0;          // [47] 0x178 0x18244B8FC  GFxMovieView vf+0x58; checks GFxValue type==4 (string); new(0x70) CFlashVariableObject
    virtual bool CreateStringW(const wchar_t* pString,
                               IFlashVariableObject*& pVarObj) = 0;         // [48] 0x180 0x18244BA24  GFxMovieView vf+0x60; type==5 (wstring)
    virtual bool CreateObject(const char* pClassName, const SFlashVarValue* pArgs,
                              unsigned int numArgs,
                              IFlashVariableObject*& pVarObj) = 0;          // [49] 0x188 0x18244B69C  GFxMovieView vf+0x68; arg array conversion as Invoke; object-type guard sub_1807F5F58
    virtual bool CreateArray(IFlashVariableObject*& pVarObj) = 0;           // [50] 0x190 0x18244ACE4  GFxMovieView vf+0x70; type==7 (array)
    virtual bool CreateFunction(IFlashVariableObject*& pFuncVarObj, IActionScriptFunction* pFunc,
                                void* pUserData = 0) = 0;                   // [51] 0x198 0x18244B4E0  builds FunctionHandlerAdaptor (named vftable) holding pFunc+player; GFxMovieView vf+0x78
    virtual unsigned int GetFrameCount() const = 0;                         // [52] 0x1A0 0x181A73120  returns dword this+0x98 (written by Load 0x180890458)
    virtual float GetFrameRate() const = 0;                                 // [53] 0x1A8 0x181A73130  returns float this+0x9C
    virtual int GetWidth() const = 0;                                       // [54] 0x1B0 0x181A731C0  returns dword this+0xA0 (ICF-folded with Concurrency::WorkQueue::Id — IDA name is bogus)
    virtual int GetHeight() const = 0;                                      // [55] 0x1B8 0x181A73140  returns dword this+0xA4 (ICF-folded with Concurrency::SchedulingRing::Id)
    virtual size_t GetMetadata(char* pBuff, unsigned int buffSize) const = 0; // [56] 0x1C0 0x18244CDD0  GFxMovieDef(this+0xC8) vf+0x80; rdx/r8 passthrough; [57] calls this slot twice (size probe + fill)
    virtual bool HasMetadata(const char* pTag) const = 0;                   // [57] 0x1C8 0x18244DA04  GetMetadata (self vf+0x1C0) into temp buffer, then case-insensitive substring search (_strnicmp loop). Name as KCD1 [56] (UNVERIFIED, not in our SDK copy)
    virtual const char* GetFilePath() const = 0;                            // [58] 0x1D0 0x181A730C0  returns **(this+0xE8) (shared filepath cell, also handed to each CFlashVariableObject)
    virtual void ResetDirtyFlags() = 0;                                     // [59] 0x1D8 0x18085D7F0  fetches CryGFxTranslator singleton (sub_18085D804) and clears its dirty byte at +0x28
    virtual void ScreenToClient(int& x, int& y) const = 0;                  // [60] 0x1E0 0x18056ADAC  GetViewport vf+0xD0; x -= x0, y -= y0
    virtual void ClientToScreen(int& x, int& y) const = 0;                  // [61] 0x1E8 0x182449C90  GetViewport vf+0xD0; x += x0, y += y0
    virtual IScaleformPlayback* GetPlayback() = 0;                          // [62] 0x1F0 0x18056B14C  KCD2-NEW (CE5 API name): recording(+0xE0) vf+0x1A8 -> CScaleformPlayback* (recording+0xE8); consumed by RenderToTexture helper and RenderProxy [1]/[3]
    virtual ~IFlashPlayer() {}                                              // [63] 0x1F8 0x18089184C  scalar deleting dtor -> ~CFlashPlayer sub_180891878 (+free sub_181AB5160 if flags&1); destruction normally via shared_ptr control block
    virtual bool SetOverrideTexture(const char* pResourceName, ITexture* pTexture,
                                    bool resize = true) = 0;                // [64] 0x200 0x182451434  GFxMovieDef vf+0xD8 GetResource(name), image-type check (vf+0x10()&0xFF00)==0x100, wraps texture in GImageInfoTextureXRender (named vftable, new(0x30)), swaps into resource+0x18. Name as KCD1 [62] (UNVERIFIED, not in our SDK copy)
};

}  // namespace Offsets
