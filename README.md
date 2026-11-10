# Music Cleaner

| Parameter | Specification |
|:---|:---|
| **Author** | [0xEtherPunk](https://github.com/0xEtherPunk) |
| **Geometry Dash** | `0xPSP` |
| **Repository** | [Music-Cleaner](https://github.com/0xEtherPunk/Music-Cleaner) |
| **License** | [MIT](LICENSE) |
| **Compatibility** | Geode SDK v5.11.2 (Geometry Dash 2.2081) |

---

A high-performance storage management utility for Geometry Dash built on the Geode mod loader. Music Cleaner provides one-click batch cleanup of downloaded custom songs while protecting favorited tracks via a whitelist.

## Overview

Geometry Dash downloads custom audio files (from Newgrounds and the Music Library) directly to local storage. Over time, cached audio files accumulate and consume multiple gigabytes of disk space. Vanilla Geometry Dash provides no batch deletion mechanism, forcing players to delete tracks manually one by one.

**Music Cleaner** solves this problem by providing:
1. **Persistent Whitelist Protection:** Star tracks to prevent them from ever being purged.
2. **One-Click Batch Deletion:** Purge all non-favorited custom audio files in a single confirmed action.
3. **Accidental Deletion Safeguards:** Starred tracks cannot be deleted from individual rows or batch operations until unstarred.
4. **Filesystem Space Calculation:** Calculates and previews exact file count and storage space freed (MB/GB) before confirming.
5. **Ergonomic UI Integration:** Clean row actions aligned directly beneath playback controls, plus quick access from Saved Levels.

## Features

- **Whitelist Protection:** Star toggle buttons allow marking any song as a favorite. Starred IDs are saved in persistent mod storage.
- **Batch Cache Purging:** Removes all unstarred downloaded songs and immediately synchronizes `MusicDownloadManager` state and table views.
- **Space Calculation:** Inspects local file metadata to report exact track counts and megabytes to be reclaimed prior to execution.
- **Two-Step Confirmation:** Native alert dialog requiring explicit confirmation before deleting files.
- **Menu Music Preservation:** Restores ambient menu music seamlessly when closing the song browser.
- **Seamless UI Integration:**
  - **Custom Song Browser:** Row trash button and star button aligned horizontally directly under the playback button.
  - **Level View:** Compact star button placed beside the level download/delete action button.
  - **Saved Levels:** Direct navigation button in the Saved Levels browser opening the custom songs browser.
  - **Info Dialog:** Built-in information popup documenting core mechanics and developer credits.

## Interface Integration

The mod integrates directly into native Geometry Dash layers:
- **Saved Levels (`LevelBrowserLayer`):** Music icon button in the action menu opening `GJSongBrowser`.
- **Downloaded Songs Browser (`GJSongBrowser` & `CustomSongCell`):**
  - Star button positioned directly under the playback button.
  - Delete button positioned beside the star button on the same horizontal baseline.
  - Header info button (`(i)`) in the top-left of the dialog frame.
  - Delete All button executes batch purge of all unstarred tracks.
- **Level Overview (`CustomSongWidget`):** Compact star button integrated into the level audio widget with safety checks against accidental deletion.

## Project Structure

```
Music-Cleaner/
├── CMakeLists.txt                 # CMake configuration with automatic git tag versioning
├── mod.json                       # Geode mod manifest
├── logo.png                       # High-resolution mod icon with transparent rounded corners
├── LICENSE                        # MIT License
├── about.md                       # In-game Geode Index description
└── src/
    ├── main.cpp                   # Mod entrypoint and lifecycle events
    ├── MusicCleaner.hpp           # Core whitelist storage and filesystem calculation logic
    ├── MusicCleaner.cpp
    └── hooks/
        ├── CustomSongCellHook.cpp     # Track row favorite star and delete button alignment
        ├── CustomSongWidgetHook.cpp   # Level view compact star toggle and USE button suppression
        ├── GJSongBrowserHook.cpp      # Batch deletion and menu music restoration
        └── LevelBrowserLayerHook.cpp  # Saved Levels browser entrypoint button
```

## Building

### Requirements
- Geode CLI (`geode-cli 4.0.0+`)
- CMake 3.25+
- Clang 19+ (or platform toolchain)
- Geode SDK v5.11.2+

### Compilation
```bash
geode build -p windows
```

The build system automatically synchronizes `mod.json` version metadata with the latest Git tag (`git describe --tags --abbrev=0`).

### Installation
Copy the compiled `.geode` package into your Geometry Dash `geode/mods/` directory:
```bash
cp build-win/0xetherpunk.music-cleaner.geode "$STEAM_GD/geode/mods/"
```

## License

This project is licensed under the [MIT License](LICENSE).
