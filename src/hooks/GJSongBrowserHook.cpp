#include <Geode/Geode.hpp>
#include <Geode/modify/GJSongBrowser.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/TextInput.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/MusicDownloadManager.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>
#include <Geode/binding/GameManager.hpp>
#include "../MusicCleaner.hpp"
#include <cctype>
#include <algorithm>

using namespace geode::prelude;

static std::string toLower(std::string_view s) {
    std::string res;
    res.reserve(s.size());
    for (char c : s) {
        res.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }
    return res;
}

static bool matchesSearch(SongInfoObject* song, const std::string& queryLower) {
    if (!song) return false;
    if (queryLower.empty()) return true;

    std::string idStr = std::to_string(song->m_songID);
    if (idStr.find(queryLower) != std::string::npos) return true;

    std::string titleLower = toLower(song->m_songName);
    if (titleLower.find(queryLower) != std::string::npos) return true;

    std::string artistLower = toLower(song->m_artistName);
    if (artistLower.find(queryLower) != std::string::npos) return true;

    return false;
}

class SongSearchPopup : public geode::Popup {
protected:
    TextInput* m_input = nullptr;
    std::function<void(std::string const&)> m_onSearch;
    std::string m_initialQuery;

    bool init(float width, float height, std::string const& initialQuery, std::function<void(std::string const&)> onSearch) {
        if (!Popup::init(width, height)) return false;

        m_onSearch = onSearch;
        m_initialQuery = initialQuery;

        this->setTitle("Search Songs");

        m_input = TextInput::create(220.f, "Title, artist or ID...");
        m_input->setCommonFilter(CommonFilter::Any);
        m_input->setMaxCharCount(50);
        if (!m_initialQuery.empty()) {
            m_input->setString(m_initialQuery);
        }
        m_input->setPosition({ m_size.width / 2.f, m_size.height / 2.f + 10.f });
        m_mainLayer->addChild(m_input);

        auto menu = CCMenu::create();
        menu->setPosition({ m_size.width / 2.f, m_size.height / 2.f - 40.f });

        auto searchSpr = ButtonSprite::create("Search", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        auto searchBtn = CCMenuItemSpriteExtra::create(
            searchSpr, this, menu_selector(SongSearchPopup::onSearchClicked)
        );

        if (!m_initialQuery.empty()) {
            searchBtn->setPosition({ -55.f, 0.f });
            auto resetSpr = ButtonSprite::create("Reset", "goldFont.fnt", "GJ_button_05.png", 0.8f);
            auto resetBtn = CCMenuItemSpriteExtra::create(
                resetSpr, this, menu_selector(SongSearchPopup::onResetClicked)
            );
            resetBtn->setPosition({ 55.f, 0.f });
            menu->addChild(resetBtn);
        }

        menu->addChild(searchBtn);
        m_mainLayer->addChild(menu);
        return true;
    }

    void onSearchClicked(CCObject*) {
        std::string q = m_input ? m_input->getString() : "";
        if (m_onSearch) m_onSearch(q);
        this->onClose(nullptr);
    }

    void onResetClicked(CCObject*) {
        if (m_onSearch) m_onSearch("");
        this->onClose(nullptr);
    }

public:
    static SongSearchPopup* create(std::string const& initialQuery, std::function<void(std::string const&)> onSearch) {
        auto ret = new SongSearchPopup();
        if (ret && ret->init(300.f, 150.f, initialQuery, onSearch)) {
            ret->autorelease();
            return ret;
        }
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
};

class $modify(MCGJSongBrowser, GJSongBrowser) {
    struct Fields {
        std::string m_searchQuery = "";
        Ref<CCArray> m_allSongs = nullptr;
        CCMenuItemSpriteExtra* m_searchBtn = nullptr;
    };

    void customSetup() {
        GJSongBrowser::customSetup();

        // Save original downloaded songs array
        m_fields->m_allSongs = this->m_downloadedSongs;

        // Ensure sprite sheets are loaded
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GJ_GameSheet03.plist");
        CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GJ_GameSheet02.plist");

        // Info button in top header of the dialog frame
        auto infoSpr = CCSprite::createWithSpriteFrameName("GJ_infoBtn_001.png");
        if (!infoSpr) {
            infoSpr = ButtonSprite::create("i", "goldFont.fnt", "GJ_button_02.png", 0.8f);
        } else {
            infoSpr->setScale(0.75f);
        }

        auto infoBtn = CCMenuItemSpriteExtra::create(
            infoSpr, this, menu_selector(MCGJSongBrowser::onInfo)
        );
        infoBtn->setID("music-cleaner-info-btn"_spr);

        // Search magnifying glass button right next to info button
        auto searchSpr = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
        if (!searchSpr) {
            searchSpr = CCSprite::createWithSpriteFrameName("GJ_searchBtn_001.png");
        }
        if (!searchSpr) {
            searchSpr = ButtonSprite::create("Q", "goldFont.fnt", "GJ_button_01.png", 0.8f);
        } else {
            searchSpr->setScale(0.75f);
        }

        m_fields->m_searchBtn = CCMenuItemSpriteExtra::create(
            searchSpr, this, menu_selector(MCGJSongBrowser::onSearchBtnClicked)
        );
        m_fields->m_searchBtn->setID("music-cleaner-search-btn"_spr);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        CCPoint centerOffset = CCPoint(0.f, 0.f);
        if (m_buttonMenu) {
            if (m_buttonMenu->getPositionX() < 10.f && m_buttonMenu->getPositionY() < 10.f) {
                centerOffset = CCPoint(winSize.width / 2.f, winSize.height / 2.f);
            }
            infoBtn->setPosition(CCPoint(centerOffset.x - 165.f, centerOffset.y + 130.f));
            m_fields->m_searchBtn->setPosition(CCPoint(centerOffset.x - 135.f, centerOffset.y + 130.f));

            m_buttonMenu->addChild(infoBtn, 100);
            m_buttonMenu->addChild(m_fields->m_searchBtn, 100);
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

    void onSearchBtnClicked(CCObject* sender) {
        SongSearchPopup::create(
            m_fields->m_searchQuery,
            [this](std::string const& query) {
                this->applySearchFilter(query);
            }
        )->show();
    }

    void applySearchFilter(std::string const& query) {
        m_fields->m_searchQuery = query;

        if (!m_fields->m_allSongs && this->m_downloadedSongs) {
            m_fields->m_allSongs = this->m_downloadedSongs;
        }
        if (!m_fields->m_allSongs) return;

        if (query.empty()) {
            this->m_downloadedSongs = m_fields->m_allSongs;
            if (m_fields->m_searchBtn) {
                auto spr = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
                if (spr) {
                    spr->setScale(0.75f);
                    m_fields->m_searchBtn->setSprite(spr);
                }
            }
            this->loadPage(0);
            return;
        }

        std::string queryLower = toLower(query);
        auto filtered = CCArray::create();

        for (int i = 0; i < m_fields->m_allSongs->count(); ++i) {
            auto song = static_cast<SongInfoObject*>(m_fields->m_allSongs->objectAtIndex(i));
            if (matchesSearch(song, queryLower)) {
                filtered->addObject(song);
            }
        }

        if (m_fields->m_searchBtn) {
            auto spr = CCSprite::createWithSpriteFrameName("gj_findBtnOff_001.png");
            if (!spr) spr = CCSprite::createWithSpriteFrameName("gj_findBtn_001.png");
            if (spr) {
                spr->setScale(0.75f);
                m_fields->m_searchBtn->setSprite(spr);
            }
        }

        this->m_downloadedSongs = filtered;
        this->loadPage(0);

        if (filtered->count() == 0) {
            FLAlertLayer::create(
                "Search",
                fmt::format("No downloaded songs match \"<cy>{}</c>\".", query),
                "OK"
            )->show();
        }
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
                    this->m_fields->m_allSongs = mdm->getDownloadedSongs();
                    if (!this->m_fields->m_searchQuery.empty()) {
                        this->applySearchFilter(this->m_fields->m_searchQuery);
                    } else {
                        this->m_downloadedSongs = this->m_fields->m_allSongs;
                        this->loadPage(0);
                    }
                }
            }
        );
    }

    void exitLayer(CCObject* sender) {
        m_fields->m_allSongs = nullptr;
        m_fields->m_searchQuery = "";
        GJSongBrowser::exitLayer(sender);
        GameManager::sharedState()->fadeInMenuMusic();
    }
};
