#include "MusicCleaner.hpp"
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/GameManager.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <filesystem>
#include <iomanip>
#include <sstream>

using namespace geode::prelude;

std::unordered_set<int> MusicCleaner::s_favorites;
bool MusicCleaner::s_loaded = false;

void MusicCleaner::init() {
    if (!s_loaded) {
        loadFavorites();
        s_loaded = true;
    }
}

void MusicCleaner::loadFavorites() {
    s_favorites.clear();
    auto saved = Mod::get()->getSavedValue<std::vector<int>>("favorite_songs");
    for (int id : saved) {
        s_favorites.insert(id);
    }
}

void MusicCleaner::saveFavorites() {
    std::vector<int> saved(s_favorites.begin(), s_favorites.end());
    Mod::get()->setSavedValue("favorite_songs", saved);
}

bool MusicCleaner::isFavorite(int songID) {
    init();
    return s_favorites.contains(songID);
}

void MusicCleaner::setFavorite(int songID, bool favorite) {
    init();
    if (favorite) {
        s_favorites.insert(songID);
    } else {
        s_favorites.erase(songID);
    }
    saveFavorites();
}

bool MusicCleaner::toggleFavorite(int songID) {
    init();
    bool newState = !isFavorite(songID);
    setFavorite(songID, newState);

    if (newState) {
        Notification::create("Added to Favorites", NotificationIcon::Success)->show();
    } else {
        Notification::create("Removed from Favorites", NotificationIcon::Info)->show();
    }

    return newState;
}

const std::unordered_set<int>& MusicCleaner::getFavorites() {
    init();
    return s_favorites;
}

size_t MusicCleaner::getFavoritesCount() {
    init();
    return s_favorites.size();
}

std::string MusicCleaner::formatBytes(uintmax_t bytes) {
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1);

    if (bytes >= 1024ULL * 1024ULL * 1024ULL) {
        double gb = static_cast<double>(bytes) / (1024.0 * 1024.0 * 1024.0);
        ss << gb << " GB";
    } else if (bytes >= 1024ULL * 1024ULL) {
        double mb = static_cast<double>(bytes) / (1024.0 * 1024.0);
        ss << mb << " MB";
    } else if (bytes >= 1024ULL) {
        double kb = static_cast<double>(bytes) / 1024.0;
        ss << kb << " KB";
    } else {
        ss << bytes << " B";
    }

    return ss.str();
}

SongStats MusicCleaner::calculateStats(bool protectActive) {
    init();
    SongStats stats;

    auto mdm = MusicDownloadManager::sharedState();
    if (!mdm) return stats;

    auto gm = GameManager::sharedState();
    int menuSongID = gm ? gm->m_customMenuSongID : 0;
    int practiceSongID = gm ? gm->m_customPracticeSongID : 0;

    auto downloaded = mdm->getDownloadedSongs();
    if (!downloaded) return stats;

    for (int i = 0; i < downloaded->count(); ++i) {
        auto song = static_cast<SongInfoObject*>(downloaded->objectAtIndex(i));
        if (!song) continue;

        stats.totalSongs++;

        int id = song->m_songID;
        gd::string songPath = mdm->pathForSong(id);

        uintmax_t fileSize = 0;
        std::error_code ec;
        auto fsPath = geode::utils::file::stringToPath(songPath);
        if (std::filesystem::exists(fsPath, ec)) {
            fileSize = std::filesystem::file_size(fsPath, ec);
            if (ec) fileSize = 0;
        }

        stats.totalBytes += fileSize;

        if (isFavorite(id)) {
            stats.favoriteSongs++;
        } else if (protectActive && (id == menuSongID || id == practiceSongID)) {
            stats.activeProtectedSongs++;
        } else {
            stats.songsToDelete++;
            stats.bytesToFree += fileSize;
        }
    }

    return stats;
}

size_t MusicCleaner::executeCleanup(bool protectActive, uintmax_t* outFreedBytes) {
    init();

    auto mdm = MusicDownloadManager::sharedState();
    if (!mdm) return 0;

    auto gm = GameManager::sharedState();
    int menuSongID = gm ? gm->m_customMenuSongID : 0;
    int practiceSongID = gm ? gm->m_customPracticeSongID : 0;

    auto downloaded = mdm->getDownloadedSongs();
    if (!downloaded) return 0;

    std::vector<int> toDelete;
    uintmax_t freedBytes = 0;

    for (int i = 0; i < downloaded->count(); ++i) {
        auto song = static_cast<SongInfoObject*>(downloaded->objectAtIndex(i));
        if (!song) continue;

        int id = song->m_songID;

        if (isFavorite(id)) continue;

        if (protectActive && (id == menuSongID || id == practiceSongID)) {
            continue;
        }

        toDelete.push_back(id);
    }

    for (int id : toDelete) {
        gd::string songPath = mdm->pathForSong(id);
        std::error_code ec;
        auto fsPath = geode::utils::file::stringToPath(songPath);

        if (std::filesystem::exists(fsPath, ec)) {
            auto sz = std::filesystem::file_size(fsPath, ec);
            if (!ec) freedBytes += sz;
            std::filesystem::remove(fsPath, ec);
        }

        mdm->clearSong(id);
    }

    if (outFreedBytes) {
        *outFreedBytes = freedBytes;
    }

    return toDelete.size();
}
