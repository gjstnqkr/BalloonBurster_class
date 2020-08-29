#include "BalloonMaker.h"
#include "spine/spine.h"
#include "Balloon.h"
#include "Rock.h"
#include "HelloWorldScene.h"



using namespace cocos2d;

#define MOVE_TIME 10.0f
#define MOVE_DIST 1334.0f
//#define MOVE_DIST 350.0f
#define MAKE_TIME_Lv1 6.0f
#define MAKE_TIME_Lv2 2.5f

#define BM_OFFSET 10.0f


#define BM_COLOR_GREEN cocos2d::Color3B(75.0f, 245.0f, 75.0f)
#define BM_COLOR_RED cocos2d::Color3B(245.0f, 20.0f, 20.0f)
#define BM_COLOR_BLACK cocos2d::Color3B(20.0f, 20.0f, 20.0f)
#define BM_COLOR_WHITE cocos2d::Color3B(255.0f, 255.0f, 255.0f)
#define BM_COLOR_BOSS cocos2d::Color3B(155.0f, 185.0f, 175.0f)


#define BM_SCALE_GREEN 1.0f
#define BM_SCALE_RED 1.1f
#define BM_SCALE_BLACK 1.3f
#define BM_SCALE_WHITE 1.4f
#define BM_SCALE_BOSS 2.5f


namespace {
#if SPINE2D_BallonMaker == 1
    cocos2d::Vec2 BM_SCALE_LV1(0.25f,0.25f);
    cocos2d::Vec2 BM_SCALE_LV2(0.27f,0.27f);
#else
    cocos2d::Vec2 BM_SCALE_LV1(1.0f,1.0f);
    cocos2d::Vec2 BM_SCALE_LV2(1.1f,1.1f);
#endif
    
};

#define Nearest 0.60
#define Near 0.485
#define NormalDist 0.37
#define Far 0.26
#define Furthermost 0.15

#define ThrowDist 15.0f


BalloonMaker::BalloonMaker()
	: m_fTime(0.0f)
	, m_fTempTime(0.0f)
    , m_fMaxLowPos(0.0f)
    , m_fMaxHighPos(0.0f)
{	
	m_listBalloon.clear();
	m_listRock.clear();
}

BalloonMaker::~BalloonMaker()
{
}

