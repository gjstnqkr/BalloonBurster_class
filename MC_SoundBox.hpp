//
//  MC_Sound.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 16/08/2020.
//
#pragma once

#ifndef MC_Sound_hpp
#define MC_Sound_hpp


#include "cocos2d.h"

class MC_SoundBox : public cocos2d::Node
{
    
public:
    MC_SoundBox();
    ~MC_SoundBox();

    static MC_SoundBox * getInstance();
    void ReleaseInstance();
    
public:
    bool init_sound();
    bool setSoundVolume(float _size);
    bool setMusicVolume(float _size);
    
    bool playSound(char * _path);
    bool playMusic(char * _path);
    bool tryVibration(float duration);
    
    void pauseMusic();
    void resumeMusic();
    
};

static MC_SoundBox* g_pSoundBox = nullptr;


#endif /* MC_Sound_hpp */
