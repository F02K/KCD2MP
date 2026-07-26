from __future__ import annotations

import json
import stat
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from tools.build_tui.core import (
    BUILD_PROFILES,
    GAME_BIN_RELATIVE,
    AUDIT_TARGET,
    BuildEnvironment,
    BuildResult,
    BuildService,
    BuildToolError,
    ConfigStore,
    deploy_artifacts,
    detect_game_root,
    normalize_game_root,
    parse_vdf,
    resolve_game_location,
)


def _vdf_path(path: Path) -> str:
    return str(path).replace("\\", "\\\\")


def _create_game_root(root: Path) -> Path:
    game_root = root / "KingdomComeDeliverance2"
    game_bin = game_root / GAME_BIN_RELATIVE
    game_bin.mkdir(parents=True)
    (game_bin / "KingdomCome.exe").write_bytes(b"game")
    return game_root


def _write_manifest(library: Path, install_dir: str = "KingdomComeDeliverance2") -> None:
    steamapps = library / "steamapps"
    steamapps.mkdir(parents=True, exist_ok=True)
    (steamapps / "appmanifest_1771300.acf").write_text(
        '"AppState"\n{\n'
        '    "appid" "1771300"\n'
        '    "installdir" "' + install_dir + '"\n'
        "}\n",
        encoding="utf-8",
    )


class VdfTests(unittest.TestCase):
    def test_parse_nested_vdf_and_comments(self) -> None:
        parsed = parse_vdf(
            '// Steam libraries\n"libraryfolders" { "0" { "path" "F:\\\\Steam" } }'
        )
        self.assertEqual(parsed["libraryfolders"]["0"]["path"], r"F:\Steam")

    def test_parse_rejects_unclosed_object(self) -> None:
        with self.assertRaises(BuildToolError):
            parse_vdf('"root" { "key" "value"')


