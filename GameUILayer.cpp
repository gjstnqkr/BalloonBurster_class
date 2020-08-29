//
//  GameUILayer.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 25/04/2019.
//


#include "GameUILayer.hpp"
#include "HelloWorldScene.h"
#include "BB_GameInfo.h"

USING_NS_CC;

GameUILayer::GameUILayer()
: m_pPauseImg(nullptr)
, m_PauseLayer(nullptr)
, m_pArrBtn(nullptr)
, m_sptLifeBG(nullptr)
, m_sptCashBG(nullptr)
, m_sptEnemyBG(nullptr)
, m_spScoreG(nullptr)
, m_MoveObjPosOld(Vec2::ZERO)
, m_touchStart(Vec2::ZERO)
, m_ArcPos(Vec2::ZERO)
, m_oldLife(0)
, m_oldScore(0)
, m_oldEnemyCnt(0)
, m_lbUserScore(nullptr)
, m_lbLifeCnt(nullptr)
, m_lbEnemyCnt(nullptr)
, m_bIsMoveTouch(false)
{
}

GameUILayer::~GameUILayer()
{
    
}

GameUILayer* GameUILayer::create(Node* pParent)
{
    GameUILayer* pUILayer = new (std::nothrow) GameUILayer();
    if(pUILayer && pUILayer->init())
    {
        pUILayer->autorelease();
        
        return pUILayer;
    }
    CC_SAFE_DELETE(pUILayer);
    return nullptr;
}

bool GameUILayer::init()
{
    Init_Value();
    Init_UI();
    Init_Labels();
    
    this->setVisible(true);
    this->schedule(schedule_selector(GameUILayer::updateData), 0.1f);
    
    return true;
}

void GameUILayer::Init_Value()
{
    m_ArcPos            = Vec2(Vec2::ZERO);
    m_touchStart        = Vec2(Vec2::ZERO);
    m_MoveObjPosOld     = Vec2(Vec2::ZERO);
    m_oldEnemyCnt       = 0;
    m_oldScore          = 0;
    m_oldLife           = 0;
}


bool GameUILayer::Init_UI()
{
    //
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    
    //Arr Btn
    m_pArrBtn = cocos2d::Sprite::create("button_Play_push.png");
    float fBtnOffset             = visibleSize.height * UI_TOP_START_RATE;
    CCSize  orgSize_ArrBtn       = m_pArrBtn->getContentSize();
    m_pArrBtn->setPosition(origin + Vec2(visibleSize.width, 0.0f) - Vec2(fBtnOffset, -fBtnOffset) - Vec2( orgSize_ArrBtn.width, 0));
    m_pArrBtn->setAnchorPoint(Vec2(0,0));
    m_pArrBtn->setScale(1.0);
    
    
    this->addChild(m_pArrBtn, 1);
    
    //MoveObject
    m_pMoveObject = cocos2d::Sprite::create("button_Check_On.png");
    fBtnOffset    = visibleSize.height * UI_LEFT_START_RATE;
    CCSize  orgSize_MoveObj       = m_pMoveObject->getContentSize();
    m_pMoveObject->setPosition(origin + Vec2(fBtnOffset, fBtnOffset) );
    m_pMoveObject->setAnchorPoint(Vec2(0,0));
    m_pMoveObject->setScale(0.5f);
    
    this->addChild(m_pMoveObject, Z_SCENE_UI);
    
    //Puase Btn
    MenuItemImage* PauseItem = MenuItemImage::create(
                                           "Pause_60.png",
                                           "Pause_60.png",
                                           CC_CALLBACK_1(GameUILayer::menuPauseCallback,this));
    
    fBtnOffset                    = visibleSize.height * UI_TOP_START_RATE;
    CCSize  orgSize_CloseBtn      = PauseItem->getContentSize();
    PauseItem->setPosition(origin + visibleSize - Vec2(fBtnOffset, fBtnOffset) - orgSize_CloseBtn);
    PauseItem->setAnchorPoint(Vec2(0,0));
    PauseItem->setScale(1.0);
    
    
    // create menu, it's an autorelease object
    m_pPauseImg = Menu::create(PauseItem, nullptr);
    m_pPauseImg->setPosition(Vec2::ZERO);
    this->addChild(m_pPauseImg, 1);
    
    // LifeBG UI Sprite
    m_sptLifeBG = Sprite::create("heart_ui.png");
    CCSize LifeSize = m_sptLifeBG->getContentSize();
    float left = origin.x + fBtnOffset;
    float top = origin.y + visibleSize.height - fBtnOffset - LifeSize.height;
    m_sptLifeBG->setPosition(Vec2(left, top));
    m_sptLifeBG->setScale(1.0f, 1.0f);
    m_sptLifeBG->setAnchorPoint(Vec2(0,0));
    
    this->addChild(m_sptLifeBG, Z_SCENE_UI);
    m_TileManager.addUiTile(0, 0, left, top, LifeSize.width, LifeSize.height, "LifeBG");
    
    
    // CashBG UI Sprite
    UI_Tile nextUi;
    m_TileManager.calculateTilePos(0, 0, emBottom, nextUi);  //calculate position
    
    m_sptCashBG = Sprite::create("Money_ui.png");
    CCSize CashBGSize = m_sptCashBG->getContentSize();
    m_sptCashBG->setPosition(Vec2(nextUi.fLeft, nextUi.fTop));
    m_sptCashBG->setScale(1.0f, 1.0f);
    m_sptCashBG->setAnchorPoint(Vec2(0,0));
    
    
    this->addChild(m_sptCashBG, Z_SCENE_UI);
    m_TileManager.addUiTile(1, 0, nextUi.fLeft, nextUi.fTop, CashBGSize.width, CashBGSize.height, "CashBG");
    
    // EnemyBG UI Sprite
    nextUi.initTile();
    m_TileManager.calculateTilePos(0, 0, emRight, nextUi);  //calculate position
    m_sptEnemyBG = Sprite::create("enemy_ui.png");
    CCSize EnemyBGSize = m_sptEnemyBG->getContentSize();
    m_sptEnemyBG->setPosition(Vec2(nextUi.fLeft, nextUi.fTop));
    m_sptEnemyBG->setScale(1.0f, 1.0f);
    m_sptEnemyBG->setAnchorPoint(Vec2(0,0));
    
    this->addChild(m_sptEnemyBG, Z_SCENE_UI);
    m_TileManager.addUiTile(0, 1, nextUi.fLeft, nextUi.fTop, EnemyBGSize.width, EnemyBGSize.height, "EnemyBG");
    
    // ScoreBack Sprite
    m_spScoreG = Sprite::create("Money_ui.png");
    CCSize ScoreBGSize = m_spScoreG->getContentSize();
    m_spScoreG->setPosition(Vec2(origin.x + visibleSize.width/2 - ScoreBGSize.width/2, nextUi.fTop));
    m_spScoreG->setScale(1.0f, 1.0f);
    m_spScoreG->setAnchorPoint(Vec2(0,0));
    
    this->addChild(m_spScoreG, Z_SCENE_UI);
    
    return true;
}

