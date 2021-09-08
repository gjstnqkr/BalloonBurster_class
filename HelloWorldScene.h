/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

#include <spine/spine-cocos2dx.h>
#include "Box2D/include/Box2D/Box2D.h"
#include "BalloonMaker.h"
#include "Archer.h"
#include "Stage.h"
#include "UserInformation.h"
#include "PauseLayer.hpp"
#include "GameUILayer.hpp"


//using namespace cocos2d;
class CAnimationScene;

class HelloWorld : public cocos2d::Scene
{

public:	
	spine::SkeletonAnimation* coinNode;
	
    //classes
    GameUILayer*                m_UILayer;
    cocos2d::Vector<Stage*>     m_ListStage;
	Archer*			            m_pArcher;
	
    //image btn
    cocos2d::MenuItemImage*         m_pGameStartBtn;
    
    //sprite
    cocos2d::Sprite*                m_pBMrock;
    cocos2d::Sprite*                m_pBtnUp;
    cocos2d::Sprite*                m_pBtnDown;
    cocos2d::Sprite*                m_pMoveObject;
    cocos2d::Sprite*                m_pGameOverSpr;

	//Throwing related variable
	float m_fForceX;		//applyForce Value
	float m_fForceY;		//applyForce Value
	float m_fVelocityX;		//Velocity Value

	cocos2d::Vec2 m_touchStart;
    cocos2d::Vec2 m_ArcPos;
    cocos2d::Vec2 m_MoveObjPosOld;
	cocos2d::Vec2 m_ptEnd;

	//Event
	bool m_bClicking;
    bool m_bMoving;
    
	bool m_bIsUp;
	bool m_bIsDown;

	bool m_bInfiniteMode;

	//Timer
	float m_fTime;
    
    //Scene
    CAnimationScene*                m_pAniScene;

public:
    virtual bool init() override;
	virtual void onEnter() override;

	static cocos2d::Scene* scene();	

	// implement the "static create()" method manually
	CREATE_FUNC(HelloWorld);

public:
	static Scene* CreateScene(bool _bInfinite = false);

    StageValue*         Get_CurrentStage();
    UserInformation*    Get_UserInfo();
    
    void erase_childBalloon();
    void erase_childRock();
    void erase_childSprite();
    
    void DecreaseEnemyNum();
    void archerDie();
    
    //archer
    void Init_Archer(bool bPositionOK = true);
    void            setArcherPos(cocos2d::Vec2 _arcPos);
    cocos2d::Vec2   getArcherPos();
    int             getArcherState();
    
    //btn
    bool pushArrowBtn();
    
    void Show_GameOver();
    void Hide_GameOver();

    bool Start_Stage(unsigned int _unStageNum);
    Stage* get_EnableStage();
    bool set_AnimationScene(unsigned int _unAniNum = 11);
    bool set_Background(unsigned int _unStageNum);
    bool set_GameSceneSprite(int _1stStageNum ,int _2ndStageNum); //ex) 2-1, set_GameSceneSprite(2,1);
    
    
    void setBMRockGravity(bool _gravity);
    unsigned int getEnemyCntFromUI();

	void Plus_StageScore(unsigned int _Point);
private:
	void Init_Value();
    void Init_UI();
	void Init_SpriteModel();
	void Init_EventListener();
    
    //stage
    void Init_Stage();
    void Delete_Stage();
    bool Enable_Stage(unsigned int nEnableNum = 1);
    
    


private:

	//Event
    //Multi
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    //Single
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    

	bool onMouseDown(cocos2d::Event* event);
	void onMouseMove(cocos2d::Event* event);
	void onMouseUp(cocos2d::Event* event);
	void onMouseScroll(cocos2d::Event* event);

	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	
	//update
	void callEveryFrame(float f);
	void updateScene(float f);
	void update(float delta);

	//Collision Func
	bool onContactBegin(cocos2d::PhysicsContact& contact);
	bool onContactBegin_Rock(cocos2d::PhysicsContact& contact);
    
    //Button Function
    void menuGameStartBtn(Ref* sender);
    void MoveUp(float fd);
    void ArcherMoveEvent(float fd);
    void menuCloseCallback(Ref* sender);

};

#endif // __HELLOWORLD_SCENE_H__
