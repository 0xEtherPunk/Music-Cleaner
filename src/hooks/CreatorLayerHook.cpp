#include <Geode/Geode.hpp>
#include <Geode/modify/CreatorLayer.hpp>
#include "../MusicCleanerPopup.hpp"

using namespace geode::prelude;

class $modify(MCCreatorLayer, CreatorLayer) {
    bool init() {
        if (!CreatorLayer::init()) return false;

        auto menu = this->getChildByID("bottom-left-menu");
        if (!menu) {
            menu = this->getChildByID("creator-buttons-menu");
        }

        auto spr = CCSprite::createWithSpriteFrameName("GJ_trashBtn_001.png");
        if (!spr) {
            spr = ButtonSprite::create("Cleaner", 40, true, "goldFont.fnt", "GJ_button_01.png", 28.f, 0.7f);
        } else {
            spr->setScale(0.8f);
        }

        auto btn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(MCCreatorLayer::onOpenMusicCleaner)
        );
        btn->setID("music-cleaner-creator-btn"_spr);

        if (menu) {
            menu->addChild(btn);
            menu->updateLayout();
        } else {
            // Fallback: create a small menu in the bottom left
            auto newMenu = CCMenu::create();
            newMenu->setID("music-cleaner-menu"_spr);
            newMenu->setPosition({ 35.f, 35.f });
            newMenu->addChild(btn);
            this->addChild(newMenu);
        }

        return true;
    }

    void onOpenMusicCleaner(CCObject* sender) {
        MusicCleanerPopup::create()->show();
    }
};