BalloonMaker* BalloonMaker::create(cocos2d::Vec2 _pos, emMakerType _emType, emMovingType _emMoveType)
{
    
#if SPINE2D_BallonMaker == 1
    BalloonMaker *pBM = new (std::nothrow) BalloonMaker();
    if (pBM)
    {
        //spAtlas* atlas = spAtlas_createFromFile("goblins.atlas", 0);
        //pBM->initWithJsonFile("goblins-pro.json", atlas, 1.0f);

#if	USE_VISUALSTUDIO == 1		
		spAtlas* atlas = spAtlas_createFromFile("spine/gobline.atlas", 0);
		pBM->initWithJsonFile("spine/gobline.json", atlas, 0.6);
#else		
		spAtlas* atlas = spAtlas_createFromFile("gobline.atlas", 0);
		pBM->initWithJsonFile("gobline.json", atlas, 0.6);
#endif
        
        pBM->init_SkelAnimation();
        pBM->init_Cocos2dInfo(_pos, _emType, _emMoveType);
        pBM->autorelease();
        pBM->unscheduleUpdate();    //warning 방지
        return pBM;
    }
    CC_SAFE_DELETE(pBM);
    return nullptr;
#else
    BalloonMaker *sprite = new (std::nothrow) BalloonMaker();
    if (sprite && sprite->initWithFile("Archer_50.png"))
    {
        sprite->init_Cocos2dInfo(_pos, _emType, _emMoveType);
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
#endif
}

void BalloonMaker::onEnterTransitionDidFinish()
{
	this->schedule(schedule_selector(BalloonMaker::Start_Scheduler), 0.1f);

	Vec2 BM_Pos = this->getPosition();
	int nRanVal = RandomHelper::random_int(0, 2);
	for (int ii = 0; ii < nRanVal; ii++)
		Make_Rock(Vec2::ZERO);
}

void BalloonMaker::init_Cocos2dInfo(cocos2d::Vec2 _pos, emMakerType _emType, emMovingType _emMoveType)
{	
	init_BMakerState(_emType, _emMoveType);

	Color3B BM_Color3B	= Color3B::WHITE;
	float	fBM_vecScale = 0.0f;
    if (_emType == emMakerType::RED)
    {
        BM_Color3B	    = BM_COLOR_RED;
        fBM_vecScale	= BM_SCALE_RED;
    }
    else if(_emType == emMakerType::WHITE)
    {
        BM_Color3B      = BM_COLOR_WHITE;
        fBM_vecScale    = BM_SCALE_WHITE;
    }
    else if(_emType == emMakerType::BLACK)
    {
        BM_Color3B      = BM_COLOR_BLACK;
        fBM_vecScale    = BM_SCALE_BLACK;
    }
    else if(_emType == emMakerType::GREEN)
    {
        BM_Color3B      = BM_COLOR_GREEN;
        fBM_vecScale    = BM_SCALE_GREEN;
    }


	//Sprite info
	this->setPosition(_pos);	
	this->setColor(BM_Color3B);
	this->setTag(BALLOONMAKER);

	//Apply PhysicsBody
    this->update(0.0f);
	Rect skelRect = this->getBoundingBox();
    Vec2 pos(Vec2::ZERO);
    pos.y = skelRect.size.height * 0.45f;
    skelRect.size.height = skelRect.size.height * 0.9;
    skelRect.size.width = skelRect.size.width * 0.7;
	PhysicsBody* pbody = PhysicsBody::createBox(skelRect.size, BASIC_MATERIAL, pos);
	pbody->setDynamic(false);
	pbody->setContactTestBitmask(0xFFFFFFFF); 		
	
	this->setPhysicsBody(pbody);
	this->setScale(fBM_vecScale);
	this->getPhysicsBody()->setGravityEnable(false);  //Apply Grabvity 
}

void BalloonMaker::init_BMakerState(emMakerType _emType, emMovingType _emMoveType)
{
    float offset = 0.0;
	switch (_emType)
	{
	case GREEN:
		m_emType		= GREEN;
		m_emState		= BM_Walking;
		m_emMovingType  = _emMoveType;
		m_fMoveSpeed	= 1.0f - offset;
		m_nMaxHP		= 200;
		m_nCurrentHP	= 200;
		break;
	case RED:
		m_emType		= RED;
		m_emState		= BM_Walking;
		m_emMovingType  = _emMoveType;
		m_fMoveSpeed	= 2.0f - offset;
		m_nMaxHP		= 300;
		m_nCurrentHP	= 300;
        break;
    case WHITE:
        m_emType        = WHITE;
        m_emState       = BM_Walking;
        m_emMovingType  = _emMoveType;
        m_fMoveSpeed    = 1.5f - offset;
        m_nMaxHP        = 300;
        m_nCurrentHP    = 300;
		break;
	case BLACK:
		m_emType		= BLACK;
		m_emState		= BM_Walking;
		m_emMovingType  = _emMoveType;
		m_fMoveSpeed	= 0.5f - offset;
		m_nMaxHP		= 300;
		m_nCurrentHP	= 300;
		break;

	default:
		break;
	}
}

void BalloonMaker::init_SkelAnimation()
{
#if SPINE2D_BallonMaker == 1
	//화면 크기 가져오기
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();	
	
	//Animation info
	this->setAnimation(0, "Run", true);
    this->setAttachment("spear", NULL);
    
#endif
}

void BalloonMaker::Start_Scheduler(float _dt)
{
    //if CurrentStage Clear, do anything
    HelloWorld* pParent = dynamic_cast<HelloWorld*>(getParent());
    
    if (pParent != nullptr)
        if(pParent->Get_CurrentStage()->getStageState() != SState_Fighting)
            return;
    
    
    //check noaction
	float CurActStat = this->getNumberOfRunningActions();   //if return value 0.0, this Sprite State is No Action.

	if ( m_fTime == 0.0f && this->get_BMakerState() == BM_Walking )
		Walk_Ground(m_emMovingType);

	m_fTime += _dt;	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 BM_Pos = this->getPosition();
	HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
	Vec2 Archer_pos = pHello->m_pArcher->getPosition();

	
	if (m_emMovingType != emMovingType::BM_Random)
	{

		float fDistance = 0.0f;
        if (m_emMovingType == emMovingType::BM_Nearest)                fDistance = Nearest;
        else if (m_emMovingType == emMovingType::BM_Near)            fDistance = Near;
        else if (m_emMovingType == emMovingType::BM_NormalDist)        fDistance = NormalDist;
        else if (m_emMovingType == emMovingType::BM_Far)            fDistance = Far;
        else if (m_emMovingType == emMovingType::BM_Furthermost)    fDistance = Furthermost;

		float fMoveDistance = visibleSize.width * fDistance;
        

        //log("fly down %f \n", getPosition().y);
		if ((visibleSize.width - fMoveDistance) + BM_OFFSET > BM_Pos.x && get_BMakerState() == emMakerState::BM_Walking)
		{			
			init_BMakerState(m_emType, m_emMovingType);

			//MoveAction
			this->stopAllActions();			
			auto MoveAction = cocos2d::MoveTo::create(0.3f, Vec2((visibleSize.width - fMoveDistance), BM_Pos.y));
			auto* pAction = CCSequence::create(MoveAction, CCCallFunc::create(this, callfunc_selector(BalloonMaker::ChangeAction)), NULL);
			this->runAction(pAction);			
			
            
            emStageType StageType = pHello->Get_CurrentStage()->getStageType();
            if(StageType == emStageType::SType_FlyingDown) set_BMakerState(BM_FlyingDown);
            else if(StageType == emStageType::SType_FlyingUp) set_BMakerState(BM_FlyingUp);
		}
		else if (get_BMakerState() == emMakerState::BM_FlyingUp && CurActStat == 0.0f) //fly up
		{
            Vec2 vecHandPt =get_BMSpineBonePos("Right_Arm_HandPt");
            Vec2 Offset(-5.0f, 55.f);
			Make_Balloon(vecHandPt + Offset, m_fMoveSpeed);
            set_StateBalloon(emBalloonState::Balloon_Up);
            setBallonMakerAnimation(emMakerState::BM_FlyingUp, false);

			if (get_BMakerState() != emMakerState::BM_Falling)
			{
                updateSpeedAndMove(m_listBalloon.size());
//
//                RiseUpDown_BMaker(_dt);
//
//                for (int ii = 0; ii < m_listBalloon.size(); ii++)
//                {
//                    Balloon* pBalloon = (Balloon*)m_listBalloon.at(ii);
//                    if (pBalloon->getReferenceCount() < 1) continue;
//
//                    pBalloon->Fly_Balloon(_dt);
//                }
			}
            else{
                
            }
            
		}
        else if (get_BMakerState() == emMakerState::BM_FlyingDown && CurActStat == 0.0f) //fly down
        {
            Vec2 vecHandPt =get_BMSpineBonePos("Right_Arm_HandPt");
            Vec2 Offset(-5.0f, 55.f);
            Make_Balloon(vecHandPt + Offset, m_fMoveSpeed);
            set_StateBalloon(emBalloonState::Balloon_Down);
            setBallonMakerAnimation(emMakerState::BM_FlyingDown, false);
            
            if (get_BMakerState() != emMakerState::BM_Falling)
            {
                updateSpeedAndMove(m_listBalloon.size());
//
//                RiseUpDown_BMaker(_dt);
//
//                for (int ii = 0; ii < m_listBalloon.size(); ii++)
//                {
//                    Balloon* pBalloon = (Balloon*)m_listBalloon.at(ii);
//                    if (pBalloon->getReferenceCount() < 1) continue;
//
//                    pBalloon->Fly_Balloon(_dt);
//                }
                
            }
            else{
                
            }
        }
        else if (get_BMakerState() == emMakerState::BM_Achieved_Down) //fly down
        {
            //log("BM_Achieved_Down");
            this->stopAllActions();
            
            //
            int nDirVal = -1;
#if SPINE2D_BallonMaker == 1
            this->getSkeleton()->flipX = nDirVal;
#else
            this->setScaleX(this->getScaleX()*(-1));
#endif
            auto Action = cocos2d::MoveBy::create(MOVE_TIME, Vec2(nDirVal * MOVE_DIST, 0.0f));
            
            this->runAction(Action);
            
            HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
            if (pHello == NULL) return;
            
            float fAttackPos = 200.0f;
            if( BM_Pos.x < -10.0f ) //SetGoblin
            {
                int nBMCnt = pHello->get_EnableStage()->get_BM_AchivedAttackCnt();
                if(nBMCnt >= 4)
                {
                    remove_BalloonMaker();
                    return;
                }
                
                this->setPosition(Vec2(fAttackPos, fAttackPos + (nBMCnt*75)));
                set_BMakerState(emMakerState::BM_Achieved_Attack);
                setBallonMakerAnimation(emMakerState::BM_Achieved_Attack, true);
                spAttachment* pAttachment = this->getAttachment("spear", "spear");
                this->setAttachment("spear", "spear");
                this->stopAllActions();
            }
        }
        else if (get_BMakerState() == emMakerState::BM_Achieved_Up) //fly up
        {
            //check noaction
            float CurActStat = this->getNumberOfRunningActions();   //if return value 0.0, this Sprite State is No Action.
            
            int nBMCnt = pHello->get_EnableStage()->get_BM_AchivedPushCnt();
            Vec2 RockPos = pHello->m_pBMrock->getPosition();
            int RockWidth = pHello->m_pBMrock->getContentSize().width;
            
            float destnationX = RockPos.x + RockWidth + (nBMCnt * 30);
            if( Vec2(destnationX,0.0f).distance(Vec2(this->getPosition().x, 0.0f)) < 0.1f )
            {
                int dd = this->getSkeleton()->flipX;
                if(this->getSkeleton()->flipX == 0)
                    this->getSkeleton()->flipX = true;
                
                this->stopAllActions();
                setBallonMakerAnimation(emMakerState::BM_Achieved_Push, true);
                set_BMakerState(emMakerState::BM_Achieved_Push);
                
                int nBMCnt = pHello->get_EnableStage()->get_BM_AchivedPushCnt();
                Vec2 RockPos = pHello->m_pBMrock->getPosition();
                int RockWidth = pHello->m_pBMrock->getContentSize().width;

                Vec2 velocity = pHello->m_pBMrock->getPhysicsBody()->getVelocity();
                
                if(velocity == Vec2(0,0)){
                    pHello->m_pBMrock->setPosition(RockPos - Vec2(RockWidth*0.5, 0.0));
                    pHello->get_EnableStage()->moveAchivedBMPos( -Vec2(RockWidth*0.5, 0.0) );
                }
            }
            else if(CurActStat == 0)
            {
                this->stopAllActions();
                
                if( destnationX - this->getPosition().x > 0 )
                    this->getSkeleton()->flipX = false;
                
                float distance = Vec2(destnationX,0.0f).distance(Vec2(this->getPosition().x, 0.0f));
                distance = (distance/300);
                float duration = 5.0f * distance;
                
                auto Action = cocos2d::MoveTo::create(duration, Vec2(destnationX, this->getPosition().y));
                this->runAction(Action);
            }

        }
        else if (get_BMakerState() == emMakerState::BM_Achieved_Attack) //fly up
        {
            //check noaction
            float CurActStat = this->getNumberOfRunningActions();   //if return value 0.0, this Sprite State is No Action.
            //m_fTempTime = m_fTime +
            
            Vec2 spearFrontPt = this->get_BMSpineBonePos("SpearFront");
            HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
            
            Rect ArcherRect = pHello->m_pArcher->getBoundingBox();
            
            if(ArcherRect.containsPoint(spearFrontPt))
            {
                //pHello->archerDie();
                log("Collision Spear!!!");
            }
            //log("x - %f, y - %f", spearFrontPt.x, spearFrontPt.y );
            //spAttachment* pAttachment = this->getAttachment("spear", "spear");
            //log("%f - m_fTime");
            if(m_fTempTime > 1.0f)
            {
                log("BM_Achieved_Up");
                this->stopAllActions();
                setBallonMakerAnimation(emMakerState::BM_Achieved_Attack, true);
            }
        }
        
		if (get_BMakerState() == emMakerState::BM_FlyingUp || get_BMakerState() == emMakerState::BM_FlyingDown)
		{
			//if Archer's PosY is similar BM's PosY, BM throw rock to Archer
			if (BM_Pos.y < Archer_pos.y + ThrowDist && BM_Pos.y > Archer_pos.y - ThrowDist)
			{
				for (int ii = 0; ii < m_listRock.size(); ii++)
				{
					Rock* pRock = m_listRock.at(ii);
					if(pRock->get_RockState() == emRockState::Rock_Standby) 
					{
                        HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
                        Vec2 ArcherPos  = pHello->getArcherPos();
                        Vec2 BMpos      = this->getPosition();
                        
						pRock->Thrown_Rock(BMpos.x - ArcherPos.x);					
						break;
					}
				}
			}
            
            //when BallonMaker's Achieve is OK
            if(get_BMakerState() == emMakerState::BM_FlyingDown)
            {
                if(m_fMaxLowPos != 0.0f && getPosition().y < m_fMaxLowPos + 40)
                {
                    set_BMakerState(emMakerState::BM_Achieved_Down);
                    set_StateBalloon(emBalloonState::Balloon_Up);
                    setBallonMakerAnimation(emMakerState::BM_Achieved_Down, true);
                    for (int ii = 0; ii < m_listBalloon.size(); ii++)
                    {
                        Balloon* pBalloon = m_listBalloon.at(ii);
                        
                        if(ii != m_listBalloon.size() - 1)
                        {
                            pHello->removeChild(pBalloon);
                            continue;
                        }
                        
                        if (pBalloon == NULL && pBalloon->getReferenceCount() < 1) continue;
                        
                        pBalloon->set_BalloonState(emBalloonState::Balloon_Free);
                        pBalloon->Fly_Balloon(_dt);
                        pBalloon->set_BalloonMaker(nullptr);
                    }
                }
            }
            
            //when BallonMaker's Achieve is OK
            if(get_BMakerState() == emMakerState::BM_FlyingUp)
            {
                if(m_fMaxHighPos != 0.0f && getPosition().y > m_fMaxHighPos - 50)
                {
                    this->stopAllActions();
                    setBallonMakerAnimation(emMakerState::BM_Walking, true);
                    set_BMakerState(emMakerState::BM_Achieved_Up);
                }
            }
		}
        
        if (get_BMakerState() == emMakerState::BM_None)
        {
            this->stopAllActions();
        }
	}
	else
	{

	}
}

void BalloonMaker::UpdateBMPos(float _dt)
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	if (this->get_BMakerState() == emMakerState::BM_Falling)
	{
		Rect BalloonMaker_box = this->getBoundingBox();

		if (this->getPosition().y < 40.0f)
		{
			this->unschedule(schedule_selector(BalloonMaker::UpdateBMPos));
            remove_BalloonMaker(true);
		}
	}
	
}

