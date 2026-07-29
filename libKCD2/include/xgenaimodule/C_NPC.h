#pragma once
#include <cstdint>
#include "I_NPC.h"
#include "C_NoOwnerOwnership.h"

// -----------------------------------------------
// wh::xgenaimodule::C_NPC : I_NPC -- the concrete NPC, bottom of the AI object-model
// spine (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x17B0 (create-site proven:
// sub_1804F75C0(6064) in BOTH NPC factories sub_1804184F8 / sub_180D48678).
// -----------------------------------------------
// RTTI TD rva 0x4F6E678; primary vtable 0x183FF1B18, 64 slots (overrides all, adds
// none). Subobject vtables: I_RWLocked @+0x20 -> 0x183FF1F18, I_XGenAINPC @+0xA8 ->
// 0x183FF1B00 ([1] sub_1804DFADC). Real ctor
// sub_180BDC5F8(this, IGameObject* host, const WUID* wuid, C_AIPuppet* puppet); dtor
// sub_180BDB910 (via slot-0 deleting dtor sub_180BDAB28). Factories: dispatcher
// sub_180417DF0 -> sub_1804184F8 / sub_180D48678 (restore variant); on inbox failure
// the factory unregisters the WUID from C_AIObjectManager and deletes the NPC, else
// attaches the inbox via slot [21] (vf +0xA8).
// Interior member map (ctor helper per region, G2_supplement §3.7 -- interiors [U],
// brain pointer NOT pinned; candidates: m_messageGate (+0x70), +0x1C0, +0x790):
//   +0xB0 sub_180BDCB00 | +0xD0 sub_180BDCF10(this) | +0x1C0 sub_180BDD268(this)
//   +0x478 sub_180BDCB50(this) | +0x490 unknown_libname_15 | +0x4C8 sub_180BDCB38
//   +0x530 sub_1823D3D70 | +0x558 sub_180BDCC2C | +0x5C0 sub_180901FD4(this)
//   +0x638/+0x640 functor slots | +0x650 sub_1820C7070(this) | +0x698 sub_180BDCE8C(this)
//   +0x758 sub_180BDCB68 | +0x780 sub_180BDCA90 (registered into host+0x9A8)
//   +0x790 sub_180BDD9D4 (HUGE, ~0x788 bytes) | +0xF18 sub_180BDD0F4(this)
//   +0x1068 sub_180BDCAA4(this) | +0x1088 sub_180BDD044(wuid) | +0x10F0 sub_180BDCE0C(this)
//   +0x1798 sub_180BDCAE4 (last member, 0x18)

