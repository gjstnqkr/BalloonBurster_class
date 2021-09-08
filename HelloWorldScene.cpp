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

#include "HelloWorldScene.h"
#include "AppMacros.h"
#include "interface.h"


#include "spine/spine.h"
#include "BalloonMaker.h"
#include "Arrow.h"
#include "Balloon.h"
#include "Rock.h"
#include "BB_GameInfo.h"
#include "BB_AnimationScene.hpp"
#include "BB_ActionTexture.hpp"
#include "audio/include/SimpleAudioEngine.h"
#include "MC_GameOption.hpp"
#include "MC_SoundBox.hpp"

USING_NS_CC;

#define BOUNDINGBOX_OFFSET_POS_Y 15
#define BOUNDINGBOX_OFFSET_HEIGHT 50
#define Std_VELOCITY 150.0f		//DART 기준 Velocity 값
#define Std_SPEED 500.0f		//DART가 날라가는 speed 기준값
#define Std_FORCE_X 0.0f		//DART가 날라가는 applyforce 기준값
#define Std_FORCE_Y 0.0f		//DART가 날라가는 applyforce 기준값

#define pooyan

#define MOVE_BTN 0


Scene* HelloWorld::scene()
{
     return HelloWorld::create();
}


Scene* HelloWorld::CreateScene(bool _binfinite)
{	
	HelloWorld* pHelloWorld = new (std::nothrow) HelloWorld();
	pHelloWorld->m_bInfiniteMode = _binfinite;
	if (pHelloWorld && pHelloWorld->init())
	{
		pHelloWorld->autorelease();
		return pHelloWorld;
	}
	CC_SAFE_DELETE(pHelloWorld);
	return nullptr;		
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	Init_Value();
    
	Init_SpriteModel();
    Init_UI();
	Init_EventListener();
    Init_Stage();
    
    if(m_UILayer == NULL)
    {
        m_UILayer = GameUILayer::create(this);
        this->addChild(m_UILayer);
        m_UILayer->Init_PauseLayer();
    }

	//Back Color
	Director::getInstance()->setClearColor(Color4F(0.7, 0.92, 0.96, 0));	

	//World Physics
	this->initWithPhysics();

	// set Gravity
	//this->getPhysicsWorld()->setGravity(Vec2(0.0f, -30.0f));
	
    
	//화면 크기 가져오기
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    

    

	// Line
	Rect ret = Director::getInstance()->getOpenGLView()->getVisibleRect();
	Vec2 RBottom = Vec2(ret.origin.x + ret.size.width, ret.origin.y);

    
	auto ground = Node::create();
	ground->addComponent(PhysicsBody::createEdgeSegment(ret.origin + Vec2(0, visibleSize.height - visibleSize.height/8),
		RBottom + Vec2(0, visibleSize.height - visibleSize.height/8)));

	//this->addChild(ground);

	//Bounding Box
	//this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);

    return true;
}

void HelloWorld::onEnter()
{
	Scene::onEnter();
    if(m_UILayer != NULL && m_UILayer->m_PauseLayer != NULL)
        m_UILayer->m_PauseLayer->Loading_Banner();
	

	return;
}

void HelloWorld::Init_SpriteModel()
{
    //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    log("visible width - %f // visible height - %f", visibleSize.width, visibleSize.height);
    log("origin width - %f // origin height - %f", origin.x, origin.y);
    
    
    
    //--------------------------------------start
    // LongRock
    Sprite* pSprtMailBox = Sprite::create("LongRock.png");
    pSprtMailBox->setPosition(Vec2((visibleSize.width / 2), (visibleSize.height / 8 )));
    pSprtMailBox->setScale(8.5, 2.5);
    
    const PhysicsMaterial PHYSICSBODY_MATERIAL_DEFAULT(0.1f, 0.5f, 0.5f);
    PhysicsMaterial PhyMat(1.0f, 0.0f, 1.0f);
    PhysicsBody* pbody2 = PhysicsBody::createBox(pSprtMailBox->getContentSize(), PhyMat);
    pbody2->setDynamic(false);
    //pbody2->setContactTestBitmask(0xFFFFFFFF);
    pSprtMailBox->setPhysicsBody(pbody2);
    pSprtMailBox->setTag(emNameTag::TERRAIN);
    
    pSprtMailBox->getPhysicsBody()->setGravityEnable(false);
    this->addChild(pSprtMailBox);
    //--------------------------------------end
    
    //--------------------------------------start
    //Rock
    Sprite* pSprtRock = Sprite::create("rock.png");
    PhysicsBody* pbody_Rock = PhysicsBody::createBox(pSprtRock->getContentSize(), ROCK_MATERIAL);
    pbody2->setDynamic(false);
    pbody_Rock->setContactTestBitmask(0xFFFFFFFF);
    pSprtRock->setPhysicsBody(pbody_Rock);
    
    
    pSprtRock->setPosition(Vec2((visibleSize.width / 2) - 160, (visibleSize.height / 2) - 80));
    pSprtRock->getPhysicsBody()->setGravityEnable(true);
    this->addChild(pSprtRock);
    //--------------------------------------end
    
    
    //--------------------------------------start
    //insert Archer
    Vec2 pos = Vec2(Vec2((visibleSize.width / 2) - 350, (visibleSize.height / 2) + 80));
    m_pArcher = Archer::create(pos);
    m_pArcher->setScale(ARCHER_SCALE);
    this->addChild(m_pArcher);
    m_pArcher->init_ArcherState();
    //--------------------------------------end
    
    
    
    
    //--------------------------------------start
    //GameOver
    m_pGameOverSpr = cocos2d::Sprite::create("gameover.png");
    m_pGameOverSpr->setPosition(Vec2(visibleSize.width/2 , visibleSize.height/2 + m_pGameOverSpr->getContentSize().height/2));
    m_pGameOverSpr->setVisible(false);
    this->addChild(m_pGameOverSpr, Z_SCENE_UI);
    
    
    //--------------------------------------end
    
    
    //--------------------------------------start
    //
    m_pBMrock = Sprite::create("rock.png");
    m_pBMrock->setPosition(Vec2( (visibleSize.width * 0.5 ) , (visibleSize.height * 0.88) ));
    m_pBMrock->setScale(2.5, 2.5);
    
    PhysicsMaterial PhyMat2(8.0f, 0.5f, 0.3f);
    PhysicsBody* pbody3 = PhysicsBody::createBox(m_pBMrock->getContentSize(), PhyMat2);
    pbody3->setDynamic(false);
    m_pBMrock->setPhysicsBody(pbody3);
    m_pBMrock->setTag(emNameTag::PROJECTTILE);
    
    m_pBMrock->getPhysicsBody()->setGravityEnable(false);
    this->addChild(m_pBMrock);
    //--------------------------------------end
    

}

