#pragma once

// -----------------------------------------------
// wh::dialogmodule::I_DialogCommand -- timeline-command root interface (KCD2 1.5.6, kd7u).
// -----------------------------------------------
// RTTI .?AVI_DialogCommand@dialogmodule@wh@@ (TD 0x184B531C0, abstract, no COL). Root of
// every data::C_*Command (13 concrete command vtables share this 10-slot base contract,
// verified across vtables 0x183A93368..0x1846FC630). Commands are parsed from dialogue
// sequence assets and executed against an I_DialogCommandReceiver during playback.
// Contract (fp16 = half-precision floats stored at C_Command +0x0A/+0x0C):
//   [0] scalar-deleting dtor           [5] GetTimeB() -> float   (fp16 +0x0C)
//   [1] 0x1803B6E80 no-op hook         [6] ResetTime()           (+0x0C = +0x0A)
//   [2] Serialize(archive)             [7] CreateHandler(out)    (build runtime handler)
//   [3] GetTimeA() -> float (fp16 +0xA)[8] 0x1805F5DA0 identity  (return this; UNVERIFIED)
//   [4] ResolveTime(ctx, float)        [9] CreateHandlerBound(out = {cmd*, handler})
//       (normalize vs ctx+0x38 duration, store fp16 +0x0C)
// Method names coined from behavior; arg types loosely typed.

namespace wh::dialogmodule {

class I_DialogCommand {
public:
    inline static constexpr auto RTTI = Offsets::RTTI_I_DialogCommand;
    virtual ~I_DialogCommand();                          // [0]
    virtual void _vf1();                                 // [1] 0x1803B6E80 no-op hook
    virtual void Serialize(void* archive) = 0;           // [2]
    virtual float GetTimeA() = 0;                        // [3] config timeline placement (fp16 +0x0A)
    virtual void ResolveTime(void* ctx, float t) = 0;    // [4] normalize vs sequence duration
    virtual float GetTimeB() = 0;                        // [5] resolved timeline placement (fp16 +0x0C)
    virtual void ResetTime() = 0;                        // [6]
    virtual void CreateHandler(void* out) = 0;           // [7]
    virtual void* _vf8();                                // [8] identity thunk (return this) [UNVERIFIED]
    virtual void CreateHandlerBound(void* out) = 0;      // [9] out = {cmd*, handler}
};
static_assert(sizeof(I_DialogCommand) == 0x08);

}  // namespace wh::dialogmodule
