# libKCD2 vendor integration

KCD2MP includes [F02K/libKCD2](https://github.com/F02K/libKCD2) as the
`libKCD2` Git submodule. The superproject pins the exact libKCD2 commit used by
the runtime. libKCD2 in turn pins KCSE as its own nested submodule.

Clone KCD2MP and initialize all vendor repositories:

```powershell
git clone https://github.com/F02K/KCD2MP.git
Set-Location KCD2MP
powershell -ExecutionPolicy Bypass -File tools/init_vendor.ps1
```

The helper initializes the outer libKCD2 submodule, overrides libKCD2's public
KCSE dependency from its SSH URL to HTTPS in the local Git configuration, and
then initializes KCSE at the commit pinned by libKCD2. It is safe to run again
after pulling KCD2MP.

`git clone --recurse-submodules` also works for developers who already have
GitHub SSH access configured for libKCD2's nested KCSE URL.

Update libKCD2 deliberately:

```powershell
git -C libKCD2 fetch origin
git -C libKCD2 checkout <tested-libKCD2-commit>
git add libKCD2
```

The `libKCD2` checkout must remain clean. KCD2MP defines the `kcd_re` and
`KCSE` build targets in its root `CMakeLists.txt`, so no KCD2MP-specific patch
is applied inside the vendor repository.

The KCD2MP-specific KCSE plugin lives in `src/kcse/plugin.cpp`. It exposes a
versioned C ABI to the independently loaded KCD2MP DLL; no C++ standard-library
objects or allocator ownership cross the DLL boundary.
