#pragma once
#include <cstdint>
#include <vector>
#include "../CryEngine/CryCommon/Cry_Math.h"   // Vec3
#include "../framework/C_Signal.h"
#include "../framework/I_ReadinessDebuggable.h"
#include "S_FastTravelConfig.h"

// -----------------------------------------------
// wh::playermodule::C_FastTravel -- KCD2 WHGame.dll 1.5.6.  sizeof 0x130.
// -----------------------------------------------
// RTTI .?AVC_FastTravel@playermodule@wh@@ (TD 0x184DCF148, COL 0x18411DB20, CHD 0x184431CC8)
// vtable 0x183A84288 (100+ slots)  inline ctor sub_180BE9BA0  dtor sub_182DDD76C.
// NOT a singleton: per-player sub-object, alloc 304 in the C_PlayerModule ctor (sub_180BE9208
// @0x180BE9291) and owned at C_PlayerModule+0x28 (see C_PlayerModule.h).
// RTTI-literal base: wh::I_ReadinessDebuggable @ mdisp 0 ONLY. Vtable slot [17] (0x182DDE114)
// writes wh::playermodule::I_FogOfWar::vftable -> an additional folded I_FogOfWar base is
// LIKELY but UNVERIFIED (CHD does not enumerate it; the huge vtable implies more listener
// interfaces) -- flagged, not modeled.
//
// === CONTROL / CHEAT SURFACE ===
//   0x182DE2CC0  StartFastTravel(this, ...) -- misnamed wrapper (cancel-if-busy + delegate to
//                SetDestination = arm); NOT forwarded. Arm via C_UIMap::SetDestination.
//   0x182DE28F4  StartTravel(this) -- the real start of an armed travel; tail-calls sub_182DE0DE8
//                whose ctx+0x130->+0x98 vtable[67] executor moves the player + skips time.
//   0x182DDFBD0  CancelFastTravel/Stop(this)
//   0x182DE2D98  EndFastTravel/FinalizeArrival(this, char endReason, ...) [clears +0x48/+0x4A,
//                teleports, notifies fog/streaming via the +0x58/+0x60 controllers]
//   0x182DE1714  internal reset
//   0x180534E5C  IsFastTraveling(this) = (m_isFastTraveling @+0x48) ||
//                (*(m_pSkipTimeSession @+0x110) + 0x68 session-active byte)
//   0x182EAA218  console dispatch (reads GameCtx+0x128 -> C_PlayerModule+0x28); arg parser
//                sub_182EAF1A4 accepts "x y z" OR an entity name
// Console cmds: wh_pl_FastTravelTo (sub_182EA894C), wh_pl_FakeFastTravelTo (sub_182EA8920).
// CVars (live in the SkipTime/RandomEvents/FastTravel cvar holder registered by sub_18159EC00,
// NOT in this class): wh_pl_FastTravelEnabled (0=off, 1=FROM ANYWHERE <- force-enable cheat,
// 2=only from FT mark), wh_pl_FastTravelDebug, wh_pl_FastTravelDistanceTolerance,
// wh_pl_FastTravelDismountedHorseFrustum{Min,Max}Distance / ConeAngle.
// Lua player:EnableFastTravel (handler 0x182ABA048) sets bit 0x10 at PLAYER-ENTITY+0xAE4 --
// a per-player permission flag, NOT a C_FastTravel field.
// Related but SEPARATE classes: entitymodule::C_ActorActionFastTravel, guimodule::
// C_FastTravelCutscene, playermodule::I_UIFastTravel, rpgmodule::C_FastTravelBuff +
// Started/Ended cause/data structs, xgenai BT node C_IsFastTraveling.

namespace wh::playermodule {

class I_UIFastTravel;   // RTTI .?AVI_UIFastTravel@playermodule@wh@@ (C_UIMap+0x18 base)

class C_FastTravel : public wh::I_ReadinessDebuggable   // +0x00  (0x8; RTTI-literal only base)
{
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_FastTravel;
    // Forwarders for the control surface above (returns not traced -> void where unknown).
    // Start an already-armed travel (arm first via C_UIMap::SetDestination).  NOP if the
    // path is not ready.
    void StartTravel();                                                                               // 0x182DE28F4
    void CancelFastTravel();                                                                          // 0x182DDFBD0
    bool IsFastTraveling() const;                                                                     // 0x180534E5C

