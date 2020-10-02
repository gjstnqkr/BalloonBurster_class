//
//  BB_GameScene.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 7. 4..
//

#ifndef BB_MainScene_hpp
#define BB_MainScene_hpp

#include "cocos2d.h"
#include "HelloWorldScene.h"
#include "BB_LogoScene.hpp"
#include "TextInsertLayer.hpp"
#include "Archer.h"
#include "Balloon.h"
#include "PauseLayer.hpp"

//class Balloon;
class RankViewLayer;
class CMainScene : public cocos2d::Scene
{
public:
    emSceneState                m_SceneState;
    
    TextInsertLayer*            m_pTextInsert;
    RankViewLayer*              m_pRankView;
    PauseLayer*                 m_PauseLayer;
    
    Archer*                     m_pArcher;
    cocos2d::Vector<Balloon*>   m_listBalloon;
    cocos2d::Vector<cocos2d::Sprite*>    m_vecCloud;
    
    
    cocos2d::Sprite*            m_pTestSprite;
    
    
public:
    cocos2d::MenuItemImage      *m_GameOptionItem;
    cocos2d::MenuItemImage      *m_StartGameItem;
	cocos2d::MenuItemImage      *m_InfiniteModeItem;
    
    
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    
    
public:
    static cocos2d::Scene* createScene();
    
    virtual bool init();
    void NextScene(float _fSec);

    
    // implement the "static create()" method manually
    CREATE_FUNC(CMainScene);
    
    bool init_value();
    bool init_option();
    bool init_music();
    bool init_ui();
    bool init_sprite();
    bool init_layer();
    bool Init_PauseLayer();
    
    bool playGameScene();
	bool playInfiniteMode();
    
    void        setSceneState(int nSceneState);
    void        setStartMenuEnable(bool bEnable);
    
    int         getSceneState();
    
    
    
public:
    // a selector callback
    void menuGameOptionCallback(Ref* sender);
    void menuStartGameCallback(Ref* sender);
	void menuInfiniteModeCallback(Ref* sender);
    
    //MainScene Action
    void schedulerShootArr(float f);
    
    //virtual void scheduleUpdate(float dt) override;
    virtual void update(float dt) override;

    
public:
    cocos2d::Scene*     m_HelloWordScene;
    cocos2d::Scene*     m_SelectStageScene;
    cocos2d::Scene*     m_AnimationScene;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
};


#endif /* BB_MainScene_hpp */