void HelloWorld::Init_UI()
{

    //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    
    //Game Start Btn
    m_pGameStartBtn = MenuItemImage::create(
                                            "Leve1Button.png",
                                            "Leve1Button.png",
                                            CC_CALLBACK_1(HelloWorld::menuGameStartBtn,this));
    float fBtnPosY = visibleSize.height * 0.5;
    float fBtnPosX = visibleSize.width * 0.5;
    CCSize conSize_Start = m_pGameStartBtn->getContentSize();
    m_pGameStartBtn->setPosition(origin + Vec2(fBtnPosX, fBtnPosY) - conSize_Start/2);
    m_pGameStartBtn->setAnchorPoint(Vec2(0,0));
    
    // create menu, it's an autorelease object
    auto menu_start = Menu::create(m_pGameStartBtn, nullptr);
    menu_start->setPosition(Vec2::ZERO);
    this->addChild(menu_start, Z_SCENE_UI);
    

    float   fBtnOffset          = 0.0f;
#if MOVE_BTN == 1
    //Move Btn
    m_pBtnUp = cocos2d::Sprite::create("button_Up.png");
    fBtnOffset                  = visibleSize.height * UI_LEFT_START_RATE;
    CCSize  orgSize_UpBtn       = m_pBtnUp->getContentSize();
    m_pBtnUp->setPosition(origin + Vec2(fBtnOffset, fBtnOffset) + Vec2( 0, orgSize_UpBtn.height + fBtnOffset*0.35f) );
    m_pBtnUp->setAnchorPoint(Vec2(0,0));
    m_pBtnUp->setScale(1.0);
    
    m_pBtnDown = cocos2d::Sprite::create("button_Down.png");
    fBtnOffset                  = visibleSize.height * UI_LEFT_START_RATE;
    CCSize  orgSize_DownBtn       = m_pBtnDown->getContentSize();
    m_pBtnDown->setPosition(origin + Vec2(fBtnOffset, fBtnOffset) );
    m_pBtnDown->setAnchorPoint(Vec2(0,0));
    m_pBtnDown->setScale(1.0);

    this->addChild(m_pBtnUp, Z_SCENE_UI);
    this->addChild(m_pBtnDown, Z_SCENE_UI);
#else
    

#endif

    
    
}
Stage* HelloWorld::get_EnableStage()
{
    Stage* pEnableStage = nullptr;
    
    int EnableCnt = 0;
    for(int ii = 0; ii < m_ListStage.size(); ii++)
    {
        Stage* pStage = m_ListStage.at(ii);
        if(pStage->m_bStageEnable)
        {
            pEnableStage = pStage;
            EnableCnt++;
        }
    }
    
    if(EnableCnt > 1 && EnableCnt < 1) pEnableStage = nullptr;
    return pEnableStage;
}


void HelloWorld::DecreaseEnemyNum()
{
	get_EnableStage()->DecreaseEnemyNum();
}

void HelloWorld::Init_Archer(bool bPositionOK)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 pos = Vec2(Vec2((visibleSize.width / 2) - 350, (visibleSize.height / 2) + 80));
    if(bPositionOK)
        m_pArcher->setPosition(pos);
    
    m_pArcher->setRotation(0);
    m_pArcher->set_ArcState(emArcherState::Arc_Idle);
    m_ArcPos = pos;
    float fBtnOffset    = visibleSize.height * UI_LEFT_START_RATE;
    m_UILayer->setMoveObjPos(origin + Vec2(fBtnOffset, fBtnOffset));
}

void HelloWorld::setArcherPos(cocos2d::Vec2 _arcPos)
{
    m_pArcher->setPosition(_arcPos);
}

cocos2d::Vec2 HelloWorld::getArcherPos()
{
    return m_pArcher->getPosition();
}

int HelloWorld::getArcherState()
{
    return m_pArcher->get_ArcState();
}


