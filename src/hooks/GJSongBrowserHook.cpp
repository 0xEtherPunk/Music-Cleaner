#include <Geode/Geode.hpp>
#include <Geode/modify/GJSongBrowser.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/GameManager.hpp>
#include "../MusicCleaner.hpp"

using namespace geode::prelude;

class $modify(MCGJSongBrowser, GJSongBrowser) {
    void customSetup() {
        GJSongBrowser::customSetup();

        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GJ_GameSheet03.plist");

        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
        if (!infoSpr) {
            infoSpr = ButtonSprite::create("i", "goldFont.fnt", "GJ_button_02.png", 0.8f);
        } else {
            infoSpr->setScale(0.7f);
        }

        auto infoBtn = CCMenuItemSpriteExtra::create(
            infoSpr, this, menu_selector(MCGJSongBrowser::onInfo)
        );
        infoBtn->setID("music-cleaner-info-btn"_spr);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setID("music-cleaner-header-menu"_spr);
        menu->setPosition(CCPoint(0.f, 0.f));
        infoBtn->setPosition(CCPoint(winSize.width / 2.f - 165.f, winSize.height / 2.f + 128.f));
        menu->addChild(infoBtn);

        if (m_mainLayer) {
            m_mainLayer->addChild(menu, 100);
        }
    }

    void onInfo(CCObject* sender) {
        FLAlertLayer::create(
            "Music Cleaner",
            "<cy>Favorites (Star)</c>\n"
            "Star songs to protect them from batch deletion and accidental removal.\n\n"
            "<cr>Delete All (Trash)</c>\n"
            "Purges all downloaded songs <cr>except</c> your favorites to free up disk space.\n\n"
            "<cg>Credits</c>\n"
            "Developer: <cy>0xEtherPunk</c> (GitHub)\n"
            "Geometry Dash: <cy>0xPSP</c>",
            "OK"
        )->show();
    }

    void onDeleteAll(CCObject* sender) {
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

                FMODAudioEngine::sharedEngine()->playEffect("deleteItem_001.ogg");

                auto mdm = MusicDownloadManager::sharedState();
                if (mdm) {
                    this->m_downloadedSongs = mdm->getDownloadedSongs();
                    this->loadPage(0);
                }
            }
        );
    }

    void exitLayer(CCObject* sender) {
        GJSongBrowser::exitLayer(sender);
        GameManager::sharedState()->fadeInMenuMusic();
    }
};
