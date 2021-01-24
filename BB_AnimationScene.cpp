//
//  BB_AnimationScene.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 10/01/2020.
//

#include "BB_AnimationScene.hpp"
USING_NS_CC;


using namespace cocos2d;

Scene* CAnimationScene::createScene()
{
    return CAnimationScene::create();
}


// on "init" you need to initialize your instance
bool CAnimationScene::init()
{
    //
    this->initWithPhysics();
    
    init_stageAnimation(ANIMATION_START);
    
    //Call_FuncAfterFewTime(schedule_selector(CAnimationScene::pushGameScene), 4.0f);
    startGameScene_AfterSec(4.0f);
    return true;
}

void CAnimationScene::init_stageAnimation(unsigned int _StageNum)
{
    init_value(_StageNum);
    init_sprite(_StageNum);
}

void CAnimationScene::init_value(unsigned int _StageNum)
{
    set_CurStageAniNum(_StageNum); //1-1 Start Animation = 11; ANIMATION_START = 11;
}

void CAnimationScene::init_sprite(unsigned int _StageNum)
{
    //
    this->removeFromParent();
    set_BGandSpriteOnStage(_StageNum);
}

void CAnimationScene::startGameScene_AfterSec(float _fSec)
{
    Call_FuncAfterFewTime(schedule_selector(CAnimationScene::pushGameScene), _fSec);
}

void CAnimationScene::set_BGandSpriteOnStage(unsigned int _StageNum)
{
    //Set BackGround Color
    if(_StageNum == ANIMATION_1_START)      Director::getInstance()->setClearColor(Color4F(0.7, 0.92, 0.96, 0));
    else if(_StageNum == ANIMATION_1_END)   Director::getInstance()->setClearColor(Color4F(0.7, 0.92, 0.96, 0));
    else if(_StageNum == ANIMATION_2_END)   Director::getInstance()->setClearColor(Color4F(0.3, 0.6, 0.96, 0));
    else                                    Director::getInstance()->setClearColor(Color4F(0.9, 0.8, 0.96, 0));
    
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    log("visible width - %f // visible height - %f", visibleSize.width, visibleSize.height);
    log("origin width - %f // origin height - %f", origin.x, origin.y);
    
    
    //--------------------------------------start
    // Back Ground(full Screen rate)
    Sprite* spt_BG = nullptr;
    if(_StageNum == ANIMATION_1_START)      spt_BG = Sprite::create("BG_Main.png");
    else if(_StageNum == ANIMATION_1_END)   spt_BG = Sprite::create("BG_Main.png");
    else if(_StageNum == ANIMATION_2_END)   spt_BG = Sprite::create("bgempty.png");
    else                                    spt_BG = Sprite::create("BG_Main.png");
    
    spt_BG->setAnchorPoint(Vec2(0,0));
    spt_BG->setPosition(origin);

    this->addChild(spt_BG, 0);
    
    //--------------------------------------start
    // LongRock
#if	USE_VISUALSTUDIO == 1	
	Sprite* pSprtMailBox = Sprite::create("image/shooter2.png");
#else
	Sprite* pSprtMailBox = Sprite::create("shooter2.png");	
#endif	
    
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
}


bool CAnimationScene::set_CurStageAniNum(unsigned int _unVal)
{
    m_unCurStageAniNum = _unVal;  //ex) 1-1 Start Animation = 11;
    return true;
}


void CAnimationScene::pushGameScene(float _fd)
{
    //
    //if(m_HelloWordScene == nullptr)
    m_HelloWordScene = HelloWorld::scene();
    
    HelloWorld* pHello  = dynamic_cast<HelloWorld*>(m_HelloWordScene); //Error pHello Null Ptr
    pHello->m_pAniScene = this;
    Director::getInstance()->pushScene(m_HelloWordScene);
    
    if(m_unCurStageAniNum == ANIMATION_1_START)
        pHello->Start_Stage(STAGE_1st_ONE);
    else if(m_unCurStageAniNum == ANIMATION_1_END)
        pHello->Start_Stage(STAGE_1st_TWO);
    else if(m_unCurStageAniNum == ANIMATION_2_END)
        pHello->Start_Stage(STAGE_1st_THREE);
}

void CAnimationScene::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
    this->scheduleOnce(SEL_SCHEDULE, _fSec);
}