void BalloonMaker::Walk_Ground(emMovingType _MovingType)
{
	//화면 크기 가져오기
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();
	Vec2 pos = this->getPosition();

	if (_MovingType != BM_Random)
	{
		this->stopAllActions();

		//이동
		int nDirVal = -1;
#if SPINE2D_BallonMaker == 1
        this->getSkeleton()->flipX = nDirVal;
#else
        this->setScaleX(this->getScaleX()*(-1));
#endif
		auto Action = cocos2d::MoveBy::create(MOVE_TIME, Vec2(nDirVal * MOVE_DIST, 0.0f));
		

		this->runAction(Action);		
	}
	else
	{
		this->stopAllActions();

		int nRanVal = RandomHelper::random_int(0, 1);
#if SPINE2D_BallonMaker == 1
        this->getSkeleton()->flipX = nRanVal;
#else
        this->setScaleX(this->getScaleX()*(-1));
#endif

		//이동
		int nDirVal = nRanVal == 0 ? 1 : -1;		
		auto Action2 = cocos2d::MoveBy::create(MOVE_TIME, Vec2(nDirVal * MOVE_DIST, 0.0f));

		this->runAction(Action2);
	}

}


void BalloonMaker::ChangeType(emMakerType _emType)
{
	init_BMakerState(_emType, m_emMovingType);	
	//auto* pAction = CCSequence::create(ActionTin, CCCallFunc::create(this, callfunc_selector(BalloonMaker::set_BMakerFlyingUp)), NULL);
}

