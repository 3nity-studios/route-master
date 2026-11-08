#!/usr/bin/env python3
"""
Route Master - SHA256 Checksums Generator
Calculates and writes SHA256 checksums for all release packages in a directory.
"""

import argparse
import hashlib
from pathlib import Path


def generate_sha256(file_path: Path) -> str:
    h = hashlib.sha256()
    with open(file_path, "rb") as f:
        while chunk := f.read(65536):
            h.update(chunk)
    return h.hexdigest()


def main():
    parser = argparse.ArgumentParser(description="Generate SHA256 checksums for release artifacts")
    parser.add_argument("--dist-dir", type=Path, default=Path("dist"), help="Directory containing release files")
    parser.add_argument("--output", type=Path, default=None, help="Output checksum file path")

    args = parser.parse_args()

    if not args.dist_dir.is_dir():
        raise FileNotFoundError(f"Directory not found: {args.dist_dir}")

    output_path = args.output or (args.dist_dir / "SHA256SUMS.txt")

    entries = []
    # Collect all release artifacts
    for item in sorted(args.dist_dir.iterdir()):
        if not item.is_file():
            continue
        if item.name.startswith("SHA256SUMS") or item.suffix in [".sha256", ".md5"]:
            continue
        # Skip temporary files or hidden files
        if item.name.startswith(".") or item.name.startswith("_"):
            continue

        sha256_hash = generate_sha256(item)
        entries.append(f"{sha256_hash}  {item.name}")

    if entries:
        content = "\n".join(entries) + "\n"
        output_path.write_text(content, encoding="utf-8")
        print(f"Generated checksums in {output_path}:")
        for entry in entries:
            print(f"  {entry}")
    else:
        print(f"No release artifacts found in {args.dist_dir} to hash.")


if __name__ == "__main__":
    main()