class SteamDiscoveryTests(unittest.TestCase):
    def test_detects_game_in_primary_library(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            steam = Path(temporary) / "Steam"
            game_root = _create_game_root(steam / "steamapps" / "common")
            _write_manifest(steam)

            self.assertEqual(detect_game_root([steam]), game_root.resolve())

    def test_detects_game_in_additional_library(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            steam = root / "Steam"
            library = root / "Games"
            (steam / "steamapps").mkdir(parents=True)
            (steam / "steamapps" / "libraryfolders.vdf").write_text(
                '"libraryfolders"\n{\n'
                '  "0" { "path" "' + _vdf_path(steam) + '" }\n'
                '  "1" { "path" "' + _vdf_path(library) + '" }\n'
                "}\n",
                encoding="utf-8",
            )
            game_root = _create_game_root(library / "steamapps" / "common")
            _write_manifest(library)

            self.assertEqual(detect_game_root([steam]), game_root.resolve())

    def test_missing_manifest_is_not_guessed(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            steam = Path(temporary) / "Steam"
            _create_game_root(steam / "steamapps" / "common")
            self.assertIsNone(detect_game_root([steam]))


class ConfigStoreTests(unittest.TestCase):
    def test_override_round_trip_and_precedence(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            game_root = _create_game_root(root)
            store = ConfigStore(root / "config" / "build-tool.json")

            saved = store.save_override(game_root)
            loaded, warning = store.load_override()
            location = resolve_game_location(store, [])

            self.assertEqual(saved, game_root.resolve())
            self.assertEqual(loaded, game_root.resolve())
            self.assertIsNone(warning)
            self.assertIsNotNone(location)
            assert location is not None
            self.assertEqual(location.source, "Saved override")

    def test_invalid_override_falls_back_to_detection(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            steam = root / "Steam"
            detected_game = _create_game_root(steam / "steamapps" / "common")
            _write_manifest(steam)
            config_path = root / "build-tool.json"
            config_path.write_text(
                json.dumps(
                    {
                        "schema_version": 1,
                        "game_root": str(root / "missing"),
                    }
                ),
                encoding="utf-8",
            )

            location = resolve_game_location(ConfigStore(config_path), [steam])

            self.assertIsNotNone(location)
            assert location is not None
            self.assertEqual(location.root, detected_game.resolve())
            self.assertIsNotNone(location.warning)

    def test_accepts_deployment_directory_as_input(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            game_root = _create_game_root(Path(temporary))
            self.assertEqual(
                normalize_game_root(game_root / GAME_BIN_RELATIVE),
                game_root.resolve(),
            )


class DeploymentTests(unittest.TestCase):
    def _result(self, root: Path) -> BuildResult:
        dll = root / "d3d12_.dll"
        pdb = root / "d3d12_.pdb"
        dll.write_bytes(b"dll")
        pdb.write_bytes(b"pdb")
        return BuildResult(BUILD_PROFILES["release"], root, dll, pdb)

    def test_deploys_and_renames_artifacts(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            artifacts = root / "artifacts"
            artifacts.mkdir()
            result = self._result(artifacts)
            game_root = _create_game_root(root / "game")

            destination = deploy_artifacts(result, game_root, lambda: False)

            self.assertEqual((destination / "d3d12.dll").read_bytes(), b"dll")
            self.assertEqual((destination / "d3d12.pdb").read_bytes(), b"pdb")
            self.assertFalse((destination / "d3d12.dll.kcd2mp.tmp").exists())

    def test_rejects_missing_game_executable(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            result = self._result(root)
            with self.assertRaisesRegex(BuildToolError, "KingdomCome.exe"):
                deploy_artifacts(result, root / "missing", lambda: False)

    def test_rejects_missing_artifact(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            artifacts = root / "artifacts"
            artifacts.mkdir()
            result = self._result(artifacts)
            result.pdb_path.unlink()
            game_root = _create_game_root(root / "game")

            with self.assertRaisesRegex(BuildToolError, "missing build artifacts"):
                deploy_artifacts(result, game_root, lambda: False)

    def test_rejects_running_game(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            artifacts = root / "artifacts"
            artifacts.mkdir()
            result = self._result(artifacts)
            game_root = _create_game_root(root / "game")

            with self.assertRaisesRegex(BuildToolError, "is running"):
                deploy_artifacts(result, game_root, lambda: True)

    def test_reports_locked_destination(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            artifacts = root / "artifacts"
            artifacts.mkdir()
            result = self._result(artifacts)
            game_root = _create_game_root(root / "game")

            with mock.patch("tools.build_tui.core.os.replace", side_effect=PermissionError):
                with self.assertRaisesRegex(BuildToolError, "Windows refused"):
                    deploy_artifacts(result, game_root, lambda: False)


class ProfileTests(unittest.TestCase):
    def test_profile_contract(self) -> None:
        self.assertEqual(BUILD_PROFILES["debug"].cmake_config, "Debug")
        self.assertFalse(BUILD_PROFILES["debug"].final)
        self.assertEqual(BUILD_PROFILES["release"].cmake_config, "RelWithDebInfo")
        self.assertTrue(BUILD_PROFILES["release"].final)


class BuildDirectoryTests(unittest.TestCase):
    def _environment(self, root: Path, generator: str) -> BuildEnvironment:
        return BuildEnvironment("cmake", generator, root / "Visual Studio")

    def test_preserves_compatible_build_directory(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            project = Path(temporary)
            build_dir = project / "out" / "build" / "debug"
            build_dir.mkdir(parents=True)
            (build_dir / "CMakeCache.txt").write_text(
                "CMAKE_GENERATOR:INTERNAL=Visual Studio 18 2026\n"
                "CMAKE_GENERATOR_PLATFORM:INTERNAL=x64\n"
                "CMAKE_TOOLCHAIN_FILE:FILEPATH={}\n"
                "VCPKG_TARGET_TRIPLET:STRING=x64-windows-static\n"
                "CMAKE_HOME_DIRECTORY:INTERNAL={}\n".format(
                    project
                    / ".cache"
                    / "vcpkg"
                    / "scripts"
                    / "buildsystems"
                    / "vcpkg.cmake",
                    project,
                ),
                encoding="utf-8",
            )
            marker = build_dir / "keep.txt"
            marker.write_text("keep", encoding="utf-8")

            BuildService(project)._prepare_build_directory(
                build_dir,
                self._environment(project, "Visual Studio 18 2026"),
                lambda _: None,
            )

            self.assertTrue(marker.is_file())

    def test_recreates_directory_when_generator_changes(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            project = Path(temporary)
            build_dir = project / "out" / "build" / "debug"
            build_dir.mkdir(parents=True)
            (build_dir / "CMakeCache.txt").write_text(
                "CMAKE_GENERATOR:INTERNAL=Visual Studio 17 2022\n"
                "CMAKE_GENERATOR_PLATFORM:INTERNAL=x64\n"
                "CMAKE_HOME_DIRECTORY:INTERNAL={}\n".format(project),
                encoding="utf-8",
            )
            marker = build_dir / "stale.txt"
            marker.write_text("stale", encoding="utf-8")
            marker.chmod(stat.S_IREAD)
            logs = []

            BuildService(project)._prepare_build_directory(
                build_dir,
                self._environment(project, "Visual Studio 18 2026"),
                logs.append,
            )

            self.assertTrue(build_dir.is_dir())
            self.assertFalse(marker.exists())
            self.assertIn("generator changed", logs[0])

    def test_recreates_legacy_directory_without_vcpkg_toolchain(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            project = Path(temporary)
            build_dir = project / "out" / "build" / "release"
            build_dir.mkdir(parents=True)
            (build_dir / "CMakeCache.txt").write_text(
                "CMAKE_GENERATOR:INTERNAL=Visual Studio 18 2026\n"
                "CMAKE_GENERATOR_PLATFORM:INTERNAL=x64\n"
                "CMAKE_HOME_DIRECTORY:INTERNAL={}\n".format(project),
                encoding="utf-8",
            )
            marker = build_dir / "stale.txt"
            marker.write_text("stale", encoding="utf-8")
            logs = []

            BuildService(project)._prepare_build_directory(
                build_dir,
                self._environment(project, "Visual Studio 18 2026"),
                logs.append,
            )

            self.assertTrue(build_dir.is_dir())
            self.assertFalse(marker.exists())
            self.assertIn("vcpkg toolchain changed or was missing", logs[0])


class SignatureAuditServiceTests(unittest.TestCase):
    def test_runs_native_audit_against_installed_whgame(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            project = root / "project"
            project.mkdir()
            artifacts = project / "artifacts"
            artifacts.mkdir()
            audit = artifacts / "{}.exe".format(AUDIT_TARGET)
            audit.write_bytes(b"audit")
            dll = artifacts / "d3d12_.dll"
            pdb = artifacts / "d3d12_.pdb"
            dll.write_bytes(b"dll")
            pdb.write_bytes(b"pdb")
            result = BuildResult(
                BUILD_PROFILES["debug"], artifacts, dll, pdb, audit
            )
            game_root = _create_game_root(root / "game")
            whgame = game_root / GAME_BIN_RELATIVE / "WHGame.dll"
            whgame.write_bytes(b"game")
            service = BuildService(project)

            with mock.patch.object(service, "_run") as run:
                audited = service.audit(
                    BUILD_PROFILES["debug"], game_root, lambda _: None, result
                )

            self.assertEqual(audited, whgame)
            run.assert_called_once_with([str(audit), str(whgame)], mock.ANY)


if __name__ == "__main__":
    unittest.main()