void BalloonMaker::ChangeAction()
{
	Color3B BM_Color3B = Color3B::WHITE;
	float fBM_vecScale = 0.0f;
    //NEEDTOCHECK
//    if (m_emType == RED) BM_Color3B = BM_COLOR_LV1;
//    if (m_emType == RED) BM_vecScale = BM_SCALE_LV1;
//    if (m_emType == WHITE) BM_Color3B = BM_COLOR_LV2;
//    if (m_emType == WHITE) BM_vecScale = BM_SCALE_LV2;


	if (m_emType == emMakerType::RED)
	{ }
	else if (m_emType == emMakerType::WHITE)
	{
		this->stopAllActions();
        BM_Color3B      = BM_COLOR_WHITE;
        fBM_vecScale    = BM_SCALE_WHITE;
		ScaleTo* ActionScale	= cocos2d::ScaleTo::create(3.0f, fBM_vecScale, fBM_vecScale);
		TintTo*  ActionTin		= cocos2d::TintTo::create(3.0f, BM_Color3B.r, BM_Color3B.g, BM_Color3B.b);
		
		this->runAction(ActionScale);
		this->runAction(ActionTin);	
	}
	else if (m_emType == emMakerType::BLACK)
	{ }
	
}


bool BalloonMaker::Make_Balloon(Vec2 _pos, float fSpeedVal)
{
	Node* pParent = getParent();
	if (pParent == NULL) return false;

    
    int MakeCnt = 0;
    if(m_emType == GREEN)
    {
        int nRanVal = RandomHelper::random_int(1, 2);
        MakeCnt = nRanVal;
    }
    else if(m_emType == RED)
    {
        int nRanVal = RandomHelper::random_int(1, 3);
        MakeCnt = nRanVal;
    }
    else if(m_emType == WHITE) {
        int nRanVal = 3;
        MakeCnt = nRanVal;
    }
    else if(m_emType == BLACK) {
        int nRanVal = 4;
        MakeCnt = nRanVal;
    }
    //MakeCnt = 2; //test
    
    for(int ii = 0; ii < MakeCnt; ii++)
    {
        Balloon* pBalloon = Balloon::create(_pos, fSpeedVal);
        pBalloon->set_BalloonMaker(this);
        
        pParent->addChild(pBalloon);
        m_listBalloon.pushBack(pBalloon);
    }



	return true;
}