namespace wh::xgenaimodule {

class C_SituationSubsystem;

class C_NPC : public I_NPC {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NPC;
    // spine overrides (only slots whose impl differs from the inherited one)
    ~C_NPC() override;                              // [0]  deleting dtor sub_180BDAB28 -> dtor sub_180BDB910
    uint32_t GetTypeMask() override;                // [1]  sub_1805D29D0
    void _vf3() override;                           // [3]  sub_1808777BC
    void _vf4() override;                           // [4]  sub_1808135AC
    void* _vf6() override;                          // [6]  sub_1805D02D0
    void* GetStaticTypeToken() override;            // [7]  sub_1808B0390
    I_Ownership* GetOwnership() override;           // [11] sub_180708E50 -> &m_ownership (+0x4A8) [V]
    void _vf20() override;                          // [20] sub_1803EF4D8
    void SetMessageInbox(C_MessageInbox*) override; // [21] sub_180D47DE8 [V-usage]
    void _vf22() override;                          // [22] sub_18041A6A0 (return 1)
    void _vf24() override;                          // [24] sub_18209CC00
    void _vf25() override;                          // [25] sub_18332215C
    void _vf26() override;                          // [26] sub_1813A7058
    // I_XGenAINPC mixin impl
    void _vf1() override;                           // I_XGenAINPC[1] sub_1804DFADC
    // the NPC contract [27..63] (impl addresses from vtable 0x183FF1B18)
    void _vf27() override;                          // [27] sub_1803BB4F4
    void _vf28() override;                          // [28] sub_180645680
    void _vf29() override;                          // [29] sub_1805D2918
    void _vf30() override;                          // [30] sub_1807FDFFC
    void _vf31() override;                          // [31] sub_180A1F500
    void _vf32() override;                          // [32] sub_18209CA10
    void _vf33() override;                          // [33] sub_18209CEC0
    void _vf34() override;                          // [34] sub_181A75250
    void _vf35() override;                          // [35] sub_180814160
    void _vf36() override;                          // [36] sub_181A72C50
    void _vf37() override;                          // [37] sub_181A947E0
    void _vf38() override;                          // [38] sub_181AA6BE0
    void _vf39() override;                          // [39] sub_181AA6BE0 (same body as [38])
    void _vf40() override;                          // [40] sub_183323374
    void _vf41() override;                          // [41] sub_180411E30
    void _vf42() override;                          // [42] sub_1809FF7D0
    void _vf43() override;                          // [43] sub_181AA6B90
    void _vf44() override;                          // [44] sub_181AA6B90 (same body as [43])
    void _vf45() override;                          // [45] sub_18075BDF0
    void _vf46() override;                          // [46] sub_181AA6BD0
    void _vf47() override;                          // [47] sub_181AA6BD0 (same body as [46])
    void _vf48() override;                          // [48] sub_180731250
    void _vf49() override;                          // [49] sub_1806C96E0
    void _vf50() override;                          // [50] sub_1806C9874
    void _vf51() override;                          // [51] sub_183322638
    void _vf52() override;                          // [52] sub_180646730
    void _vf53() override;                          // [53] sub_180877140
    void _vf54() override;                          // [54] sub_18075C104
    void _vf55() override;                          // [55] sub_180B200B0
    void _vf56() override;                          // [56] sub_180731F20
    void _vf57() override;                          // [57] sub_180DA5168
    void _vf58() override;                          // [58] sub_18209C7B0
    void _vf59() override;                          // [59] sub_18209C750
    void _vf60() override;                          // [60] sub_1809C593C
    void _vf61() override;                          // [61] sub_180A1AC1C
    void _vf62() override;                          // [62] sub_180A1AB94
    void _vf63() override;                          // [63] sub_180D18670

    uint8_t            _blockB0[0x68];      // +0xB0..+0x117   member blocks (see header comment)
    int32_t            m_hostChainId;       // +0x118  id read via host->vf chain [U meaning]
    uint8_t            m_byte11C;           // +0x11C  ctor: 0
    uint8_t            _pad11D[3];          // +0x11D
    C_NPC*             m_self120;           // +0x120  self back-ref (after sub_180BDCFA4 init)
    uint8_t            _block128[0x70];     // +0x128..+0x197  member blocks
    framework::WUID    m_npcWuid;           // +0x198  third WUID copy (NPC level, after sub_180BDCAC4)
    uint8_t            _block1A0[0x308];    // +0x1A0..+0x4A7  incl. big member +0x1C0 (sub_180BDD268)
    C_NoOwnerOwnership m_ownership;         // +0x4A8  embedded; returned by GetOwnership slot [11] [V]
    uint8_t            _block4C8[0xBB8];    // +0x4C8..+0x107F member blocks (see header comment)
    C_SituationSubsystem* m_situationSubsystem;  // +0x1080 optional heap C_SituationSubsystem (new 0x88, ctor sub_180BDBFDC, back-refs this NPC @obj+0x18); gated by entity bool property (unk_1853386E0)
    uint8_t            _block1088[0x6C0];   // +0x1088..+0x1747 member blocks
    C_NPC*             m_self1748;          // +0x1748 self back-ref
    uint8_t            _block1750[0x28];    // +0x1750..+0x1777 zeroed
    uint8_t            m_infoFlags;         // +0x1778 ctor &= 0x21; bit5 = entity bool property "bCanHoldInformation" [V] -- crime/information gate
    uint8_t            m_flags1779;         // +0x1779 ctor &= 0x80 [U roles]
    uint8_t            _pad177A[6];         // +0x177A word zeroed + pad
    framework::WUID    m_wuid1780;          // +0x1780 = qword_18533A2C0 (runtime-init sentinel WUID)
    uint8_t            _block1788[0x10];    // +0x1788 dword/word/dword zeros; word @+0x1794 = 1
    uint8_t            m_tail1798[0x18];    // +0x1798 last member (sub_180BDCAE4) -> object end 0x17B0
};
static_assert(sizeof(C_NPC) == 0x17B0, "C_NPC must be 0x17B0 (6064)");

}  // namespace wh::xgenaimodule
