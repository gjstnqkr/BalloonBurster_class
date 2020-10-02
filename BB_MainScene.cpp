//
//  BB_GameScene.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 7. 4..
//

#include "BB_MainScene.hpp"
#include "MC_FileUtils.hpp"
#include "MC_GameOption.hpp"
#include "BB_GameInfo.h"
#include "BB_AnimationScene.hpp"
#include "RankViewLayer.hpp"
#include "Arrow.h"
#include "MC_SoundBox.hpp"

#include "audio/include/SimpleAudioEngine.h"


class CLogoScene;

#define Height_Gap 35.00
#define TAG_CLOUD_BATCHNODE 50

USING_NS_CC;


// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


Scene* CMainScene::createScene()
{
    return CMainScene::create();
}

// on "init" you need to initialize your instance
bool CMainScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    //Single
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan     = CC_CALLBACK_2(CMainScene::onTouchBegan, this);
    touchListener->onTouchMoved     = CC_CALLBACK_2(CMainScene::onTouchMoved, this);
    touchListener->onTouchEnded     = CC_CALLBACK_2(CMainScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    
    //adds Contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(CMainScene::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    
    //Set BackGround Color
    Director::getInstance()->setClearColor(Color4F(0.7, 0.92, 0.96, 0));
    
    setSceneState(emSceneState::Scene_Init);
    init_ui();
    init_value();
    init_sprite();
    init_layer();
    init_option();
    
    this->scheduleUpdate();
    this->initWithPhysics();
    this->schedule(schedule_selector(CMainScene::schedulerShootArr), 3.0f);
    
    setSceneState(emSceneState::Scene_None);
    
    //m_pRankView
    m_pRankView  = nullptr;
    return true;
}

bool CMainScene::init_value()
{
    m_SceneState = emSceneState::Scene_None;
    m_HelloWordScene = nullptr;
    m_AnimationScene = nullptr;
    return true;
}

bool CMainScene::init_ui()
{
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    
    // add "HelloWorld" splash screen"
    auto spt_bgHome = Sprite::create("BG_Main.png");
    
    m_GameOptionItem = MenuItemImage::create(
                                        "button_Option.png",
                                        "button_Option_push.png",
                                        CC_CALLBACK_1(CMainScene::menuGameOptionCallback,this));
    m_StartGameItem = MenuItemImage::create(
                                            "main_START.png",
                                            "main_START_push.png",
                                            CC_CALLBACK_1(CMainScene::menuStartGameCallback,this));

	m_InfiniteModeItem = MenuItemImage::create(
											"main_START.png",
											"main_START_push.png",
											CC_CALLBACK_1(CMainScene::menuInfiniteModeCallback, this));
    
    if (spt_bgHome == nullptr || m_GameOptionItem == nullptr || m_StartGameItem == nullptr || m_InfiniteModeItem == nullptr)
    {
        problemLoading("'MainScene Image Load Error'");
        return false;
    }
    else
    {
        // Back Ground(full Screen rate)
        spt_bgHome->setAnchorPoint(Vec2(0,0));
        spt_bgHome->setPosition(origin);
        
        CCSize tSizeOrig = spt_bgHome->getContentSize();
        
        float ScaleX = 1.0f;
        float ScaleY = 1.0f;
        
        spt_bgHome->setScaleX(ScaleX);
        spt_bgHome->setScaleY(ScaleY);
        this->addChild(spt_bgHome, 0);
        
    
        
        // Close Btn
        float   fBtnOffset          = visibleSize.height * UI_TOP_START_RATE;
        CCSize  orgSize_CloseBtn    = m_GameOptionItem->getContentSize();
        m_GameOptionItem->setPosition(origin + visibleSize - Vec2(fBtnOffset, fBtnOffset) - orgSize_CloseBtn*0.5);
        //m_GameOptionItem->setPosition(origin + Vec2(0 , visibleSize.height) + Vec2(fBtnOffset, fBtnOffset) + orgSize_CloseBtn*0.5);
        m_GameOptionItem->setAnchorPoint(Vec2(0.0,0.0));
        m_GameOptionItem->setScale(0.5);
        
        auto menuClose = Menu::create(m_GameOptionItem, nullptr);
        menuClose->setPosition(Vec2::ZERO);
        this->addChild(menuClose, 1);
        //
        
        
        // Game Start Btn
        float fBtnPosY = visibleSize.height * 0.5;
        float fBtnPosX = visibleSize.width * 0.5;
        CCSize orgSize_StartBtn = m_StartGameItem->getContentSize();
        m_StartGameItem->setPosition(origin + Vec2(fBtnPosX, fBtnPosY) - orgSize_StartBtn/2);
        m_StartGameItem->setAnchorPoint(Vec2(0,0));
        m_StartGameItem->setScale(1.0);
        
        auto menuStartGame = Menu::create(m_StartGameItem, nullptr);
        menuStartGame->setPosition(Vec2::ZERO);
        this->addChild(menuStartGame, 1);

		// Infinite Mode Btn
		fBtnPosY = visibleSize.height * 0.5;
		fBtnPosX = visibleSize.width * 0.5;
		orgSize_StartBtn = m_InfiniteModeItem->getContentSize();
		m_InfiniteModeItem->setPosition(origin + Vec2(fBtnPosX, fBtnPosY - orgSize_StartBtn.height/2 + 20) - orgSize_StartBtn / 2);
		m_InfiniteModeItem->setAnchorPoint(Vec2(0, 0));
		m_InfiniteModeItem->setScale(0.7);

		auto menuInifiMode = Menu::create(m_InfiniteModeItem, nullptr);
		menuInifiMode->setPosition(Vec2::ZERO);
		this->addChild(menuInifiMode, 1);


    
    }


    //Bounding Box
    //this->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    return true;
}

