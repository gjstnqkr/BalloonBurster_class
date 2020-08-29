//
//  MC_Sound.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 16/08/2020.
//

#include "MC_SoundBox.hpp"
#include "MC_GameOption.hpp"
#include "audio/include/SimpleAudioEngine.h"
#include "interface.h"

using namespace cocos2d;


MC_SoundBox::MC_SoundBox()
{
}


MC_SoundBox::~MC_SoundBox()
{
}

MC_SoundBox* MC_SoundBox::getInstance()
{
    if(g_pSoundBox == nullptr)
        g_pSoundBox = new (std::nothrow) MC_SoundBox();
    
    return g_pSoundBox;
}

void MC_SoundBox::ReleaseInstance()
{
    if(g_pSoundBox != nullptr)
        delete g_pSoundBox;
}

bool MC_SoundBox::init_sound()
{
    MC_GameOption* pOption = MC_GameOption::GetInstance();
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SOUND_PATH_ARRSHOTING);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(MUSIC_PATH_MAIN);
    
    setSoundVolume(pOption->m_fSoundVolume);
    setMusicVolume(pOption->m_fMusicVolume);
    return true;
}


bool MC_SoundBox::setSoundVolume(float _size)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(_size);
    return true;
}

bool MC_SoundBox::setMusicVolume(float _size)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(_size);
    return true;
}

bool MC_SoundBox::playSound(char * _path)
{
    if(MC_GameOption::GetInstance()->getSound())
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(_path, false, 1.0f,0.0f,1.0f);
    return true;
}

bool MC_SoundBox::playMusic(char * _path)
{
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(_path, true);
    return true;
}

bool MC_SoundBox::tryVibration(float duration)
{
    if(MC_GameOption::GetInstance()->getVibration()) Device::vibrate(duration);
    return true;
}

void MC_SoundBox::resumeMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}

void MC_SoundBox::pauseMusic()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}
