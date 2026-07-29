// libKCD2 foundation compile-check: pulls the whole umbrella (real SDK types + the RE'd
// class graph), so every static_assert(sizeof) fires against the SDK-typed layouts.
//   standalone: cl /c /std:c++17 /EHsc /I include /I include/CryEngine/CryCommon .buildenv/check_sizes.cpp
#include "kcd.h"