void HelloWorld::Show_GameOver()
{
    m_pGameOverSpr->setVisible(true);
    m_pGameOverSpr->setOpacity(0.0f);
    m_pGameOverSpr->stopAllActions();
    FadeIn* ActionFade      = FadeIn::create(3.0f);
    m_pGameOverSpr->runAction(ActionFade);
}

void HelloWorld::Hide_GameOver()
{
    this->stopAllActions();
    FadeOut* ActionFade      = FadeOut::create(3.0f);
    this->runAction(ActionFade);
    m_pGameOverSpr->setVisible(false);
}


void HelloWorld::Plus_StageScore(unsigned int _Point)
{
    unsigned int unUScore = get_EnableStage()->get_UserScore();
    unUScore = unUScore + _Point;
    
    get_EnableStage()->Set_UserScore(unUScore);
    //m_pStage->Set_UserScoreLabel(unUScore);
    m_UILayer->Set_UserScoreLabel(unUScore);
}

unsigned int HelloWorld::getEnemyCntFromUI()
{
    return m_UILayer->GetEnemyCntFromLabel();
}


StageValue* HelloWorld::Get_CurrentStage()
{
    return get_EnableStage()->getCurrentStageVal();
}


void HelloWorld::Init_Value()
{
	m_fForceX = Std_FORCE_X;
	m_fForceY = Std_FORCE_Y;
	m_fVelocityX = Std_VELOCITY;
	m_bClicking = false;
    m_bMoving = false;
	m_fTime = 0.0f;
    m_touchStart        = Vec2(Vec2::ZERO);
    m_ArcPos            = Vec2(Vec2::ZERO);
    m_MoveObjPosOld     = Vec2(Vec2::ZERO);
	m_ptEnd             = Vec2(Vec2::ZERO);
    m_bIsUp = false;
    m_bIsDown = false;
    m_UILayer = nullptr;
    m_pAniScene = nullptr;
    m_pBMrock = nullptr;
    
    
    MC_SoundBox::getInstance()->playMusic(MUSIC_PATH_MAIN);
}

void HelloWorld::Init_Stage()
{
    //Start Here, modify m_pStage -> m_ListStage
    
    //this->addChild(m_pStage);
    Delete_Stage();
    
	if (!m_bInfiniteMode)
	{
		for (int ii = 0; ii < STAGE_1st_MAX; ii++)
		{
			Stage* pStage = Stage::create(ii + 1, this);
			m_ListStage.pushBack(pStage);
		}
	}
	else
	{
		for (int ii = 10; ii < STAGE_Infinite_max; ii++)
		{
			Stage* pStage = Stage::create(ii + 1, this);
			m_ListStage.pushBack(pStage);
		}
	}

    
}

void HelloWorld::Delete_Stage()
{
    for(int ii = 0; ii < m_ListStage.size(); ii++)
    {
        Stage* pStage = m_ListStage.at(ii);
        if(pStage != nullptr) delete pStage;
    }
    
    m_ListStage.clear();
}

bool HelloWorld::Enable_Stage(unsigned int nEnableNum)
{

	if (!m_bInfiniteMode)
	{
		for (int ii = 0; ii < m_ListStage.size(); ii++)
		{
			Stage* pStage = m_ListStage.at(ii);
			if (pStage != nullptr && ii + 1 == nEnableNum)
			{
				pStage->m_bStageEnable = true;
				return true;
			}
		}
	}
	else
	{
		for (int ii = 0; ii < m_ListStage.size(); ii++)
		{
			Stage* pStage = m_ListStage.at(ii);			
			if (pStage != nullptr && ii + 1 + 10 == nEnableNum)
			{
				pStage->m_bStageEnable = true;
				return true;
			}
		}
	}


    
    return false;
}


void HelloWorld::Init_EventListener()
{
	//add Touch event listener
    auto touchListenerAll = EventListenerTouchAllAtOnce::create();
    touchListenerAll->onTouchesBegan = CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    touchListenerAll->onTouchesMoved = CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
    touchListenerAll->onTouchesEnded = CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListenerAll, this);
    
    //Single
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan     = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    touchListener->onTouchMoved     = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    touchListener->onTouchEnded     = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    
	//add Touch event listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);	
	listener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	

	//adds Contact event listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	//adds Contact event listener
	auto contactListener_Rock = EventListenerPhysicsContact::create();
	contactListener_Rock->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin_Rock, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener_Rock, this);


	auto _mouseListener = EventListenerMouse::create();
	_mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
	_mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	_mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	_mouseListener->onMouseScroll = CC_CALLBACK_1(HelloWorld::onMouseScroll, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(_mouseListener, this);
    
}


