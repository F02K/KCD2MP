#pragma once
#include "I_RPGPerceptionHelper.h"

// -----------------------------------------------
// wh::xgenaimodule::S_RPGPerceptionHelper : I_RPGPerceptionHelper -- stateless RPG
// perception bridge (KCD2 WHGame.dll 1.5.6, kd7u).  sizeof 0x08 (vptr only;
// ctor sub_181889578 writes the vtable at C_XGenAIModule+0xA0 @0x1818895e7,
// next member at +0xA8).
// -----------------------------------------------
// RTTI TD rva 0x4FF3770 (.?AU -- struct tag). Vtable 0x184772508, 2 slots:
// [0] sub_180D45B6C GetPerceptibleVolumeData: PM = sub_180418170();
//     v = FindVolumeByWuid(PM+0x120 volume mgr, wuid) (sub_180D45BA4); returns
//     v+120 or default &unk_18506CA60 (@0x180d45b75).
// [1] sub_1823CDA14 GetStealthWatcher: thunk -> sub_180F667DC (lazy watcher
//     getter).
// NOTE: this is a THIN bridge -- the big 27-slot recognition evaluator is
// wh::rpgmodule::C_RPGPerception (vtable 0x183D18E68, singleton 0x1855E63B0),
// a different object (stored at C_PerceptionManager+0x1A8).

namespace wh::xgenaimodule {

class S_RPGPerceptionHelper : public I_RPGPerceptionHelper {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_S_RPGPerceptionHelper;
    void* GetPerceptibleVolumeData(const void* wuid) override;  // [0] sub_180D45B6C
    void* GetStealthWatcher() override;                         // [1] sub_1823CDA14 -> sub_180F667DC
};
static_assert(sizeof(S_RPGPerceptionHelper) == 0x08, "S_RPGPerceptionHelper is vptr-only");

}  // namespace wh::xgenaimodule
