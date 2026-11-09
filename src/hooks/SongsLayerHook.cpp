#include <Geode/Geode.hpp>
#include <Geode/modify/SongsLayer.hpp>
#include <Geode/binding/ButtonSprite.hpp>
#include "../MusicCleanerPopup.hpp"

using namespace geode::prelude;

class $modify(MCSongsLayer, SongsLayer) {
    void customSetup() {
        SongsLayer::customSetup();

        if (!m_buttonMenu) return;

        auto spr = ButtonSprite::create(
            "Clean All", 60, true, "goldFont.fnt", "GJ_button_06.png", 28.f, 0.7f
        );

        auto cleanBtn = CCMenuItemSpriteExtra::create(
            spr, this, menu_selector(MCSongsLayer::onOpenMusicCleaner)
        );
        cleanBtn->setID("music-cleaner-btn"_spr);

        // Place near top-right corner of the drop-down layer
        cleanBtn->setPosition({ 140.f, 130.f });

        m_buttonMenu->addChild(cleanBtn);
    }

    void onOpenMusicCleaner(CCObject* sender) {
        MusicCleanerPopup::create()->show();
    }
};
