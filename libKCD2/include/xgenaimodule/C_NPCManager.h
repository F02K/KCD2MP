#pragma once
#include <cstdint>
#include "I_NPCManager.h"
#include "I_DebugDraw.h"
#include "I_XGenAINPCManager.h"
#include "../crysystem/IEntitySystemSink.h"
#include "../game/I_EntitySideEffectCallback.h"

// -----------------------------------------------
// wh::xgenaimodule::C_NPCManager -- the NPC factory/manager, HUB SLOT 2 (KCD2
// WHGame.dll 1.5.6, kd7u).  sizeof 0x748 (create-site proven: tracked alloc 1864 in
// the lazy-init getter sub_1819DEB3C -> qword_185494490).
// -----------------------------------------------
// RTTI TD rva 0x4F6E458; primary vtable 0x183FF1968, 38 slots. 5 MI bases:
// I_NPCManager(+0, primary) / IEntitySystemSink(+0x08, vt 0x183FF1AA0, 8 slots) /
// I_DebugDraw(+0x10, vt 0x183FF1AE8) / game::I_EntitySideEffectCallback(+0x18,
// vt 0x183FF1930) / wh::I_XGenAINPCManager(+0x20, vt 0x183FF1948). ctor
// sub_180CA0614; dtor sub_183320608 (tears down from +0x708). Subsystem hookup:
// entity-system AddSink(this+0x08, 30, 0x80000000001); side-effect cb for entity
// events 0xA7/0xBB; I_XGenAINPCManager registered via (*q_18492D8D8)->vf[0x3C0/8].
// NPC factories: dispatcher sub_180417DF0 -> sub_1804184F8 / sub_180D48678.
// Console: wh_ai_DisplayNPCHistory -> sub_183321640, wh_ai_DumpNPCHistory ->
// sub_1833218B4. Cvar-backed globals registered here (address book, G2_supplement
// §5): wh_ai_NPCNavMeshPosUpdateBudget->dword_18492F374, wh_ai_NPCHidePPU->
// dword_1855ABFB0, wh_ai_NPCStateActionSubsequentPathReachableTest->dword_18492F378
// (+Tolerance->dword_18492F37C), wh_ai_NPCContextGameLoadUpdatesBudget->
// dword_18492F488, wh_ai_NPCContextGameLoadExecuteStateChange->dword_18492F48C,
// wh_ai_NPCContextStateSearchPostProcess->dword_18492F484, wh_ai_PlayerSchedulerProxy
// ->off_18491E970, wh_ai_PlayerHorseSchedulerProxy->off_18491E978.
// Member blocks (ctor helpers; interiors [U]): +0xF8 sub_181851D04 | +0x148
// sub_18209B590 (dtor sub_18209B6B0) | +0x2A8 sub_1819AA0D8 | +0x560
// sub_1820AFEE0(this+0x148 view) | +0x630/+0x640 intrusive list heads | +0x650
// sub_1819CA018(this) | +0x6A0 sub_180A2B984 | +0x6B0 sub_1819C6B3C | +0x6C0/+0x6D0
// sub_180CA0D30 | +0x6E0 sub_1819C6B20 | +0x708 sub_1806B5C70 (last).
// The IEntitySystemSink pure slots are intentionally NOT overridden in this mirror
// (interfuscator-shuffled order, cannot map names to impls) -- the class stays
// abstract here; it is only ever reached by pointer.