bool CMainScene::init_sprite()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    
    //--------------------------------------
    //insert Archer
    Vec2 pos = Vec2(Vec2((visibleSize.width / 2) - 300, (visibleSize.height / 2) - 200 ));
    Archer* pArcher = Archer::create(pos);
    pArcher->setScale(4.0f);
    this->addChild(pArcher, 1);
    m_pArcher = pArcher;
    m_pArcher->init_ArcherState();

    
    //--------------------------------------
    //insert Cluod Batch Node
#if	USE_VISUALSTUDIO == 1
    auto batch = SpriteBatchNode::create("image/clowdlarge.png");
#else
	auto batch = SpriteBatchNode::create("clowdlarge.png");
#endif

    addChild(batch,0,TAG_CLOUD_BATCHNODE);
    
    auto spt1 = Sprite::createWithTexture(batch->getTexture());
    auto spt2 = Sprite::createWithTexture(batch->getTexture());
    auto spt3 = Sprite::createWithTexture(batch->getTexture());
    auto spt4 = Sprite::createWithTexture(batch->getTexture());
    auto spt5 = Sprite::createWithTexture(batch->getTexture());
    auto spt6 = Sprite::createWithTexture(batch->getTexture());
    
    
    m_vecCloud.pushBack(spt1);
    m_vecCloud.pushBack(spt2);
    m_vecCloud.pushBack(spt3);
    m_vecCloud.pushBack(spt4);
    m_vecCloud.pushBack(spt5);
    m_vecCloud.pushBack(spt6);
    
    for(int ii = 0; ii < m_vecCloud.size(); ii++)
    {
        Sprite* itor = m_vecCloud.at(ii);
        
        int valueX = RandomHelper::random_int(1, 10);
        float fRandomX = 0.1 * valueX;
        int valueY = RandomHelper::random_int(3, 10);
        float fRandomY = 0.1 * valueY;
        itor->setPosition(visibleSize.width * fRandomX, visibleSize.height * fRandomY);
        
        int value = RandomHelper::random_int(3, 10);
        float fRanScale = 0.1 * value;
        itor->setScale(fRanScale);
        
        batch->addChild(itor,0);
    }
    
    
    //--------------------------------------
    //insert Cluod Batch Node
    int BallonCnt = 10;
    for(int ii = 0; ii < BallonCnt; ii++)
    {
        float valueX = RandomHelper::random_real(4.0f, 10.0f);
        int val = RandomHelper::random_int(1, 10);
        float fRandomX = 0.1 * valueX;
        Balloon* pBalloon = Balloon::create(Vec2(visibleSize.width * fRandomX, -20*val), 1.0f * fRandomX);
        addChild(pBalloon, 0);
        pBalloon->set_UpSpeed(RandomHelper::random_int(200, 350));
        pBalloon->set_MoveDistance(200);
        pBalloon->Fly_Balloon(10);
        
        int value = RandomHelper::random_real(1.0f, 2.0f);
        pBalloon->setScale(value);
        m_listBalloon.pushBack(pBalloon);
    }
    
    //---------------------------------------
    //test Sprite
#if	USE_VISUALSTUDIO == 1
	m_pTestSprite = cocos2d::Sprite::create("button_Up.png");
#else
	m_pTestSprite = cocos2d::Sprite::create("button_Up.png");	
