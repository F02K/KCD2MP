#pragma once
#include <cstdint>
#include "Offsets/vtables/IFlashPlayer.h"
#include "Offsets/vtables/IFlashPlayer_RenderProxy.h"
#include "Scaleform/GFxMovieRoot.h"

// -----------------------------------------------
// CFlashPlayer -- CryEngine's thread-safe Scaleform wrapper (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// The concrete class behind every Offsets::IFlashPlayer. RTTI .?AVCFlashPlayer@@,
// sizeof 0x288, std::shared_ptr-managed (make_shared factory 0x18088FAB8; never
// delete/Release one -- drop the shared_ptr). Bases via RTTI COLs:
// IFlashPlayer at +0x00 (vtable 0x183DC07A8), IFlashPlayer_RenderProxy at +0x08
// (vtable 0x183DC09B8). Ctor 0x18088FD78.
//
// Field identities below are the VERIFIED ones from the per-slot analysis in
// Offsets/vtables/IFlashPlayer.h (each wrapper slot's decompile names the field
// it touches); everything else stays opaque. The one that matters for mods:
// +0xD0 is the raw GFxMovieView* every wrapper slot forwards to under the
// SRWLOCK at +0xF8 -- and the concrete movie object is a GFxMovieRoot
// (its Advance slot [37] == 0x181CA4D90, the function CFlashPlayer::Advance
// invokes via vf+0x128 on this exact pointer). GetMovieRoot() is the supported
// way to reach the movie's load queue (see GFxMovieRoot.h).

struct CFlashPlayer : Offsets::IFlashPlayer, Offsets::IFlashPlayer_RenderProxy {
    void*                 m_weakSelf[2];        // +0x10  enable_shared_from_this weak ref (factory 0x18088FAB8)
    uint32_t              m_clearFlags;         // +0x20  SetClearFlags [14]
    float                 m_clearColor[4];      // +0x24  ColorF, SetClearFlags [14]
    float                 m_compositingDepth;   // +0x34  SetCompositingDepth [15]
    float                 m_stereoMaxParallax;  // +0x38  StereoSetCustomMaxParallax [17], ctor -1.0f
    uint8_t               m_renderFlags[2];     // +0x3C  bit1 fixed-depth, bit2/bit3/+0x3D bit5 = the 3 bool render flags
    uint8_t               _pad3E[2];            // +0x3E
    uint8_t               _unk40[0x58];         // +0x40  NOT MAPPED
    uint32_t              m_frameCount;         // +0x98  GetFrameCount [52] (written by Load 0x180890458)
    float                 m_frameRate;          // +0x9C  GetFrameRate [53]
    int32_t               m_width;              // +0xA0  GetWidth [54]
    int32_t               m_height;             // +0xA4  GetHeight [55]
    IFSCommandHandler*    m_pFSCommandHandler;  // +0xA8  SetFSCommandHandler [29]
    void*                 m_pFSCommandData;     // +0xB0
    IExternalInterfaceHandler* m_pExtIfHandler; // +0xB8  SetExternalInterfaceHandler [30]
    void*                 m_pExtIfData;         // +0xC0
    GFxMovieDef*          m_pMovieDef;          // +0xC8  movie-def queries (GetMetadata [56], SetOverrideTexture [64])
    GFxMovieRoot*         m_pMovie;             // +0xD0  THE movie instance (see banner); null until Load
    void*                 _unkD8;               // +0xD8
    void*                 m_pRecording;         // +0xE0  process-global CScaleformRecording (vtable 0x183DC0338)
    const char**          m_pFilePath;          // +0xE8  shared filepath cell (GetFilePath [58]; handed to each CFlashVariableObject)
    void*                 _unkF0;               // +0xF0
    void*                 m_lock;               // +0xF8  SRWLOCK taken by nearly every wrapper slot
    void*                 _unk100;              // +0x100
    uint8_t               _tail[0x288 - 0x108]; // +0x108 returned-string cache + NOT MAPPED
    // The movie instance, typed. May be null (player created but no movie loaded).
    GFxMovieRoot* GetMovieRoot() const { return m_pMovie; }
};
static_assert(sizeof(CFlashPlayer) == 0x288, "CFlashPlayer must be 0x288 (make_shared alloc 0x298 = 0x10 hdr + 0x288)");
static_assert(offsetof(CFlashPlayer, m_frameCount) == 0x98, "frame count at 0x98");
static_assert(offsetof(CFlashPlayer, m_pMovie) == 0xD0, "GFxMovieRoot* at 0xD0");
static_assert(offsetof(CFlashPlayer, m_lock) == 0xF8, "SRWLOCK at 0xF8");