    wh::shared::C_Signal<> m_sig08;   // +0x08  (0x10) delegate table &unk_185665430; signature unresolved
    wh::shared::C_Signal<> m_sig18;   // +0x18  (0x10) delegate table &unk_185665430; signature unresolved
    wh::shared::C_Signal<> m_sig28;   // +0x28  (0x10) FastTravelStarted/Ended broadcast
                                      //        (distinct delegate table &unk_185669B60)
    I_UIFastTravel* m_pTravelExecutor; // +0x38  the map UI's I_UIFastTravel base (C_UIMap+0x18),
                                      //        registered by C_UIMap::Init via sub_180ED10A8
                                      //        (0x180ED10AE: [this+0x38]=listener). C_FastTravel drives it:
                                      //        sub_182DE0DE8 @0x182DE0E7D calls vtable[1]
                                      //        OnFastTravelStarted(bool); arm goes through vtable[0]
                                      //        SetDestination. Sibling playermodule header, no guimodule dep.
    uint64_t m_qw40;                  // +0x40  (ctor 0; role unresolved)
    bool     m_isFastTraveling;       // +0x48  half of IsFastTraveling(); cleared by EndFastTravel
    uint8_t  m_flags49;               // +0x49  low 3 bits used (ctor &= 0xF8); bit 0x4 cleared by reset
    uint8_t  m_flag4A;                // +0x4A  cleared by EndFastTravel (role unresolved)
    uint8_t  _pad4B;                  // +0x4B
    uint32_t m_state4C;               // +0x4C  (ctor 0; INFERRED state/counter)
    uint32_t m_unk50;                 // +0x50  ctor zeroes ONLY these 4 bytes; the vtable[7]
                                      //        broadcast (sub_182DE15EC) reads [+0x50..+0x58) as a
                                      //        ptr range -- reading conflict UNRESOLVED (verify
                                      //        pass ruled OUT a clean std::vector here)
    uint32_t m_unk54;                 // +0x54  ctor-UNWRITTEN (runtime-set)
    void*    m_pCtrl0;                // +0x58  view/travel controller (EndFastTravel calls its
                                      //        vtable +0x38/+0x1D0/+0x398; type UNRESOLVED)
    void*    m_pCtrl1;                // +0x60  secondary controller (invoked if != m_pCtrl0;
                                      //        vtable +0x398/+0x3B8; type UNRESOLVED)
    Vec3     m_destination;           // +0x68  FT destination: the arm (sub_182DE27D0) stores the
                                      //        SetDestination arg here verbatim; mover goal; GetPos
                                      //        binder 0x1849E4140 returns it
    Vec3     m_dirA;                  // +0x74  arrival/heading dir (runtime-set; EndFastTravel lenSq)
    Vec3     m_dirB;                  // +0x80  second dir/look vector (runtime-set)
    uint32_t _pad8C;                  // +0x8C
    std::vector<uint32_t> m_pathNodeIds;  // +0x90  (0x18) 4-byte elems (dtor sub_1807D5ACC);
                                      //        waypoint/POI id list (semantic INFERRED)
    S_FastTravelConfig m_config;      // +0xA8  (0x58) copied from global default &unk_1855EE200
    uint32_t m_u100;                  // +0x100  (ctor 0; role unresolved)
    uint32_t _pad104;                 // +0x104
    bool     m_flag108;               // +0x108  streaming/concept-active flag (reset clears via
                                      //        manager method +0xC0; semantic INFERRED)
    uint8_t  _pad109[7];              // +0x109
    void*    m_pSkipTimeSession;      // +0x110  other half of IsFastTraveling() (+0x68 active
                                      //        byte); released via virtual +0x38 (sub_180968324);
                                      //        concrete type UNRESOLVED
    void*    m_handleA;               // +0x118  owning ptr (dtor sub_1807D048C ->
                                      //        sub_18194B6E0(obj,1)); built by the I_UIFastTravel
                                      //        registration sub_180ED10A8 (sub_180ED11B8 from the
                                      //        GUI module's C_FaderController, backref@+0x18=this);
                                      //        concrete type UNRESOLVED
    uint64_t m_qw120;                 // +0x120  (ctor 0; role unresolved)
    void*    m_handleB;               // +0x128  owning ptr, released via virtual[2]/Release
                                      //        (dtor sub_1804A760C; fog-of-war reveal or cutscene
                                      //        handle -- INFERRED)
};
static_assert(sizeof(C_FastTravel) == 0x130, "C_FastTravel must be 0x130");
static_assert(offsetof(C_FastTravel, m_isFastTraveling) == 0x48, "traveling flag at 0x48");
static_assert(offsetof(C_FastTravel, m_pathNodeIds) == 0x90, "path ids at 0x90");
static_assert(offsetof(C_FastTravel, m_config) == 0xA8, "config at 0xA8");
static_assert(offsetof(C_FastTravel, m_pSkipTimeSession) == 0x110, "skip-time session at 0x110");

}  // namespace wh::playermodule