bool BalloonMaker::Make_Rock(Vec2 _pos)
{
	Node* pParent = getParent();
	if (pParent == NULL) return false;


	Rock* pRock = Rock::create(_pos);
	pRock->set_BalloonMaker(this);
    pRock->setVisible(false);

	this->addChild(pRock);
	m_listRock.pushBack(pRock);
	return true;
}

void BalloonMaker::RiseUpDown_BMaker(float _dt)
{
	if (this->getReferenceCount() < 1) return;

	//---------------------------------------------------------
	auto visibleSize = Director::getInstance()->getVisibleSize();

	this->stopAllActions();	

	//
	this->getPhysicsBody()->setGravityEnable(false);
    
    MoveBy* Action = NULL;
    
    float fSpeed = 0.0f;
    fSpeed = m_fMoveSpeed;
    if(m_emType == emMakerType::WHITE)
        fSpeed = m_listBalloon.size() * m_fMoveSpeed;
    if(get_BMakerState() == emMakerState::BM_FlyingUp)
        Action = cocos2d::MoveBy::create(FLYING_TIME, Vec2(0.0f, visibleSize.height + (FLYING_DIST * fSpeed)));
    else if(get_BMakerState() == emMakerState::BM_FlyingDown)
        Action = cocos2d::MoveBy::create(FLYING_TIME, Vec2(0.0f, -(visibleSize.height + (FLYING_DIST * fSpeed))));
	
	this->runAction(Action);		
}

