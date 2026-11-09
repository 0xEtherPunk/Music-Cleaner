#pragma once

#include <Geode/Geode.hpp>
#include <unordered_set>
#include <vector>
#include <string>

struct SongStats {
    size_t totalSongs = 0;
    size_t favoriteSongs = 0;
    size_t activeProtectedSongs = 0;
    size_t songsToDelete = 0;
    uintmax_t totalBytes = 0;
    uintmax_t bytesToFree = 0;
};

class MusicCleaner {
public:
    static void init();

    static bool isFavorite(int songID);
    static void setFavorite(int songID, bool favorite);
    static bool toggleFavorite(int songID);

    static const std::unordered_set<int>& getFavorites();
    static size_t getFavoritesCount();

    static SongStats calculateStats(bool protectActive = true);
    static size_t executeCleanup(bool protectActive = true, uintmax_t* outFreedBytes = nullptr);

    static std::string formatBytes(uintmax_t bytes);

private:
    static std::unordered_set<int> s_favorites;
    static bool s_loaded;

    static void loadFavorites();
    static void saveFavorites();
};
