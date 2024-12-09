#ifndef __MUSICA_FONDO_H__
#define __MUSICA_FONDO_H__

#include "cocos2d.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

class audioControl {
private:
    static audioControl* instance;
    int bgmId;

    audioControl();

public:
    static audioControl* getInstance();
    void playBackgroundMusic(const std::string& filePath, bool loop = true);
    bool isMusicPlaying() const;
    float getVolume() const;
    void pauseBackgroundMusic();
    void resumeBackgroundMusic();
    void stopBackgroundMusic();
    void setBackgroundMusicVolume(float volume);
};

#endif //__MUSICA_FONDO_H__