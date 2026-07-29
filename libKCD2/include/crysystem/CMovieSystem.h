#pragma once
#include <cstdint>
#include "../Offsets/Offsets_VTABLE.h"
#include "../Offsets/vtables/IMovieSystem.h"
#include "SCameraParams.h"

// -----------------------------------------------
// CMovieSystem : IMovieSystem  (CryMovie / TrackView) -- KCD2 WHGame.dll 1.5.6 (kd7u)
// -----------------------------------------------
// RTTI ??_7CMovieSystem@@6B@   primary vtable 0x18407E648   ctor sub_180D61DA4
// sizeof 0xBC8 (scalar-deleting dtor slot0 @0x18387B080 -> operator delete(this,3016)).
// Single base (IMovieSystem @ +0x00). GetCameraParams=slot59, SetCameraParams=slot60.
//
// [FUNDAMENTAL vs KCD1] primary vtable RVA 0x279FD10 -> 0x407E648, and the get/set
// camera-params pair shifted +2 (vtable grew 92->93 slots): SetCameraParams slot 58 -> 60.
// [MODERATE vs KCD1] m_pUser +0x70->+0x40, m_pCallback +0x78->+0x48, m_cameraParams +0xE4->+0x118.

namespace Offsets { struct ISystem; struct IMovieUser; struct IMovieCallback; }

struct CMovieSystem : Offsets::IMovieSystem {   // vtable @ +0x00
    inline static constexpr auto VTABLE = Offsets::VTABLE_CMovieSystem;  // primary vtable @ 0x407E648 (slot 60 = SetCameraParams)

    CryStringT<char>     m_str08;            // +0x08  CryStringT<char> (ctor stores shared-empty &unk_185676010)
    int16_t              m_id08_10;          // +0x10  ctor -1 (sentinel index)
    int16_t              m_id08_12;          // +0x12  ctor -1 (sentinel index)
    uint8_t              _pad08_14[4];       // +0x14
    CryStringT<char>     m_str18;            // +0x18  CryStringT<char> (ctor = shared empty)
    int16_t              m_id18_20;          // +0x20  ctor -1
    int16_t              m_id18_22;          // +0x22  ctor -1
    uint8_t              _pad18_24[4];       // +0x24
    CryStringT<char>     m_str28;            // +0x28  CryStringT<char> (ctor = shared empty)
    int16_t              m_id28_30;          // +0x30  ctor -1
    int16_t              m_id28_32;          // +0x32  ctor -1
    uint8_t              _pad28_34[4];       // +0x34
    Offsets::ISystem*    m_pSystem;          // +0x38  ctor arg                                    VERIFIED
    Offsets::IMovieUser* m_pUser;            // +0x40  notified by SetCameraParams (user-vtbl+0x08) VERIFIED
    Offsets::IMovieCallback* m_pCallback;    // +0x48  notified by SetCameraParams (cb-vtbl+0x10 = OnSetCamera) VERIFIED
    void*                m_field50;          // +0x50  ctor 0; not freed in dtor (non-owning ptr / handle)
    std::vector<void*>   m_vec58;            // +0x58  std::vector, 8-byte _smart_ptr elems (dtor sub_180FB8E30 Releases each) -- inferred m_sequences
    std::vector<void*>   m_vec70;            // +0x70  std::vector, 8-byte _smart_ptr elems (dtor sub_180FB8E30 Releases each) -- inferred m_movieSequences
    std::vector<void*>   m_vec88;            // +0x88  std::vector, 0x20-byte POD elems (dtor frees buffer only; stride mask ~0x1F)
    std::vector<void*>   m_vecA0;            // +0xA0  std::vector, 8-byte POD elems (dtor frees buffer only)
    void*                m_lockB8;           // +0xB8  SRWLOCK (InitializeSRWLock @ctor)
    void*                m_lockB8_head;      // +0xC0  ptr guarded by m_lockB8 (ctor 0)
    void*                m_lockC8;           // +0xC8  SRWLOCK (InitializeSRWLock @ctor)
    void*                m_lockC8_head;      // +0xD0  ptr guarded by m_lockC8 (ctor 0)
    void*                m_lockD8;           // +0xD8  SRWLOCK (InitializeSRWLock @ctor)
    void*                m_lockD8_head;      // +0xE0  ptr guarded by m_lockD8 (ctor 0)
    std::vector<void*>   m_vecE8;            // +0xE8  std::vector, 0x20-byte elems (dtor sub_181793768 virtual-Releases elem[0]) -- inferred m_playingSequences
    void*                m_map100_head;      // +0x100 std::map/set _Tree head (64-byte nodes; sentinel {self,self,self,color=1,isnil=1})
    uint64_t             m_map100_size;      // +0x108 std::map/set element count (ctor 0)
    uint32_t             m_field110;         // +0x110 ctor 0x01000000 (packed flags/defaults)
    uint8_t              m_field114;         // +0x114 ctor 0
    uint8_t              _pad115[3];         // +0x115
    SCameraParams        m_cameraParams;     // +0x118 GetCameraParams returns &this                VERIFIED
    uint16_t             m_field13C;         // +0x13C ctor 0
    uint8_t              _pad13E[2];         // +0x13E
    // +0x140  capture/recording context (constructed by sub_180D620AC); sizeof 0xA20 -> ends +0xB60
    void*                m_cap_field140;     // +0x140 ctor 0
    uint32_t             m_cap_field148;     // +0x148 ctor 0
    float                m_cap_field14C;     // +0x14C ctor 0x3D072B02 (~0.033f)
    char                 m_captureFolder[2048];  // +0x150 cvar "capture_folder" (bounded copy sub_180AAF2D4, max 2048)
    uint8_t              m_cap_field950;     // +0x950 ctor 0
    char                 m_capturePrefix[512];   // +0x951 cvar "capture_file_prefix" (bounded copy, max 512)
    uint8_t              _pad_B51[3];        // +0xB51
    uint32_t             m_cap_fieldB54;     // +0xB54 ctor 0
    const char*          m_captureFileFormat;// +0xB58 ctor "tga"; cvar "capture_file_format"
    // +0xB60  tail state / cvar cache
    uint8_t              _pad_B60[0x48];     // +0xB60 ctor-zeroed (9 QWORDs; internals unresolved)
    void*                m_cvar_overrideCam;    // +0xBA8 ICVar* "mov_overrideCam" (sub_180B95258)
    int32_t              m_cvar_fakeProjection; // +0xBB0 int backing cvar "mov_enableFakeProjection" (default 0)
    float                m_cvar_maxTimeStep; // +0xBB4 float backing cvar "mov_maxTimeStepForMovieSystem"
    uint16_t             m_fieldBB8;         // +0xBB8 ctor 1
    uint8_t              _pad_BBA[2];        // +0xBBA
    uint32_t             m_fieldBBC;         // +0xBBC ctor 1
    void*                m_fieldBC0;         // +0xBC0 ctor 0
};
static_assert(sizeof(CMovieSystem) == 0xBC8, "CMovieSystem must be 0xBC8");
