# Music Cleaner

A utility mod for Geometry Dash (Geode SDK) designed to manage local song cache storage and provide batch deletion with whitelist protection.

## Overview

Geometry Dash stores downloaded Newgrounds and Music Library audio assets locally. Over extended gameplay sessions, custom songs accumulate and consume significant disk storage. Vanilla Geometry Dash provides no mechanism for batch cleanup, requiring players to delete songs individually.

**Music Cleaner** addresses this limitation by introducing:
1. A favorites whitelist system to protect selected tracks from deletion.
2. In-browser search filtering by song title, artist, or song ID.
3. Direct batch deletion purging all unstarred custom audio files in a single action.
4. Quick access button in the Saved Levels browser opening the custom songs browser.
5. Built-in info dialog explaining features and developer credits.

## Features

- **Whitelist Protection:** Star songs to mark them as favorites. Favorited songs are stored in persistent mod storage and excluded from deletion routines.
- **Search Filtering:** Dedicated search button opening an input dialog to search downloaded tracks by title, artist, or song ID without cluttering list space.
- **Batch Deletion:** Removes all downloaded custom songs except those marked as favorites.
- **Accidental Deletion Safeguards:** Favorited songs cannot be deleted via the row trash button or batch delete unless first unstarred.
- **Disk Space Calculation:** Queries filesystem metrics to report exact file counts and reclaimed disk space prior to execution.
- **Two-Step Confirmation:** Native alert dialogs displaying file counts and freed storage before proceeding with file deletion.
- **Seamless UI Integration:**
  - Row controls: Replaces the redundant USE button with the favorite star toggle, placed neatly beside the trash can button.
  - Level view: Compact star toggle on the level song widget, perfectly offset from the download/delete action button.
  - Navigation: Quick-access button in the Saved Levels browser.
  - Header controls: Info button and search magnifying glass located in the top-left corner of the custom songs browser.

## Interface Integration

The mod integrates directly into standard game layers:
- **Saved Levels Browser (`LevelBrowserLayer`):** Music icon button adjacent to folder navigation, opening the custom songs browser (`GJSongBrowser`).
- **Custom Song Browser (`GJSongBrowser` & `CustomSongCell`):**
  - Beside each row's trash button: favorite star toggle (`GJ_starBtn_001.png` / `GJ_starBtn2_001.png`).
  - Top-left header: info button (`GJ_infoBtn_001.png`) and search button (`gj_findBtn_001.png`).
  - Delete-all button: initiates batch deletion of all non-favorited tracks with space calculation.
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
        ├── CustomSongCellHook.cpp     # Track row star button in place of USE button
        ├── CustomSongWidgetHook.cpp   # Level view compact star toggle
        ├── GJSongBrowserHook.cpp      # Batch deletion, search filter, and info button
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