void BalloonMaker::Fall_BalloonMaker()
{
	if (m_listBalloon.size() < 1) 
	{
		auto visibleSize = Director::getInstance()->getVisibleSize();

		this->set_BMakerState(emMakerState::BM_Falling);
        setBallonMakerAnimation(emMakerState::BM_Falling, true);
        PhysicsBody* pPhysicBody = this->getPhysicsBody();
        if(pPhysicBody == NULL ) return;
		pPhysicBody->setGravityEnable(false);
        

		this->stopAllActions();		
		auto Action2 = cocos2d::MoveBy::create(FLYING_TIME - 4.0f , Vec2(0.0f, -(visibleSize.height + FLYING_DIST)));
		this->runAction(Action2);
		this->schedule(schedule_selector(BalloonMaker::UpdateBMPos), 0.1);
	}
}

bool BalloonMaker::eraseBalloonInList(cocos2d::Sprite* _pSprite)
{
	Balloon* pBalloon = dynamic_cast<Balloon*>(_pSprite);
	if(pBalloon == NULL) return false;

	m_listBalloon.eraseObject(pBalloon);
	return true;	
}

bool BalloonMaker::eraseRockInList(cocos2d::Sprite * _pSprite)
{
	Rock* pRock = dynamic_cast<Rock*>(_pSprite);
	if (pRock == NULL) return false;

	m_listRock.eraseObject(pRock);
	return true;
}

