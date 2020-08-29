//
//  BB_SeleteStageScene.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 08/01/2020.
//

/*
#ifndef BB_SeleteStageScene_hpp
#define BB_SeleteStageScene_hpp

#include <stdio.h>

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "BB_LogoScene.hpp"
#include "BB_MainScene.hpp"

//class Balloon;
class RankViewLayer;
class CSelectStageScene : public cocos2d::Scene
{
public:
    emSceneState                m_SceneState;
    
    RankViewLayer*              m_pRankView;
    PauseLayer*                 m_PauseLayer;

    
    
public:
    cocos2d::MenuItemImage      *m_GameOptionItem;
    cocos2d::MenuItemImage      *m_StartGameItem;
    
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void NextScene(float _fSec);
    
    
    // implement the "static create()" method manually
    CREATE_FUNC(CMainScene);
    
    bool init_value();
    bool init_ui();
    bool init_sprite();
    bool init_layer();
    bool Init_PauseLayer();
    
    bool playGameScene();
    
    
    void        setSceneState(int nSceneState);
    int         getSceneState();
    
    
    
public:
    // a selector callback
    void menuGameOptionCallback(Ref* sender);
    void menuStartGameCallback(Ref* sender);

    
    
public:
    cocos2d::Scene*     m_HelloWordScene;
    cocos2d::Scene*     m_LogoScene;
    
};



#endif /* BB_SeleteStageScene_hpp 
*/

