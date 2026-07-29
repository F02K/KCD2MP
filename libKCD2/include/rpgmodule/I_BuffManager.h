#pragma once
#include <cstdint>
#include "../framework/WUID.h"

// -----------------------------------------------
// wh::rpgmodule::I_BuffManager -- KCD2 WHGame.dll 1.5.6 (kd7u).  Pure interface (vtable only).
// -----------------------------------------------
// RTTI .?AVI_BuffManager@rpgmodule@wh@@ (BCD 0x18438A550). Sole implementor C_BuffManager (its
// 16-slot vtable 0x183EFA3B0 IS this interface's slot order). Slot names are behavioral readings
// of the implementations; ARGUMENT TYPES largely UNVERIFIED (kept as void*/conservative).

namespace wh::rpgmodule {

class C_BuffInstanceBase;

class I_BuffManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_BuffManager;
    virtual void* AddBuffToSoul(void* a_pSoul, const void* a_buffGuid, const void* a_paramsGuid) = 0; // [0]  sub_1804711BC (soul, guid, guid)
    virtual void* AddBuffByEntity(void* a_entity, const void* a_buffGuid) = 0;  // [1]  sub_1809DC070 (resolves soul via world ctx qword_1853322A0)
    virtual void* CreateBuffInstance(const void* a_buffGuid, void* a_params) = 0; // [2]  sub_1804675A8 -> global RTTR factory sub_18063ACE4
    virtual void  slot3() = 0;                                                  // [3]  sub_180B4CDF4 (unresolved)
    virtual bool  RemoveBuff(void* a_pBuff) = 0;                                // [4]  sub_18063A664 (fires signalA, detaches from soul)
    virtual void  slot5() = 0;                                                  // [5]  sub_180B46D70 (unresolved)
    virtual void  slot6() = 0;                                                  // [6]  sub_182BC81DC (fwd sub_181F97700; unresolved)
    virtual C_BuffInstanceBase* GetActiveBuff(wh::framework::WUID a_wuid) = 0;  // [7]  sub_18046D6C4 (m_activeBuffsByWuid lookup)
    virtual const void* GetBuffDefinition(const void* a_guid) = 0;              // [8]  sub_1808F81DC (global sorted def table, binary search)
    virtual void  slot9() = 0;                                                  // [9]  sub_18146BD18 (unresolved)
    virtual const void* GetDefaultBuffDefinition() = 0;                         // [10] sub_1808F8180 (returns &qword_185326EE0)
    virtual void  ConnectSignalA(void* a_delegate) = 0;                         // [11] sub_180C3CAC0 (op on this+0x08)
    virtual void  DisconnectSignalA(void* a_delegate) = 0;                      // [12] sub_182BC62AC
    virtual void  ConnectSignalB(void* a_delegate) = 0;                         // [13] sub_180964DD4 (op on this+0x18)
    virtual void  DisconnectSignalB(void* a_delegate) = 0;                      // [14] sub_1809643B8
    virtual ~I_BuffManager() = default;                                         // [15] scalar-deleting dtor
};

}  // namespace wh::rpgmodule
