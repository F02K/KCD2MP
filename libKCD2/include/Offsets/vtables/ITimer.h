#pragma once
#include <cstdint>

// -----------------------------------------------
// ITimer -- CryEngine timer interface, KCD2 binary vtable order (impl CTimer).
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  CTimer vtable 0x183A6CCD0, 31 slots (KCD1: 30).
// Accessed via gEnv->pTimer (gEnv+0x88; devirt-guard cross-check sub_180433438 compares
// vtbl[3] == GetCurrTime of this exact vtable). CTimer is embedded at CSystem+0x28.
// Slot semantics matched against Timer.cpp log strings + field behavior; evidence dumps in
// KCD2/analysis/vtables_reslot/.
//
// [FUNDAMENTAL vs KCD1] interfuscator reshuffle: slots 0-5 unchanged; the async pair moved
// (KCD1 [6]/[7] -> KCD2 [8]/[6]) with a NEW slot [7] inserted; everything from KCD1 [8] on
// shifted (8->9, 9->10, 10->11, 11->12, 12->17, 13->19, 14->15, 15->16, 18->22, 19->23,
// 20->13, 21->14, 22->18). A KCD1-positioned detour on any of these hits the wrong function.

namespace Offsets {

struct ITimer {
    enum ETimer : int32_t {
        ETIMER_GAME = 0,
        ETIMER_UI   = 1,
    };

    virtual void    Dtor(char flags) = 0;                                   // [0]  0x00  scalar deleting dtor (0x1824585E4)
    virtual void    ResetTimer() = 0;                                       // [1]  0x08  0x180A549B8 -- "ResetTimer: m_lUIBaseTime" (Timer.cpp:610)
    virtual void    UpdateOnFrameStart() = 0;                               // [2]  0x10  0x18045F7F0 -- frame tick; QPC deltas, fps-cap Sleep (Timer.cpp:542)
    virtual float   GetCurrTime(ETimer which = ETIMER_GAME) const = 0;      // [3]  0x18  0x180509C50 -- (float)ticks[which] * 1e-5
    virtual double  GetCurrTimeD(ETimer which = ETIMER_GAME) const = 0;     // [4]  0x20  0x181A73920 -- double variant
    virtual void*   GetFrameStartTime(ETimer which = ETIMER_GAME) const = 0;// [5]  0x28  0x1805D5F60 -- &CTimeValue at this+8*(which+2)
    virtual void*   GetAsyncTime(void* outTimeValue) const = 0;             // [6]  0x30  0x1805BEC80 -- QPC -> [7] -> CTimeValue out  [KCD1 slot 7]
    // [7] 0x38  0x181DE01F0 -- out = (int64)(secsPerTick * 1e5 * ticks); NEW slot vs KCD1
    //     (identity LIKELY TicksToTimeValue; called by [6])
    virtual void    TicksToTimeValue(int64_t ticks, void* outTimeValue) = 0;
    virtual float   GetAsyncCurTime() = 0;                                  // [8]  0x40  0x1804611DC -- QPC-base, clamped  [KCD1 slot 6]
    virtual float   GetFrameTime(ETimer which = ETIMER_GAME) const = 0;     // [9]  0x48  0x1805D5F7C -- enabled/paused gates  [KCD1 8]
    virtual float   GetRealFrameTime() const = 0;                           // [10] 0x50  0x180666794 -- unscaled dt @+80  [KCD1 9]
    virtual float   GetTimeScaleChannel(uint32_t channel) const = 0;        // [11] 0x58  0x182468818 -- chanScale[+536]*mul(+580)  [KCD1 10 -- KCD1's
                                                                            //            "GetReplicationNetworkFrameTime" name was suspect]
    virtual float   GetTimeScale() const = 0;                               // [12] 0x60  0x1810C0EA0 -- +580 * +568  [KCD1 11]
    virtual void    ClearTimeScales() = 0;                                  // [13] 0x68  0x180A543D4 -- Timer.cpp:242  [KCD1 20 ResetTimeScale]
    virtual void    SetTimeScale(float scale, uint32_t channel) = 0;        // [14] 0x70  0x1810C0EB4 -- Timer.cpp:210  [KCD1 21 SetTimeScaleChannel]
    virtual void    EnableTimer(bool bEnable) = 0;                          // [15] 0x78  0x181A73910 -- byte +96  [KCD1 14]
    virtual bool    IsTimerEnabled() const = 0;                             // [16] 0x80  0x181A72E80 -- byte +96  [KCD1 15]
    virtual float   GetFrameRate() = 0;                                     // [17] 0x88  0x180441468 -- 1.0 / realFrameTime  [KCD1 12]
    virtual float   GetProfileFrameBlending(float* pf, int* pi) = 0;        // [18] 0x90  0x180441B68 -- outs +532/+600, ret +528  [KCD1 22]
    virtual void    Serialize(void* ser) = 0;                               // [19] 0x98  0x18246B860 -- "curTime"/"ticksPerSecond" (Timer.cpp:667)  [KCD1 13]
    virtual void    PauseTimer(ETimer which, bool bPause) = 0;              // [20] 0xA0  0x180A558D8 -- "Pausing ON/OFF" strings, byte +84
    virtual bool    IsTimerPaused(ETimer which) = 0;                        // [21] 0xA8  0x182468D3C -- which ? 0 : byte +84
    virtual void    SetTimer(ETimer which, float val) = 0;                  // [22] 0xB0  0x18246BF08 -- Timer.cpp:720  [KCD1 18]
    virtual void    SecondsToDateUTC(void* out) = 0;                        // [23] 0xB8  0x18246B714 -- _gmtime64  [KCD1 19]
    virtual void    DateToSecondsUTC(void* in) = 0;                         // [24] 0xC0  0x182466EF8 -- _mktime64 + TZ bias
    virtual float   TicksToSeconds(int64_t ticks) = 0;                      // [25] 0xC8  0x180461230 -- ticks * secsPerTick(+112)
    virtual int64_t GetTicksPerSecond() = 0;                                // [26] 0xD0  0x181A735C0 -- qword +104
    virtual ITimer* CreateNewTimer() = 0;                                   // [27] 0xD8  0x182466DA8 -- alloc 0x260 + CTimer ctor
    virtual bool    IsFrameSmoothingEnabled() const = 0;                    // [28] 0xE0  0x1810C100C -- (int)+588 > 0 [name LIKELY]
    virtual void    EnableFrameSmoothing(bool b) = 0;                       // [29] 0xE8  0x1810C1000 -- +588 = arg [name LIKELY]
    virtual void    SetFixedFrameTime(float t) = 0;                         // [30] 0xF0  0x181A73760 -- +8 = arg; forces dt in UpdateOnFrameStart [name LIKELY]
};

}  // namespace Offsets
