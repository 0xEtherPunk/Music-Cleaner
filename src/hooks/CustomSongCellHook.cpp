#include <Geode/Geode.hpp>
#include <Geode/modify/CustomSongCell.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/CustomSongWidget.hpp>
#include "../MusicCleaner.hpp"

using namespace geode::prelude;

class $modify(MCCustomSongCell, CustomSongCell) {
    struct Fields {
        CCMenuItemSpriteExtra* m_starBtn = nullptr;
    };

    void loadFromObject(SongInfoObject* object) {
        CustomSongCell::loadFromObject(object);

        if (!object || object->m_songID <= 0) return;
        int songID = object->m_songID;

        CCMenu* menu = nullptr;
        if (m_mainLayer) {
            for (auto child : CCArrayExt<CCNode*>(m_mainLayer->getChildren())) {
                if (auto m = typeinfo_cast<CCMenu*>(child)) {
                    menu = m;
                    break;
                }
            }
        }
        if (!menu) return;

        CCMenuItemSpriteExtra* deleteBtn = nullptr;
        for (auto item : CCArrayExt<CCNode*>(menu->getChildren())) {
            if (auto btn = typeinfo_cast<CCMenuItemSpriteExtra*>(item)) {
                if (btn != m_fields->m_starBtn) {
                    deleteBtn = btn;
                    break;
                }
            }
        }

        CCPoint delPos = deleteBtn ? deleteBtn->getPosition() : CCPoint{0.f, 0.f};

        // Determine vertical row offset matching [USE] and [PLAY] buttons
        float dy = 28.f;
        if (m_mainLayer) {
            if (auto widget = m_mainLayer->getChildByType<CustomSongWidget>(0)) {
                if (widget->m_playbackBtn && widget->m_selectSongBtn) {
                    float diff = widget->m_playbackBtn->getPositionY() - widget->m_selectSongBtn->getPositionY();
                    if (diff > 10.f) {
                        dy = diff;
                    }
                }
            }
        }

        bool isFav = MusicCleaner::isFavorite(songID);
        const char* starSprName = isFav ? "GJ_starBtn_001.png" : "GJ_starBtn2_001.png";

        // Position star button directly above the trash can button
        if (!m_fields->m_starBtn) {
            auto spr = CCSprite::createWithSpriteFrameName(starSprName);
            if (!spr) {
                spr = CCSprite::createWithSpriteFrameName("GJ_likeBtn_001.png");
            }
            if (spr) {
                spr->setScale(0.5f);
            }

            m_fields->m_starBtn = CCMenuItemSpriteExtra::create(
                spr, this, menu_selector(MCCustomSongCell::onStarClicked)
            );
            m_fields->m_starBtn->setID("mc-cell-star-btn"_spr);
            m_fields->m_starBtn->setPosition({ delPos.x, delPos.y + dy });
            menu->addChild(m_fields->m_starBtn);
        } else {
            m_fields->m_starBtn->setVisible(true);
            auto spr = CCSprite::createWithSpriteFrameName(starSprName);
            if (spr) {
                spr->setScale(0.5f);
                m_fields->m_starBtn->setSprite(spr);
            }
            m_fields->m_starBtn->setPosition({ delPos.x, delPos.y + dy });
        }
    }

    void onStarClicked(CCObject* sender) {
        if (!m_songInfoObject || m_songInfoObject->m_songID <= 0) return;
        int songID = m_songInfoObject->m_songID;

        bool isFav = MusicCleaner::toggleFavorite(songID);
        const char* starSprName = isFav ? "GJ_starBtn_001.png" : "GJ_starBtn2_001.png";

        if (m_fields->m_starBtn) {
            auto spr = CCSprite::createWithSpriteFrameName(starSprName);
            if (spr) {
                spr->setScale(0.5f);
                m_fields->m_starBtn->setSprite(spr);
            }
        }
    }

    void onDelete(CCObject* sender) {
        if (m_songInfoObject && MusicCleaner::isFavorite(m_songInfoObject->m_songID)) {
            FLAlertLayer::create(
                "Protected Song",
                "This song is marked as a <cy>Favorite</c>.\n"
                "Unstar it before deleting.",
                "OK"
            )->show();
            return;
        }

        CustomSongCell::onDelete(sender);
    }
};
