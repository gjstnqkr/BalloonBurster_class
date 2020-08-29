//
//  MC_GameOption.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 01/12/2019.
//

#ifndef MC_GameOption_hpp
#define MC_GameOption_hpp

#include <stdio.h>
#include "cocos2d.h"

//Game option 등을 관리하는 클래스이다. 진동, 배경음악, 사운드 등
//This class manage Game option
class MC_GameOption
{
public:
    bool                                m_bVibration;
    bool                                m_bMusic;
    bool                                m_bSound;
    
    float                               m_fMusicVolume;
    float                               m_fSoundVolume;
    
public:
    MC_GameOption();
    ~MC_GameOption();
    
    static MC_GameOption* GetInstance();
    static void ReleaseInstance();
    
    
public:
    bool init();
    void init_value();
    
    void setVibration(bool _bEnable) { m_bVibration = _bEnable; }
    void setMusic(bool _bEnable) { m_bMusic = _bEnable; }
    void setSound(bool _bEnable) { m_bSound = _bEnable; }
    
    bool getVibration(){ return m_bVibration; }
    bool getMusic(){ return m_bMusic; }
    bool getSound(){ return m_bSound; }
};

static MC_GameOption* g_pGameOp;


#endif /* MC_GameOption_hpp */
