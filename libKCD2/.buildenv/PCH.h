// Precompiled header for the libKCD2 RE project.
//
// Force-includes the kcd.h umbrella (std + CryEngine SDK types + the RE'd class graph),
// mirroring libKCD1. Every RE'd header can therefore rely on the real SDK Vec3/Quat/
// Matrix34/CryStringT/DynArray being complete.
#pragma once

#include <cstdint>
#include <cstring>
#include <cassert>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <set>
#include <functional>
#include <string>

#include "kcd.h"
