"""Build, Steam discovery, configuration, and deployment primitives."""

from __future__ import annotations

import json
import os
import re
import shutil
import stat
import struct
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path
from typing import Any, Callable, Dict, Iterable, List, Optional, Sequence, Tuple

APP_ID = "1771300"
GAME_BIN_RELATIVE = Path("Bin") / "Win64MasterMasterSteamPGO"
GAME_EXECUTABLE = "KingdomCome.exe"
PROJECT_TARGET = "KCD2MPRuntime"
SERVER_TARGET = "KCD2MPServer"
AUDIT_TARGET = "KCD2MPSignatureAudit"
TEST_TARGETS = ("KCD2MPTests",)
VCPKG_BASELINE = "908da3a305a0a8028d9602ab241b433652b3df69"
VCPKG_REPOSITORY = "https://github.com/microsoft/vcpkg.git"
VCPKG_TRIPLET = "x64-windows-static"
PROJECT_ROOT = Path(__file__).resolve().parents[2]
ADDRESS_LIBRARY_SUBMODULE = Path("vendor") / "Address-Library-For-KCSE"
ADDRESS_LIBRARY_DATA = ADDRESS_LIBRARY_SUBMODULE / "kcd2_address_library"
ADDRESS_LIBRARY_GLOB = "kcd_addresslib_*.bin"
ADDRESS_LIBRARY_HEADER = struct.Struct("<4sIII")
ADDRESS_LIBRARY_RECORD = struct.Struct("<II")
ADDRESS_LIBRARY_NAME = re.compile(
    r"^kcd_addresslib_(steam|gog|epic)_(.+)\.bin$", re.IGNORECASE
)
ADDRESS_LIBRARY_DISTRIBUTIONS = {"steam": 1, "gog": 2, "epic": 3}

LogCallback = Callable[[str], None]


class BuildToolError(RuntimeError):
    """An actionable error suitable for display in the TUI."""


@dataclass(frozen=True)
class BuildProfile:
    key: str
    label: str
    cmake_config: str
    final: bool


BUILD_PROFILES: Dict[str, BuildProfile] = {
    "debug": BuildProfile("debug", "Debug", "Debug", False),
    "release": BuildProfile("release", "Release", "RelWithDebInfo", True),
}


@dataclass(frozen=True)
class BuildEnvironment:
    cmake: str
    generator: str
    visual_studio_path: Path


@dataclass(frozen=True)
class BuildResult:
    profile: BuildProfile
    build_dir: Path
    dll_path: Path
    pdb_path: Path
    audit_path: Optional[Path] = None
    server_path: Optional[Path] = None
    kcse_loader_path: Optional[Path] = None
    kcse_loader_pdb_path: Optional[Path] = None
    kcse_client_path: Optional[Path] = None
    kcse_client_pdb_path: Optional[Path] = None
    address_library_paths: Tuple[Path, ...] = ()


@dataclass(frozen=True)
class GameLocation:
    root: Path
    source: str
    warning: Optional[str] = None


class ConfigStore:
    """Persist the manual game-root override outside the repository."""

    def __init__(self, path: Optional[Path] = None) -> None:
        self.path = path or default_config_path()

    def load_override(self) -> Tuple[Optional[Path], Optional[str]]:
        if not self.path.exists():
            return None, None
        try:
            payload = json.loads(self.path.read_text(encoding="utf-8"))
        except (OSError, json.JSONDecodeError) as exc:
            return None, "Could not read the saved game path: {}".format(exc)

        if payload.get("schema_version") != 1 or not isinstance(payload.get("game_root"), str):
            return None, "The saved game-path configuration is invalid."

        root = normalize_game_root(Path(payload["game_root"]))
        if not is_valid_game_root(root):
            return None, "The saved game path no longer contains {}.".format(GAME_EXECUTABLE)
        return root, None

    def save_override(self, root: Path) -> Path:
        normalized = normalize_game_root(root)
        if not is_valid_game_root(normalized):
            raise BuildToolError(
                "The selected directory does not contain {} under {}.".format(
                    GAME_EXECUTABLE, GAME_BIN_RELATIVE
                )
            )

        self.path.parent.mkdir(parents=True, exist_ok=True)
        payload = {
            "schema_version": 1,
            "game_root": str(normalized),
        }
        _atomic_write_text(self.path, json.dumps(payload, indent=2) + "\n")
        return normalized

    def clear_override(self) -> None:
        try:
            self.path.unlink()
        except FileNotFoundError:
            pass
        except OSError as exc:
            raise BuildToolError("Could not remove the saved game path: {}".format(exc)) from exc


