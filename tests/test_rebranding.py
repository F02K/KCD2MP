from __future__ import annotations

import json
import os
import re
import unittest
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parents[1]
OLD_NAME = "KCD2" + "ModLoader"


class RebrandingTests(unittest.TestCase):
    def test_project_version_is_the_wire_and_package_version(self) -> None:
        cmake = (PROJECT_ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
        match = re.search(r"project\(KCD2MP VERSION ([0-9]+\.[0-9]+\.[0-9]+)", cmake)
        self.assertIsNotNone(match)
        version = match.group(1)
        self.assertEqual(version, "0.1.0")

        manifest = json.loads((PROJECT_ROOT / "vcpkg.json").read_text(encoding="utf-8"))
        self.assertEqual(manifest["version-string"], version)

        generated_version = (
            PROJECT_ROOT / "src" / "multiplayer" / "version.hpp.in"
        ).read_text(encoding="utf-8")
        resources = (PROJECT_ROOT / "src" / "resources.rc.in").read_text(
            encoding="utf-8"
        )
        proto = (PROJECT_ROOT / "protocol" / "kcd2mp.proto").read_text(
            encoding="utf-8"
        )
        self.assertIn('kcd2mp_version = "@PROJECT_VERSION@"', generated_version)
        self.assertIn('"@PROJECT_VERSION@\\0"', resources)
        self.assertIn("string version = 1;", proto)
        self.assertNotIn("uint32 protocol_version", proto)

    def test_old_name_only_remains_in_readme_attribution(self) -> None:
        matches = []
        ignored_directories = {".git", "build", "out", ".venv-build", "__pycache__"}
        text_suffixes = {
            ".bat",
            ".cmake",
            ".cpp",
            ".h",
            ".hpp",
            ".in",
            ".json",
            ".lua",
            ".md",
            ".ps1",
            ".py",
            ".rc",
            ".sh",
            ".txt",
            ".xml",
            ".yml",
            ".yaml",
        }
        for directory, names, files in os.walk(PROJECT_ROOT):
            names[:] = [name for name in names if name not in ignored_directories]
            for file_name in files:
                path = Path(directory) / file_name
                if path.suffix.lower() not in text_suffixes and path.name != "CMakeLists.txt":
                    continue
                try:
                    lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
                except OSError:
                    continue
                for line_number, line in enumerate(lines, 1):
                    if OLD_NAME in line:
                        matches.append((path.relative_to(PROJECT_ROOT), line_number))

        self.assertEqual(len(matches), 1)
        self.assertEqual(matches[0][0], Path("README.md"))

    def test_cmake_uses_new_target_and_generated_version_source(self) -> None:
        cmake = (PROJECT_ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
        git_cmake = (PROJECT_ROOT / "cmake_scripts" / "git.cmake").read_text(
            encoding="utf-8"
        )

        self.assertIn("project(KCD2MP", cmake)
        self.assertIn("project(KCD2MP VERSION 0.1.0", cmake)
        self.assertIn("generated/kcd2mp_version.hpp", cmake)
        self.assertIn("GENERATED_RESOURCE_FILE", cmake)
        self.assertIn("add_library(KCD2MP", cmake)
        self.assertIn("target_compile_definitions(KCD2MP PRIVATE", cmake)
        self.assertIn("GENERATED_VERSION_SOURCE", cmake)
        self.assertIn('version\\\\.cpp$"', cmake)
        self.assertIn("${CMAKE_CURRENT_BINARY_DIR}/generated/version.cpp", git_cmake)
        self.assertNotIn('"${SRC_DIR}/version.cpp"', git_cmake)

    def test_example_directory_was_renamed(self) -> None:
        plugins = PROJECT_ROOT / "examples" / "plugins"
        self.assertTrue((plugins / "KCD2MP-TestMod" / "main.lua").is_file())
        self.assertFalse((plugins / OLD_NAME.replace("ModLoader", "ModLoader-TestMod")).exists())


if __name__ == "__main__":
    unittest.main()
