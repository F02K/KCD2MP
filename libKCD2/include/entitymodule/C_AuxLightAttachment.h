#pragma once
#include <cstdint>
#include "E_AuxLightType.h"
#include "../CryEngine/CryCommon/CryString.h"

// -----------------------------------------------
// wh::entitymodule::C_AuxLightAttachment -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x48.
// -----------------------------------------------
// NAME IS SYNTHETIC (no RTTI, no vtable -- plain embedded value type). A torch / auxiliary-light
// attachment helper: descriptor base 0x28 (ctor sub_1804CE554; builder sub_1804CE30C fills
// "PlayerAuxTorchLight"/"NPCAuxTorchLight" + "Lights.Standard.Light_Torch_*" presets) + runtime
// extension (ctor sub_1804CE520). Embedded in C_Player at +0xBA8 and in C_HumanHandHelper at +104.
// The prior "camera/aim helper" label was WRONG -- all recovered evidence is light-attachment.

namespace wh::entitymodule {

class C_AuxLightAttachment {
public:
    // --- descriptor (0x28) ---
    CryStringT<char> m_name;         // +0x00  "PlayerAuxTorchLight" / "NPCAuxTorchLight"
    CryStringT<char> m_presetPath;   // +0x08  "Lights.Standard.Light_Torch_*"
    int32_t  m_attachmentSlot;       // +0x10  (= actor+48)
    E_AuxLightType m_typeId;         // +0x14  builder sub_1804CE30C (Player/NPC torch light)
    uint8_t  _pad16[2];              // +0x16
    float    m_param0;               // +0x18  0.2 / 0.0
    float    m_param1;               // +0x1C  -0.2 / 0.2
    float    m_radiusOrScale;        // +0x20  0.1 / 0.35
    bool     m_flag24;               // +0x24  1 / 0
    uint8_t  _pad25[3];              // +0x25
    // --- runtime state (0x20; roles beyond m_intensity unresolved) ---
    int32_t  m_runtime28;            // +0x28  (ctor 0)
    bool     m_runtimeFlag2C;        // +0x2C  (ctor 0)
    uint8_t  _pad2D[3];              // +0x2D
    void*    m_pSpawnedLight;        // +0x30  spawned light/attachment handle (type unverified)
    int32_t  m_runtime38;            // +0x38  (ctor 0)
    float    m_intensity;            // +0x3C  init 1.0
    void*    m_runtime40;            // +0x40  (ctor 0; type unverified)
};
static_assert(sizeof(C_AuxLightAttachment) == 0x48, "C_AuxLightAttachment must be 0x48");

}  // namespace wh::entitymodule