def default_config_path() -> Path:
    local_app_data = os.environ.get("LOCALAPPDATA")
    if local_app_data:
        return Path(local_app_data) / "KCD2MP" / "build-tool.json"
    return Path.home() / "AppData" / "Local" / "KCD2MP" / "build-tool.json"


def normalize_game_root(path: Path) -> Path:
    """Accept a game root, executable, or Win64 deployment directory."""

    candidate = path.expanduser()
    try:
        candidate = candidate.resolve(strict=False)
    except OSError:
        candidate = candidate.absolute()

    if candidate.is_file() or candidate.name.lower() == GAME_EXECUTABLE.lower():
        candidate = candidate.parent

    if (candidate / GAME_EXECUTABLE).is_file() and candidate.name.lower() == GAME_BIN_RELATIVE.name.lower():
        return candidate.parents[1]
    return candidate


def game_bin_dir(game_root: Path) -> Path:
    return normalize_game_root(game_root) / GAME_BIN_RELATIVE


def is_valid_game_root(game_root: Path) -> bool:
    return (game_bin_dir(game_root) / GAME_EXECUTABLE).is_file()


def resolve_game_location(
    config_store: Optional[ConfigStore] = None,
    steam_roots: Optional[Iterable[Path]] = None,
) -> Optional[GameLocation]:
    store = config_store or ConfigStore()
    override, warning = store.load_override()
    if override is not None:
        return GameLocation(override, "Saved override")

    detected = detect_game_root(steam_roots)
    if detected is not None:
        return GameLocation(detected, "Steam auto-detection", warning)
    return None


def _vdf_tokens(text: str) -> List[str]:
    tokens: List[str] = []
    index = 0
    length = len(text)

    while index < length:
        char = text[index]
        if char.isspace():
            index += 1
            continue
        if char == "/" and index + 1 < length and text[index + 1] == "/":
            newline = text.find("\n", index + 2)
            index = length if newline == -1 else newline + 1
            continue
        if char in "{}":
            tokens.append(char)
            index += 1
            continue
        if char == '"':
            index += 1
            value: List[str] = []
            while index < length:
                char = text[index]
                if char == '"':
                    index += 1
                    break
                if char == "\\" and index + 1 < length:
                    next_char = text[index + 1]
                    if next_char in ('"', "\\"):
                        value.append(next_char)
                        index += 2
                        continue
                value.append(char)
                index += 1
            else:
                raise BuildToolError("Unterminated quoted string in a Steam VDF file.")
            tokens.append("".join(value))
            continue

        end = index
        while end < length and not text[end].isspace() and text[end] not in "{}":
            end += 1
        tokens.append(text[index:end])
        index = end

    return tokens


def parse_vdf(text: str) -> Dict[str, Any]:
    """Parse the KeyValues subset used by Steam library and app manifests."""

    tokens = _vdf_tokens(text)
    position = 0

    def parse_object(expect_close: bool) -> Dict[str, Any]:
        nonlocal position
        result: Dict[str, Any] = {}
        while position < len(tokens):
            token = tokens[position]
            if token == "}":
                if not expect_close:
                    raise BuildToolError("Unexpected closing brace in a Steam VDF file.")
                position += 1
                return result
            if token == "{":
                raise BuildToolError("Unexpected opening brace in a Steam VDF file.")

            key = token
            position += 1
            if position >= len(tokens):
                raise BuildToolError("Missing value for {!r} in a Steam VDF file.".format(key))

            if tokens[position] == "{":
                position += 1
                value: Any = parse_object(True)
            else:
                value = tokens[position]
                position += 1
            result[key] = value

        if expect_close:
            raise BuildToolError("Missing closing brace in a Steam VDF file.")
        return result

    parsed = parse_object(False)
    if position != len(tokens):
        raise BuildToolError("Could not parse the complete Steam VDF file.")
    return parsed


