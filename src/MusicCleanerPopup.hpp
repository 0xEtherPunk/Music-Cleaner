#pragma once

#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include "MusicCleaner.hpp"

class MusicCleanerPopup : public geode::Popup {
protected:
    cocos2d::CCLabelBMFont* m_totalLabel = nullptr;
    cocos2d::CCLabelBMFont* m_favLabel = nullptr;
    cocos2d::CCLabelBMFont* m_deleteLabel = nullptr;
    CCMenuItemToggler* m_protectActiveToggler = nullptr;

    bool init();
    void refreshStats();

    void onClean(cocos2d::CCObject* sender);
    void onToggleProtectActive(cocos2d::CCObject* sender);

public:
    static MusicCleanerPopup* create();
};
