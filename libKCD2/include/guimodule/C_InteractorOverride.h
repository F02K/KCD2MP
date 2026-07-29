#pragma once
#include <cstdint>
#include "conceptmodule/C_Effect.h"
#include "conceptmodule/C_EventNode.h"
#include "conceptmodule/C_TypedPortRef.h"
#include "conceptmodule/C_TypedArrayPortRef.h"
#include "conceptmodule/S_Trigger.h"
#include "framework/C_LocalizedString.h"
#include "entitymodule/E_InteractionType.h"
#include "xgenaimodule/I_ObjectManagerBaseListener.h"

// -----------------------------------------------
// wh::guimodule::C_InteractorOverride -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x368 (ALLOC-PROVEN).
// -----------------------------------------------
// RTTI TD 0x184C9E1F8; COLs +0x00/0x413AD38, +0xB0/0x413AD60; primary vtable 0x183B38EA8
// (47-slot C_EventNode<C_Effect> node table -- not modeled), listener vtable 0x183B38910.
// Bases: C_EventNode<C_Effect> @+0x00 + I_ObjectManagerBaseListener @+0xB0 (the
// C_ShowMapMarker removal-listener pattern). ctor sub_18121E028; as_wh creator
// sub_182B0BCF4 (alloc 0x368, memset @0x182b0bd55).
// Own slots vs C_Effect: [0] dtor 0x180968D44, [5]/[7] trio, [12] event-port reader
// 0x18127FEA0, [27] 0x182B131EC, [33] 0x18181AADC, [43] TRIGGER 0x180DC0324,
// [44] UNTRIGGER 0x18168CAD8.
// Behavior: TRIGGER resolves m_objectPort and registers an interactor prompt override
// (localized texts + interaction type) on the target C_LinkableObject via the interactor
// manager (ctx+8 vf[+0x200] -> +2824), adding self+0xB0 as an object-removal listener
// (sub_180C9B198 keyed by linkable+0x08 WUID); UNTRIGGER removes both. Port
// display-names coined (registrar not isolated).

namespace wh::xgenaimodule { class C_LinkableObject; }

namespace wh::guimodule {

class C_InteractorOverride
    : public wh::conceptmodule::C_EventNode<wh::conceptmodule::C_Effect>   // +0x00 (0xB0)
    , public wh::xgenaimodule::I_ObjectManagerBaseListener                 // +0xB0
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_InteractorOverride;
    // I_ObjectManagerBaseListener: event 1 (object removed) -> untrigger.
    void OnObjectEvent(int event) override;   // 0x183B38910[0]

    wh::conceptmodule::C_TypedPortRef<wh::xgenaimodule::C_LinkableObject*>       m_objectPort;          // +0xB8
    wh::conceptmodule::C_TypedPortRef<wh::entitymodule::E_InteractionType>       m_interactionTypePort; // +0xF8
    wh::conceptmodule::C_TypedArrayPortRef<wh::framework::C_LocalizedString>     m_textsPort;           // +0x138
    wh::conceptmodule::C_TypedArrayPortRef<bool>                                 m_enabledFlagsPort;    // +0x178
    wh::conceptmodule::C_TypedArrayPortRef<wh::framework::C_LocalizedString>     m_altTextsPort;        // +0x1B8
    wh::conceptmodule::C_TypedArrayPortRef<int>                                  m_intsPort;            // +0x1F8
    wh::conceptmodule::C_TypedPortRef<wh::conceptmodule::S_Trigger>              m_triggerPort;         // +0x238
    wh::entitymodule::E_InteractionType m_interactionType;  // +0x278  cached from m_interactionTypePort (trigger @0x180dc0351, converter sub_1817C6664)
    uint8_t _pad27C[4];          // +0x27C
    // Trigger-time working state (+0x280 sub_180ABB35C obj; ints @+0x290/+0x298/+0x2AC;
    // strings via sub_1804F3344 around +0x2A0/+0x2C8; bytes @+0x2A4/+0x2C0) -- the
    // dossier anchors partially overlap, kept OPAQUE until the trigger body is re-read
    // [G4c §A4, UNVERIFIED].
    uint8_t m_workingState[0xE0];   // +0x280..+0x360
    wh::xgenaimodule::C_LinkableObject* m_pTarget;   // +0x360  trigger caches; untrigger clears
};
static_assert(sizeof(C_InteractorOverride) == 0x368, "C_InteractorOverride must be 0x368 (creator sub_182B0BCF4)");
static_assert(offsetof(C_InteractorOverride, m_objectPort) == 0xB8, "object port at 0xB8");
static_assert(offsetof(C_InteractorOverride, m_pTarget) == 0x360, "target cache at 0x360");

}  // namespace wh::guimodule
