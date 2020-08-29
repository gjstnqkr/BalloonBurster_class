//
//  UserInformation.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 9. 29..
//

#include "BB_GameInfo.h"

using namespace cocos2d;

BB_GameInfo::BB_GameInfo()
: m_unLifeCnt(0)
, m_unCurEnemyCnt(0)
, m_unGameScore(0)
, m_nGameState(emBB_State::emLogScene)
{
}

BB_GameInfo::~BB_GameInfo()
{
}


BB_GameInfo* BB_GameInfo::GetInstance()
{
    if(g_pGameInfo == nullptr)
        g_pGameInfo = new (std::nothrow) BB_GameInfo();

    return g_pGameInfo;
}

void BB_GameInfo::ReleaseInstance()
{
    if(g_pGameInfo != nullptr)
        delete g_pGameInfo;
}


bool BB_GameInfo::init()
{
    init_GameInfo();
    
    return true;
}

void BB_GameInfo::init_GameInfo()
{
    m_unLifeCnt         = 0;
    m_unCurEnemyCnt     = 0;
    m_unGameScore       = 0;
    m_nGameState        = emBB_State::emLogScene;
}



