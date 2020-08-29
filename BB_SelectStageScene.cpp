//
//  BB_SeleteStageScene.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 08/01/2020.
//

/*
#include "BB_SelectStageScene.hpp"



USING_NS_CC;




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
    
    if (spt_bgHome == nullptr || m_GameOptionItem == nullptr || m_StartGameItem == nullptr )
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
    auto batch = SpriteBatchNode::create("clowdlarge.png");
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
        Balloon* pBalloon = Balloon::create(Vec2(visibleSize.width * fRandomX, -20*val));
        addChild(pBalloon, 0);
        pBalloon->set_UpSpeed(RandomHelper::random_int(15, 20));
        pBalloon->set_MoveDistance(500);
        pBalloon->Fly_Balloon(10);
        
        int value = RandomHelper::random_real(1.0f, 2.0f);
        pBalloon->setScale(value);
        m_listBalloon.pushBack(pBalloon);
    }
    
    //---------------------------------------
    //test Sprite
    m_pTestSprite = cocos2d::Sprite::create("button_Up.png");
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

int CMainScene::getSceneState()
{
    return (int)m_SceneState;
}

void CMainScene::NextScene(float _fSec)
{
    //Director::getInstance()->popScene();
}


void CMainScene::menuGameOptionCallback(Ref* sender)
{

    //- - - - - - - - - - - - - - - - - -
    //GameOption start
    
     if(m_PauseLayer != NULL && !m_PauseLayer->isVisible())
     {
     m_PauseLayer->VisiblePauseLayer(true);
     }
     else
     {
     m_PauseLayer->VisiblePauseLayer(false);
     }
    


    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    //exit(0);
#endif
    
    //IOS EXIT - end
}

void CMainScene::menuStartGameCallback(Ref* sender)
{
    if(getSceneState() != emSceneState::Scene_None) return;
    
    BB_GameInfo::GetInstance()->setGameState(emGameOff);
    playGameScene();
    
    
}



bool CMainScene::playGameScene()
{
    if(m_SceneState == emSceneState::Scene_None)
    {
        //        if(m_HelloWordScene != nullptr)
        //            delete m_HelloWordScene;
        m_HelloWordScene = HelloWorld::scene();
        Director::getInstance()->pushScene(m_HelloWordScene);
        return true;
    }
    
    return false;
}


*/
