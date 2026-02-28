#!/usr/bin/env python3

import os
import re
import sys
from pathlib import Path


def generate_guard(filename: str) -> tuple[str, str]:
    """Generate header guard macro from filename."""
    # Convert to uppercase, replace non-alnum with _, prefix/suffix with __
    name = re.sub(r'[^a-zA-Z0-9]', '_', filename.upper())
    macro = f"__{name}_H__"
    return macro, f"//{macro}"


def add_header_guard_if_empty(hfile: Path):
    """Add header guard to empty .h file."""
    if hfile.stat().st_size != 0:
        return  # Skip non-empty

    macro, comment = generate_guard(hfile.stem)
    content = f"""#ifndef {macro}
#define {macro}

#endif {comment}
"""
    hfile.write_text(content)
    print(f"✅ Added guard to {hfile}")


def main(paths: list[Path]):
    """Scan paths recursively for empty .h files and add guards."""
    for path in paths:
        for hfile in path.rglob("*.h"):
            add_header_guard_if_empty(hfile)


if __name__ == "__main__":
    # Par défaut: cherche dans "../"
    if len(sys.argv) < 2:
        paths = [Path("../")]
        print("📁 Pas de chemin spécifié → utilisation de '../' par défaut")
    else:
        paths = [Path(arg) for arg in sys.argv[1:]]

    main(paths)
    print("🎉 Header guards ajoutés avec succès !")
