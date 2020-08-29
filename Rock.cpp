#include "Rock.h"
#include "HelloworldScene.h"

using namespace cocos2d;


Rock::Rock()
	: m_pParentBM(NULL)
	, m_RockState(emRockState::Rock_Standby)
{
}


Rock::~Rock()
{
}

Rock* Rock::create(Vec2 _Pos)
{
	Rock *sprite = new (std::nothrow) Rock();
	if (sprite && sprite->initWithFile("rock.png"))
	{
		sprite->init_Cocos2dInfo(_Pos);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Rock::init_Cocos2dInfo(Vec2 _Pos)
{
	PhysicsBody* pbody_Balloon = PhysicsBody::createBox(this->getContentSize(), BASIC_MATERIAL);
	pbody_Balloon->setContactTestBitmask(0xFFFFFFFF);
	pbody_Balloon->setDynamic(false);

	this->setPhysicsBody(pbody_Balloon);
	this->setPosition(_Pos);
	this->setTag(emNameTag::PROJECTTILE);
	this->getPhysicsBody()->setGravityEnable(true);
	this->setScale(1.0);
}

void Rock::Thrown_Rock(float fDist)
{
    this->setVisible(true);
	BalloonMaker* pBM	= dynamic_cast<BalloonMaker*>(this->getParent());
	HelloWorld* pHello	= dynamic_cast<HelloWorld*>(this->getParent()->getParent());

	removeFromParentAndCleanup(false);
	//pBM->eraseRockInList(this);
	this->setPosition(pBM->getPosition());
	
	if (pHello == NULL) return;	
	pHello->addChild(this);

	this->setScale(1.0);
	//
	PhysicsMaterial PhyMat = PhysicsMaterial(1.0f, 0.0f, 1.0f);
	PhysicsBody* pbody_Dart = PhysicsBody::createBox(this->getContentSize(), PhyMat);
	pbody_Dart->setDynamic(true);
	pbody_Dart->setContactTestBitmask(0xFFFFFFFF);
	pbody_Dart->applyForce(Vec2(0.0f, 500.0f));
	pbody_Dart->setVelocity(Vec2(-160, 80));

	this->setPhysicsBody(pbody_Dart);
	set_RockState(emRockState::Rock_Throwing);
    
    Call_FuncAfterFewTime(schedule_selector(Rock::explode_Rock), 15.0f);
}
void Rock::explode_Rock(float _dt)
{
	set_RockState(emRockState::Rock_explode);

	this->stopAllActions();
	auto Action = cocos2d::ScaleBy::create(0.3, 4.0f);
	auto Action2 = cocos2d::FadeOut::create(0.3f);
	this->runAction(Action);
	this->runAction(Action2);
	removeFromParentAndCleanup(true);
}


void Rock::set_BalloonMaker(BalloonMaker * pArcher)
{
	m_pParentBM = pArcher;
}

BalloonMaker * Rock::get_BalloonMaker()
{
	return m_pParentBM;
}

void Rock::set_RockState(emRockState _RockState)
{
	m_RockState = _RockState;
}

emRockState Rock::get_RockState()
{
	return m_RockState;
}

void Rock::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
    this->scheduleOnce(SEL_SCHEDULE, _fSec);
}
