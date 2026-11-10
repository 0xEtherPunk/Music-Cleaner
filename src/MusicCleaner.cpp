#include "MusicCleaner.hpp"
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <filesystem>
#include <iomanip>
#include <sstream>

using namespace geode::prelude;

std::unordered_set<int> MusicCleaner::s_favorites;
bool MusicCleaner::s_loaded = false;

static std::filesystem::path toPath(const gd::string& str) {
#ifdef GEODE_IS_WINDOWS
    return std::filesystem::path(geode::utils::string::utf8ToWide(str.c_str()));
#else
    return std::filesystem::path(str.c_str());
#endif
}

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

uintmax_t MusicCleaner::getSongFileSize(int songID) {
    auto mdm = MusicDownloadManager::sharedState();
    if (!mdm || songID <= 0) return 0;

    gd::string songPath = mdm->pathForSong(songID);
    std::error_code ec;
    auto fsPath = toPath(songPath);
    if (std::filesystem::exists(fsPath, ec)) {
        auto sz = std::filesystem::file_size(fsPath, ec);
        return ec ? 0 : sz;
    }
    return 0;
}

bool MusicCleaner::deleteSingleSong(int songID, uintmax_t* outFreedBytes) {
    auto mdm = MusicDownloadManager::sharedState();
    if (!mdm || songID <= 0) return false;

    gd::string songPath = mdm->pathForSong(songID);
    std::error_code ec;
    auto fsPath = toPath(songPath);

    uintmax_t sz = 0;
    if (std::filesystem::exists(fsPath, ec)) {
        sz = std::filesystem::file_size(fsPath, ec);
        if (ec) sz = 0;
        std::filesystem::remove(fsPath, ec);
    }

    if (outFreedBytes) {
        *outFreedBytes = sz;
    }

    mdm->clearSong(songID);
    return true;
}

size_t MusicCleaner::deleteSongs(const std::vector<int>& songIDs, uintmax_t* outFreedBytes) {
    uintmax_t totalFreed = 0;
    size_t count = 0;

    for (int id : songIDs) {
        if (isFavorite(id)) continue;
        uintmax_t freed = 0;
        if (deleteSingleSong(id, &freed)) {
            totalFreed += freed;
            count++;
        }
    }

    if (outFreedBytes) {
        *outFreedBytes = totalFreed;
    }

    return count;
}

size_t MusicCleaner::deleteAllUnstarredSongs(uintmax_t* outFreedBytes) {
    auto mdm = MusicDownloadManager::sharedState();
    if (!mdm) return 0;
    auto downloaded = mdm->getDownloadedSongs();
    if (!downloaded) return 0;

    std::vector<int> toDelete;
    for (int i = 0; i < downloaded->count(); ++i) {
        auto song = static_cast<SongInfoObject*>(downloaded->objectAtIndex(i));
        if (!song) continue;
        int id = song->m_songID;
        if (!isFavorite(id)) {
            toDelete.push_back(id);
        }
    }

    size_t count = deleteSongs(toDelete, outFreedBytes);
    return count;
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
