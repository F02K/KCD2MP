#pragma once

// Autobrew's own config persistence -- self-contained (no PersistCVarEdits dependency), and
// deliberately MCM-ONLY: HandleMcmMessage (mcm.h) calls PersistSetting for every menu edit;
// console edits of the kcse_autobrew_* cvars apply for the session but are NOT recorded.
//
// Write side (ours): PersistSetting rewrites Mods/Autobrew/mod.cfg.
// Read side (the engine's): WHGame evals Mods/<mod>/mod.cfg at startup (ISystem vf+0x4E8
// LoadConfiguration, same path as user.cfg -- mod loader 0x181DDDAD0), and the console HOLDS
// values for not-yet-registered cvars, applying them when RegisterCVar* runs at DataLoaded
// -- so the round trip needs no load code at all.  (The native VF_DUMPTODISK flag is set on
// many engine cvars but has NO consumer in WHGame; the options screen persists its cvars via
// a hardcoded sync list into the profile attributes.xml instead -- see the cvar-persistence
// RE notes.)

namespace Autobrew {

// Seed the entry map from last session's mod.cfg so a rewrite never forgets settings that
// weren't re-edited this session.  Call once at plugin load.
void LoadPersistedEdits();

// Record one MCM edit (cvar line in mod.cfg) and rewrite the file.  No-ops when the value
// matches what is already persisted.
void PersistSetting(const char* cvarName, double value, bool isFloat);

}  // namespace Autobrew
