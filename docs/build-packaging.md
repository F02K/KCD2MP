# Build and release packaging

Every successful build from `build.bat` produces a clean distribution tree at:

```text
out/package/<profile>/
```

The raw CMake configuration directory also contains the language files in the
runtime-relative layout `mods/KCD2MP/Lang/`. This applies both to builds started
through `build.bat` and direct `cmake --build` invocations of the `KCD2MP`
target. Translation-only changes are copied again on the next build without
requiring the client DLL to be relinked.

`<profile>` is `debug` or `release`. Existing package contents are replaced only
after a new package has been staged successfully, so stale files from earlier
builds cannot leak into a release.

## Layout

```text
out/package/release/
|-- client/
|   |-- KingdomComeDeliverance2/
|   |   |-- Bin/Win64MasterMasterSteamPGO/
|   |   |   |-- d3d12.dll
|   |   |   |-- d3d12.pdb
|   |   |   |-- dinput8.dll
|   |   |   `-- dinput8.pdb
|   |   |-- mods/KCD2MP/KCSE/Plugins/
|   |   |   |-- KCD2MPKCSEClient.dll
|   |   |   `-- KCD2MPKCSEClient.pdb
|   |   |-- mods/KCD2MP/Lang/
|   |   |   |-- de.lang
|   |   |   |-- en.lang
|   |   |   `-- README.md
|   |   `-- KCSE/addresslib/
|   |       `-- kcd_addresslib_*.bin
|   `-- KCD2MP-Client-v0.0.9.zip
|-- server/
|   |-- KCD2MPServer.exe
|   |-- KCD2MPServer.pdb
|   |-- server.toml.example
|   |-- starter_profile.toml
|   |-- npc_archetypes.json
|   `-- tools/
|       |-- KCD2MPSignatureAudit.exe
|       `-- KCD2MPSignatureAudit.pdb
|-- tests/
|   |-- KCD2MP*Tests.exe
|   `-- KCD2MP*Tests.pdb
`-- SHA256SUMS.txt
```

The loose client tree and ZIP are built from the same deployment mapping used
by the build tool. Changing a deploy destination therefore changes package
generation through the same code path instead of requiring a second manually
maintained file list.

## Client ZIP

The archive root is `KingdomComeDeliverance2/`. A Steam user can extract the ZIP
into:

```text
<Steam library>/steamapps/common/
```

The archive then merges into the existing game directory without requiring
manual relocation or DLL renaming.

ZIP entries are sorted and use normalized timestamps and permissions. Given
identical input binaries, packaging produces identical archive bytes. The ZIP
contains the same runtime files, symbols, plugin, and Address Library tables as
the normal deploy operation.

## Standalone packaging

Packaging can be repeated from an already completed CMake build without
recompiling:

```powershell
python tools/package_build.py `
  --build-dir out/build/release `
  --config RelWithDebInfo `
  --output out/package/release
```

The command supports Visual Studio multi-configuration output and Ninja
single-configuration output. Missing or ambiguous artifacts stop packaging with
an actionable error.

## GitHub Actions

The nightly workflow uploads the complete `client`, `server`, and `tests`
directory tree as one Actions artifact. GitHub Releases receive:

- the install-ready client ZIP;
- the dedicated-server files;
- the signature-audit tool; and
- `SHA256SUMS.txt`.

The test executables and all symbols remain available in the Actions artifact
without cluttering the normal end-user download list.
