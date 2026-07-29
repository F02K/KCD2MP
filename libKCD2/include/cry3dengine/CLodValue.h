#pragma once
#include <cstdint>

// -----------------------------------------------
// CLodValue -- LOD selection result returned by IShadowCaster::ComputeLod (slot [2]),
// KCD2 WHGame.dll 1.5.6 (e4cp).
// -----------------------------------------------
// Layout proven by the IRenderNode default impl 0x180659750:
//   mov [rdx], r8w         ; lodA = wantedLod
//   mov word [rdx+2], -1   ; lodB = -1
//   mov byte [rdx+4], 0    ; dissolveRef = 0
//   mov rax, rdx           ; returned via sret (6 bytes -> memory return, not RAX)
// CBrush::ComputeLod (0x1809EBBD0) writes the same {uint16, uint16=0xFFFF, bool} shape.

struct CLodValue
{
    int16_t lodA        = -1;
    int16_t lodB        = -1;
    uint8_t dissolveRef = 0;
};
static_assert(sizeof(CLodValue) == 6, "CLodValue must be 6 bytes (sret-returned)");
