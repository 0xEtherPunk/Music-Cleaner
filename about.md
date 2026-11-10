# Music Cleaner

A storage management utility for Geometry Dash (Geode SDK) designed to manage custom music downloads with persistent favorites protection and one-click batch cache cleanup.

## Key Features

- **Favorites Whitelist:** Star songs directly in the level widget or custom song browser. Favorited tracks are persistently saved and protected from deletion.
- **Batch Cache Purging:** Removes all unstarred downloaded custom songs in a single operation while synchronizing game state with `MusicDownloadManager`.
- **Per-Row Controls:** Each track row in the custom song browser features a favorite star toggle positioned directly above the trash can button.
- **Accidental Deletion Protection:** Favorited tracks cannot be deleted via the row trash button or batch delete unless first unstarred.
- **Storage Metrics:** Displays exact track count and disk space calculations (MB/GB) in confirmation dialogs before execution.
- **Saved Levels Integration:** Direct navigation button in the Saved Levels browser to open the custom songs browser.
- **In-Game Help & Credits:** Dedicated info button inside the song browser providing concise feature guidelines and author attribution.

## Access Points

1. **Saved Levels:** Navigation panel, beside the folder button.
2. **Custom Song Browser:** Standard custom song browser with star toggle above each row's trash button and a top-left info button.
3. **Level Page:** Compact star toggle integrated into the song audio widget.

Developer: 0xEtherPunk (GD: 0xPSP)