void HelloWorld::menuCloseCallback(Ref* sender)
{
//    //m_PauseLayer->moveAdmobView(40, 40);
//
//    //Director::getInstance()->popScene();
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
    log("onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touchPoint.x, touchPoint.y);
    
    if(touch->getID() != 0) return false;
    if(m_bClicking == false) m_bClicking = true;
    
    
#if MOVE_BTN == 1
    
    
    m_bIsDown   = false;
    m_bIsUp     = false;
    Rect DownRect = m_pBtnDown->getBoundingBox();
    if(DownRect.containsPoint(touchPoint))
    {
        m_bIsDown   = true;
        m_bIsUp     = false;
    }
    
    Rect UpRect = m_pBtnUp->getBoundingBox();
    if(UpRect.containsPoint(touchPoint))
    {
        m_bIsUp     = true;
        m_bIsDown   = false;
    }
    
    if((m_bIsDown == true || m_bIsUp == true) && !this->isScheduled(schedule_selector(HelloWorld::ArcherMoveEvent)))
        this->schedule(schedule_selector(HelloWorld::ArcherMoveEvent));
#else
    
#endif

    
    return true;
}



void HelloWorld::onTouchMoved(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();

    if (m_bClicking == false || touch->getID() != 0)  return;
    
#if MOVE_BTN == 1
    
    float fCursorY = touchPoint.y;
    
    if(m_pArcher->get_ArcState() != emArcherState::Arc_Dying)
    {
        Vec2 vecPos = m_pArcher->getPosition();
        float fGabY = fCursorY - m_touchStart.y;
    }
    
    
    Rect DownRect = m_pBtnDown->getBoundingBox();
    if(DownRect.containsPoint(touchPoint))
    {
        m_bIsDown   = true;
        m_bIsUp     = false;
    }
    else
    {
        m_bIsDown   = false;
        m_bIsUp     = false;
    }
    
    Rect UpRect = m_pBtnUp->getBoundingBox();
    if(UpRect.containsPoint(touchPoint))
    {
        m_bIsUp     = true;
        m_bIsDown   = false;
    }
#else
    
#endif
    

}



void HelloWorld::onTouchEnded(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
    if(touch->getID() != 0) return;
    m_bClicking     = false;
    
    bool isTouch = m_UILayer->m_pArrBtn->getBoundingBox().containsPoint(touchPoint);
    if(isTouch)
        m_UILayer->m_pArrBtn->setTexture("button_Play.png");
    
#if MOVE_BTN == 16
    m_bIsUp     = false;
    m_bIsDown   = false;
    if(this->isScheduled(schedule_selector(HelloWorld::ArcherMoveEvent)))
        this->unschedule(schedule_selector(HelloWorld::ArcherMoveEvent));
#else
    

#endif
}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
    if(touch->getID() != 0) return;

    log("touch Cancel");
}



void HelloWorld::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    for (int ii = 0; ii < touches.size() ; ii++) {
        auto touchPoint = touches[ii]->getLocation();
        
#if MOVE_BTN == 0
        auto visibleSize = Director::getInstance()->getVisibleSize();
        if(touchPoint.x < visibleSize.width*0.33f)
        {
            m_touchStart    = Vec2(touchPoint.x, touchPoint.y);
            m_ArcPos        = m_pArcher->getPosition();
        }
        
        bool isTouch = m_UILayer->m_pArrBtn->getBoundingBox().containsPoint(touchPoint);
        if(isTouch)
        {
            pushArrowBtn();
            m_UILayer->m_pArrBtn->setTexture("button_Play_push.png");
            return;
        }
#else
        Rect DownRect = m_pBtnDown->getBoundingBox();
        if(DownRect.containsPoint(touchPoint))
        {
            m_bIsDown   = true;
            m_bIsUp     = false;
        }
        
        Rect UpRect = m_pBtnUp->getBoundingBox();
        if(UpRect.containsPoint(touchPoint))
        {
            m_bIsUp     = true;
            m_bIsDown   = false;
        }
        
        if((m_bIsDown == true || m_bIsUp == true) && !this->isScheduled(schedule_selector(HelloWorld::ArcherMoveEvent)))
        {
            this->schedule(schedule_selector(HelloWorld::ArcherMoveEvent));
            break;
        }
#endif
    }
}

void HelloWorld::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    
    for (int ii = 0; ii < touches.size() ; ii++) {
        auto touchPoint = touches[ii]->getLocation();
#if MOVE_BTN == 0
        
        auto visibleSize = Director::getInstance()->getVisibleSize();
        if(touchPoint.x < visibleSize.width*0.33f)
        {
            float offset = m_touchStart.y - touchPoint.y;

            Vec2 posArcher = getArcherPos();
            
            if(m_ArcPos.y - offset < visibleSize.height * 0.25f)
            {
                if(getArcherState() != emArcherState::Arc_Dying)
                    setArcherPos(Vec2(getArcherPos().x, visibleSize.height * 0.25f));
            }
            else if( m_ArcPos.y - offset > visibleSize.height * 0.7f )
            {
                if(getArcherState() != emArcherState::Arc_Dying)
                    setArcherPos(Vec2(getArcherPos().x, visibleSize.height * 0.7f));
            }
            else
            {
                if(getArcherState() != emArcherState::Arc_Dying)
                    setArcherPos(Vec2(getArcherPos().x, m_ArcPos.y - offset));
            }
            
        }
        
#else
        
        Rect DownRect = m_pBtnDown->getBoundingBox();
        if(DownRect.containsPoint(touchPoint))
        {
            m_bIsDown   = true;
            m_bIsUp     = false;
            log("Multi m_bIsDown true");
        }
        
        Rect UpRect = m_pBtnUp->getBoundingBox();
        if(UpRect.containsPoint(touchPoint))
        {
            m_bIsUp     = true;
            m_bIsDown   = false;
            log("Multi m_bIsUp true");
        }
#endif
    }
}

