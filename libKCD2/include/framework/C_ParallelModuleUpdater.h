#pragma once
#include <cstdint>
#include <functional>
#include "../Offsets/vtables/IGame.h"   // Offsets::IGameFrameworkListener

// -----------------------------------------------
// wh::framework::C_ParallelModuleUpdater -- KCD2 WHGame.dll 1.5.6 (kd7u).  sizeof 0x70.
// -----------------------------------------------
// Job-system update pump mixed into module classes as their SECOND base (e.g. C_RPGModule @+0x10,
// per its RTTI CHD: C_ParallelModuleUpdater mdisp 16 containing IGameFrameworkListener mdisp 16 --
// i.e. IGameFrameworkListener is this class's primary base sharing its vptr). Base ctor
// sub_180BE9170. The span (0x70) and the anchors below are verify-confirmed; the interior
// +0x18..+0x58 was only partially walked (honest gap) and is left as flagged qwords.

namespace JobManager { struct SJobState; }   // CryEngine job state; pImpl alloc 2096B via sub_18078BDEC

namespace wh::framework {

struct S_ParallelModuleUpdateJobData;   // RTTI US_ParallelModuleUpdateJobData@framework@wh@@ (8B; interior unresolved)

class C_ParallelModuleUpdater : public Offsets::IGameFrameworkListener {   // vptr +0x00
public:
    inline static constexpr auto RTTI = Offsets::RTTI_C_ParallelModuleUpdater;
    bool     m_bParallelUpdateEnabled;    // +0x08  gate; writer sub_180669C04 stores (vtbl[10]()!=0)
    uint8_t  _pad09[7];                    // +0x09  alignment before m_pJobState
    JobManager::SJobState* m_pJobState;   // +0x10  update-job state (impl object 2096B, sub_18078BDEC)
    uint64_t m_jobData;                                              // +0x18  wh::framework::S_ParallelModuleUpdateJobData (by-ref arg to m_updateFn; 8B, interior unresolved)
    std::function<void(S_ParallelModuleUpdateJobData&)> m_updateFn;   // +0x20  parallel update body (0x40; impl ptr @+0x58, ctor-nulled)
    volatile int32_t m_atomic60;          // +0x60  atomic int
    uint32_t _pad64;                      // +0x64
    int32_t  m_unk68;                     // +0x68
    uint32_t _pad6C;                      // +0x6C  align to 0x70
};
static_assert(sizeof(C_ParallelModuleUpdater) == 0x70, "C_ParallelModuleUpdater must be 0x70");

}  // namespace wh::framework
