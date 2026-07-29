#pragma once
#include <cstdint>
#include "../../crysystem/SCameraParams.h"

// -----------------------------------------------
// IMovieSystem - KCD2 binary vtable order (CryMovie CMovieSystem)
// -----------------------------------------------
// Target: WHGame.dll 1.5.6 (kd7u).  RTTI ??_7CMovieSystem@@6B@, vtable 0x18407E648 (93 slots, 0..92).
//
// [FUNDAMENTAL vs KCD1] The get/set camera-params pair shifted DOWN by 2 (vtable grew 92 -> 93 slots):
//     KCD1: GetCameraParams = slot 57, SetCameraParams = slot 58
//     KCD2: GetCameraParams = slot 59, SetCameraParams = slot 60
//   A detour must target slot 60 in KCD2; slot 58 is a different function.
// Only slots 0 / 59 / 60 are positively identified; the rest are stock IMovieSystem (IDA's other
// slot symbols are COMDAT-folded Concurrency template garbage -- ignore them).

namespace Offsets {

struct IMovieSystem {
    virtual void _vf0() = 0;                                         // [0]   scalar-deleting dtor (0x18387B080)
    virtual void _vf1() = 0;
    virtual void _vf2() = 0;
    virtual void _vf3() = 0;
    virtual void _vf4() = 0;
    virtual void _vf5() = 0;
    virtual void _vf6() = 0;
    virtual void _vf7() = 0;
    virtual void _vf8() = 0;
    virtual void _vf9() = 0;
    virtual void _vf10() = 0;
    virtual void _vf11() = 0;
    virtual void _vf12() = 0;
    virtual void _vf13() = 0;
    virtual void _vf14() = 0;
    virtual void _vf15() = 0;
    virtual void _vf16() = 0;
    virtual void _vf17() = 0;
    virtual void _vf18() = 0;
    virtual void _vf19() = 0;
    virtual void _vf20() = 0;
    virtual void _vf21() = 0;
    virtual void _vf22() = 0;
    virtual void _vf23() = 0;
    virtual void _vf24() = 0;
    virtual void _vf25() = 0;
    virtual void _vf26() = 0;
    virtual void _vf27() = 0;
    virtual void _vf28() = 0;
    virtual void _vf29() = 0;
    virtual void _vf30() = 0;
    virtual void _vf31() = 0;
    virtual void _vf32() = 0;
    virtual void _vf33() = 0;
    virtual void _vf34() = 0;
    virtual void _vf35() = 0;
    virtual void _vf36() = 0;
    virtual void _vf37() = 0;
    virtual void _vf38() = 0;
    virtual void _vf39() = 0;
    virtual void _vf40() = 0;
    virtual void _vf41() = 0;
    virtual void _vf42() = 0;
    virtual void _vf43() = 0;
    virtual void _vf44() = 0;
    virtual void _vf45() = 0;
    virtual void _vf46() = 0;
    virtual void _vf47() = 0;
    virtual void _vf48() = 0;
    virtual void _vf49() = 0;
    virtual void _vf50() = 0;
    virtual void _vf51() = 0;
    virtual void _vf52() = 0;
    virtual void _vf53() = 0;
    virtual void _vf54() = 0;
    virtual void _vf55() = 0;
    virtual void _vf56() = 0;
    virtual void _vf57() = 0;
    virtual void _vf58() = 0;                                        // [58] 
    virtual const SCameraParams& GetCameraParams() const = 0;        // [59]  0x1807EEDA0  VERIFIED
    virtual void SetCameraParams(const SCameraParams& params) = 0;   // [60]  0x1807EEDE0  VERIFIED
    virtual void _vf61() = 0;
    virtual void _vf62() = 0;
    virtual void _vf63() = 0;
    virtual void _vf64() = 0;
    virtual void _vf65() = 0;
    virtual void _vf66() = 0;
    virtual void _vf67() = 0;
    virtual void _vf68() = 0;
    virtual void _vf69() = 0;
    virtual void _vf70() = 0;
    virtual void _vf71() = 0;
    virtual void _vf72() = 0;
    virtual void _vf73() = 0;
    virtual void _vf74() = 0;
    virtual void _vf75() = 0;
    virtual void _vf76() = 0;
    virtual void _vf77() = 0;
    virtual void _vf78() = 0;
    virtual void _vf79() = 0;
    virtual void _vf80() = 0;
    virtual void _vf81() = 0;
    virtual void _vf82() = 0;
    virtual void _vf83() = 0;
    virtual void _vf84() = 0;
    virtual void _vf85() = 0;
    virtual void _vf86() = 0;
    virtual void _vf87() = 0;
    virtual void _vf88() = 0;
    virtual void _vf89() = 0;
    virtual void _vf90() = 0;
    virtual void _vf91() = 0;
    virtual void _vf92() = 0;             // [92] last slot (KCD1 had 0..91)
};

}  // namespace Offsets
