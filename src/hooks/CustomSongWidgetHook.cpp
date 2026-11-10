#include <Geode/Geode.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include "../MusicCleaner.hpp"

using namespace geode::prelude;

class $modify(MCCustomSongWidget, CustomSongWidget) {
    struct Fields {
        CCMenuItemSpriteExtra* m_starBtn = nullptr;
    };

    void updateSongInfo() {
        CustomSongWidget::updateSongInfo();

        // CustomSongWidget is also used inside CustomSongCell where m_songDelegate is set.
        // Cell layout is handled independently by CustomSongCellHook.
        if (m_songDelegate != nullptr) {
            return;
        }

        if (m_customSongID <= 0 || m_isRobtopSong || !m_buttonMenu) {
            if (m_fields->m_starBtn) {
                m_fields->m_starBtn->setVisible(false);
            }
            return;
        }

        bool isFav = MusicCleaner::isFavorite(m_customSongID);
        const char* sprName = isFav ? "GJ_starBtn_001.png" : "GJ_starBtn2_001.png";

        if (!m_fields->m_starBtn) {
            auto spr = CCSprite::createWithSpriteFrameName(sprName);
            if (!spr) {
                spr = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            }
            if (spr) {
                spr->setScale(0.45f);
            }

            m_fields->m_starBtn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(MCCustomSongWidget::onStarClicked)
            );
            m_fields->m_starBtn->setID("music-cleaner-star-btn"_spr);

            if (m_deleteBtn) {
                auto delPos = m_deleteBtn->getPosition();
                m_fields->m_starBtn->setPosition({ delPos.x - 26.f, delPos.y });
            } else if (m_playbackBtn) {
                auto playPos = m_playbackBtn->getPosition();
                m_fields->m_starBtn->setPosition({ playPos.x + 30.f, playPos.y });
            } else {
                m_fields->m_starBtn->setPosition({ 140.f, 0.f });
            }

            m_buttonMenu->addChild(m_fields->m_starBtn);
        } else {
            m_fields->m_starBtn->setVisible(true);
            auto newSpr = CCSprite::createWithSpriteFrameName(sprName);
            if (newSpr) {
                newSpr->setScale(0.45f);
                m_fields->m_starBtn->setSprite(newSpr);
            }
        }
    }

    void onStarClicked(CCObject* sender) {
        if (m_customSongID <= 0) return;

        bool isFav = MusicCleaner::toggleFavorite(m_customSongID);
        const char* sprName = isFav ? "GJ_starBtn_001.png" : "GJ_starBtn2_001.png";

        if (m_fields->m_starBtn) {
            auto spr = CCSprite::createWithSpriteFrameName(sprName);
            if (spr) {
                spr->setScale(0.45f);
                m_fields->m_starBtn->setSprite(spr);
            }
        }
    }

    void deleteSong() {
        if (MusicCleaner::isFavorite(m_customSongID)) {
            FLAlertLayer::create(
                "Protected Song",
                "This song is marked as a <cy>Favorite</c>.\n"
                "Unstar it before deleting.",
                "OK"
            )->show();
            return;
        }

        CustomSongWidget::deleteSong();
    }
};
