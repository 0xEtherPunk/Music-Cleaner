# Music Cleaner

A storage management utility for Geometry Dash built for the Geode mod loader. Music Cleaner provides fast batch cleanup of downloaded custom songs with persistent favorites protection and space calculation.

## Key Features

- **Favorites Whitelist:** Star songs directly in the level widget or custom song browser. Favorited tracks are persistently saved and protected from deletion.
- **Batch Cache Purging:** Removes all unstarred downloaded custom songs in a single operation while synchronizing game state with `MusicDownloadManager`.
- **Streamlined Cell UI:** Star toggle and trash button aligned side-by-side directly beneath the playback button.
- **Accidental Deletion Protection:** Favorited tracks cannot be deleted via the row trash button or batch delete unless first unstarred.
- **Storage Metrics:** Displays exact track count and disk space calculations (MB/GB) in confirmation dialogs before execution.
- **Saved Levels Integration:** Direct navigation button in the Saved Levels browser to open the custom songs browser.
- **Menu Music Preservation:** Automatically restores ambient menu music when closing the song browser.
- **In-Game Help & Credits:** Dedicated info button inside the song browser providing concise feature guidelines and author attribution.

## Access Points

1. **Saved Levels:** Navigation panel, beside the folder button.
2. **Custom Song Browser:** Standard custom song browser with star toggle beside each row's trash button, plus top-left info button.
3. **Level Page:** Compact star toggle integrated into the song audio widget.

Developer: 0xEtherPunk (GD: 0xPSP)
