from __future__ import annotations

import re
import unittest
from pathlib import Path


PROJECT_ROOT = Path(__file__).resolve().parents[1]


class SignatureArchitectureTests(unittest.TestCase):
    def test_every_configuration_builds_the_active_kcse_client(self) -> None:
        cmake = (PROJECT_ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
        self.assertFalse((PROJECT_ROOT / "src" / "kcse" / "plugin_stub.cpp").exists())
        self.assertNotIn("<CONFIG:Debug>:${SRC_DIR}/kcse/plugin_stub.cpp", cmake)
        self.assertIn('"${SRC_DIR}/kcse/plugin.cpp"', cmake)
        self.assertIn('set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")', cmake)
        self.assertIn("set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG Release)", cmake)

    def test_registry_is_the_single_source_for_65_signatures(self) -> None:
        core = (
            PROJECT_ROOT / "src" / "signatures" / "signature_core.cpp"
        ).read_text(encoding="utf-8")
        init = (PROJECT_ROOT / "src" / "kcd2_init.cpp").read_text(encoding="utf-8")
        entries = re.findall(r'signature_spec\{"([^"]+)",\s*"([^"]+)"', core)

        self.assertEqual(len(entries), 65)
        self.assertEqual(len({name for name, _ in entries}), 65)
        self.assertIn("static_assert(signature_registry.size()", core)
        self.assertNotIn("kcd2_address::scan(", init)
        self.assertNotIn(".get_call()", init)
        self.assertNotIn(".rip()", init)
        self.assertIn(
            'validate_named_vtable("CXConsole vtable", "CXConsole vtable[35]"',
            core,
        )
        self.assertIn(
            'validate_named_vtable("CEntity vtable", "CEntity::Activate", 52',
            core,
        )
        self.assertIn(
            'validate_named_vtable("CEntity vtable", "CEntity::SetFlags", 5',
            core,
        )
        self.assertIn(
            'validate_named_vtable("CEntity vtable", "CEntity::Hide", 63',
            core,
        )
        self.assertIn('"CEntitySystem vtable"', core)
        self.assertIn('"CEntitySystem::SpawnEntity"', core)
        self.assertIn('"CEntitySystem::RemoveEntity"', core)
        self.assertIn('"CEntitySystem::GetEntityIterator"', core)
        self.assertIn('"IEntitySystem::AddSink ABI"', core)
        self.assertIn('"CCryAction::EndGameContext ABI"', core)
        self.assertIn('"CEntity::ResolvePhysicsProxy ABI"', core)
        self.assertIn('"C_SoulList::ApplySharedSoul ABI"', core)
        self.assertIn('"CEntitySystem::GetEntityLayerData"', core)
        self.assertIn('"gEnv pConsole pointer"', core)
        self.assertIn('resolved("gEnv pConsole pointer")', init)
        self.assertIn("attach_existing_engine_console()", init)
        self.assertIn("vtable[23]", init)

    def test_zydis_is_explicit_and_shared_with_native_audit(self) -> None:
        cmake = (PROJECT_ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
        core = (
            PROJECT_ROOT / "src" / "signatures" / "signature_core.cpp"
        ).read_text(encoding="utf-8")
        audit = (PROJECT_ROOT / "tools" / "signature_audit_main.cpp").read_text(
            encoding="utf-8"
        )

        self.assertIn("add_library(KCD2MPSignatureCore STATIC", cmake)
        self.assertIn("target_link_libraries(KCD2MPSignatureCore PUBLIC Zydis)", cmake)
        self.assertIn("add_executable(KCD2MPSignatureAudit", cmake)
        self.assertIn("ZydisDecoderDecodeFull", core)
        self.assertIn("resolve_all(*image)", audit)
        self.assertFalse((PROJECT_ROOT / "tools" / "signature_audit.py").exists())

    def test_magic_address_offsets_were_removed(self) -> None:
        source = (PROJECT_ROOT / "src" / "kcd2_init.cpp").read_text(encoding="utf-8")
        self.assertNotIn("offset(0x3D)", source)
        self.assertNotIn("offset(0x95)", source)
        self.assertNotIn("offset(0x65)", source)
        self.assertIn('derived("CVegetation vtable")', source)
        self.assertIn('derived("gEnv pGame pointer")', source)


class StartupSafetyTests(unittest.TestCase):
    def test_dllmain_only_starts_bootstrap_after_proxy_setup(self) -> None:
        source = (PROJECT_ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
        dllmain = source[source.index("BOOL APIENTRY DllMain") :]
        self.assertIn("dll_proxy::init()", dllmain)
        self.assertIn("DisableThreadLibraryCalls", dllmain)
        self.assertIn("CreateThread", dllmain)
        self.assertNotIn("rom::init", dllmain)
        self.assertNotIn("kcd2_init()", dllmain)
        self.assertNotIn("new hooking", dllmain)

    def test_hook_creation_is_committed_only_after_full_preflight(self) -> None:
        source = (PROJECT_ROOT / "src" / "kcd2_init.cpp").read_text(encoding="utf-8")
        self.assertIn("#define hooking transactional_hooking", source)
        preflight = source.index("kcd2_address::begin_scan_session()")
        implementation = source.index("kcd2_init_impl();", preflight)
        commit = source.index("for (auto &register_hook", implementation)
        self.assertLess(preflight, implementation)
        self.assertLess(implementation, commit)
        self.assertIn("summary.requested == summary.resolved", source)
        self.assertIn("summary.derived_requested == summary.derived_resolved", source)

    def test_exception_filter_does_not_install_anti_remover(self) -> None:
        source = (PROJECT_ROOT / "src" / "main.cpp").read_text(encoding="utf-8")
        self.assertIn("new exception_handler(false, big_exception_handler)", source)

    def test_proxy_has_no_shared_dispatch_pointer(self) -> None:
        cpp = (PROJECT_ROOT / "src" / "dll_proxy" / "dll_proxy.cpp").read_text(
            encoding="utf-8"
        )
        assembly = (
            PROJECT_ROOT / "src" / "dll_proxy" / "d3d12_proxy.asm"
        ).read_text(encoding="utf-8")
        self.assertNotIn("FARPROC PA", cpp)
        self.assertNotIn("EXTERN PA", assembly)
        self.assertIn("pD3D12CreateDevice", cpp)
        self.assertIn("jmp qword ptr [pD3D12CreateDevice]", assembly)

    def test_environment_bootstrap_precedes_destructive_native_mutation(self) -> None:
        source = (
            PROJECT_ROOT / "src" / "kcse" / "native_runtime.cpp"
        ).read_text(encoding="utf-8")
        begin = source.index("sandbox_start_result native_runtime::begin_sandbox")
        end = source.index("sandbox_poll_result native_runtime::poll_sandbox", begin)
        sandbox = source[begin:end]

        environment = sandbox.index("apply_environment_state(")
        save_lock = sandbox.index('"join.sandbox.save-load-lock.begin"')
        profile_apply = sandbox.index("reconcile_profile(m_profiles, target)")
        self.assertLess(environment, save_lock)
        self.assertLess(environment, profile_apply)

        environment_failure = sandbox.index(
            '"join.sandbox.environment.failed"', environment
        )
        environment_success = sandbox.index(
            '"join.sandbox.environment.ok"', environment_failure
        )
        self.assertNotIn(
            "begin_native_unload", sandbox[environment_failure:environment_success]
        )

    def test_environment_cvar_override_restores_engine_flags(self) -> None:
        source = (
            PROJECT_ROOT / "src" / "kcse" / "native_runtime.cpp"
        ).read_text(encoding="utf-8")
        self.assertIn("environment_cvar_override_mask", source)
        self.assertIn("cvar->ClearFlags(*overridden_flags)", source)
        self.assertIn(
            "cvar->SetFlags(current_flags | *overridden_flags)", source
        )
        self.assertIn('"join.environment.cvar.applied"', source)


if __name__ == "__main__":
    unittest.main()