def _read_vdf(path: Path) -> Optional[Dict[str, Any]]:
    try:
        return parse_vdf(path.read_text(encoding="utf-8-sig", errors="replace"))
    except (OSError, BuildToolError):
        return None


def _registry_steam_roots() -> List[Path]:
    if os.name != "nt":
        return []
    try:
        import winreg
    except ImportError:
        return []

    queries = [
        (winreg.HKEY_CURRENT_USER, r"Software\Valve\Steam", "SteamPath", 0),
        (winreg.HKEY_LOCAL_MACHINE, r"SOFTWARE\Valve\Steam", "InstallPath", 0),
        (
            winreg.HKEY_LOCAL_MACHINE,
            r"SOFTWARE\WOW6432Node\Valve\Steam",
            "InstallPath",
            getattr(winreg, "KEY_WOW64_32KEY", 0),
        ),
    ]
    roots: List[Path] = []
    for hive, key_name, value_name, extra_flags in queries:
        try:
            with winreg.OpenKey(hive, key_name, 0, winreg.KEY_READ | extra_flags) as key:
                value, _ = winreg.QueryValueEx(key, value_name)
        except OSError:
            continue
        path = Path(value)
        if path not in roots:
            roots.append(path)
    return roots


def _steam_libraries(steam_root: Path) -> List[Path]:
    libraries = [steam_root]
    data = _read_vdf(steam_root / "steamapps" / "libraryfolders.vdf")
    if not data:
        return libraries

    entries = data.get("libraryfolders")
    if not isinstance(entries, dict):
        return libraries

    for entry in entries.values():
        path_value: Optional[str] = None
        if isinstance(entry, dict) and isinstance(entry.get("path"), str):
            path_value = entry["path"]
        elif isinstance(entry, str):
            path_value = entry
        if path_value:
            library = Path(path_value)
            if library not in libraries:
                libraries.append(library)
    return libraries


def detect_game_root(steam_roots: Optional[Iterable[Path]] = None) -> Optional[Path]:
    roots = list(steam_roots) if steam_roots is not None else _registry_steam_roots()
    seen: set[Path] = set()

    for steam_root in roots:
        for library in _steam_libraries(Path(steam_root)):
            try:
                normalized_library = library.resolve(strict=False)
            except OSError:
                normalized_library = library.absolute()
            if normalized_library in seen:
                continue
            seen.add(normalized_library)

            manifest = _read_vdf(
                normalized_library / "steamapps" / "appmanifest_{}.acf".format(APP_ID)
            )
            if not manifest:
                continue
            app_state = manifest.get("AppState")
            if not isinstance(app_state, dict):
                continue
            if str(app_state.get("appid", APP_ID)) != APP_ID:
                continue
            install_dir = app_state.get("installdir")
            if not isinstance(install_dir, str) or not install_dir:
                continue

            root = (
                normalized_library / "steamapps" / "common" / install_dir
            ).resolve(strict=False)
            if is_valid_game_root(root):
                return root
    return None


def detect_build_environment() -> BuildEnvironment:
    cmake = shutil.which("cmake")
    if not cmake:
        raise BuildToolError("CMake was not found on PATH.")

    vswhere = _find_vswhere()
    if vswhere is None:
        raise BuildToolError(
            "vswhere.exe was not found. Install Visual Studio with the MSVC x64 C++ workload."
        )

    command = [
        str(vswhere),
        "-products",
        "*",
        "-requires",
        "Microsoft.VisualStudio.Component.VC.Tools.x86.x64",
        "-format",
        "json",
        "-utf8",
    ]
    try:
        completed = subprocess.run(
            command,
            check=True,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
        )
        installations = json.loads(completed.stdout)
    except (OSError, subprocess.CalledProcessError, json.JSONDecodeError) as exc:
        raise BuildToolError("Visual Studio detection failed: {}".format(exc)) from exc

    if not isinstance(installations, list):
        installations = []
    installations.sort(
        key=lambda item: _version_tuple(str(item.get("installationVersion", "0"))),
        reverse=True,
    )

    cmake_help = _capture_text([cmake, "--help"])
    generator_names = {
        18: "Visual Studio 18 2026",
        17: "Visual Studio 17 2022",
    }
    for installation in installations:
        version = _version_tuple(str(installation.get("installationVersion", "0")))
        if not version:
            continue
        generator = generator_names.get(version[0])
        install_path = installation.get("installationPath")
        if generator and install_path and generator in cmake_help:
            return BuildEnvironment(cmake, generator, Path(str(install_path)))

    raise BuildToolError(
        "No CMake-compatible Visual Studio installation with the MSVC x64 workload was found."
    )


