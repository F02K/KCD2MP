#pragma once
#include <cstdint>

// -----------------------------------------------
// SCameraParams -- CryMovie/TrackView camera params (KCD2 WHGame.dll 1.5.6, kd7u)
// -----------------------------------------------
// sizeof 0x24; byte-identical to KCD1 (SetCameraParams @0x1807EEDE0 copies exactly 0x24 bytes).
// Consumer CViewSystem::SetActiveCamera reads bCustomAspect (+0x1C) and fAspectRatio (+0x20)
// and forwards them to the letterbox/camera call -- identical semantics to KCD1.

struct SCameraParams {
    uint32_t cameraEntityId;  // +0x00  EntityId of the camera
    float    fFOV;            // +0x04  vertical FOV, radians (open-matte: widen here)              VERIFIED
    float    fNearZ;          // +0x08  near plane
    float    fFarZ;           // +0x0C  far plane (0 == use default)
    bool     justActivated;   // +0x10  camera just activated this frame
    bool     bBlendOut;       // +0x11  blend back to pre-sequence view on end
    uint8_t  _pad12[2];       // +0x12
    float    fBlendPosSpeed;  // +0x14  camera-cut position blend speed
    float    fBlendRotSpeed;  // +0x18  camera-cut rotation blend speed
    bool     bCustomAspect;   // +0x1C  in-engine cutscene letterbox gate (set false = no bars)    VERIFIED
    uint8_t  _pad1D[3];       // +0x1D
    float    fAspectRatio;    // +0x20  cinematic aspect (e.g. 2.35f) -> letterbox                 VERIFIED
};
static_assert(sizeof(SCameraParams) == 0x24, "SCameraParams must be 0x24 (SetCameraParams copies 0x24 bytes)");
