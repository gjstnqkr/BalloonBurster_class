//
//  UserInformation.h
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 9. 29..
//
#pragma once

#ifndef BB_GameInfo_h
#define BB_GameInfo_h

#include "cocos2d.h"
#include "interface.h"


//Game이 On되고 있는 동안, 실시간으로 바뀌는 유져라이프/게임스코어/남은적수 등을 체크하여 GameUI에 반영시키기 위한 클래스다.
//While Game is playing, This class is to check UserLife/GameScore/CurEnemyCnt in real time and reflect on GameUI 
class BB_GameInfo //: public cocos2d::Node
{
public:
    
public:
    unsigned int                        m_unLifeCnt;
    unsigned int                        m_unCurEnemyCnt;
    unsigned int                        m_unGameScore;
    int                                 m_nGameState;
    
    
public:
    BB_GameInfo();
    ~BB_GameInfo();
    
    static BB_GameInfo* GetInstance();
    static void ReleaseInstance();
    

public:
    bool init();
    void init_GameInfo();
    
    virtual void setLifeCnt(const unsigned int _unHighScore) { m_unLifeCnt = _unHighScore; }
    virtual void setCurEnemyCnt(const unsigned int _unBestStage) { m_unCurEnemyCnt = _unBestStage; }
    virtual void setGameScore(const unsigned int _unGameScore) { m_unGameScore = _unGameScore; }
    virtual void setGameState(const int _nGameState) { m_nGameState = _nGameState; }
    
    virtual unsigned int getLifeCnt(){ return m_unLifeCnt; }
    virtual unsigned int getCurEnemyCnt(){ return m_unCurEnemyCnt; }
    virtual unsigned int getGameScore(){ return m_unGameScore; }
    virtual int getGameState(){ return m_nGameState; }
};

static BB_GameInfo* g_pGameInfo;


#endif /* BB_GameInfo_h */
