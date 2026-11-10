#include <Geode/Geode.hpp>
#include <Geode/modify/GJSongBrowser.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include "../MusicCleaner.hpp"

using namespace geode::prelude;

class $modify(MCGJSongBrowser, GJSongBrowser) {
    void onDeleteAll(CCObject* sender) {
        size_t selectedCount = MusicCleaner::getSelectedCount();

        if (selectedCount > 0) {
            std::vector<int> toDelete;
            uintmax_t totalBytes = 0;

            for (int id : MusicCleaner::getSelected()) {
                if (!MusicCleaner::isFavorite(id)) {
                    toDelete.push_back(id);
                    totalBytes += MusicCleaner::getSongFileSize(id);
                }
            }

            if (toDelete.empty()) {
                FLAlertLayer::create(
                    "Delete Songs",
                    "All selected songs are protected as <cy>Favorites</c>.\nNone will be deleted.",
                    "OK"
                )->show();
                return;
            }

            std::string sizeStr = MusicCleaner::formatBytes(totalBytes);
            std::string msg = fmt::format(
                "Are you sure you want to delete <cr>{}</c> selected song(s)?\n"
                "Total space to free: <cg>{}</c>",
                toDelete.size(), sizeStr
            );

            createQuickPopup(
                "Delete Selected Songs",
                msg,
                "Cancel", "Delete",
                [this, toDelete](FLAlertLayer*, bool btn2) {
                    if (!btn2) return;

                    uintmax_t freedBytes = 0;
                    MusicCleaner::deleteSongs(toDelete, &freedBytes);
                    MusicCleaner::clearSelection();

                    FMODAudioEngine::sharedEngine()->playEffect("deleteItem_001.ogg");

                    auto mdm = MusicDownloadManager::sharedState();
                    if (mdm) {
                        this->m_downloadedSongs = mdm->getDownloadedSongs();
                        int totalSongs = this->m_downloadedSongs ? this->m_downloadedSongs->count() : 0;
                        if (totalSongs == 0 || this->m_page * 10 >= totalSongs) {
                            this->loadPage(0);
                        } else {
                            this->loadPage(this->m_page);
                        }
                    }
                }
            );
        } else {
            auto mdm = MusicDownloadManager::sharedState();
            if (!mdm) return;
            auto downloaded = mdm->getDownloadedSongs();
            if (!downloaded || downloaded->count() == 0) {
                FLAlertLayer::create("Delete Songs", "No downloaded songs found.", "OK")->show();
                return;
            }

            std::vector<int> toDelete;
            uintmax_t totalBytes = 0;
            size_t favCount = 0;

            for (int i = 0; i < downloaded->count(); ++i) {
                auto song = static_cast<SongInfoObject*>(downloaded->objectAtIndex(i));
                if (!song) continue;
                int id = song->m_songID;
                if (MusicCleaner::isFavorite(id)) {
                    favCount++;
                } else {
                    toDelete.push_back(id);
                    totalBytes += MusicCleaner::getSongFileSize(id);
                }
            }

            if (toDelete.empty()) {
                FLAlertLayer::create(
                    "Protected Songs",
                    "All downloaded songs are marked as <cy>Favorites</c>.\n"
                    "No songs to delete.",
                    "OK"
                )->show();
                return;
            }

            std::string sizeStr = MusicCleaner::formatBytes(totalBytes);
            std::string msg = fmt::format(
                "Are you sure you want to delete <cr>{}</c> song(s)?\n"
                "<cy>{}</c> favorite song(s) will be kept.\n"
                "Total space to free: <cg>{}</c>",
                toDelete.size(), favCount, sizeStr
            );

            createQuickPopup(
                "Delete All Unstarred",
                msg,
                "Cancel", "Delete",
                [this, toDelete](FLAlertLayer*, bool btn2) {
                    if (!btn2) return;

                    uintmax_t freedBytes = 0;
                    MusicCleaner::deleteSongs(toDelete, &freedBytes);
                    MusicCleaner::clearSelection();

                    FMODAudioEngine::sharedEngine()->playEffect("deleteItem_001.ogg");

                    auto mdm = MusicDownloadManager::sharedState();
                    if (mdm) {
                        this->m_downloadedSongs = mdm->getDownloadedSongs();
                        this->loadPage(0);
                    }
                }
            );
        }
    }

    void exitLayer(CCObject* sender) {
        MusicCleaner::clearSelection();
        GJSongBrowser::exitLayer(sender);
    }
};