def _find_vswhere() -> Optional[Path]:
    candidates: List[Path] = []
    program_files_x86 = os.environ.get("ProgramFiles(x86)")
    if program_files_x86:
        candidates.append(
            Path(program_files_x86) / "Microsoft Visual Studio" / "Installer" / "vswhere.exe"
        )
    candidates.append(
        Path(r"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe")
    )
    executable = shutil.which("vswhere")
    if executable:
        candidates.append(Path(executable))
    for candidate in candidates:
        if candidate.is_file():
            return candidate
    return None


def _version_tuple(value: str) -> Tuple[int, ...]:
    return tuple(int(part) for part in re.findall(r"\d+", value))


def _capture_text(command: Sequence[str]) -> str:
    try:
        return subprocess.run(
            list(command),
            check=True,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
        ).stdout
    except (OSError, subprocess.CalledProcessError) as exc:
        raise BuildToolError("Command failed: {}".format(subprocess.list2cmdline(command))) from exc


def discover_address_libraries(directory: Path) -> Tuple[Path, ...]:
    """Return all valid KASL tables in deterministic order."""

    paths = tuple(sorted(directory.glob(ADDRESS_LIBRARY_GLOB)))
    if not paths:
        raise BuildToolError("No Address Library tables were found in {}.".format(directory))

    seen = set()
    for path in paths:
        name_match = ADDRESS_LIBRARY_NAME.fullmatch(path.name)
        if name_match is None:
            raise BuildToolError("Address Library table has an invalid filename: {}".format(path))
        try:
            size = path.stat().st_size
            with path.open("rb") as stream:
                header = stream.read(ADDRESS_LIBRARY_HEADER.size)
        except OSError as exc:
            raise BuildToolError("Could not read Address Library table {}: {}".format(path, exc)) from exc
        if len(header) != ADDRESS_LIBRARY_HEADER.size:
            raise BuildToolError("Address Library table has a truncated header: {}".format(path))
        magic, format_version, distribution, count = ADDRESS_LIBRARY_HEADER.unpack(header)
        if magic != b"KASL" or format_version == 0 or distribution not in {1, 2, 3}:
            raise BuildToolError("Address Library table has an invalid KASL header: {}".format(path))
        named_distribution = ADDRESS_LIBRARY_DISTRIBUTIONS[name_match.group(1).lower()]
        if distribution != named_distribution:
            raise BuildToolError(
                "Address Library filename/header distribution mismatch: {}".format(path)
            )
        expected_size = ADDRESS_LIBRARY_HEADER.size + count * ADDRESS_LIBRARY_RECORD.size
        if size != expected_size:
            raise BuildToolError(
                "Address Library table {} has {} bytes; its header requires {}.".format(
                    path, size, expected_size
                )
            )
        key = (distribution, name_match.group(2).lower())
        if key in seen:
            raise BuildToolError("Duplicate Address Library distribution/build table: {}".format(path))
        seen.add(key)
    return paths