#endif

    float fBtnOffset            = visibleSize.height * UI_LEFT_START_RATE;
    CCSize  orgSize_UpBtn       = m_pTestSprite->getContentSize();
    m_pTestSprite->setPosition(origin + Vec2(fBtnOffset, fBtnOffset) + Vec2( 0, orgSize_UpBtn.height + fBtnOffset*0.35f) );
    m_pTestSprite->setAnchorPoint(Vec2(0.5,0.5));
    m_pTestSprite->setScale(1.0);
    
    this->addChild(m_pTestSprite, Z_SCENE_UI);

    return true;
}

bool CMainScene::init_layer()
{
    if(m_pTextInsert == NULL)
    {
        m_pTextInsert = TextInsertLayer::create(this);
        this->addChild(m_pTextInsert);
        m_pTextInsert->showLayer(false);
    }
    
    Init_PauseLayer();
    
    return true;
}

bool CMainScene::Init_PauseLayer()
{
    if(m_PauseLayer == NULL)
    {
        auto PauseLayer = PauseLayer::create(emUserOption::MAINPAGE);
        this->addChild(PauseLayer);
        m_PauseLayer = PauseLayer;
        m_PauseLayer->VisiblePauseLayer(false);
        return true;
    }
    
    return false;
}

void CMainScene::setSceneState(int nSceneState)
{
    m_SceneState = (emSceneState)nSceneState;
}

void CMainScene::setStartMenuEnable(bool bEnable)
{
    m_StartGameItem->setEnabled(bEnable);
}

int CMainScene::getSceneState()
{
    return (int)m_SceneState;
}

void CMainScene::NextScene(float _fSec)
{
    //Director::getInstance()->popScene();
}

void CMainScene::update(float dt)
{
    Vec2 Pos = m_pArcher->get_ArcSpineBonePos("Left_Arm_HandPt");
    float fRotX = m_pArcher->get_ArcSpineBoneRotX("Left_Arm");
    float fRotY = m_pArcher->get_ArcSpineBoneRotY("Left_Arm");
    
    
    
    //Pos
    m_pTestSprite->setScale(0.1);
    m_pTestSprite->setPosition(Pos);
    //m_pTestSprite->setRotation(-fRotY + 95.46f);
    //m_pTestSprite->setRotationSkewX(fRotX);
    //m_pTestSprite->setRotationSkewY(fRotY);
    
    //log("fRot X / Y - %f / %f", fRotX, fRotY);
    
}


void CMainScene::menuGameOptionCallback(Ref* sender)
{
    //- - - - - - - - - - - - - - - - - -
    //Rank Test
//    if(m_pRankView == nullptr)
//    {
//        m_pRankView = RankViewLayer::create(this);
//        this->addChild(m_pRankView);
//        m_pRankView->init();
//    }
    
    
    //- - - - - - - - - - - - - - - - - -
    //Spine Animation Test
//    m_pArcher->setArcherAnimation(1);
//    m_pArcher->ShootArrow();
    

    
    
    
    //- - - - - - - - - - - - - - - - - -
    //GameOption start
    
    if(m_PauseLayer != NULL && !m_PauseLayer->isVisible())
    {
        m_PauseLayer->VisiblePauseLayer(true);
        m_PauseLayer->setLayerState(emLayerState::Layer_Play);
    }
    else
    {
        m_PauseLayer->VisiblePauseLayer(false);
        m_PauseLayer->setLayerState(emLayerState::Layer_Stop);
    }
     
    //GameOption end
    //- - - - - - - - - - - - - - - - - -
    
    
    
    

    //- - - - - - - - - - - - - - - - - -
    //IOS EXIT - Start
    //Director::getInstance()->end();

    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //exit(0);
#endif
    
    //IOS EXIT - end
}

void CMainScene::menuStartGameCallback(Ref* sender)
{
    if(getSceneState() != emSceneState::Scene_None) return;
    if(m_PauseLayer->getLayerState() == emLayerState::Layer_Play) return;
    if(m_pTextInsert->getLayerState() == emLayerState::Layer_Play) return;
    
    
    bool isUserFile = MC_FileUtils::init_UserInfo();
    

    if(!isUserFile)
    {
        m_pTextInsert->showLayer(true);
        setStartMenuEnable(false);
        setSceneState(emSceneState::Scene_Stop);
    }
    else
    {
        
        BB_GameInfo::GetInstance()->setGameState(emGameOn); //When Game Started, This Line must On
        playGameScene();
    }
}

void CMainScene::menuInfiniteModeCallback(Ref* sender)
{
	if (getSceneState() != emSceneState::Scene_None) return;
	if (m_PauseLayer->getLayerState() == emLayerState::Layer_Play) return;
	if (m_pTextInsert->getLayerState() == emLayerState::Layer_Play) return;


	bool isUserFile = MC_FileUtils::init_UserInfo();


	if (!isUserFile)
	{
		m_pTextInsert->showLayer(true);
		setStartMenuEnable(false);
		setSceneState(emSceneState::Scene_Stop);
	}
	else
	{
		BB_GameInfo::GetInstance()->setGameState(emGameOn); //When Game Started, This Line must On
		playInfiniteMode();
	}
}


