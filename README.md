# Music Cleaner

A utility mod for Geometry Dash (Geode SDK) designed to manage local song cache storage and provide batch deletion with whitelist protection.

## Overview

Geometry Dash stores downloaded Newgrounds and Music Library audio assets locally. Over extended gameplay sessions, custom songs accumulate and consume significant disk storage. Vanilla Geometry Dash provides no mechanism for batch cleanup, requiring players to delete songs individually.

**Music Cleaner** addresses this limitation by introducing:
1. A favorites whitelist system to protect selected tracks from deletion.
2. A batch deletion mechanism that purges unstarred audio files while synchronizing game state with `MusicDownloadManager`.
3. Quick access buttons in the main menu and the Saved Levels browser.

## Features

- **Whitelist Protection:** Star songs to mark them as favorites. Favorited songs are stored in persistent mod storage and excluded from deletion routines.
- **Batch Deletion:** Removes all downloaded `.mp3` and `.ogg` files that are not explicitly favorited.
- **Active Track Safeguard:** Configurable protection for currently active custom Menu and Practice mode music (`GameManager::m_customMenuSongID` and `GameManager::m_customPracticeSongID`).
- **Disk Space Calculation:** Queries filesystem metrics to report exact file counts and reclaimed disk space prior to execution.
- **Confirmation Flow:** Two-step verification before irreversible deletion operations.
- **Deletion Safeguard:** Direct song deletion on level pages is intercepted for favorited songs to prevent accidental removal.

## Interface Integration

The mod adds navigation shortcuts to standard game layers:
- **Main Menu (`MenuLayer`):** Music icon button on the bottom bar, positioned adjacent to the Geode button.
- **Saved Levels Browser (`LevelBrowserLayer`):** Music icon button adjacent to the folder navigation controls.
- **Song Browser (`SongsLayer`):** "Clean All" action button on the top-right header.
- **Song Widget (`CustomSongWidget`):** Star toggle button placed beside playback controls across all song cell instances.

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
    ├── MusicCleanerPopup.hpp      # Cleanup interface and confirmation dialog
    ├── MusicCleanerPopup.cpp
    └── hooks/
        ├── MenuLayerHook.cpp          # Main menu entrypoint
        ├── LevelBrowserLayerHook.cpp  # Saved levels browser entrypoint
        ├── SongsLayerHook.cpp         # Saved songs layer integration
        └── CustomSongWidgetHook.cpp   # Song cell star button and deletion guard
```

## Building

### Requirements
- **Geode SDK:** v5.11.2 or later
- **CMake:** 3.25 or later
- **Clang:** 19.0+ (with `lld`)
- **Geode CLI:** v4.0.0 or later

### Local Build (Linux -> Windows x64 via Geode CLI)
Ensure `clang-20` and `lld-20` are installed:
```bash
sudo apt install clang-20 lld-20 llvm-20
```

Compile the mod package:
```bash
export CLANG_VER=20 LLVM_VER=20
geode build -p windows
```

The resulting package `0xetherpunk.music-cleaner.geode` is placed into the active profile's `geode/mods/` directory.

### Continuous Integration
The repository includes a GitHub Actions workflow (`.github/workflows/multi-platform.yml`) that automatically compiles release packages across Windows, macOS, Android (arm64/armv7), and iOS upon push.

## Authors

- **Developer:** 0xEtherPunk (GitHub: [@0xEtherPunk](https://github.com/0xEtherPunk))
- **Geometry Dash:** 0xPSP

## License

This project is licensed under the MIT License.