void HelloWorld::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{

    
    for (int ii = 0; ii < touches.size() ; ii++) {
        m_UILayer->m_pArrBtn->setTexture("button_Play.png");
        
        auto touchPoint = touches[ii]->getLocation();
        if(touches[ii]->getID() != 0) continue;
        
#if MOVE_BTN == 1
        Rect DownRect = m_pBtnDown->getBoundingBox();
        if(DownRect.containsPoint(touchPoint))
            m_bIsDown   = false;
        
        Rect UpRect = m_pBtnUp->getBoundingBox();
        if(UpRect.containsPoint(touchPoint))
            m_bIsUp     = false;
#else
#endif
    }

    
}


bool HelloWorld::onMouseDown(cocos2d::Event * event)
{
#ifdef pooyan

	m_bClicking = true;
#else
	e->getType();
	float fCursorX = e->getCursorX();
	float fCursorY = e->getCursorY();
	m_ptStart = Vec2(fCursorX, fCursorY);



	m_bClicking = true;

	this->schedule(schedule_selector(HelloWorld::callEveryFrame));
	//log("onMouseDown - x : %f // y : %f\n", fCursorX, fCursorY);
#endif

	
	return false;
}

void HelloWorld::onMouseMove(cocos2d::Event * event)
{


#ifdef pooyan
	if (m_bClicking == false)  return;
	EventMouse* e = (EventMouse*)event;
	float fCursorX = e->getCursorX();
	float fCursorY = e->getCursorY();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if(m_pArcher->get_ArcState() != emArcherState::Arc_Dying && fCursorX < visibleSize.width * 0.35)
	{
		Vec2 vecPos = m_pArcher->getPosition();
		m_pArcher->Set_TouchPosition(vecPos.x, fCursorY);
	}
#else
	if (m_bClicking == false)  return;
	EventMouse* e = (EventMouse*)event;
	float fCursorX = e->getCursorX();
	float fCursorY = e->getCursorY();
#endif



	//log("onMouseMove - x : %f // y : %f\n", fCursorX, fCursorY);
}

void HelloWorld::onMouseUp(cocos2d::Event * event)
{

#ifdef pooyan
	EventMouse* e = (EventMouse*)event;
	float fCursorX = e->getCursorX();
	float fCursorY = e->getCursorY();

	m_ptEnd = Vec2(fCursorX, fCursorY);

	m_bClicking = false;
#else
	EventMouse* e = (EventMouse*)event;
	float fCursorX = e->getCursorX();
	float fCursorY = e->getCursorY();

	m_ptEnd = Vec2(fCursorX, fCursorY);
	Vec2 dirDart = (m_ptEnd - m_ptStart);
	dirDart = dirDart.getNormalized();
	float fradian = Vec2(1, 0).getAngle(dirDart);
	float fAngle = RadiToAngle(fradian);
	float fDistance = m_ptStart.getDistance(m_ptEnd);
	float fTime = m_fTime;
	float fSpeed = fDistance / fTime;

	//터치 다운/업 속도에 따라 다트의 속도 변경
	m_fVelocityX = (fSpeed - Std_SPEED) / 10 + Std_VELOCITY;

	//m_fForceX 값은 그대로 두고 m_fForceY 값만 바꾼다.
	float offset = fAngle * 10;
	m_fForceY = offset;


	//-----------------------------------
	//Dart Apply PhysicsBody  	
	PhysicsMaterial PhyMat = PhysicsMaterial(1.0f, 0.0f, 1.0f);
	//PhysicsBody* pbody_Dart = PhysicsBody::createBox(m_pSprtDart->getContentSize(), PhyMat);
	//pbody_Dart->setDynamic(false);
	pbody_Dart->setContactTestBitmask(0xFFFFFFFF);
	pbody_Dart->applyForce(Vec2(m_fForceX, m_fForceY) * 400.0f);
	pbody_Dart->setVelocity(Vec2(m_fVelocityX, 0.0f));

	//m_pSprtDart->setPhysicsBody(pbody_Dart);

	this->unschedule(schedule_selector(HelloWorld::callEveryFrame));

	//값 복귀
	m_fTime = 0;
	m_fVelocityX = Std_VELOCITY;
	m_fForceX = Std_FORCE_X;
	m_fForceY = Std_FORCE_Y;
	m_bClicking = false;

#endif
	

}

void HelloWorld::onMouseScroll(cocos2d::Event * event)
{
	//
	//m_fTime = 0.0f;
	//m_label0->setString("0.00");

	log("==================================================");
	log("m_fForceX - %f // m_fForceY - %f", m_fForceX, m_fForceY);
	log("m_fVelocityX - %f ", m_fVelocityX);
	log("==================================================");
}

