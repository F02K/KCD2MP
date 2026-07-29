#pragma once
#include <cstdint>

// -----------------------------------------------
// ::fixed_t<T, FracBits> -- Warhorse fixed-point value (KCD2 WHGame.dll 1.5.6, kd7u).
// -----------------------------------------------
// GLOBAL-scope template (mangled `U?$fixed_t@I$0BJ@@@` = fixed_t<unsigned int, 25>) -- RTTI-proven
// as the value type of C_SoulValueEffect<E_SoulStat/E_SoulSkill, fixed_t<uint32_t, 25>> (the
// stat/skill XP amount). Only the raw storage is modeled; the engine passes it by pointer and
// copies the raw integer (C_StatXPEffect ctor reads *pAmount into +0x1C).

template <typename T, int FracBits>
struct fixed_t {
    T m_raw;   // +0x00  fixed-point raw value (FracBits fractional bits)
};
static_assert(sizeof(fixed_t<uint32_t, 25>) == 0x04, "fixed_t is the raw integer");
