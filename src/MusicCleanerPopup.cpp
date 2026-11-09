#include "MusicCleanerPopup.hpp"
#include <Geode/binding/ButtonSprite.hpp>
#include <Geode/binding/FMODAudioEngine.hpp>

using namespace geode::prelude;

MusicCleanerPopup* MusicCleanerPopup::create() {
    auto ret = new MusicCleanerPopup();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool MusicCleanerPopup::init() {
    if (!Popup::init(380.f, 260.f, "GJ_square01.png")) {
        return false;
    }

    this->setTitle("Music Cleaner", "goldFont.fnt", 0.9f);

    float cx = m_size.width / 2.f;

    m_totalLabel = CCLabelBMFont::create("Downloaded: ...", "chatFont.fnt");
    m_totalLabel->setPosition({ cx, m_size.height - 70.f });
    m_mainLayer->addChild(m_totalLabel);

    m_favLabel = CCLabelBMFont::create("Favorites Protected: ...", "chatFont.fnt");
    m_favLabel->setPosition({ cx, m_size.height - 95.f });
    m_favLabel->setColor({ 255, 230, 100 });
    m_mainLayer->addChild(m_favLabel);

    m_deleteLabel = CCLabelBMFont::create("To Delete: ...", "chatFont.fnt");
    m_deleteLabel->setPosition({ cx, m_size.height - 120.f });
    m_deleteLabel->setColor({ 255, 120, 120 });
    m_mainLayer->addChild(m_deleteLabel);

    bool protectActive = Mod::get()->getSettingValue<bool>("protect-active-songs");

    auto onSpr = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto offSpr = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    onSpr->setScale(0.7f);
    offSpr->setScale(0.7f);

    m_protectActiveToggler = CCMenuItemToggler::create(
        offSpr, onSpr, this, menu_selector(MusicCleanerPopup::onToggleProtectActive)
    );
    m_protectActiveToggler->toggle(protectActive);
    m_protectActiveToggler->setPosition({ cx - 95.f, 75.f });
    m_buttonMenu->addChild(m_protectActiveToggler);

    auto protectLabel = CCLabelBMFont::create("Protect Menu & Practice Songs", "bigFont.fnt");
    protectLabel->setScale(0.35f);
    protectLabel->setAnchorPoint({ 0.f, 0.5f });
    protectLabel->setPosition({ cx - 75.f, 75.f });
    m_mainLayer->addChild(protectLabel);

    auto cleanBtnSpr = ButtonSprite::create(
        "Delete Unstarred", 160, true, "goldFont.fnt", "GJ_button_06.png", 32.f, 0.8f
    );
    auto cleanBtn = CCMenuItemSpriteExtra::create(
        cleanBtnSpr, this, menu_selector(MusicCleanerPopup::onClean)
    );
    cleanBtn->setPosition({ cx, 35.f });
    m_buttonMenu->addChild(cleanBtn);

    refreshStats();
    return true;
}

void MusicCleanerPopup::onToggleProtectActive(CCObject* sender) {
    bool newState = !m_protectActiveToggler->isToggled();
    Mod::get()->setSettingValue("protect-active-songs", newState);
    refreshStats();
}

void MusicCleanerPopup::refreshStats() {
    bool protectActive = Mod::get()->getSettingValue<bool>("protect-active-songs");
    auto stats = MusicCleaner::calculateStats(protectActive);

    std::string totalStr = fmt::format(
        "Downloaded: {} songs ({})",
        stats.totalSongs,
        MusicCleaner::formatBytes(stats.totalBytes)
    );
    m_totalLabel->setString(totalStr.c_str());

    std::string favStr = fmt::format(
        "Favorites Protected: {} songs",
        stats.favoriteSongs
    );
    if (stats.activeProtectedSongs > 0 && protectActive) {
        favStr += fmt::format(" (+{} active)", stats.activeProtectedSongs);
    }
    m_favLabel->setString(favStr.c_str());

    std::string delStr = fmt::format(
        "To Delete: {} songs (reclaimable: {})",
        stats.songsToDelete,
        MusicCleaner::formatBytes(stats.bytesToFree)
    );
    m_deleteLabel->setString(delStr.c_str());
}

void MusicCleanerPopup::onClean(CCObject* sender) {
    bool protectActive = Mod::get()->getSettingValue<bool>("protect-active-songs");
    auto stats = MusicCleaner::calculateStats(protectActive);

    if (stats.songsToDelete == 0) {
        FLAlertLayer::create(
            "Nothing to Delete",
            "All downloaded songs are either favorited or currently protected.",
            "OK"
        )->show();
        return;
    }

    bool showConfirm = Mod::get()->getSettingValue<bool>("show-confirm-popup");
    if (!showConfirm) {
        uintmax_t freed = 0;
        size_t deleted = MusicCleaner::executeCleanup(protectActive, &freed);
        FMODAudioEngine::sharedEngine()->playEffect("deleteItem_001.ogg");
        refreshStats();
        return;
    }

    std::string confirmMsg = fmt::format(
        "Are you sure you want to delete <cr>{}</c> songs?\n\n"
        "Approximately <cg>{}</c> of storage will be reclaimed.\n\n"
        "<cy>{}</c> favorited songs will remain untouched.",
        stats.songsToDelete,
        MusicCleaner::formatBytes(stats.bytesToFree),
        stats.favoriteSongs
    );

    createQuickPopup(
        "Confirm Deletion",
        confirmMsg,
        "Cancel",
        "Delete",
        [this, protectActive](FLAlertLayer*, bool btn2) {
            if (btn2) {
                uintmax_t freed = 0;
                size_t deleted = MusicCleaner::executeCleanup(protectActive, &freed);

                FMODAudioEngine::sharedEngine()->playEffect("deleteItem_001.ogg");

                std::string resMsg = fmt::format(
                    "Cleaned up <cg>{}</c> songs.\nFreed: <cg>{}</c>",
                    deleted,
                    MusicCleaner::formatBytes(freed)
                );
                FLAlertLayer::create("Operation Complete", resMsg, "OK")->show();

                this->refreshStats();
            }
        }
    );
}