// Implementation of the keyboard event callback function prototype
void HelloWorld::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	log("Key with keycode %d pressed", keyCode);

	if ((int)keyCode == 7) //BAckspace
	{

	}
	else if ((int)keyCode == 28) // '↑'
	{
		m_bIsUp = true;
		m_bIsDown = false;
		this->schedule(schedule_selector(HelloWorld::updateScene), 0.1f);
	}		
	else if ((int)keyCode == 29) // '↓'
	{
		m_bIsUp = false;
		m_bIsDown = true;
		this->schedule(schedule_selector(HelloWorld::updateScene), 0.1f);
	}
	else if ((int)keyCode == 14) //'Ctrl'
	{	
		m_pArcher->ShootArrow();
	}
	else if ((int)keyCode == 136) //'m'
	{
	}


	log("==================================================");
	log("m_fForceX - %f // m_fForceY - %f", m_fForceX, m_fForceY);
	log("m_fVelocityX - %f ", m_fVelocityX);
	log("==================================================");
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event * event)
{
	if ((int)keyCode == 28) // '↑'
	{
		m_bIsUp = false;
		this->unschedule(schedule_selector(HelloWorld::updateScene));
		m_pArcher->stopAllActions();
		
		//이동
		int nDirVal = 1;
		auto Action = cocos2d::MoveBy::create(0.1f, Vec2(0.0f, nDirVal * 7.0f));
		m_pArcher->runAction(Action);
	}
	else if ((int)keyCode == 29) // '↓'
	{
		m_bIsDown = false;		
		this->unschedule(schedule_selector(HelloWorld::updateScene));
		m_pArcher->stopAllActions();

		//이동
		int nDirVal = -1;
		auto Action = cocos2d::MoveBy::create(0.1f, Vec2(0.0f, nDirVal * 7.0f));
		m_pArcher->runAction(Action);
	}
}


//Sprite Collision
bool HelloWorld::onContactBegin(PhysicsContact& contact)
{
	//Only Balloon, BalloonMaker and Arrow pass
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA == NULL || nodeB == NULL) return false;		
	if (nodeA->getTag() == nodeB->getTag()) return false;
	if (nodeA->getTag() != emNameTag::ARROW && nodeB->getTag() != emNameTag::ARROW) return false;
	if (nodeA->getTag() != emNameTag::ARROW && nodeA->getTag() != emNameTag::BALLOON && nodeA->getTag() != emNameTag::BALLOONMAKER) return false;
	if (nodeB->getTag() != emNameTag::ARROW && nodeB->getTag() != emNameTag::BALLOON && nodeB->getTag() != emNameTag::BALLOONMAKER) return false;

	log("%d and %d is Collided ", nodeA->getTag(), nodeB->getTag());

	
	Balloon* pBalloon = NULL; Arrow* pArr = NULL;
	BalloonMaker* pBM = NULL;	

	if (nodeA->getTag() == emNameTag::BALLOONMAKER) pBM	= dynamic_cast<BalloonMaker*>(nodeA);
	else if (nodeA->getTag() == emNameTag::BALLOON) pBalloon = dynamic_cast<Balloon*>(nodeA);
	else if (nodeA->getTag() == emNameTag::ARROW) pArr = dynamic_cast<Arrow*>(nodeA);

	if (nodeB->getTag() == emNameTag::BALLOONMAKER) pBM = dynamic_cast<BalloonMaker*>(nodeB);
	else if (nodeB->getTag() == emNameTag::BALLOON) pBalloon = dynamic_cast<Balloon*>(nodeB);
	else if (nodeB->getTag() == emNameTag::ARROW) pArr = dynamic_cast<Arrow*>(nodeB);
	
	if ( pArr == NULL || (pBalloon == NULL && pBM == NULL) ) return false;
	if (pBalloon != NULL) if(pBalloon->getReferenceCount() < 1) return false;
	if (pBM != NULL) if( pBM->getReferenceCount() < 1 ) return false;


	if(pArr->get_ArrowState() == emArrowState::Arr_Shooting)
	{	
		if (pBalloon != nullptr) //exception excute
		{
            pArr->HitArrow();
            
            Vec2 pos = pBalloon->getPosition();
            ActionTexture* ActTexture = nullptr;
            if(pBalloon->get_BalloonState() == emBalloonState::Balloon_Free)
            {
                ActTexture = ActionTexture::create(pos, emActionTextureType::AT_Number, this);
                ActTexture->SetNumLabel(SCORE_BALLOON, SMALL_SIZE, Color3B(255,130,100));
                Plus_StageScore(SCORE_BALLOON);
                this->removeChild(pBalloon, true);
                ActTexture->showActionTexture();
                return true;
            }
            
            BalloonMaker* itBM = pBalloon->get_BalloonMaker();
            if (itBM == nullptr || itBM->getReferenceCount() < 1) return false;
            
            itBM->eraseBalloonInList(pBalloon);
            this->removeChild(pBalloon, true);
            
            if(itBM->m_emType == emMakerType::ORC)
                itBM->updateSpeedAndMove(itBM->get_BalloonCount());
            
            if(itBM->get_BalloonCount() > 0) return true;
            
            ActTexture = ActionTexture::create(pos, emActionTextureType::AT_Number, this);
            if(pBalloon->get_BalloonState() == emBalloonState::Balloon_Free){
                ActTexture->SetNumLabel(SCORE_BALLOON, SMALL_SIZE, Color3B(255,130,100));
                Plus_StageScore(SCORE_BALLOON);
            }
            else{
                ActTexture->SetNumLabel(SCORE_BM, NORMAL_SIZE, Color3B(255,130,100));
                Plus_StageScore(SCORE_BM);
            }
            ActTexture->showActionTexture();
        
            itBM->Fall_BalloonMaker();
            MC_SoundBox::getInstance()->tryVibration(0.1);
		}
		else if (pBM != NULL)
		{
			pArr->FallingArrow();
		}
		
	}
	
	return true;
}

