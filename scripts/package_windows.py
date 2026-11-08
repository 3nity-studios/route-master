#!/usr/bin/env python3
"""
Route Master - Windows Packaging Script
Creates portable .zip and NSIS installer (.exe) packages.
"""

import argparse
import os
import shutil
import subprocess
import sys
import time
import zipfile
from pathlib import Path


def find_binary(build_dir: Path) -> Path:
    candidates = [
        build_dir / "bin" / "Release" / "route_master.exe",
        build_dir / "bin" / "route_master.exe",
        build_dir / "Release" / "route_master.exe",
        build_dir / "route_master.exe",
    ]
    for candidate in candidates:
        if candidate.is_file():
            return candidate.resolve()
    # Recursive search as fallback
    for p in build_dir.rglob("route_master.exe"):
        if p.is_file():
            return p.resolve()
    raise FileNotFoundError(f"Could not locate 'route_master.exe' binary in {build_dir}")


def find_makensis() -> str:
    makensis = shutil.which("makensis")
    if makensis:
        return makensis

    default_paths = [
        r"C:\Program Files (x86)\NSIS\makensis.exe",
        r"C:\Program Files\NSIS\makensis.exe",
        r"C:\ProgramData\chocolatey\bin\makensis.exe",
    ]
    for p in default_paths:
        if os.path.isfile(p):
            return p
    return None


def make_portable_zip(binary_path: Path, assets_dir: Path, source_dir: Path, display_version: str, output_dir: Path) -> Path:
    zip_name = f"RouteMaster-{display_version}-Windows-x86_64.zip"
    zip_path = output_dir / zip_name
    print(f"[1/2] Creating portable ZIP package: {zip_path}")

    with zipfile.ZipFile(zip_path, "w", compression=zipfile.ZIP_DEFLATED) as zf:
        # Binary
        zf.write(binary_path, arcname="RouteMaster/route_master.exe")

        # Assets
        for item in sorted(assets_dir.rglob("*")):
            if item.is_file():
                rel_path = item.relative_to(assets_dir).as_posix()
                arcname = f"RouteMaster/assets/{rel_path}"
                zf.write(item, arcname=arcname)

        # Empty data directory for runtime save games
        data_zinfo = zipfile.ZipInfo("RouteMaster/data/")
        data_zinfo.date_time = time.localtime(time.time())[:6]
        zf.writestr(data_zinfo, "")

        # README if available
        readme = source_dir / "README.md"
        if readme.is_file():
            zf.write(readme, arcname="RouteMaster/README.md")

    print(f"  -> Created {zip_path} ({zip_path.stat().st_size:,} bytes)")
    return zip_path


