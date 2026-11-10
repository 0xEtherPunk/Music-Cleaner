#include <Geode/Geode.hpp>
#include <Geode/modify/CustomSongCell.hpp>
#include <Geode/binding/SongInfoObject.hpp>
#include <Geode/binding/CCMenuItemToggler.hpp>
#include "../MusicCleaner.hpp"

using namespace geode::prelude;

class $modify(MCCustomSongCell, CustomSongCell) {
    struct Fields {
        CCMenuItemSpriteExtra* m_starBtn = nullptr;
        CCMenuItemToggler* m_selectToggler = nullptr;
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

        CCPoint basePos = deleteBtn ? deleteBtn->getPosition() : CCPoint{0.f, 0.f};

        bool isFav = MusicCleaner::isFavorite(songID);
        const char* starSprName = isFav ? "GJ_starBtn_001.png" : "GJ_starBtn2_001.png";

        // Star button (favorites whitelist)
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
            m_fields->m_starBtn->setPosition({ basePos.x - 28.f, basePos.y });
            menu->addChild(m_fields->m_starBtn);
        } else {
            m_fields->m_starBtn->setVisible(true);
            auto spr = CCSprite::createWithSpriteFrameName(starSprName);
            if (spr) {
                spr->setScale(0.5f);
                m_fields->m_starBtn->setSprite(spr);
            }
            m_fields->m_starBtn->setPosition({ basePos.x - 28.f, basePos.y });
        }

        // Checkbox toggler (selective deletion)
        if (!m_fields->m_selectToggler) {
            m_fields->m_selectToggler = CCMenuItemToggler::createWithStandardSprites(
                this, menu_selector(MCCustomSongCell::onToggleSelected), 0.55f
            );
            m_fields->m_selectToggler->setID("mc-cell-select-toggler"_spr);
            m_fields->m_selectToggler->setPosition({ basePos.x - 58.f, basePos.y });
            menu->addChild(m_fields->m_selectToggler);
        } else {
            m_fields->m_selectToggler->setVisible(true);
            m_fields->m_selectToggler->setPosition({ basePos.x - 58.f, basePos.y });
        }

        if (isFav) {
            MusicCleaner::setSelected(songID, false);
            m_fields->m_selectToggler->toggle(false);
        } else {
            m_fields->m_selectToggler->toggle(MusicCleaner::isSelected(songID));
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

        if (isFav) {
            MusicCleaner::setSelected(songID, false);
            if (m_fields->m_selectToggler) {
                m_fields->m_selectToggler->toggle(false);
            }
        }
    }

    void onToggleSelected(CCObject* sender) {
        if (!m_songInfoObject || m_songInfoObject->m_songID <= 0) return;
        int songID = m_songInfoObject->m_songID;

        auto toggler = static_cast<CCMenuItemToggler*>(sender);

        if (MusicCleaner::isFavorite(songID)) {
            toggler->toggle(false);
            MusicCleaner::setSelected(songID, false);
            FLAlertLayer::create(
                "Protected Song",
                "This song is marked as a <cy>Favorite</c>.\n"
                "Unstar it before selecting for deletion.",
                "OK"
            )->show();
            return;
        }

        bool newState = !MusicCleaner::isSelected(songID);
        MusicCleaner::setSelected(songID, newState);
        toggler->toggle(newState);
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
