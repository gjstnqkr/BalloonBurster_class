#include "Arrow.h"
#include "Archer.h"
#include "HelloWorldScene.h"
#include "BB_MainScene.hpp"

using namespace cocos2d;
#define Arr_Speed 1.3f
#define arrX ARCHER_SCALE * 0.6f
#define arrY ARCHER_SCALE * 0.6f

Arrow::Arrow()
: m_emState(emArrowState::Arr_Preparing)
, m_pParentArcher(NULL)
, angle(0.0f)
, bArrowShooting(false)
{
}


Arrow::~Arrow()
{
}


Arrow* Arrow::create(Vec2 _Pos, Archer* _parent)
{
	Arrow *sprite = new (std::nothrow) Arrow();
#if	USE_VISUALSTUDIO == 1	
	if (sprite && sprite->initWithFile("Arrow.png"))
#else
	if (sprite && sprite->initWithFile("Arrow.png"))
#endif
    
	{
        sprite->set_Archer(_parent);
		sprite->init_Cocos2dInfo(_Pos);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Arrow::init_Cocos2dInfo(Vec2 _Pos)
{
    this->stopAllActions();
    this->setRotation(0);
    removeFromParentAndCleanup(false);
    
	PhysicsBody* pbody_Arrow = PhysicsBody::createBox(this->getContentSize()/2, BASIC_MATERIAL);
	pbody_Arrow->setContactTestBitmask(0xFFFFFFFF);
	pbody_Arrow->setDynamic(false);
	this->setPhysicsBody(pbody_Arrow);
    this->setPosition(m_pParentArcher->get_ArcSpineBonePos("Left_Arm"));
    
	this->setScale(arrX, arrY);
	this->setTag(emNameTag::ARROW);
	this->getPhysicsBody()->setGravityEnable(false);
    this->setVisible(false);
}

void Arrow::ShootArrow()
{
    //
	Node* pNode = dynamic_cast<Node*>(m_pParentArcher->getParent());
	removeFromParentAndCleanup(false);
	if (pNode == NULL) return;
	pNode->addChild(this);	
	
    
    PhysicsBody* pbody_Arrow = PhysicsBody::createBox(this->getContentSize(), BASIC_MATERIAL);
    pbody_Arrow->setContactTestBitmask(0xFFFFFFFF);
    pbody_Arrow->setDynamic(false);
    this->setPhysicsBody(pbody_Arrow);
    
    this->setVisible(true);
	this->setPosition(m_pParentArcher->get_ArcSpineBonePos("Left_Arm"));
    this->setScale(arrX, arrY);
	this->set_ArrowState(emArrowState::Arr_Shooting);
	this->stopAllActions();
	auto Action = cocos2d::MoveBy::create(Arr_Speed, Vec2(SCREEN_WIDTH, 0.0f));
	this->runAction(Action);
	this->schedule(schedule_selector(Arrow::UpdateArrowPos), 0.1);
}

void Arrow::ShootParabola()
{
	//
	Node* pNode = dynamic_cast<Node*>(m_pParentArcher->getParent());
	removeFromParentAndCleanup(false);
	if (pNode == NULL) return;
	pNode->addChild(this);


	PhysicsBody* pbody_Arrow = PhysicsBody::createBox(this->getContentSize(), BASIC_MATERIAL);
	pbody_Arrow->setContactTestBitmask(0xFFFFFFFF);
	pbody_Arrow->setDynamic(false);
	this->setPhysicsBody(pbody_Arrow);

	this->setVisible(true);
	this->setPosition(m_pParentArcher->get_ArcSpineBonePos("Left_Arm"));
	this->setScale(arrX, arrY);
	this->set_ArrowState(emArrowState::Arr_Shooting);
	this->stopAllActions();
	//auto Action = cocos2d::MoveBy::create(Arr_Speed, Vec2(SCREEN_WIDTH, 0.0f));
	//this->runAction(Action);
	
	StopArrowUpdate(0);
	bArrowShooting = true;
	Call_FuncAfterFewTime(schedule_selector(Arrow::StopArrowUpdate), 5.0f);
	this->schedule(schedule_selector(Arrow::UpdateArrowPos), 0.1);
}

void Arrow::FallingArrow()
{
	this->set_ArrowState(emArrowState::Arr_Falling);
	
	this->stopAllActions();
	auto Action = cocos2d::RotateTo::create(0.05f, 90.0f);
	this->runAction(Action);

	auto Action2 = cocos2d::MoveBy::create(1.9f, Vec2(0.0f, -950.0f));
	this->runAction(Action2);
	this->schedule(schedule_selector(Arrow::UpdateArrowPos), 0.5);
}

void Arrow::HitArrow()
{
	this->set_ArrowState(emArrowState::Arr_Hit);
    
	this->stopAllActions();
	auto Action2 = cocos2d::RotateTo::create(0.1f, 0);
	this->runAction(Action2);
    
	if (m_pParentArcher != NULL)
        this->setPosition(Vec2::ZERO);

	this->schedule(schedule_selector(Arrow::UpdateArrowPos), 0.1);
}

void Arrow::UpdateArrowPos(float f)
{	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	
	if (this->get_ArrowState() == emArrowState::Arr_Shooting)
	{
		if (visibleSize.width < this->getPosition().x || this->getPosition().y < 0.0f || this->getPosition().x < 0.0f)
		{
			this->unschedule(schedule_selector(Arrow::UpdateArrowPos));
			this->set_ArrowState(emArrowState::Arr_Preparing);

			StopArrowUpdate(0);
            if (m_pParentArcher != NULL){
                init_Cocos2dInfo();
			}
		}
	}
	else if (this->get_ArrowState() == emArrowState::Arr_Falling)
	{
		if ( this->getPosition().y < 0.0f)
		{
			this->unschedule(schedule_selector(Arrow::UpdateArrowPos));
			this->set_ArrowState(emArrowState::Arr_Preparing);
            if (m_pParentArcher != NULL){
                init_Cocos2dInfo();
			}
		}
	}
	else if (this->get_ArrowState() == emArrowState::Arr_Hit)
	{
		this->unschedule(schedule_selector(Arrow::UpdateArrowPos));
		this->set_ArrowState(emArrowState::Arr_Preparing);
        if (m_pParentArcher != NULL){
            init_Cocos2dInfo();
		}
	}	
}

void Arrow::StopArrowUpdate(float f)
{
	bArrowShooting = false;
	angle = 0.0f;
}

void Arrow::update(float delta)
{
	if (!bArrowShooting) return;

	Vec2 pos = this->getPosition();

	float offset = 40;	
	angle = angle + (offset * delta); // angle은 0부터 시작
	
	float radian = (angle*M_PI) / 180;
	float radian2 = ((angle + 0)*M_PI) / 180;

	float fHeight = sin(radian);
	float fDistance = cos(radian2);	

	//log("\n fHeight %f", fHeight);
	//log("\n fDistance %f", fDistance);

	pos = pos + Vec2(fDistance * 15, -fHeight * 10);

	log("\n fDistance*15 = %f", fDistance * 15);
	log("\n pos.x %f", pos.x);
	this->setPosition(pos);
}


emArrowState Arrow::get_ArrowState(void)
{
	return m_emState;
}

void Arrow::set_ArrowState(emArrowState _emState)
{
	m_emState = _emState;
}

void Arrow::set_Archer(Archer * pArcher)
{
	m_pParentArcher = pArcher;
}


void Arrow::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
	this->scheduleOnce(SEL_SCHEDULE, _fSec);
}