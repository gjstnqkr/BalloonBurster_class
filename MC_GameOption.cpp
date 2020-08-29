//
//  MC_GameOption.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 01/12/2019.
//

#include "MC_GameOption.hpp"

using namespace cocos2d;

MC_GameOption::MC_GameOption()
: m_bVibration(true)
, m_bMusic(true)
, m_bSound(true)
, m_fMusicVolume(0.8f)
, m_fSoundVolume(0.8f)
{
    
}

MC_GameOption::~MC_GameOption()
{

}

MC_GameOption* MC_GameOption::GetInstance()
{
    if(g_pGameOp == nullptr)
        g_pGameOp = new (std::nothrow) MC_GameOption();
    
    return g_pGameOp;
}

void MC_GameOption::ReleaseInstance()
{
    if(g_pGameOp != nullptr)
        delete g_pGameOp;
    g_pGameOp = nullptr;
}

bool MC_GameOption::init()
{
    init_value();
    
    
    return true;
}

void MC_GameOption::init_value()
{
    m_bVibration = true;
    m_bMusic = true;
    m_bSound = true;
    m_fMusicVolume = 0.8f;
    m_fSoundVolume = 0.8f;
}