bool HelloWorld::onContactBegin_Rock(cocos2d::PhysicsContact & contact)
{

	//Only Arrow, Archer and Rock pass
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA == NULL || nodeB == NULL) return false;
	if (nodeA->getTag() == nodeB->getTag()) return false;	
	if (nodeA->getTag() != emNameTag::ARROW && nodeA->getTag() != emNameTag::PROJECTTILE && nodeA->getTag() != emNameTag::ARCHER) return false;
	if (nodeB->getTag() != emNameTag::ARROW && nodeB->getTag() != emNameTag::PROJECTTILE && nodeB->getTag() != emNameTag::ARCHER) return false;
	
	Archer* pArcher = NULL; Arrow* pArr = NULL;
	Rock* pRock = NULL;
    cocos2d::Sprite* pSprite = nullptr;

    if (nodeA->getTag() == emNameTag::PROJECTTILE) pSprite = dynamic_cast<Sprite*>(nodeA);
	if (nodeA->getTag() == emNameTag::PROJECTTILE) pRock = dynamic_cast<Rock*>(nodeA);
	else if (nodeA->getTag() == emNameTag::ARCHER) pArcher = dynamic_cast<Archer*>(nodeA);
	else if (nodeA->getTag() == emNameTag::ARROW) pArr = dynamic_cast<Arrow*>(nodeA);

    if (nodeB->getTag() == emNameTag::PROJECTTILE) pSprite = dynamic_cast<Sprite*>(nodeB);
	if (nodeB->getTag() == emNameTag::PROJECTTILE) pRock = dynamic_cast<Rock*>(nodeB);
	else if (nodeB->getTag() == emNameTag::ARCHER) pArcher = dynamic_cast<Archer*>(nodeB);
	else if (nodeB->getTag() == emNameTag::ARROW) pArr = dynamic_cast<Arrow*>(nodeB);

	if ((pRock == NULL && pSprite == NULL) || (pArcher == NULL && pArr == NULL)) return false;
	if (pArcher != NULL) if (pArcher->getReferenceCount() < 1) return false;
	if (pRock != NULL) if (pRock->getReferenceCount() < 1) return false;
	if (pArr != NULL) if (pArr->getReferenceCount() < 1) return false;

    
	if ( (pArcher != NULL && pSprite != NULL) || pRock != nullptr && pRock->get_RockState() == emRockState::Rock_Throwing )
	{
		if (pArcher != NULL && (pRock != nullptr || pSprite->getTag() == emNameTag::PROJECTTILE)) //when Rock hit Archer
		{
            archerDie();
		}
		else if (pArr != NULL && pArr->get_ArrowState() == emArrowState::Arr_Shooting)
		{
            Vec2 pos = pRock->getPosition();
            ActionTexture* ActTexture = ActionTexture::create(pos, emActionTextureType::AT_Number, this);
            ActTexture->SetNumLabel(SCORE_ROCK, SMALL_SIZE, Color3B(10,60,170));
            ActTexture->showActionTexture();
            
			pRock->explode_Rock();
			pArr->HitArrow();
            
            Plus_StageScore(SCORE_ROCK);
		}
	}

	return true;
}

void HelloWorld::callEveryFrame(float f)
{
}

void HelloWorld::updateScene(float f)
{

	Vec2 pos = Vec2::ZERO;
	if (m_bIsUp)
	{	
		m_pArcher->stopAllActions();
		//이동
		int nDirVal = 1;
		auto Action = cocos2d::MoveBy::create(7.0f, Vec2(0.0f, nDirVal * 750.0f));
		m_pArcher->runAction(Action);
		
	}
	else if(m_bIsDown)
	{
		m_pArcher->stopAllActions();
		//이동
		int nDirVal = -1;
		auto Action = cocos2d::MoveBy::create(7.0f, Vec2(0.0f, nDirVal * 750.0f));
		m_pArcher->runAction(Action);
	}
}

void HelloWorld::update(float delta)
{
}

void HelloWorld::menuGameStartBtn(Ref* sender)
{
    //Start Game
    //Init_Stage();
    Start_Stage(STAGE_1st_ONE);
    
//    if(get_EnableStage() == nullptr)
//    {
//        //Start Here, modify m_pStage -> m_ListStage
//        m_pStage = Stage::create(1, this);
//        this->addChild(m_pStage);
//
//        m_pGameStartBtn->setVisible(false);
//    }
//    else
//    {
//        m_pStage->init_Current();
//    }
    
    BB_GameInfo::GetInstance()->setGameState(emGameOn);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //exit(0);
#endif
}

bool HelloWorld::pushArrowBtn()
{
    log("pushArrowBtn");
    //Shoot Arrow;
    if(m_pArcher->get_ArcState() != emArcherState::Arc_Dying)
    {
        if(m_pArcher->ShootArrow() == 1)
			m_pArcher->setArcherAnimation(1);
    }
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //exit(0);
#endif
	MC_SoundBox::getInstance()->playSound(SOUND_PATH_ARRSHOTING);
    
    return true;
}

void HelloWorld::MoveUp(float fd)
{
    Vec2 pos = m_pArcher->getPosition();
    m_pArcher->setPosition(Vec2(pos.x, pos.y+1));
}

void HelloWorld::ArcherMoveEvent(float fd)
{
    if(m_pArcher->get_ArcState() == emArcherState::Arc_Dying) return;
        
    Vec2 pos = m_pArcher->getPosition();
    if(m_bIsDown == true)
    {
        //log("archer down");
        m_pArcher->setPosition(Vec2(pos.x, pos.y-1));
    }
    else if(m_bIsUp == true)
    {
        //log("archer up");
        m_pArcher->setPosition(Vec2(pos.x, pos.y+1));
    }
}