void BalloonMaker::eraseBalloon()
{
    for(int ii = 0; ii < m_listBalloon.size(); ii++)
    {
        Balloon* pBalloon = m_listBalloon.at(ii);
        if (pBalloon == NULL) continue;
        
        m_listBalloon.eraseObject(pBalloon);
    }
}

void BalloonMaker::eraseRock()
{
    for(int ii = 0; ii < m_listRock.size(); ii++)
    {
        Rock* pRock = m_listRock.at(ii);
        if (pRock == NULL) continue;
        
        m_listRock.eraseObject(pRock);
    }
}

bool BalloonMaker::remove_BalloonMaker(bool bDecreaseEnemy)
{
    this->set_BMakerState(emMakerState::BM_Died);
    this->stopAllActions();
    HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
    if (pHello == NULL) return false;
    
    if(bDecreaseEnemy)
        pHello->DecreaseEnemyNum();
    pHello->removeChild(this);
    
    return true;
}

emMakerState BalloonMaker::get_BMakerState()
{
	return m_emState;
}

unsigned int BalloonMaker::get_BalloonCount()
{
    return m_listBalloon.size();
}

void BalloonMaker::set_BMakerState(emMakerState _emState)
{
	m_emState = _emState;
}

void BalloonMaker::set_BMakerFlyingUp()
{
	m_emState = emMakerState::BM_FlyingUp;
}

