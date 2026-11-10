# Music Cleaner

A utility mod for Geometry Dash (Geode SDK) designed to manage local song cache storage and provide selective or batch deletion with whitelist protection.

## Overview

Geometry Dash stores downloaded Newgrounds and Music Library audio assets locally. Over extended gameplay sessions, custom songs accumulate and consume significant disk storage. Vanilla Geometry Dash provides no mechanism for batch cleanup, requiring players to delete songs individually.

**Music Cleaner** addresses this limitation by introducing:
1. A favorites whitelist system to protect selected tracks from deletion.
2. Checkbox selection on each song entry for selective batch removal.
3. Smart batch deletion: purging either selected tracks or all unstarred tracks.
4. Quick access button in the Saved Levels browser opening the custom songs browser.

## Features

- **Whitelist Protection:** Star songs to mark them as favorites. Favorited songs are stored in persistent mod storage and excluded from deletion routines.
- **Selective & Batch Deletion:**
  - When $\ge 1$ checkboxes are selected: clicking the delete-all button purges only the marked tracks (excluding favorites).
  - When 0 checkboxes are selected: clicking the delete-all button purges all downloaded custom tracks except favorites.
- **Accidental Deletion Safeguards:** Favorited songs cannot be deleted via the row trash button or batch delete unless first unstarred.
- **Disk Space Calculation:** Queries filesystem metrics to report exact file counts and reclaimed disk space prior to execution.
- **Two-Step Confirmation:** Native alert dialogs displaying file counts and freed storage before proceeding with file deletion.
- **Seamless UI Integration:**
  - Row controls: Checkbox, Star button, and Trash button arranged side-by-side.
  - Level view: Compact star toggle on the level song widget.
  - Navigation: Quick-access button in the Saved Levels browser.

## Interface Integration

The mod integrates directly into standard game layers:
- **Saved Levels Browser (`LevelBrowserLayer`):** Music icon button adjacent to the folder navigation controls, opening the custom songs browser (`GJSongBrowser`).
- **Custom Song Browser (`GJSongBrowser` & `CustomSongCell`):**
  - Left of each row's trash button: favorite star toggle (`GJ_starBtn_001.png` / `GJ_starBtn2_001.png`).
  - Left of the star toggle: multi-select checkbox (`GJ_checkOn_001.png` / `GJ_checkOff_001.png`).
  - Header delete-all button: initiates smart deletion based on active checkbox state.
- **Song Widget (`CustomSongWidget`):** Compact star toggle button placed beside playback controls on level overview pages.

## Project Structure

```
Music-Cleaner/
├── CMakeLists.txt                 # CMake configuration targeting Geode SDK
├── mod.json                       # Geode mod manifest and metadata
├── about.md                       # Description rendered in Geode Index
├── changelog.md                   # Version history
├── .github/
│   └── workflows/
│       └── multi-platform.yml    # CI/CD multi-platform build workflow
└── src/
    ├── main.cpp                   # Mod entrypoint and lifecycle events
    ├── MusicCleaner.hpp           # Core storage and calculation logic
    ├── MusicCleaner.cpp
    └── hooks/
        ├── CustomSongCellHook.cpp     # Track row checkbox and star controls
        ├── CustomSongWidgetHook.cpp   # Level view compact star toggle
        ├── GJSongBrowserHook.cpp      # Smart batch deletion logic
        └── LevelBrowserLayerHook.cpp  # Saved levels browser entrypoint
```

## Building

### Requirements
- Geode CLI (`geode-cli`)
- CMake 3.25+
- Clang 19+ (or MSVC / GCC according to platform target)
- Geode SDK v5.11.2+

### Compilation
```bash
geode build -p windows
```

### Installation
Copy the generated `.geode` package from your build directory into the Geometry Dash `geode/mods/` directory:
```bash
cp build-win/0xetherpunk.music-cleaner.geode "$STEAM_GD/geode/mods/"
```

## License

This project is licensed under the MIT License.
