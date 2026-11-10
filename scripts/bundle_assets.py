import os
import zipfile
import shutil

root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
build_pkg = os.path.join(root_dir, "build-win", "0xetherpunk.music-cleaner.geode")
steam_pkg = os.path.expanduser("~/.local/share/Steam/steamapps/common/Geometry Dash/geode/mods/0xetherpunk.music-cleaner.geode")
unzipped_dir = os.path.expanduser("~/.local/share/Steam/steamapps/common/Geometry Dash/geode/unzipped/0xetherpunk.music-cleaner")

assets = [
    "saved-levels-btn.png",
    "song-browser.png",
    "info-popup.png",
    "delete-confirm-popup.png",
]

def add_assets_to_zip(zip_path):
    if not os.path.exists(zip_path):
        return
    with zipfile.ZipFile(zip_path, "a") as z:
        existing = z.namelist()
        for f in assets:
            src = os.path.join(root_dir, f)
            if os.path.exists(src):
                # Add at root
                if f not in existing:
                    z.write(src, f)
                # Add in resources/0xetherpunk.music-cleaner/
                rel_res = f"resources/0xetherpunk.music-cleaner/{f}"
                if rel_res not in existing:
                    z.write(src, rel_res)

# Process build package
if os.path.exists(build_pkg):
    add_assets_to_zip(build_pkg)

# Copy to steam package
if os.path.exists(build_pkg):
    os.makedirs(os.path.dirname(steam_pkg), exist_ok=True)
    shutil.copy2(build_pkg, steam_pkg)

if os.path.exists(steam_pkg):
    add_assets_to_zip(steam_pkg)

# Update unzipped directory for live game runtime
if os.path.exists(steam_pkg):
    if os.path.exists(unzipped_dir):
        shutil.rmtree(unzipped_dir)
    os.makedirs(unzipped_dir, exist_ok=True)
    with zipfile.ZipFile(steam_pkg, "r") as z:
        z.extractall(unzipped_dir)
    print(f"Extracted latest mod contents into: {unzipped_dir}")
