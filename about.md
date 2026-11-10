# Music Cleaner

A storage management utility for Geometry Dash (Geode SDK) designed to manage custom music downloads with selective checkbox purging and persistent favorites protection.

## Key Features

- **Favorites Whitelist:** Star songs directly in the level widget or custom song browser. Favorited tracks are persistently saved and protected from deletion.
- **Smart Batch Deletion:**
  - With checkboxes marked: deletes only the selected tracks (excluding any favorited).
  - With zero checkboxes marked: deletes all downloaded custom tracks except favorites.
- **Per-Row Controls:** Each track row in the custom song browser features a selection checkbox, a favorite star toggle, and an individual delete button.
- **Accidental Deletion Protection:** Favorited tracks are guarded against both individual and batch deletion.
- **Storage Metrics:** Displays exact track count and disk space calculations (MB/GB) in confirmation dialogs before execution.
- **Saved Levels Integration:** Direct navigation button in the Saved Levels browser to quickly open the custom songs manager.

## Access Points

1. **Saved Levels:** Navigation panel, beside the folder button.
2. **Custom Song Browser:** Standard custom song browser with per-row selection checkboxes, star toggles, and guarded delete actions.
3. **Level Page:** Compact star toggle integrated into the song audio widget.

Developer: 0xEtherPunk (GD: 0xPSP)
