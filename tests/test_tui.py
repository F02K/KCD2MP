from __future__ import annotations

import tempfile
import threading
import unittest
from pathlib import Path

try:
    from tools.build_tui.app import BuildApp
    from tools.build_tui.core import (
        BUILD_PROFILES,
        GAME_BIN_RELATIVE,
        BuildResult,
        BuildToolError,
        ConfigStore,
        GameLocation,
    )
except ImportError:
    BuildApp = None  # type: ignore[assignment]


@unittest.skipIf(BuildApp is None, "Textual is not installed")
class BuildAppTests(unittest.IsolatedAsyncioTestCase):
    async def test_build_flow_updates_status_and_log(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            game_root = root / "KingdomComeDeliverance2"
            game_bin = game_root / GAME_BIN_RELATIVE
            game_bin.mkdir(parents=True)
            (game_bin / "KingdomCome.exe").write_bytes(b"game")

            class FakeService:
                def build(self, profile, log):
                    artifact_dir = root / "artifacts"
                    artifact_dir.mkdir(exist_ok=True)
                    dll = artifact_dir / "d3d12_.dll"
                    pdb = artifact_dir / "d3d12_.pdb"
                    dll.write_bytes(b"dll")
                    pdb.write_bytes(b"pdb")
                    log("fake build output")
                    return BuildResult(profile, artifact_dir, dll, pdb)

            app = BuildApp(
                service=FakeService(),  # type: ignore[arg-type]
                config_store=ConfigStore(root / "build-tool.json"),
                location_resolver=lambda: GameLocation(game_root, "Test"),
            )
            async with app.run_test() as pilot:
                self.assertEqual(app._selected_profile(), BUILD_PROFILES["debug"])
                await pilot.click("#build")
                for _ in range(50):
                    await pilot.pause(0.02)
                    if "completed successfully" in str(app.query_one("#status").render()):
                        break

                status = str(app.query_one("#status").render())
                self.assertIn("Debug build completed successfully", status)
                self.assertFalse(app.query_one("#build").disabled)

    async def test_live_build_output_uses_full_log_view(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            game_root = root / "KingdomComeDeliverance2"
            game_bin = game_root / GAME_BIN_RELATIVE
            game_bin.mkdir(parents=True)
            (game_bin / "KingdomCome.exe").write_bytes(b"game")
            release_build = threading.Event()

            class SlowService:
                def build(self, profile, log):
                    artifact_dir = root / "artifacts"
                    artifact_dir.mkdir(exist_ok=True)
                    dll = artifact_dir / "d3d12_.dll"
                    pdb = artifact_dir / "d3d12_.pdb"
                    dll.write_bytes(b"dll")
                    pdb.write_bytes(b"pdb")
                    log("live compiler output")
                    release_build.wait(timeout=2)
                    return BuildResult(profile, artifact_dir, dll, pdb)

            app = BuildApp(
                service=SlowService(),  # type: ignore[arg-type]
                config_store=ConfigStore(root / "build-tool.json"),
                location_resolver=lambda: GameLocation(game_root, "Test"),
            )
            async with app.run_test() as pilot:
                await pilot.click("#build")
                for _ in range(50):
                    await pilot.pause(0.02)
                    if len(app.query_one("#log").lines) >= 2:
                        break

                self.assertTrue(app.query_one("#settings").has_class("build-active"))
                self.assertGreaterEqual(len(app.query_one("#log").lines), 2)
                self.assertIn(
                    "Live output is shown below",
                    str(app.query_one("#status").render()),
                )

                release_build.set()
                for _ in range(50):
                    await pilot.pause(0.02)
                    if not app.query_one("#settings").has_class("build-active"):
                        break
                self.assertFalse(app.query_one("#settings").has_class("build-active"))

    async def test_audit_action_streams_output(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            game_root = root / "KingdomComeDeliverance2"
            game_bin = game_root / GAME_BIN_RELATIVE
            game_bin.mkdir(parents=True)
            (game_bin / "KingdomCome.exe").write_bytes(b"game")
            (game_bin / "WHGame.dll").write_bytes(b"whgame")

            class FakeService:
                def audit(self, profile, selected_root, log, build_result=None):
                    self.profile = profile
                    self.root = selected_root
                    log("Signatures: 65/65 uniquely resolved")
                    log("Derived targets: 80/80 valid")

            service = FakeService()
            app = BuildApp(
                service=service,  # type: ignore[arg-type]
                config_store=ConfigStore(root / "build-tool.json"),
                location_resolver=lambda: GameLocation(game_root, "Test"),
            )
            async with app.run_test() as pilot:
                await pilot.click("#audit")
                for _ in range(50):
                    await pilot.pause(0.02)
                    if "completed successfully" in str(
                        app.query_one("#status").render()
                    ):
                        break

                self.assertEqual(service.profile, BUILD_PROFILES["debug"])
                self.assertEqual(service.root, game_root.resolve())
                self.assertIn(
                    "Signature audit completed successfully",
                    str(app.query_one("#status").render()),
                )

    async def test_failed_audit_prevents_deployment(self) -> None:
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            game_root = root / "KingdomComeDeliverance2"
            game_bin = game_root / GAME_BIN_RELATIVE
            game_bin.mkdir(parents=True)
            (game_bin / "KingdomCome.exe").write_bytes(b"game")
            (game_bin / "WHGame.dll").write_bytes(b"unsupported")

            class FailingAuditService:
                def build(self, profile, log):
                    artifact_dir = root / "artifacts"
                    artifact_dir.mkdir(exist_ok=True)
                    dll = artifact_dir / "d3d12_.dll"
                    pdb = artifact_dir / "d3d12_.pdb"
                    dll.write_bytes(b"dll")
                    pdb.write_bytes(b"pdb")
                    return BuildResult(profile, artifact_dir, dll, pdb)

                def audit(self, profile, selected_root, log, build_result=None):
                    log("ERROR: unsupported WHGame.dll")
                    raise BuildToolError("Signature audit failed.")

            app = BuildApp(
                service=FailingAuditService(),  # type: ignore[arg-type]
                config_store=ConfigStore(root / "build-tool.json"),
                location_resolver=lambda: GameLocation(game_root, "Test"),
            )
            async with app.run_test() as pilot:
                await pilot.click("#build-deploy")
                for _ in range(50):
                    await pilot.pause(0.02)
                    if "Signature audit failed" in str(
                        app.query_one("#status").render()
                    ):
                        break

                self.assertFalse((game_bin / "d3d12.dll").exists())
                self.assertIn(
                    "Signature audit failed",
                    str(app.query_one("#status").render()),
                )


if __name__ == "__main__":
    unittest.main()
