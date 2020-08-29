//
//  BB_AnimationScene.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 10/01/2020.
//

#ifndef BB_AnimationScene_hpp
#define BB_AnimationScene_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "interface.h"
#include "HelloWorldScene.h"

#include "Archer.h"
class CAnimationScene : public cocos2d::Scene
{
public:
    emSceneState                m_SceneState;
    unsigned int                m_unCurStageAniNum; //Current Stage animation Number;  ex) 1-1 Start Ani = 11, 3-4 End Ani = 30
    
public:
    Archer*                     m_pArcher;
    
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void NextScene(float _fSec);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(CAnimationScene);
    
    void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);
    
public:
    void init_value(unsigned int _StageNum);
    void init_sprite(unsigned int _StageNum);
    void init_stageAnimation(unsigned int _StageNum = ANIMATION_START);
    
    void startGameScene_AfterSec(float _fSec);
    void pushGameScene(float _fd);
    

    
    void        setSceneState(int nSceneState);
    bool        set_CurStageAniNum(unsigned int _unVal);
    void        set_BGandSpriteOnStage(unsigned int _StageNum);
    
    int         getSceneState();
    
public:
    //virtual void scheduleUpdate(float dt) override;
    //virtual void update(float dt) override;
    
    
public:
    cocos2d::Scene*     m_HelloWordScene;
    

    
};


#endif /* BB_AnimationScene_hpp */