void HelloWorld::erase_childBalloon()
{
    Vector<Node*> childList = this->getChildren();
    
    for(int ii = 0; ii < childList.size(); ii++)
    {
        Balloon* pBalloon = dynamic_cast<Balloon*>(childList.at(ii));
        if(pBalloon == NULL) continue;
        
        this->removeChild(pBalloon);
    }
}

void HelloWorld::erase_childRock()
{
    Vector<Node*> childList = this->getChildren();
    
    for(int ii = 0; ii < childList.size(); ii++)
    {
        Rock* pRock = dynamic_cast<Rock*>(childList.at(ii));
        if(pRock == NULL) continue;
        
        this->removeChild(pRock);
    }
}

void HelloWorld::erase_childSprite()
{
    erase_childRock();
    erase_childBalloon();
}


bool HelloWorld::set_AnimationScene(unsigned int _unAniNum)
{
    if(m_pAniScene == nullptr) return false;
    m_pAniScene->init_stageAnimation(_unAniNum);
    m_pAniScene->startGameScene_AfterSec(4.0f);
    
    return true;
}

bool HelloWorld::Start_Stage(unsigned int _unStageNum)
{
    Enable_Stage(_unStageNum);
    
    Stage* pCurEnableStage = get_EnableStage();
    if(pCurEnableStage == NULL) return false;
    
    this->addChild(pCurEnableStage);
    pCurEnableStage->Start_Stage();
    m_pGameStartBtn->setVisible(false);
    
    set_Background(_unStageNum);
    
    return true;
}

bool HelloWorld::set_Background(unsigned int _unStageNum)
{
    
    if(_unStageNum == STAGE_1st_ONE)      Director::getInstance()->setClearColor(Color4F(0.7, 0.92, 0.96, 0));
    else if(_unStageNum == STAGE_1st_ONE)   Director::getInstance()->setClearColor(Color4F(0.7, 0.92, 0.96, 0));
    else if(_unStageNum == STAGE_1st_TWO)   Director::getInstance()->setClearColor(Color4F(0.3, 0.6, 0.96, 0));
    else{
        Director::getInstance()->setClearColor(Color4F(0.9, 0.8, 0.96, 0));
    }    
    
    return true;
}


bool HelloWorld::set_GameSceneSprite(int _1stStageNum ,int _2ndStageNum) //ex) 2-1, set_GameSceneSprite(2,1);
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    Vec2 pos = Vec2(Vec2((visibleSize.width / 2) - 350, (visibleSize.height / 2) + 80));
    
    if(_1stStageNum == STAGE_1st_ONE){
        emStageType StageType = Get_CurrentStage()->getStageType();
        if(StageType == emStageType::SType_FlyingUp)
        {
            m_pBMrock->setVisible(true);
        }
        else{
            m_pBMrock->setVisible(true);
        }
    }
    else if(_1stStageNum == STAGE_1st_TWO){
        
    }
    else if(_1stStageNum == STAGE_1st_THREE){
    }
    else if(_1stStageNum == STAGE_1st_FOUR){
    }
    else{
        Director::getInstance()->setClearColor(Color4F(0.9, 0.8, 0.96, 0));
    }
    
    return true;
}


void HelloWorld::setBMRockGravity(bool _gravity)
{
    if(_gravity)
    {
        PhysicsBody* pbody_Rock = PhysicsBody::createBox(m_pBMrock->getContentSize(), ROCK_MATERIAL);
        pbody_Rock->setContactTestBitmask(0xFFFFFFFF);
        pbody_Rock->applyForce(Vec2(0.0f, 50.0f));
        pbody_Rock->setVelocity(Vec2(-20, 0));
        m_pBMrock->setPhysicsBody(pbody_Rock);
        m_pBMrock->getPhysicsBody()->setGravityEnable(true);
    }
    else
    {
        PhysicsBody* pbody_Rock = PhysicsBody::createBox(m_pBMrock->getContentSize(), ROCK_MATERIAL);
        pbody_Rock->setContactTestBitmask(0xFFFFFFFF);
        pbody_Rock->applyForce(Vec2(0.0f, 0.0f));
        pbody_Rock->setVelocity(Vec2(0, 0));
        m_pBMrock->setPhysicsBody(pbody_Rock);
        m_pBMrock->getPhysicsBody()->setGravityEnable(false);
    }
}



void HelloWorld::archerDie() { 
    StageValue* pCurStageV = get_EnableStage()->getCurrentStageVal();
    BB_GameInfo* pGameInfo = BB_GameInfo::GetInstance();
    unsigned int nLifeCnt = pGameInfo->getLifeCnt();
    
    Archer*    pArcher = m_pArcher;
    if (nLifeCnt < 1 && pArcher->get_ArcState() != emArcherState::Arc_Dying)
    {
        if(pCurStageV->getStageState() != emStageState::SState_GameOver)
            pCurStageV->SetStageState(emStageState::SState_GameOver);
    }
    
    pArcher->set_ArcState(emArcherState::Arc_Dying);
    pArcher->DyingAction();
    
    if(pCurStageV->getStageState() != emStageState::SState_GameOver)
        pCurStageV->SetStageState(emStageState::SState_UserDie);
}
