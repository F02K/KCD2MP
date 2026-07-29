#pragma once
#include <cstdint>
#include "C_ObjectManagerProxyStorage.h"
#include "C_LinkModificationManager.h"

// -----------------------------------------------
// wh::xgenaimodule::C_LinkablesManager -- registry of all static C_LinkableObject
// instances + the embedded link-modification arbiter (KCD2 WHGame.dll 1.5.6, kd7u).
// sizeof 0x178 (create-site proven: alloc 376 in the lazy-init getter sub_1819E016C
// -> qword_1854961B0).
// -----------------------------------------------
// RTTI TD rva 0x4FF5880; primary vtable 0x184013E10, 3 slots.  Base @+0:
// C_ObjectManagerProxyStorage<C_LinkableObject, C_LinkablesManager,
// C_AIObjectManager> (full template chain in that header).  ctor sub_180BEC09C:
// base block +0x00..+0x67, then +0x68 sub_1808DDC28 (listener callback-list),
// +0x78 sub_180BEC7D4 (track-sequencing/callback dispatch, to +0x140), +0x140
// embedded C_LinkModificationManager; also writes the static default link limits
// xmmword_18533ADF0[1] = xmmword_18533AE28[1] = 1000.
// NOTE: qword_1854961B0 IS the G2 "linkable-object registration list" -- every
// C_LinkableObject ctor registers here (++m_liveCount @+0x60).

namespace wh::xgenaimodule {

class C_LinkableObject;
class C_AIObjectManager;

class C_LinkablesManager
    : public C_ObjectManagerProxyStorage<C_LinkableObject, C_LinkablesManager, C_AIObjectManager> {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_LinkablesManager;
    ~C_LinkablesManager() override;   // [0] deleting dtor sub_1834421F8
    void _vf2() override;             // [2] sub_18047B884 -- remove/unregister object (cf. C_AIObjectManager [2] sub_18047B810) [U role]

    uint8_t m_listenerBlock[0x10];    // +0x68..+0x77  callback-list holder (init sub_1808DDC28) [U interior]
    uint8_t m_dispatchBlock[0xC8];    // +0x78..+0x13F  track-sequencing / callback dispatch (init sub_180BEC7D4) [U interior]
    C_LinkModificationManager m_linkModificationManager;  // +0x140 (0x38) embedded [V]
};
static_assert(sizeof(C_LinkablesManager) == 0x178, "C_LinkablesManager must be 0x178 (376)");

}  // namespace wh::xgenaimodule
