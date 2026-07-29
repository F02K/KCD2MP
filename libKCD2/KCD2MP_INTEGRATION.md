# KCD2MP integration snapshot

This directory vendors `F02K/libKCD2` at commit
`6d92dc392b64a468ccfa7351597fbe5bc6cb0094` (2026-07-28).

KCD2MP builds only `kcd_re` and `Projects/KCSE` from this snapshot. The other
projects remain upstream reference material and are disabled by
`LIBKCD2_BUILD_BUNDLED_PROJECTS=OFF`.

The KCD2MP-specific KCSE plugin lives in `../src/kcse/plugin.cpp`. It exposes a
versioned C ABI to the independently loaded `d3d12.dll`; no C++ standard-library
objects cross the DLL boundary.
