#!/usr/bin/env python3
"""
Route Master - Linux Packaging Script
Creates portable .zip, Debian/Ubuntu .deb, and Arch Linux .pkg.tar.zst packages.
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
        build_dir / "bin" / "route_master",
        build_dir / "route_master",
        build_dir / "bin" / "Release" / "route_master",
    ]
    for candidate in candidates:
        if candidate.is_file():
            return candidate.resolve()
    # Search recursively as fallback
    for p in build_dir.rglob("route_master"):
        if p.is_file() and not p.name.endswith((".o", ".a", ".txt")):
            return p.resolve()
    raise FileNotFoundError(f"Could not locate 'route_master' binary in {build_dir}")


def make_portable_zip(binary_path: Path, assets_dir: Path, source_dir: Path, display_version: str, output_dir: Path) -> Path:
    zip_name = f"RouteMaster-{display_version}-Linux-x86_64.zip"
    zip_path = output_dir / zip_name
    print(f"[1/3] Creating portable ZIP package: {zip_path}")

    with zipfile.ZipFile(zip_path, "w", compression=zipfile.ZIP_DEFLATED) as zf:
        # Binary with 0o755 executable permissions
        bin_zinfo = zipfile.ZipInfo(f"RouteMaster/route_master")
        bin_zinfo.external_attr = (0o755 << 16) | 0o100000
        bin_zinfo.date_time = time.localtime(time.time())[:6]
        zf.writestr(bin_zinfo, binary_path.read_bytes())

        # Assets
        for item in sorted(assets_dir.rglob("*")):
            if item.is_file():
                rel_path = item.relative_to(assets_dir).as_posix()
                arcname = f"RouteMaster/assets/{rel_path}"
                zf.write(item, arcname=arcname)

        # Empty data directory for runtime save games
        data_zinfo = zipfile.ZipInfo("RouteMaster/data/")
        data_zinfo.external_attr = (0o755 << 16) | 0o040000
        data_zinfo.date_time = time.localtime(time.time())[:6]
        zf.writestr(data_zinfo, "")

        # README if available
        readme = source_dir / "README.md"
        if readme.is_file():
            zf.write(readme, arcname="RouteMaster/README.md")

    print(f"  -> Created {zip_path} ({zip_path.stat().st_size:,} bytes)")
    return zip_path


def create_launcher_script() -> str:
    return """#!/bin/sh
set -e

# Setup user data directory according to XDG base directory specification
USER_DATA_DIR="${XDG_DATA_HOME:-$HOME/.local/share}/route-master"
mkdir -p "$USER_DATA_DIR/data"

# Create symlink to read-only assets in user data directory if not present
if [ ! -e "$USER_DATA_DIR/assets" ]; then
    ln -s /opt/route-master/assets "$USER_DATA_DIR/assets"
fi

cd "$USER_DATA_DIR"
exec /opt/route-master/bin/route_master "$@"
"""


def create_desktop_file() -> str:
    return """[Desktop Entry]