void BalloonMaker::set_BMakerFlyingDown()
{
	m_emState = emMakerState::BM_FlyingDown;
}

void BalloonMaker::set_StateBalloon(emBalloonState _emState)
{
    for (int ii = 0; ii < m_listBalloon.size(); ii++) {
        Balloon* pBalloon = m_listBalloon.at(ii);
        pBalloon->set_BalloonState(_emState);
    }
}

void BalloonMaker::set_MaxLow(float _fLowVal)
{
    m_fMaxLowPos = _fLowVal;
}

void BalloonMaker::set_MaxHigh(float _fHighVal)
{
    m_fMaxHighPos = _fHighVal;
}

bool BalloonMaker::setBallonMakerAnimation(int _emState, bool _bLoop)
{
    
    spTrackEntry* AniTrackEntry = this->getCurrent();
    const char* const pAniName = AniTrackEntry->animation->name;
    switch (_emState) {
        case BM_Walking:
        case BM_Achieved_Up:
        case BM_Achieved_Down:
            if(!strncmp(pAniName, "Run", strlen(pAniName)))
                return false;
            this->setAnimation(0, "Run", _bLoop); //true
            
            break;
        case BM_FlyingDown:
        case BM_FlyingUp:
            if(!strncmp(pAniName, "Flying", strlen(pAniName)))
                return false;
            this->setAnimation(0, "Flying", _bLoop); //false
            
            break;
        case BM_Falling:
            if(!strncmp(pAniName, "Falling", strlen(pAniName)))
                return false;
            this->setAnimation(0, "Falling", _bLoop); //true
            
            break;
        case BM_Died:
            this->setAnimation(0, "Falling", _bLoop); //false
            this->clearTrack();
            
            break;
        case BM_Achieved_Attack:
            if(!strncmp(pAniName, "Attack", strlen(pAniName)))
                return false;
            this->setAnimation(0, "Attack", _bLoop); //true
            
            break;
        case BM_Achieved_Push:
            this->setAnimation(0, "Push", _bLoop); //true
            
            break;
        default:
            break;
    }
    
    //log("setBallonMakerAnimation");
    
    
    return true;
}

Vec2 BalloonMaker::get_BMSpineBonePos(const char* pStr)
{
    spBone* pLArmBone = this->findBone(pStr);
    Vec2 Pos = Vec2::ZERO;
    Pos = this->convertToWorldSpace(Vec2(pLArmBone->worldX, pLArmBone->worldY));
    
    return Pos;
}

bool BalloonMaker::setBMSpineSpear(bool bSpear)
{
    if(bSpear)
    {
        //let spear = this.findSlot("spear").attachment.name;
        this->setAttachment("spear", "spear");
    }
    else
    {
        this->setAttachment("spear", NULL);
    }
    
    
    
    return true;
}

void BalloonMaker::updateSpeedAndMove(int balloonCnt) {
    int nothing = 0;
    RiseUpDown_BMaker(nothing);
    
    for (int ii = 0; ii < m_listBalloon.size(); ii++)
    {
        Balloon* pBalloon = (Balloon*)m_listBalloon.at(ii);
        if (pBalloon->getReferenceCount() < 1) continue;
        
        if(m_emType == WHITE)
            pBalloon->set_MoveDistance(m_listBalloon.size() * m_fMoveSpeed);
        
        pBalloon->Fly_Balloon(nothing);
    };
}




