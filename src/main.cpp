#include <Geode/Geode.hpp>
#include "MusicCleaner.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    MusicCleaner::init();
    log::info("Music Cleaner initialized with {} favorite songs.", MusicCleaner::getFavoritesCount());
}