Version=1.0
Type=Application
Name=Route Master
GenericName=Bus Transit Simulation
Comment=Route Master transit simulation by Trinity Studios
Exec=/usr/bin/route-master
Path=/opt/route-master
Icon=route-master
Terminal=false
Categories=Game;Simulation;
Keywords=game;simulation;bus;transit;
"""


def make_debian_package(binary_path: Path, assets_dir: Path, source_dir: Path, version: str, display_version: str, output_dir: Path) -> Path:
    deb_name = f"RouteMaster-{display_version}-Linux-x86_64.deb"
    deb_path = output_dir / deb_name
    print(f"[2/3] Creating Debian/Ubuntu package: {deb_path}")

    # Standardize Debian version format (lowercase, digits and dots, optional +suffix)
    clean_ver = version.lstrip("v").replace("-", "+")
    if not clean_ver or not clean_ver[0].isdigit():
        clean_ver = f"0.0.1+{clean_ver}" if clean_ver else "0.0.1"

    staging = output_dir / "_deb_staging"
    if staging.exists():
        shutil.rmtree(staging)

    opt_bin = staging / "opt" / "route-master" / "bin"
    opt_assets = staging / "opt" / "route-master" / "assets"
    usr_bin = staging / "usr" / "bin"
    usr_apps = staging / "usr" / "share" / "applications"
    usr_icons = staging / "usr" / "share" / "icons" / "hicolor" / "256x256" / "apps"
    debian_dir = staging / "DEBIAN"

    for d in [opt_bin, opt_assets, usr_bin, usr_apps, usr_icons, debian_dir]:
        d.mkdir(parents=True, exist_ok=True)

    # 1. Binary
    dest_binary = opt_bin / "route_master"
    shutil.copy2(binary_path, dest_binary)
    dest_binary.chmod(0o755)

    # 2. Assets
    shutil.copytree(assets_dir, opt_assets, dirs_exist_ok=True)

    # 3. Launcher wrapper
    launcher = usr_bin / "route-master"
    launcher.write_text(create_launcher_script(), encoding="utf-8")
    launcher.chmod(0o755)

    # 4. Desktop entry
    desktop = usr_apps / "route-master.desktop"
    desktop.write_text(create_desktop_file(), encoding="utf-8")
    desktop.chmod(0o644)

    # 5. Icon
    icon_src = source_dir / "res" / "windows" / "icon_256.png"
    if not icon_src.exists():
        icon_src = source_dir / "res" / "linux" / "sfml.png"
    if icon_src.exists():
        shutil.copy2(icon_src, usr_icons / "route-master.png")

    # 6. DEBIAN/control
    control_content = f"""Package: route-master
Version: {clean_ver}
Section: games
Priority: optional
Architecture: amd64
Maintainer: Trinity Studios <route-master@users.noreply.github.com>
Depends: libc6 (>= 2.31), libx11-6, libxrandr2, libxcursor1, libxi6, libgl1, libudev1, libfreetype6, libflac8 | libflac12 | libflac-dev, libvorbis0a, libvorbisfile3
Description: Route Master by Trinity Studios
 Route Master is a bus transit simulation game.
 Manage routes, buses, employees, and build a thriving transit system.
"""
    (debian_dir / "control").write_text(control_content, encoding="utf-8")

    # Build .deb with dpkg-deb
    dpkg_deb = shutil.which("dpkg-deb")
    if not dpkg_deb:
        print("  [WARN] 'dpkg-deb' not found in PATH; skipping .deb creation")
        shutil.rmtree(staging, ignore_errors=True)
        return None

    cmd = ["dpkg-deb", "--build", "--root-owner-group", str(staging), str(deb_path)]
    subprocess.check_call(cmd)
    shutil.rmtree(staging, ignore_errors=True)

    print(f"  -> Created {deb_path} ({deb_path.stat().st_size:,} bytes)")
    return deb_path


def make_arch_package(binary_path: Path, assets_dir: Path, source_dir: Path, version: str, display_version: str, output_dir: Path) -> Path:
    arch_name = f"route-master-{display_version}-x86_64.pkg.tar.zst"
    arch_path = output_dir / arch_name
    print(f"[3/3] Creating Arch Linux package: {arch_path}")

    # Arch pkgver must be alphanumeric + dots/underscores, no hyphens
    clean_ver = version.lstrip("v").replace("-", ".")
    if not clean_ver or not clean_ver[0].isalnum():
        clean_ver = "0.0.1"

    staging = output_dir / "_arch_staging"
    if staging.exists():
        shutil.rmtree(staging)

    opt_bin = staging / "opt" / "route-master" / "bin"
    opt_assets = staging / "opt" / "route-master" / "assets"
    usr_bin = staging / "usr" / "bin"
    usr_apps = staging / "usr" / "share" / "applications"
    usr_icons = staging / "usr" / "share" / "icons" / "hicolor" / "256x256" / "apps"

    for d in [opt_bin, opt_assets, usr_bin, usr_apps, usr_icons]:
        d.mkdir(parents=True, exist_ok=True)

    # 1. Binary
    dest_binary = opt_bin / "route_master"
    shutil.copy2(binary_path, dest_binary)
    dest_binary.chmod(0o755)

    # 2. Assets
    shutil.copytree(assets_dir, opt_assets, dirs_exist_ok=True)

    # 3. Launcher wrapper
    launcher = usr_bin / "route-master"
    launcher.write_text(create_launcher_script(), encoding="utf-8")
    launcher.chmod(0o755)

    # 4. Desktop entry
    desktop = usr_apps / "route-master.desktop"
    desktop.write_text(create_desktop_file(), encoding="utf-8")
    desktop.chmod(0o644)

    # 5. Icon
    icon_src = source_dir / "res" / "windows" / "icon_256.png"
    if not icon_src.exists():
        icon_src = source_dir / "res" / "linux" / "sfml.png"
    if icon_src.exists():
        shutil.copy2(icon_src, usr_icons / "route-master.png")

    # Calculate installed size in bytes
    total_size = sum(f.stat().st_size for f in staging.rglob("*") if f.is_file())

    # 6. .PKGINFO
    pkginfo_content = f"""# Generated by route-master packaging script