namespace wh::xgenaimodule {

class C_NPCManager : public I_NPCManager,
                     public IEntitySystemSink,
                     public I_DebugDraw,
                     public game::I_EntitySideEffectCallback,
                     public ::wh::I_XGenAINPCManager {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_NPCManager;
    ~C_NPCManager() override;              // [0]  deleting dtor sub_181A735C0 -> dtor sub_183320608
    // primary slots [1..37] -- roles UNRESOLVED (addresses from vtable 0x183FF1968)
    virtual void _vf1();                   // [1]  sub_183321FC4
    virtual void _vf2();                   // [2]  sub_1805D0BA8
    virtual void _vf3();                   // [3]  sub_180689268
    virtual void _vf4();                   // [4]  sub_1805D0DAC
    virtual void _vf5();                   // [5]  sub_1833228DC
    virtual void _vf6();                   // [6]  sub_1805D3908
    virtual void _vf7();                   // [7]  sub_183321FA8
    virtual void _vf8();                   // [8]  sub_180C06B54
    virtual void _vf9();                   // [9]  sub_1833211E4
    virtual void _vf10();                  // [10] sub_181164E9C
    virtual void _vf11();                  // [11] sub_1816EE4E8
    virtual void _vf12();                  // [12] sub_180B2D8A0
    virtual void _vf13();                  // [13] sub_18040F2F0
    virtual void _vf14();                  // [14] sub_180A9E554
    virtual void _vf15();                  // [15] sub_180A9E638
    virtual void _vf16();                  // [16] sub_181822374
    virtual void _vf17();                  // [17] sub_1833239B8
    virtual void _vf18();                  // [18] sub_1833221A8
    virtual void _vf19();                  // [19] sub_183322130
    virtual void _vf20();                  // [20] sub_1803A5860
    virtual void _vf21();                  // [21] sub_18209C590
    virtual void _vf22();                  // [22] sub_18332118C
    virtual void _vf23();                  // [23] sub_180622E08
    virtual void _vf24();                  // [24] sub_180622E00
    virtual void _vf25();                  // [25] sub_1807FAEF0
    virtual void _vf26();                  // [26] sub_181A7E430
    virtual void _vf27();                  // [27] sub_181696630
    virtual void _vf28();                  // [28] sub_180D3AC54
    virtual void _vf29();                  // [29] sub_1833213F0
    virtual void _vf30();                  // [30] sub_18099CCA4
    virtual void _vf31();                  // [31] sub_180B44CF4
    virtual void _vf32();                  // [32] sub_180B4524C
    virtual void _vf33();                  // [33] sub_180B44DE0
    virtual void _vf34();                  // [34] sub_180D3AC28
    virtual void _vf35();                  // [35] sub_183321478
    virtual void _vf36();                  // [36] sub_180A6B990
    virtual void _vf37();                  // [37] sub_180D47640
    // mixin impls
    void DebugDraw() override;             // I_DebugDraw[1] = nullsub_1
    void OnEntitySideEffectAdded(void* sideEffect) override;    // I_EntitySideEffectCallback subobject vt 0x183FF1930 [0] sub_1816DE384
    void OnEntitySideEffectRemoved(void* sideEffect) override;  // ESEC [1] sub_1816E9DB0
    void _xnpcm_vf1() override;            // I_XGenAINPCManager[1] sub_180AEBC3C
    void _xnpcm_vf2() override;            // I_XGenAINPCManager[2] sub_180AEEA88

    uint8_t  m_enabled;          // +0x28  ctor: 1 [V]
    uint8_t  _pad29[7];          // +0x29
    uint8_t  _block30[0x658];    // +0x30..+0x687  member blocks / list heads (see header comment)
    int32_t  m_lodHideBudget[5]; // +0x688  {v,v,v+3,10,v+3} bound to cvar wh_ai_LOD_Hide ("Number of NPCS that can change LOD in a single frame")
    uint8_t  m_byte69C;          // +0x69C  [U role]
    uint8_t  _pad69D[3];         // +0x69D
    uint8_t  _block6A0[0x50];    // +0x6A0..+0x6EF  member blocks
    int32_t  m_npcLookTarget;    // +0x6F0  ctor: 1; cvar wh_ai_NPCLookTarget ("Enables NPC look callback")
    int32_t  m_npcDryUpdateMode; // +0x6F4  ctor: 2; cvar wh_ai_NPCDryUpdateMode (0=all dry-updated / 1=registered only / 2=registered+assert)
    uint64_t _unk6F8;            // +0x6F8  ctor: 0 [U role]
    uint8_t  m_byte700;          // +0x700  ctor: 1 [U role]
    uint8_t  _pad701[7];         // +0x701
    uint8_t  m_tail708[0x40];    // +0x708  last member block (sub_1806B5C70) -> object end 0x748
};
static_assert(sizeof(C_NPCManager) == 0x748, "C_NPCManager must be 0x748 (1864)");

}  // namespace wh::xgenaimodule
