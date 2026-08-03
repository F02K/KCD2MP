"""Create KCD2MP client, server, test, and release-ZIP packages."""

from __future__ import annotations

import argparse
from pathlib import Path
from typing import Optional, Sequence

from build_tui.core import (
    BUILD_PROFILES,
    BuildResult,
    BuildToolError,
    discover_address_libraries,
    package_artifacts,
)


def _artifact(build_dir: Path, config: str, name: str) -> Path:
    candidates = (build_dir / config / name, build_dir / name)
    for candidate in candidates:
        if candidate.is_file():
            return candidate
    matches = tuple(
        path
        for path in build_dir.rglob(name)
        if "_deps" not in path.parts and path.is_file()
    )
    if len(matches) == 1:
        return matches[0]
    if not matches:
        raise BuildToolError("Build artifact is missing: {}".format(name))
    raise BuildToolError(
        "Build artifact is ambiguous: {}\n{}".format(
            name, "\n".join(str(path) for path in matches)
        )
    )


def _address_libraries(build_dir: Path, config: str):
    candidates = (
        build_dir / config / "KCSE" / "addresslib",
        build_dir / "KCSE" / "addresslib",
    )
    for candidate in candidates:
        if candidate.is_dir():
            return discover_address_libraries(candidate)
    raise BuildToolError("Bundled Address Library output was not found.")


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--build-dir", type=Path, required=True)
    parser.add_argument("--config", default="RelWithDebInfo")
    parser.add_argument("--project-root", type=Path, default=Path(__file__).parents[1])
    parser.add_argument("--output", type=Path)
    options = parser.parse_args(argv)

    project_root = options.project_root.resolve()
    build_dir = options.build_dir.resolve()
    profile = BUILD_PROFILES[
        "debug" if options.config.lower() == "debug" else "release"
    ]
    result = BuildResult(
        profile=profile,
        build_dir=build_dir,
        dll_path=_artifact(build_dir, options.config, "d3d12_.dll"),
        pdb_path=_artifact(build_dir, options.config, "d3d12_.pdb"),
        audit_path=_artifact(build_dir, options.config, "KCD2MPSignatureAudit.exe"),
        server_path=_artifact(build_dir, options.config, "KCD2MPServer.exe"),
        kcse_loader_path=_artifact(build_dir, options.config, "dinput8.dll"),
        kcse_loader_pdb_path=_artifact(build_dir, options.config, "dinput8.pdb"),
        kcse_client_path=_artifact(
            build_dir, options.config, "KCD2MPKCSEClient.dll"
        ),
        kcse_client_pdb_path=_artifact(
            build_dir, options.config, "KCD2MPKCSEClient.pdb"
        ),
        address_library_paths=_address_libraries(build_dir, options.config),
    )
    package = package_artifacts(
        result,
        project_root,
        options.output.resolve() if options.output is not None else None,
    )
    print(package.root)
    print(package.client_zip)
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except BuildToolError as exception:
        raise SystemExit("ERROR: {}".format(exception)) from exception