class BuildService:
    def __init__(
        self,
        project_root: Path = PROJECT_ROOT,
        environment_detector: Callable[[], BuildEnvironment] = detect_build_environment,
    ) -> None:
        self.project_root = project_root.resolve()
        self._environment_detector = environment_detector

    def build(self, profile: BuildProfile, log: LogCallback = print) -> BuildResult:
        self._ensure_address_library_submodule(log)
        environment = self._environment_detector()
        build_dir = self.project_root / "out" / "build" / profile.key
        self._prepare_build_directory(build_dir, environment, log)
        self._configure(environment, build_dir, profile, log)

        build_command = [
            environment.cmake,
            "--build",
            str(build_dir),
            "--config",
            profile.cmake_config,
            "--target",
            PROJECT_TARGET,
            SERVER_TARGET,
            AUDIT_TARGET,
            "--parallel",
        ]
        self._run(build_command, log)
        self._run(
            [
                environment.cmake,
                "--build",
                str(build_dir),
                "--config",
                profile.cmake_config,
                "--target",
                *TEST_TARGETS,
                "--parallel",
            ],
            log,
        )
        self._run(
            [
                "ctest",
                "--test-dir",
                str(build_dir),
                "-C",
                profile.cmake_config,
                "-R",
                "^KCD2MP",
                "--output-on-failure",
            ],
            log,
        )

        artifact_dir = build_dir / profile.cmake_config
        result = BuildResult(
            profile=profile,
            build_dir=build_dir,
            dll_path=artifact_dir / "d3d12_.dll",
            pdb_path=artifact_dir / "d3d12_.pdb",
            audit_path=artifact_dir / "{}.exe".format(AUDIT_TARGET),
            server_path=artifact_dir / "{}.exe".format(SERVER_TARGET),
            kcse_loader_path=artifact_dir / "dinput8.dll",
            kcse_loader_pdb_path=artifact_dir / "dinput8.pdb",
            kcse_client_path=artifact_dir / "KCD2MPKCSEClient.dll",
            kcse_client_pdb_path=artifact_dir / "KCD2MPKCSEClient.pdb",
            address_library_paths=discover_address_libraries(
                artifact_dir / "KCSE" / "addresslib"
            ),
        )
        missing = [
            str(path)
            for path in (
                result.dll_path,
                result.pdb_path,
                result.audit_path,
                result.server_path,
                result.kcse_loader_path,
                result.kcse_loader_pdb_path,
                result.kcse_client_path,
                result.kcse_client_pdb_path,
                *result.address_library_paths,
            )
            if path is not None and not path.is_file()
        ]
        if missing:
            raise BuildToolError(
                "Build completed, but expected artifacts are missing:\n{}".format(
                    "\n".join(missing)
                )
            )
        return result

    def update_address_library(self, log: LogCallback = print) -> str:
        """Fast-forward the pinned Address Library submodule to origin's HEAD."""

        self._ensure_address_library_submodule(log)
        git = shutil.which("git")
        if not git:
            raise BuildToolError("Git was not found on PATH.")
        root = self.project_root / ADDRESS_LIBRARY_SUBMODULE
        dirty = _capture_text([git, "-C", str(root), "status", "--porcelain"]).strip()
        if dirty:
            raise BuildToolError(
                "The Address Library submodule has local changes. Commit or stash them before updating."
            )

        current = _capture_text([git, "-C", str(root), "rev-parse", "HEAD"]).strip()
        self._run([git, "-C", str(root), "fetch", "origin", "HEAD"], log)
        latest = _capture_text([git, "-C", str(root), "rev-parse", "FETCH_HEAD"]).strip()
        if current == latest:
            log("Address Library is already current at {}.".format(current[:12]))
        else:
            ancestry = subprocess.run(
                [git, "-C", str(root), "merge-base", "--is-ancestor", current, latest],
                check=False,
                capture_output=True,
            )
            if ancestry.returncode != 0:
                raise BuildToolError(
                    "Upstream Address Library history is not a fast-forward from the pinned commit; "
                    "review it manually before updating."
                )
            self._run([git, "-C", str(root), "checkout", "--detach", latest], log)
            log("Updated Address Library {} -> {}.".format(current[:12], latest[:12]))

        discover_address_libraries(root / "kcd2_address_library")
        self._run(
            [
                sys.executable,
                str(self.project_root / "tools" / "audit_address_library.py"),
                "--project-root",
                str(self.project_root),
            ],
            log,
        )
        self._run(
            [
                sys.executable,
                str(self.project_root / "tools" / "generate_address_library_manifest.py"),
                "--project-root",
                str(self.project_root),
            ],
            log,
        )
        return latest

    def _ensure_address_library_submodule(self, log: LogCallback) -> Path:
        root = self.project_root / ADDRESS_LIBRARY_SUBMODULE
        data = self.project_root / ADDRESS_LIBRARY_DATA
        if data.is_dir():
            return root
        git = shutil.which("git")
        if not git:
            raise BuildToolError(
                "Address Library is not initialized and Git was not found on PATH."
            )
        self._run(
            [
                git,
                "submodule",
                "update",
                "--init",
                "--",
                ADDRESS_LIBRARY_SUBMODULE.as_posix(),
            ],
            log,
        )
        if not data.is_dir():
            raise BuildToolError(
                "Address Library submodule initialization completed without its data directory."
            )
        return root

    def audit(
        self,
        profile: BuildProfile,
        game_root: Path,
        log: LogCallback = print,
        build_result: Optional[BuildResult] = None,
    ) -> Path:
        root = normalize_game_root(game_root)
        whgame = root / GAME_BIN_RELATIVE / "WHGame.dll"
        if not whgame.is_file():
            raise BuildToolError(
                "The selected game directory does not contain {}.".format(whgame)
            )

        audit_path = build_result.audit_path if build_result else None
        if audit_path is None or not audit_path.is_file():
            environment = self._environment_detector()
            build_dir = self.project_root / "out" / "build" / profile.key
            self._prepare_build_directory(build_dir, environment, log)
            self._configure(environment, build_dir, profile, log)
            self._run(
                [
                    environment.cmake,
                    "--build",
                    str(build_dir),
                    "--config",
                    profile.cmake_config,
                    "--target",
                    AUDIT_TARGET,
                    "--parallel",
                ],
                log,
            )
            audit_path = (
                build_dir / profile.cmake_config / "{}.exe".format(AUDIT_TARGET)
            )
        if not audit_path.is_file():
            raise BuildToolError(
                "Signature audit executable is missing: {}".format(audit_path)
            )

        self._run([str(audit_path), str(whgame)], log)
        return whgame

    def _configure(
        self,
        environment: BuildEnvironment,
        build_dir: Path,
        profile: BuildProfile,
        log: LogCallback,
    ) -> None:
        toolchain = self._ensure_vcpkg(log)
        self._run(
            [
                environment.cmake,
                "-S",
                str(self.project_root),
                "-B",
                str(build_dir),
                "-G",
                environment.generator,
                "-A",
                "x64",
                "-D",
                "FINAL={}".format("YES" if profile.final else "NO"),
                "-D",
                "BUILD_TESTING=ON",
                "-D",
                "CMAKE_TOOLCHAIN_FILE={}".format(toolchain),
                "-D",
                "VCPKG_TARGET_TRIPLET={}".format(VCPKG_TRIPLET),
                "-D",
                "VCPKG_OVERLAY_PORTS={}".format(
                    self.project_root / "cmake_scripts" / "vcpkg" / "ports"
                ),
            ],
            log,
        )

    def _ensure_vcpkg(self, log: LogCallback) -> Path:
        git = shutil.which("git")
        if not git:
            raise BuildToolError("Git was not found on PATH; it is required to bootstrap vcpkg.")

        root = self.project_root / ".cache" / "vcpkg"
        if not (root / ".git").is_dir():
            root.parent.mkdir(parents=True, exist_ok=True)
            self._run(
                [
                    git,
                    "clone",
                    "--filter=blob:none",
                    "--no-checkout",
                    VCPKG_REPOSITORY,
                    str(root),
                ],
                log,
            )

        try:
            current = _capture_text([git, "-C", str(root), "rev-parse", "HEAD"]).strip()
        except BuildToolError:
            current = ""
        if current != VCPKG_BASELINE:
            self._run(
                [git, "-C", str(root), "fetch", "origin", VCPKG_BASELINE],
                log,
            )
            self._run(
                [git, "-C", str(root), "checkout", "--detach", VCPKG_BASELINE],
                log,
            )

        executable = root / "vcpkg.exe"
        if not executable.is_file():
            bootstrap = root / "bootstrap-vcpkg.bat"
            if not bootstrap.is_file():
                raise BuildToolError("The pinned vcpkg checkout has no bootstrap-vcpkg.bat.")
            self._run([str(bootstrap), "-disableMetrics"], log)

        toolchain = root / "scripts" / "buildsystems" / "vcpkg.cmake"
        if not toolchain.is_file():
            raise BuildToolError("The vcpkg CMake toolchain is missing: {}".format(toolchain))
        return toolchain

    def _prepare_build_directory(
        self,
        build_dir: Path,
        environment: BuildEnvironment,
        log: LogCallback,
    ) -> None:
        """Recreate a profile when CMake's selected VS environment changed."""

        build_root = (self.project_root / "out" / "build").resolve()
        resolved_build_dir = build_dir.resolve(strict=False)
        if resolved_build_dir.parent != build_root:
            raise BuildToolError(
                "Refusing to manage a build directory outside {}: {}".format(
                    build_root, resolved_build_dir
                )
            )

        cache_path = resolved_build_dir / "CMakeCache.txt"
        cached = _read_cmake_cache(cache_path)
        incompatibilities: List[str] = []

        cached_generator = cached.get("CMAKE_GENERATOR")
        if cached_generator and cached_generator != environment.generator:
            incompatibilities.append(
                "generator changed from {!r} to {!r}".format(
                    cached_generator, environment.generator
                )
            )

        cached_platform = cached.get("CMAKE_GENERATOR_PLATFORM")
        if cached_platform and cached_platform.lower() != "x64":
            incompatibilities.append(
                "generator platform changed from {!r} to 'x64'".format(
                    cached_platform
                )
            )

        cached_source = cached.get("CMAKE_HOME_DIRECTORY")
        if cached_source:
            try:
                same_source = Path(cached_source).resolve(strict=False) == self.project_root
            except OSError:
                same_source = False
            if not same_source:
                incompatibilities.append("source directory changed")

        if cache_path.is_file():
            expected_toolchain = (
                self.project_root
                / ".cache"
                / "vcpkg"
                / "scripts"
                / "buildsystems"
                / "vcpkg.cmake"
            ).resolve(strict=False)
            cached_toolchain = cached.get("CMAKE_TOOLCHAIN_FILE")
            try:
                same_toolchain = (
                    cached_toolchain is not None
                    and Path(cached_toolchain).resolve(strict=False) == expected_toolchain
                )
            except OSError:
                same_toolchain = False
            if not same_toolchain:
                incompatibilities.append("vcpkg toolchain changed or was missing")

            cached_triplet = cached.get("VCPKG_TARGET_TRIPLET")
            if cached_triplet != VCPKG_TRIPLET:
                incompatibilities.append(
                    "vcpkg triplet changed from {!r} to {!r}".format(
                        cached_triplet, VCPKG_TRIPLET
                    )
                )

        if incompatibilities and resolved_build_dir.exists():
            log(
                "Build environment changed ({}). Recreating {}.".format(
                    "; ".join(incompatibilities), resolved_build_dir
                )
            )
            try:
                shutil.rmtree(resolved_build_dir, onerror=_remove_readonly)
            except OSError as exc:
                raise BuildToolError(
                    "Could not recreate the incompatible build directory {}: {}".format(
                        resolved_build_dir, exc
                    )
                ) from exc

        resolved_build_dir.mkdir(parents=True, exist_ok=True)

    def _run(self, command: Sequence[str], log: LogCallback) -> None:
        log("> {}".format(subprocess.list2cmdline(list(command))))
        creation_flags = getattr(subprocess, "CREATE_NO_WINDOW", 0)
        try:
            process = subprocess.Popen(
                list(command),
                cwd=str(self.project_root),
                stdout=subprocess.PIPE,
                stderr=subprocess.STDOUT,
                text=True,
                encoding="utf-8",
                errors="replace",
                bufsize=1,
                creationflags=creation_flags,
            )
        except OSError as exc:
            raise BuildToolError("Could not start command: {}".format(exc)) from exc

        assert process.stdout is not None
        for line in process.stdout:
            log(line.rstrip())
        return_code = process.wait()
        if return_code != 0:
            raise BuildToolError(
                "Command exited with code {}: {}".format(
                    return_code, subprocess.list2cmdline(list(command))
                )
            )