bool GameUILayer::Init_PauseLayer()
{
    if(m_PauseLayer == NULL && this->getParent() != NULL)
    {
        auto PauseLayer = PauseLayer::create(emUserOption::INGAME);
        this->getParent()->addChild(PauseLayer);
        m_PauseLayer = PauseLayer;
        m_PauseLayer->m_pGameUI = this;
        m_PauseLayer->VisiblePauseLayer(false);
        return true;
    }
    
    return false;
}


void GameUILayer::Init_Labels()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    
    UI_Tile tempTile;
    //Enemy Count Label init
    if (m_lbEnemyCnt == NULL)
    {
        m_TileManager.getUiTile("EnemyBG", tempTile);
        float fPosX = (tempTile.fLeft + tempTile.fWidth/2) + UI_W_OFFSET;
        float fPosY = (tempTile.fTop + tempTile.fHeight/2);
        
        m_lbEnemyCnt = Label::createWithBMFont("fonts/bitmapFontTest4.fnt", "0");
        m_lbEnemyCnt->setPosition(Vec2(fPosX,fPosY));
        this->addChild(m_lbEnemyCnt, Z_SCENE_UI_VALUE);
        
        SetEnemyCntLabel(0);
    }
    
    
    //life Label init
    if (m_lbLifeCnt == NULL)
    {
        m_TileManager.getUiTile("LifeBG", tempTile);
        float fPosX = (tempTile.fLeft + tempTile.fWidth/2) + UI_W_OFFSET;
        float fPosY = (tempTile.fTop + tempTile.fHeight/2);
        
        m_lbLifeCnt = Label::createWithBMFont("fonts/bitmapFontTest4.fnt", "0");
        m_lbLifeCnt->setPosition(Vec2(fPosX, fPosY));
        m_lbLifeCnt->setVisible(true);
        this->addChild(m_lbLifeCnt, Z_SCENE_UI_VALUE);
        
        SetLifeCntLabel(LIFE_CNT);
        BB_GameInfo::GetInstance()->setLifeCnt(LIFE_CNT);
    }
    
    //Stage UserScore Label init
    if (m_lbUserScore == NULL)
    {
        m_TileManager.getUiTile("LifeBG", tempTile);
        float fPosY = (tempTile.fTop + tempTile.fHeight/2);
        m_lbUserScore = Label::createWithBMFont("fonts/bitmapFontTest4.fnt", "00000000");
        m_lbUserScore->setPosition(Vec2(origin.x + visibleSize.width/2 + UI_W_OFFSET, fPosY));
        this->addChild(m_lbUserScore, Z_SCENE_UI_VALUE);
        
        Set_UserScoreLabel(0);
    }

}


