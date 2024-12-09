#include "musicaFondo.h"
USING_NS_CC;

audioControl* audioControl::instance = nullptr;

audioControl::audioControl() : bgmId(-1) {}

audioControl* audioControl::getInstance() {
    if (instance == nullptr) {
        instance = new audioControl();
    }
    return instance;
}

void audioControl::playBackgroundMusic(const std::string& filePath, bool loop) {
    if (bgmId != -1) {
        cocos2d::AudioEngine::stop(bgmId);
    }
    bgmId = cocos2d::AudioEngine::play2d(filePath, loop);
}

void audioControl::pauseBackgroundMusic() {
    if (bgmId != -1) {
        cocos2d::AudioEngine::pause(bgmId);
    }
}

void audioControl::resumeBackgroundMusic() {
    if (bgmId != -1) {
        cocos2d::AudioEngine::resume(bgmId);
    }
}

void audioControl::stopBackgroundMusic() {
    if (bgmId != -1) {
        cocos2d::AudioEngine::stop(bgmId);
        bgmId = -1;
    }
}

void audioControl::setBackgroundMusicVolume(float volume) {
    if (bgmId != -1) {
        cocos2d::AudioEngine::setVolume(bgmId, volume);
    }
}
bool audioControl::isMusicPlaying() const {
    return bgmId != -1;
}

float audioControl::getVolume() const {
    return bgmId != -1 ? cocos2d::AudioEngine::getVolume(bgmId) : 1.0f;
}