def _read_cmake_cache(path: Path) -> Dict[str, str]:
    if not path.is_file():
        return {}
    try:
        lines = path.read_text(encoding="utf-8", errors="replace").splitlines()
    except OSError:
        return {}

    values: Dict[str, str] = {}
    for line in lines:
        if not line or line.startswith(("#", "//")) or "=" not in line:
            continue
        key_with_type, value = line.split("=", 1)
        key = key_with_type.split(":", 1)[0]
        values[key] = value
    return values


def _remove_readonly(
    function: Callable[[str], None],
    path: str,
    error_info: Tuple[type, BaseException, Any],
) -> None:
    """Allow rmtree to remove read-only files created by FetchContent's Git."""

    error = error_info[1]
    if not isinstance(error, PermissionError):
        raise error
    os.chmod(path, stat.S_IWRITE)
    function(path)


def is_game_running() -> bool:
    if os.name != "nt":
        return False
    creation_flags = getattr(subprocess, "CREATE_NO_WINDOW", 0)
    try:
        completed = subprocess.run(
            ["tasklist", "/FI", "IMAGENAME eq {}".format(GAME_EXECUTABLE), "/FO", "CSV", "/NH"],
            check=False,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="replace",
            creationflags=creation_flags,
        )
    except OSError:
        return False
    return GAME_EXECUTABLE.lower() in completed.stdout.lower()