void GameUILayer::menuPauseCallback(Ref* sender)
{

    //Pause
    if(Director::getInstance()->isPaused() == true)
    {
        ShowGameUI();
        Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
        Director::getInstance()->resume();
        this->getParent()->resume();
        
    }
    else
    {
        HideGameUI();
        Call_FuncAfterFewTime(schedule_selector(GameUILayer::BB_GamePause), UI_ACTION_TIME+0.02);
    }
    
    
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

}


void GameUILayer::onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
    
    
}

void GameUILayer::onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
}

void GameUILayer::onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event)
{
}

void GameUILayer::setMoveObjPos(cocos2d::Vec2 _arcPos)
{
    m_pMoveObject->setPosition(_arcPos);
}

cocos2d::Vec2 GameUILayer::getMoveObjPos()
{
    return m_pMoveObject->getPosition();
}


void GameUILayer::SetEnemyCntLabel(unsigned int _Value)
{
    char str[10] = { 0 };
    sprintf(str, "%d", _Value);
    m_lbEnemyCnt->setString(str);
}

void GameUILayer::SetLifeCntLabel(unsigned int _Value)
{
    char str[10] = { 0 };
    sprintf(str, "%d", _Value);
    m_lbLifeCnt->setString(str);
}

bool GameUILayer::Set_UserScoreLabel(unsigned int _unScore)
{
    unsigned int unOut = _unScore/10;
    char arrZero[10];
    
    if(unOut < 10) sprintf(arrZero, "%s", "000000");
    else if(unOut < 100) sprintf(arrZero, "%s", "00000");
    else if(unOut < 1000) sprintf(arrZero, "%s", "0000");
    else if(unOut < 10000) sprintf(arrZero, "%s", "000");
    else if(unOut < 100000) sprintf(arrZero, "%s", "00");
    else if(unOut < 1000000) sprintf(arrZero, "%s", "0");
    
    char str[10] = { 0 };
    sprintf(str, "%s%d0", arrZero, unOut);
    
    m_lbUserScore->setString(str);
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Size strSize    = m_lbUserScore->getContentSize();
    int  nStrWidth  = strSize.width;
    
    return true;
}

unsigned int GameUILayer::GetEnemyCntFromLabel()
{
    std::string cntStr = m_lbEnemyCnt->getString();
    int dd = std::stoi(cntStr);
    
    return dd;
}


//
void GameUILayer::updateData(float _dt)
{
    BB_GameInfo* pGameInfo = BB_GameInfo::GetInstance();
    
    if(pGameInfo->getGameState() != emBB_State::emGameOn) return;
    
    unsigned int unLifeCnt      = pGameInfo->getLifeCnt();
    unsigned int unEnemyCnt     = pGameInfo->getCurEnemyCnt();
    unsigned int unScore        = pGameInfo->getGameScore();
    
    if(unLifeCnt != m_oldLife){
        SetLifeCntLabel(unLifeCnt);
        m_oldLife = unLifeCnt;
    }
    
    if(unScore != m_oldScore){
        Set_UserScoreLabel(unScore);
        m_oldScore = unScore;
    }
    if(unEnemyCnt != m_oldEnemyCnt){
        SetEnemyCntLabel(unEnemyCnt);
        m_oldEnemyCnt = unEnemyCnt; 
    }
}

void GameUILayer::HideGameUI()
{
    float MoveTime = 0.2;
    Vec2 Move = Vec2(0.0f, 150.0f);
    Vec2 Move2 = Vec2(0.0f, 100.0f);

    m_sptLifeBG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_sptCashBG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_sptEnemyBG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_spScoreG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_lbEnemyCnt->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_lbLifeCnt->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_lbUserScore->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_pPauseImg->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_pArrBtn->runAction(MoveBy::create(UI_ACTION_TIME,-(Move + Move2)));
    m_pMoveObject->runAction(MoveBy::create(UI_ACTION_TIME,-(Move + Move2)));
    
}


void GameUILayer::ShowGameUI()
{
    float MoveTime = 0.2;
    Vec2 Move = Vec2(0.0f, -150.0f);
    Vec2 Move2 = Vec2(0.0f, -100.0f);
    float CurActStat = m_lbUserScore->getNumberOfRunningActions();
    if(CurActStat != 0.0f) return;
    
    m_sptLifeBG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_sptCashBG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_sptEnemyBG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_spScoreG->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_lbEnemyCnt->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_lbLifeCnt->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_lbUserScore->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_pPauseImg->runAction(MoveBy::create(UI_ACTION_TIME, Move));
    m_pArrBtn->runAction(MoveBy::create(UI_ACTION_TIME,-(Move + Move2)));
    m_pMoveObject->runAction(MoveBy::create(UI_ACTION_TIME,-(Move + Move2)));
}

void GameUILayer::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
    this->scheduleOnce(SEL_SCHEDULE, _fSec);
}


void GameUILayer::BB_GamePause(float _dt)
{
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
    Director::getInstance()->pause();
    this->getParent()->pause();
}

void GameUILayer::BB_GameResume(float _dt)
{
    Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
    Director::getInstance()->resume();
    this->getParent()->resume();
}