pkgname = route-master
pkgbase = route-master
pkgver = {clean_ver}-1
pkgdesc = Route Master bus transit simulation by Trinity Studios
url = https://github.com/3nity-studios/route-master
builddate = {int(time.time())}
packager = Trinity Studios <route-master@users.noreply.github.com>
size = {total_size}
arch = x86_64
license = MIT
depend = glibc
depend = libx11
depend = libxrandr
depend = libxcursor
depend = libxi
depend = mesa
depend = systemd-libs
depend = freetype2
depend = flac
depend = libvorbis
"""
    (staging / ".PKGINFO").write_text(pkginfo_content, encoding="utf-8")

    # 7. .BUILDINFO
    buildinfo_content = f"""format = 2
pkgname = route-master
pkgbase = route-master
pkgver = {clean_ver}-1
pkgarch = x86_64
pkgbuild_sha256sum = 0000000000000000000000000000000000000000000000000000000000000000
packager = Trinity Studios <route-master@users.noreply.github.com>
builddate = {int(time.time())}
buildenv = !distcc !color !ccache check !sign
"""
    (staging / ".BUILDINFO").write_text(buildinfo_content, encoding="utf-8")

    # Package into .tar.zst using tar --zstd
    tar_cmd = shutil.which("tar")
    if not tar_cmd:
        print("  [WARN] 'tar' not found in PATH; skipping Arch package creation")
        shutil.rmtree(staging, ignore_errors=True)
        return None

    cmd = [
        tar_cmd,
        "--zstd",
        "-cf",
        str(arch_path.resolve()),
        "-C",
        str(staging.resolve()),
        ".PKGINFO",
        ".BUILDINFO",
        "opt",
        "usr",
    ]
    subprocess.check_call(cmd)
    shutil.rmtree(staging, ignore_errors=True)

    print(f"  -> Created {arch_path} ({arch_path.stat().st_size:,} bytes)")
    return arch_path


def main():
    parser = argparse.ArgumentParser(description="Package Route Master for Linux")
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
    deb_path = make_debian_package(binary_path, assets_dir, args.source_dir, args.version, display_version, args.output_dir)
    arch_path = make_arch_package(binary_path, assets_dir, args.source_dir, args.version, display_version, args.output_dir)

    print("\nLinux packaging complete!")
    for p in [zip_path, deb_path, arch_path]:
        if p and p.is_file():
            print(f"  - {p.name}")


if __name__ == "__main__":
    main()
