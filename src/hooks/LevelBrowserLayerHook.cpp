#include <Geode/Geode.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>
#include <Geode/binding/SongsLayer.hpp>
#include <Geode/binding/GJSearchObject.hpp>

using namespace geode::prelude;

class $modify(MCLevelBrowserLayer, LevelBrowserLayer) {
    bool init(GJSearchObject* searchObj) {
        if (!LevelBrowserLayer::init(searchObj)) return false;

        // Add button when viewing Saved Levels
        if (searchObj && (searchObj->m_searchType == SearchType::SavedLevels || searchObj->m_searchType == SearchType::FavouriteLevels)) {
            auto spr = CCSprite::createWithSpriteFrameName("GJ_musicLibraryBtn_001.png");
            if (!spr) {
                spr = CCSprite::createWithSpriteFrameName("GJ_musicOnBtn_001.png");
            }
            if (spr) spr->setScale(0.8f);

            auto btn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(MCLevelBrowserLayer::onOpenMusicList)
            );
            btn->setID("music-cleaner-saved-btn"_spr);

            // In LevelBrowserLayer, place next to m_folderBtn if available
            if (m_folderBtn && m_folderBtn->getParent()) {
                auto menu = static_cast<CCMenu*>(m_folderBtn->getParent());
                menu->addChild(btn);
                if (menu->getLayout()) {
                    menu->updateLayout();
                } else {
                    btn->setPosition({ m_folderBtn->getPositionX() + 35.f, m_folderBtn->getPositionY() });
                }
            } else if (auto deleteCategoryMenu = this->getChildByID("delete-category-menu")) {
                deleteCategoryMenu->addChild(btn);
                deleteCategoryMenu->updateLayout();
            } else {
                auto winSize = CCDirector::sharedDirector()->getWinSize();
                auto menu = CCMenu::create();
                menu->setID("music-cleaner-browser-menu"_spr);
                menu->setPosition({ winSize.width - 30.f, 30.f });
                menu->addChild(btn);
                this->addChild(menu);
            }
        }

        return true;
    }

    void onOpenMusicList(CCObject* sender) {
        auto songsLayer = SongsLayer::create();
        if (songsLayer) {
            auto scene = CCDirector::sharedDirector()->getRunningScene();
            scene->addChild(songsLayer);
            songsLayer->showLayer(false);
        }
    }
};
