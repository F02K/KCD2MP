#!/usr/bin/env python3
"""Verify that native REL::ID call sites are covered by every vendored table."""

from __future__ import annotations

import argparse
import re
import struct
import sys
from pathlib import Path
from typing import Dict, Iterable, Iterator, Set, Tuple

HEADER = struct.Struct("<4sIII")
RECORD = struct.Struct("<II")
MAGIC = b"KASL"
ADDRESS_LIBRARY_GLOB = "kcd_addresslib_*.bin"
TABLE_NAME_RE = re.compile(r"^kcd_addresslib_(steam|gog|epic)_(.+)\.bin$", re.IGNORECASE)
DISTRIBUTIONS = {"steam": 1, "gog": 2, "epic": 3}

CONST_RE = re.compile(
    r"\b(?:inline\s+)?(?:static\s+)?constexpr\b[^;=\n]*\b([A-Za-z_]\w*)\s*=\s*(\d+)\s*;"
)
CALL_RE = re.compile(r"\bREL::ID\(\s*([A-Za-z_]\w*|\d+)\s*\)")
DECL_RE = re.compile(r"::REL::ID\s+[A-Za-z_]\w*\s*\{\s*(\d+)\s*\}")


class AuditError(RuntimeError):
    pass


def _source_files(roots: Iterable[Path]) -> Iterator[Path]:
    for root in roots:
        if not root.is_dir():
            continue
        for path in root.rglob("*"):
            if path.suffix.lower() not in {".h", ".hpp", ".cpp", ".cc", ".cxx"}:
                continue
            # Generated RTTI/vtable catalogs are declarations, not native
            # resolution call sites. Audit executable call sites here; the
            # upstream generator owns full-catalog cross-distribution coverage.
            if "CryEngine" in path.parts or "Offsets" in path.parts:
                continue
            yield path


def collect_rel_ids(roots: Iterable[Path]) -> Tuple[Set[int], Dict[str, Set[Path]]]:
    used_ids: Set[int] = set()
    unresolved: Dict[str, Set[Path]] = {}

    for path in _source_files(roots):
        text = path.read_text(encoding="utf-8", errors="replace")
        constants = {name: int(value) for name, value in CONST_RE.findall(text)}
        used_ids.update(int(value) for value in DECL_RE.findall(text))

        for token in CALL_RE.findall(text):
            if token.isdigit():
                used_ids.add(int(token))
            elif token in constants:
                used_ids.add(constants[token])
            else:
                unresolved.setdefault(token, set()).add(path)

    return used_ids, unresolved


def audit_table(path: Path, required_ids: Set[int]) -> Tuple[int, Set[int]]:
    name_match = TABLE_NAME_RE.fullmatch(path.name)
    if name_match is None:
        raise AuditError(f"{path}: invalid Address Library filename")
    size = path.stat().st_size
    with path.open("rb") as stream:
        header = stream.read(HEADER.size)
        if len(header) != HEADER.size:
            raise AuditError(f"{path}: truncated KASL header")
        magic, format_version, distribution, count = HEADER.unpack(header)
        if magic != MAGIC:
            raise AuditError(f"{path}: invalid magic {magic!r}")
        if format_version == 0 or distribution not in {1, 2, 3} or count == 0:
            raise AuditError(
                f"{path}: invalid format/distribution {format_version}/{distribution}"
            )
        if distribution != DISTRIBUTIONS[name_match.group(1).lower()]:
            raise AuditError(f"{path}: filename/header distribution mismatch")
        expected_size = HEADER.size + count * RECORD.size
        if size != expected_size:
            raise AuditError(
                f"{path}: expected {expected_size} bytes for {count} records, got {size}"
            )

        found: Set[int] = set()
        for _ in range(count):
            record = stream.read(RECORD.size)
            address_id, _rva = RECORD.unpack(record)
            if address_id in required_ids:
                found.add(address_id)

    return count, required_ids - found


def run(project_root: Path) -> None:
    library_dir = (
        project_root
        / "vendor"
        / "Address-Library-For-KCSE"
        / "kcd2_address_library"
    )
    tables = sorted(library_dir.glob(ADDRESS_LIBRARY_GLOB))
    if not tables:
        raise AuditError(
            "Address Library submodule is not initialized; run tools/init_vendor.ps1"
        )

    source_roots = (
        project_root / "libKCD2" / "include",
        project_root / "libKCD2" / "src",
        project_root / "libKCD2" / "Projects" / "KCSE" / "src",
        project_root / "src" / "kcse",
    )
    required_ids, unresolved = collect_rel_ids(source_roots)
    if unresolved:
        details = ", ".join(
            f"{name} ({len(paths)} file(s))" for name, paths in sorted(unresolved.items())
        )
        raise AuditError(f"Could not resolve symbolic REL::ID constants: {details}")
    if not required_ids:
        raise AuditError("No native REL::ID usages were found")

    print(f"Found {len(required_ids)} native REL::ID values.")
    failures = []
    for table in tables:
        count, missing = audit_table(table, required_ids)
        print(f"{table.name}: {count} records, {len(missing)} required IDs missing")
        if missing:
            failures.append(f"{table.name}: {', '.join(map(str, sorted(missing)))}")
    if failures:
        raise AuditError("Address Library coverage failed:\n" + "\n".join(failures))


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--project-root",
        type=Path,
        default=Path(__file__).resolve().parents[1],
    )
    args = parser.parse_args()
    try:
        run(args.project_root.resolve())
    except (AuditError, OSError, struct.error) as exc:
        print(f"ERROR: {exc}", file=sys.stderr)
        return 1
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
