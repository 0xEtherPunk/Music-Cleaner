import os
import zipfile

root_dir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
build_pkg = os.path.join(root_dir, "build-win", "0xetherpunk.music-cleaner.geode")

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

if os.path.exists(build_pkg):
    add_assets_to_zip(build_pkg)
