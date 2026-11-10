#pragma once

#include <Geode/Geode.hpp>
#include <unordered_set>
#include <vector>
#include <string>
#include <cstdint>

class MusicCleaner {
public:
    static void init();

    // Favorites whitelist management
    static bool isFavorite(int songID);
    static void setFavorite(int songID, bool favorite);
    static bool toggleFavorite(int songID);
    static const std::unordered_set<int>& getFavorites();
    static size_t getFavoritesCount();

    // File operations and space metrics
    static uintmax_t getSongFileSize(int songID);
    static bool deleteSingleSong(int songID, uintmax_t* outFreedBytes = nullptr);
    static size_t deleteSongs(const std::vector<int>& songIDs, uintmax_t* outFreedBytes = nullptr);
    static size_t deleteAllUnstarredSongs(uintmax_t* outFreedBytes = nullptr);

    static std::string formatBytes(uintmax_t bytes);

private:
    static std::unordered_set<int> s_favorites;
    static bool s_loaded;

    static void loadFavorites();
    static void saveFavorites();
};