def make_nsis_installer(binary_path: Path, assets_dir: Path, source_dir: Path, version: str, display_version: str, output_dir: Path) -> Path:
    installer_name = f"RouteMaster-{display_version}-Windows-x86_64-Installer.exe"
    installer_path = output_dir / installer_name
    print(f"[2/2] Creating Windows Installer: {installer_path}")

    makensis = find_makensis()
    if not makensis:
        print("  [WARN] 'makensis' not found in PATH or standard locations; skipping installer creation.")
        return None

    icon_path = source_dir / "res" / "windows" / "app.ico"
    clean_ver = version.lstrip("v").split("-")[0]
    if not clean_ver or not clean_ver[0].isdigit():
        clean_ver = "0.0.1"

    # Prepare NSIS script
    staging_nsi = output_dir / "_installer.nsi"

    # Normalize paths with forward slashes for NSIS
    norm_bin = str(binary_path.resolve()).replace("\\", "/")
    norm_assets = str(assets_dir.resolve()).replace("\\", "/")
    norm_icon = str(icon_path.resolve()).replace("\\", "/") if icon_path.exists() else ""
    norm_out = str(installer_path.resolve()).replace("\\", "/")

    icon_directives = ""
    if norm_icon:
        icon_directives = f"""!define MUI_ICON "{norm_icon}"
!define MUI_UNICON "{norm_icon}"
"""

    nsis_content = f"""; Auto-generated NSIS installer script for Route Master
!include "MUI2.nsh"
!include "FileFunc.nsh"

Name "Route Master"
OutFile "{norm_out}"
InstallDir "$LOCALAPPDATA\\Programs\\Route Master"
InstallDirRegKey HKCU "Software\\Route Master" "InstallDir"
RequestExecutionLevel user

; Interface Settings
!define MUI_ABORTWARNING
{icon_directives}

; Pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!define MUI_FINISHPAGE_RUN "$INSTDIR\\route_master.exe"
!define MUI_FINISHPAGE_RUN_TEXT "Launch Route Master"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Languages
!insertmacro MUI_LANGUAGE "English"

Section "Route Master (required)" SecCore
    SectionIn RO
    SetOutPath "$INSTDIR"

    ; Main executable
    File "/oname=route_master.exe" "{norm_bin}"

    ; Copy game assets
    SetOutPath "$INSTDIR\\assets"
    File /r "{norm_assets}/*.*"

    ; Create data directory for user saves
    CreateDirectory "$INSTDIR\\data"

    ; Store install dir
    WriteRegStr HKCU "Software\\Route Master" "InstallDir" "$INSTDIR"

    ; Create uninstaller
    SetOutPath "$INSTDIR"
    WriteUninstaller "$INSTDIR\\Uninstall.exe"

    ; Create Shortcuts
    CreateDirectory "$SMPROGRAMS\\Route Master"
    CreateShortcut "$SMPROGRAMS\\Route Master\\Route Master.lnk" "$INSTDIR\\route_master.exe" "" "$INSTDIR\\route_master.exe" 0 "" "" "Route Master by Trinity Studios"
    CreateShortcut "$SMPROGRAMS\\Route Master\\Uninstall Route Master.lnk" "$INSTDIR\\Uninstall.exe"
    CreateShortcut "$DESKTOP\\Route Master.lnk" "$INSTDIR\\route_master.exe" "" "$INSTDIR\\route_master.exe" 0 "" "" "Route Master by Trinity Studios"

    ; Add/Remove Programs registry keys
    WriteRegStr HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster" "DisplayName" "Route Master"
    WriteRegStr HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster" "DisplayVersion" "{clean_ver}"
    WriteRegStr HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster" "Publisher" "Trinity Studios"
    WriteRegStr HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster" "DisplayIcon" "$INSTDIR\\route_master.exe"
    WriteRegStr HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster" "UninstallString" '"$INSTDIR\\Uninstall.exe"'
    WriteRegDWORD HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster" "NoModify" 1
    WriteRegDWORD HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster" "NoRepair" 1
SectionEnd

Section "Uninstall"
    RMDir /r "$INSTDIR\\assets"
    RMDir /r "$INSTDIR\\data"
    Delete "$INSTDIR\\route_master.exe"
    Delete "$INSTDIR\\Uninstall.exe"
    RMDir "$INSTDIR"

    Delete "$SMPROGRAMS\\Route Master\\Route Master.lnk"
    Delete "$SMPROGRAMS\\Route Master\\Uninstall Route Master.lnk"
    RMDir "$SMPROGRAMS\\Route Master"
    Delete "$DESKTOP\\Route Master.lnk"

    DeleteRegKey HKCU "Software\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\RouteMaster"
    DeleteRegKey HKCU "Software\\Route Master"
SectionEnd
"""

    staging_nsi.write_text(nsis_content, encoding="utf-8")

    cmd = [makensis, str(staging_nsi)]
    try:
        subprocess.check_call(cmd)
    finally:
        staging_nsi.unlink(missing_ok=True)

    print(f"  -> Created {installer_path} ({installer_path.stat().st_size:,} bytes)")
    return installer_path


def main():
    parser = argparse.ArgumentParser(description="Package Route Master for Windows")
    parser.add_argument("--build-dir", type=Path, default=Path("build"), help="Path to CMake build directory")
    parser.add_argument("--source-dir", type=Path, default=Path("."), help="Path to repository root")
    parser.add_argument("--version", type=str, default="0.0.1", help="Version string")
    parser.add_argument("--display-version", type=str, default=None, help="Display version string for filenames")
    parser.add_argument("--output-dir", type=Path, default=Path("dist"), help="Directory for output packages")

    args = parser.parse_args()

    display_version = args.display_version or f"v{args.version}" if not args.version.startswith("v") else args.version
    args.output_dir.mkdir(parents=True, exist_ok=True)

    binary_path = find_binary(args.build_dir)
    assets_dir = args.source_dir / "assets"
    if not assets_dir.is_dir():
        raise FileNotFoundError(f"Assets directory not found at {assets_dir}")

    print(f"Found binary: {binary_path}")
    print(f"Packaging Route Master version '{args.version}' (display: '{display_version}')...")

    zip_path = make_portable_zip(binary_path, assets_dir, args.source_dir, display_version, args.output_dir)
    installer_path = make_nsis_installer(binary_path, assets_dir, args.source_dir, args.version, display_version, args.output_dir)

    print("\nWindows packaging complete!")
    for p in [zip_path, installer_path]:
        if p and p.is_file():
            print(f"  - {p.name}")


if __name__ == "__main__":
    main()
