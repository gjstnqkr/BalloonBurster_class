#include "Balloon.h"

#include "HelloworldScene.h"


using namespace cocos2d;

Balloon::Balloon()
: m_pParentBM(NULL)
, m_fUpSpeed(FLYING_TIME)
, m_fMoveDistance(1.0f)
{	
}

Balloon::~Balloon()
{
}

Balloon* Balloon::create(Vec2 _Pos, float fSpeedVal)
{
	Balloon *sprite = new (std::nothrow) Balloon();
#if	USE_VISUALSTUDIO == 1	
	if (sprite && sprite->initWithFile("image/Balloon.png"))	
#else
	if (sprite && sprite->initWithFile("Balloon.png"))
#endif	
	{
        sprite->init_Balloon(fSpeedVal);
		sprite->init_Cocos2dInfo(_Pos);
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

void Balloon::init_Balloon(float fSpeedVal)
{
    m_pParentBM = NULL;
    m_fMoveDistance = fSpeedVal;
    m_emBalloonState = emBalloonState::Balloon_Up;
}

void Balloon::init_Cocos2dInfo(Vec2 _Pos)
{	
	PhysicsBody* pbody_Balloon = PhysicsBody::createBox(this->getContentSize(), BASIC_MATERIAL);
	pbody_Balloon->setContactTestBitmask(0xFFFFFFFF);
	pbody_Balloon->setDynamic(false);
	
	this->setPhysicsBody(pbody_Balloon);
	this->setPosition(_Pos);
    //this->setAnchorPoint(Vec2(0.5,0.5));
	this->setTag(emNameTag::BALLOON);
	this->getPhysicsBody()->setGravityEnable(true);
}

void Balloon::Fly_Balloon(float _dt)
{
	if (this->getReferenceCount() < 1) return;

	if (this->getParent() == NULL) return;

	auto visibleSize	= Director::getInstance()->getVisibleSize();	

	this->stopAllActions();	
	
	//ÀÌµ¿	
	this->getPhysicsBody()->setGravityEnable(false);
    
    MoveBy* Action = NULL;
    if(get_BalloonState() == emBalloonState::Balloon_Up || get_BalloonState() == emBalloonState::Balloon_Free)
    {
        Call_FuncAfterFewTime(schedule_selector(Balloon::cleanupBallon), 60.0f);
        Action = cocos2d::MoveBy::create(FLYING_TIME, Vec2(0.0f, visibleSize.height + (FLYING_DIST * m_fMoveDistance)));
    }
    else if(get_BalloonState() == emBalloonState::Balloon_Down)
    {
        Action = cocos2d::MoveBy::create(FLYING_TIME, Vec2(0.0f, -(visibleSize.height + (FLYING_DIST * m_fMoveDistance))));
    }
    
	
	this->runAction(Action);
}

void Balloon::set_BalloonMaker(BalloonMaker * pBM)
{
	m_pParentBM = pBM;
}


BalloonMaker * Balloon::get_BalloonMaker()
{
	return m_pParentBM;
}

void Balloon::set_BalloonState(emBalloonState _State)
{
    m_emBalloonState = _State;
}

int Balloon::get_BalloonState()
{
    return m_emBalloonState;
}

void Balloon::Free_fromBalloonMaker()
{
    set_BalloonState(emBalloonState::Balloon_Up);
    m_fMoveDistance = 1.0f;
}

void Balloon::set_UpSpeed(float _fVal)
{
    m_fUpSpeed = _fVal;
}

void Balloon::set_MoveDistance(float _fVal)
{
    m_fMoveDistance = _fVal;
}


void Balloon::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
    this->scheduleOnce(SEL_SCHEDULE, _fSec);
}

void Balloon::cleanupBallon(float _fVal)
{
    removeFromParentAndCleanup(true);
}


