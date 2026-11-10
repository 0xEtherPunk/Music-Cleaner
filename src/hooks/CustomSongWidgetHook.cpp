#include <Geode/Geode.hpp>
#include <Geode/modify/CustomSongWidget.hpp>
#include "../MusicCleaner.hpp"

using namespace geode::prelude;

class $modify(MCCustomSongWidget, CustomSongWidget) {
    struct Fields {
        CCMenuItemSpriteExtra* m_starBtn = nullptr;
    };

    void updateSongInfo() {
        // CustomSongWidget is also used inside CustomSongCell where m_songDelegate or m_isInCell is set.
        // Cell layout is handled independently by CustomSongCellHook.
        if (m_songDelegate != nullptr || m_isInCell) {
            m_showSelectSongBtn = false;
            CustomSongWidget::updateSongInfo();
            if (m_selectSongBtn) {
                m_selectSongBtn->setVisible(false);
                m_selectSongBtn->setEnabled(false);
                m_selectSongBtn->setPosition(CCPoint(-9999.f, -9999.f));
            }
            return;
        }

        CustomSongWidget::updateSongInfo();

        if (m_customSongID <= 0 || m_isRobtopSong || !m_buttonMenu) {
            if (m_fields->m_starBtn) {
                m_fields->m_starBtn->setVisible(false);
            }
            return;
        }

        bool isFav = MusicCleaner::isFavorite(m_customSongID);
        const char* sprName = isFav ? "GJ_starBtn_001.png" : "GJ_starBtn2_001.png";

        // Determine target position: slightly left of download or delete button (offset -33px)
        CCPoint targetPos(140.f, 0.f);
        if (m_deleteBtn && m_deleteBtn->isVisible()) {
            targetPos = CCPoint(m_deleteBtn->getPositionX() - 33.f, m_deleteBtn->getPositionY());
        } else if (m_downloadBtn && m_downloadBtn->isVisible()) {
            targetPos = CCPoint(m_downloadBtn->getPositionX() - 33.f, m_downloadBtn->getPositionY());
        } else if (m_deleteBtn) {
            targetPos = CCPoint(m_deleteBtn->getPositionX() - 33.f, m_deleteBtn->getPositionY());
        } else if (m_downloadBtn) {
            targetPos = CCPoint(m_downloadBtn->getPositionX() - 33.f, m_downloadBtn->getPositionY());
        } else if (m_playbackBtn) {
            targetPos = CCPoint(m_playbackBtn->getPositionX() + 30.f, m_playbackBtn->getPositionY());
        }

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
            m_fields->m_starBtn->setPosition(targetPos);

            m_buttonMenu->addChild(m_fields->m_starBtn);
        } else {
            m_fields->m_starBtn->setVisible(true);
            auto newSpr = CCSprite::createWithSpriteFrameName(sprName);
            if (newSpr) {
                newSpr->setScale(0.45f);
                m_fields->m_starBtn->setSprite(newSpr);
            }
            m_fields->m_starBtn->setPosition(targetPos);
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
