#pragma once
#include <cstdint>

// -----------------------------------------------
// SFlashCxform — Flash movie-clip color transform (KCD2 BINARY layout)
// -----------------------------------------------
// CryCommon/IFlashPlayer.h type, GLOBAL namespace. KCD2 1.5.6: the binary's
// SetCxform/GetCxform slots consume {float mul[4]; uint8 add[4]} — sizeof 0x14 —
// NOT the stock CE3 SDK shape {ColorF mul; ColorF add} (0x20). Proven from the
// CFlashPlayer slot bodies (analysis/guimodule_port/flash_player_layer.md).
//
// Do NOT include the stock CryCommon/IFlashPlayer.h in a TU using this replica
// (it defines the stale shape under the same name). mul is kept as four floats
// (identical layout to ColorF r,g,b,a) so this header stays engine-dep-free.

struct SFlashCxform {
    float         mul[4];   // +0x00  multiply channels (r,g,b,a)
    unsigned char add[4];   // +0x10  add channels, 0..255 (r,g,b,a)
};
static_assert(sizeof(SFlashCxform) == 0x14, "KCD2 binary SFlashCxform is 0x14");
