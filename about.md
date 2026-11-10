# Music Cleaner

A storage management utility for Geometry Dash (Geode SDK) designed to manage custom music downloads with persistent favorites protection, search filtering, and one-click batch cache cleanup.

## Key Features

- **Favorites Whitelist:** Star songs directly in the level widget or custom song browser. Favorited tracks are persistently saved and protected from deletion.
- **Search Filtering:** Dedicated search button opening an input dialog to search downloaded tracks by title, artist name, or song ID.
- **Batch Cache Purging:** Removes all unstarred downloaded custom songs in a single operation while synchronizing game state with `MusicDownloadManager`.
- **Streamlined Cell UI:** Replaces the editor USE button with the favorite star toggle, placed neatly beside the trash button on the bottom row.
- **Accidental Deletion Protection:** Favorited tracks cannot be deleted via the row trash button or batch delete unless first unstarred.
- **Storage Metrics:** Displays exact track count and disk space calculations (MB/GB) in confirmation dialogs before execution.
- **Saved Levels Integration:** Direct navigation button in the Saved Levels browser to open the custom songs browser.
- **In-Game Help & Credits:** Dedicated info button inside the song browser providing concise feature guidelines and author attribution.

## Access Points

1. **Saved Levels:** Navigation panel, beside the folder button.
2. **Custom Song Browser:** Standard custom song browser with star toggle beside each row's trash button, plus top-left info and search buttons.
3. **Level Page:** Compact star toggle integrated into the song audio widget.

Developer: 0xEtherPunk (GD: 0xPSP)