def deploy_artifacts(
    result: BuildResult,
    game_root: Path,
    process_checker: Callable[[], bool] = is_game_running,
) -> Path:
    normalized_root = normalize_game_root(game_root)
    destination = game_bin_dir(normalized_root)
    executable = destination / GAME_EXECUTABLE
    if not executable.is_file():
        raise BuildToolError("Deployment target does not contain {}: {}".format(GAME_EXECUTABLE, destination))

    artifacts = [
        result.dll_path,
        result.pdb_path,
        result.kcse_loader_path,
        result.kcse_loader_pdb_path,
        result.kcse_client_path,
        result.kcse_client_pdb_path,
        *result.address_library_paths,
    ]
    missing = [str(path) for path in artifacts if path is not None and not path.is_file()]
    if missing:
        raise BuildToolError("Cannot deploy missing build artifacts:\n{}".format("\n".join(missing)))
    if process_checker():
        raise BuildToolError(
            "{} is running. Close the game before deploying.".format(GAME_EXECUTABLE)
        )
    if result.kcse_loader_path is not None:
        if not result.address_library_paths:
            raise BuildToolError(
                "KCSE deployment is missing the bundled Address Library tables."
            )

    targets = [
        (result.pdb_path, destination / "d3d12.pdb"),
        (result.dll_path, destination / "d3d12.dll"),
    ]
    if result.kcse_loader_path is not None:
        targets.append((result.kcse_loader_path, destination / "dinput8.dll"))
    if result.kcse_loader_pdb_path is not None:
        targets.append((result.kcse_loader_pdb_path, destination / "dinput8.pdb"))
    plugin_destination = normalized_root / "mods" / "KCD2MP" / "KCSE" / "Plugins"
    if result.kcse_client_path is not None or result.kcse_client_pdb_path is not None:
        plugin_destination.mkdir(parents=True, exist_ok=True)
    if result.kcse_client_path is not None:
        targets.append(
            (result.kcse_client_path, plugin_destination / "KCD2MPKCSEClient.dll")
        )
    if result.kcse_client_pdb_path is not None:
        targets.append(
            (result.kcse_client_pdb_path, plugin_destination / "KCD2MPKCSEClient.pdb")
        )
    if result.address_library_paths:
        address_library_destination = normalized_root / "KCSE" / "addresslib"
        address_library_destination.mkdir(parents=True, exist_ok=True)
        targets.extend(
            (path, address_library_destination / path.name)
            for path in result.address_library_paths
        )
    temporary_paths: List[Path] = []
    try:
        for source, target in targets:
            temporary = target.with_name(target.name + ".kcd2mp.tmp")
            shutil.copy2(source, temporary)
            temporary_paths.append(temporary)
        for temporary, (_, target) in zip(temporary_paths, targets):
            os.replace(temporary, target)
    except PermissionError as exc:
        raise BuildToolError(
            "Windows refused to replace a KCD2MP/KCSE runtime file. "
            "Close the game and any debugger, then retry."
        ) from exc
    except OSError as exc:
        raise BuildToolError("Deployment failed: {}".format(exc)) from exc
    finally:
        for temporary in temporary_paths:
            try:
                temporary.unlink()
            except FileNotFoundError:
                pass
            except OSError:
                pass
    return destination


def _atomic_write_text(path: Path, content: str) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    file_descriptor, temporary_name = tempfile.mkstemp(
        prefix=path.name + ".", suffix=".tmp", dir=str(path.parent)
    )
    temporary_path = Path(temporary_name)
    try:
        with os.fdopen(file_descriptor, "w", encoding="utf-8", newline="\n") as stream:
            stream.write(content)
        os.replace(temporary_path, path)
    except Exception:
        try:
            temporary_path.unlink()
        except OSError:
            pass
        raise