bool CMainScene::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
    
    return false;
}

void CMainScene::onTouchMoved(Touch* touch, Event* event)
{


    //log("Multi touch move");
}

void CMainScene::onTouchEnded(Touch* touch, Event* event)
{

    //log("Multi touch end");
}


bool CMainScene::playGameScene()
{
    if(m_SceneState == emSceneState::Scene_None)
    {
//        if(m_AnimationScene == nullptr)
        m_AnimationScene = CAnimationScene::create(); // NeedToCheck, Check whether Cocos2d dealloc this class.
        
        Director::getInstance()->pushScene(m_AnimationScene);
        return true;
    }
    
    return false;
}

bool CMainScene::playInfiniteMode()
{
	if (m_SceneState == emSceneState::Scene_None)
	{		
		m_HelloWordScene = HelloWorld::CreateScene(true); // NeedToCheck, Check whether Cocos2d dealloc this class.
		HelloWorld* pHello = dynamic_cast<HelloWorld*>(m_HelloWordScene); //Error pHello Null Ptr
		pHello->Start_Stage(STAGE_Infinite_01);
		Director::getInstance()->pushScene(m_HelloWordScene);
		return true;
	}

	return false;
}


void CMainScene::schedulerShootArr(float f)
{
    m_pArcher->setArcherAnimation(1);
    m_pArcher->ShootArrow();
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    for(int ii = 0; ii < m_vecCloud.size(); ii++)
    {
        Sprite* itor = m_vecCloud.at(ii);
        Vec2 vec = itor->getPosition();
        float fRandom = CCRANDOM_0_1();
        if(vec.x > SCREEN_WIDTH)
        {
            if(fRandom < 0.3) fRandom = fRandom * 2;
            int valueY = RandomHelper::random_int(3, 10);
            float fRandomY = 0.1 * valueY;
            itor->setPosition(-(visibleSize.width * fRandom), visibleSize.height * fRandomY);
        }
        
        int nCloudSpeed = RandomHelper::random_int(500, 700);
        auto Action = cocos2d::MoveBy::create(nCloudSpeed, Vec2(SCREEN_WIDTH, 0.0f));
        itor->runAction(Action);
        
    }
    
    for(int ii = 0; ii < m_listBalloon.size(); ii++)
    {
        Balloon* pBalloon = m_listBalloon.at(ii);
        Vec2 vec = pBalloon->getPosition();
        if(vec.y > SCREEN_HEIGHT)
        {
            float valueX = RandomHelper::random_real(4.0f, 10.0f);
            int val = RandomHelper::random_int(1, 10);
            float fRandomX = 0.1 * valueX;
            pBalloon->setPosition( Vec2(visibleSize.width * fRandomX, -20*val) );
            pBalloon->set_UpSpeed(RandomHelper::random_int(150, 350));
            pBalloon->set_MoveDistance(200);
            pBalloon->Fly_Balloon(10);
            int value = RandomHelper::random_real(1.0f, 2.0f);
            pBalloon->setScale(value);
        }
    }
    
    
    log("Arrow");
    return;
}


//Sprite Collision
bool CMainScene::onContactBegin(PhysicsContact& contact)
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
    
    if (nodeA->getTag() == emNameTag::BALLOONMAKER) pBM    = dynamic_cast<BalloonMaker*>(nodeA);
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
        if (pBalloon != NULL) //exception excute
        {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            auto origin = Director::getInstance()->getVisibleOrigin();
         
            
            float valueX = RandomHelper::random_real(4.0f, 10.0f);
            int val = RandomHelper::random_int(1, 10);
            float fRandomX = 0.1 * valueX;
            pBalloon->setPosition( Vec2(visibleSize.width * fRandomX, -20*val) );
            pBalloon->set_UpSpeed(RandomHelper::random_int(15, 20));
            pBalloon->Fly_Balloon(10);
            int value = RandomHelper::random_real(1.0f, 2.0f);
            pBalloon->setScale(value);
        }
        
    }
    
    return true;
}

bool CMainScene::init_option() {
    bool isOptionFile = MC_FileUtils::init_OptionInfo();
    m_PauseLayer->updateUI();
    init_music();
	return true;
}

bool CMainScene::init_music() {
    MC_SoundBox* pSoundbox = MC_SoundBox::getInstance();
    pSoundbox->init_sound();
    
    
    return true;
}

