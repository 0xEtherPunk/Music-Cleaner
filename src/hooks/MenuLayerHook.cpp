#include <Geode/Geode.hpp>
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/binding/SongsLayer.hpp>

using namespace geode::prelude;

class $modify(MCMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        auto bottomMenu = this->getChildByID("bottom-menu");
        if (bottomMenu) {
            auto spr = CCSprite::createWithSpriteFrameName("GJ_musicLibraryBtn_001.png");
            if (!spr) {
                spr = CCSprite::createWithSpriteFrameName("GJ_musicOnBtn_001.png");
            }
            if (!spr) {
                spr = ButtonSprite::create("Music", 30, true, "goldFont.fnt", "GJ_button_01.png", 25.f, 0.6f);
            } else {
                spr->setScale(0.85f);
            }

            auto btn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(MCMenuLayer::onOpenMusicList)
            );
            btn->setID("music-cleaner-menu-button"_spr);

            // Place right after Geode button if it exists
            auto geodeBtn = bottomMenu->getChildByID("geode.loader/geode-button");
            if (geodeBtn) {
                btn->setZOrder(geodeBtn->getZOrder());
            }

            bottomMenu->addChild(btn);
            bottomMenu->updateLayout();
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
