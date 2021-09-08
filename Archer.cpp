#include "Archer.h"


using namespace cocos2d;

#define DEFALUT_ARR_CNT 5		//
#define SCREEN_ARR_CNT 2		//Screen에 보이는 최대 화살 갯수
//#define	SHOOT_TERM 0.001f			//




Archer::Archer()
: m_emArcherState(emArcherState::Arc_Idle)
{
}

Archer::~Archer()
{
}

Archer* Archer::create(cocos2d::Vec2 _pos)
{
#if SPINE2D == 1
	Archer *pArc = new (std::nothrow) Archer();
	if (pArc)
	{
		//SkeletonAnimation* node = new SkeletonAnimation();
//        spAtlas* atlas = spAtlas_createFromFile("spineboy1.atlas", 0);
//        pArc->initWithJsonFile("spineboy-ess.json", atlas, 0.6);
        
#if	USE_VISUALSTUDIO == 1
		spAtlas* atlas = spAtlas_createFromFile("spine/Elf_Archer_01.atlas", 0);
		pArc->initWithJsonFile("spine/Elf_Archer_01.json", atlas, 0.6);
#else
		spAtlas* atlas = spAtlas_createFromFile("Elf_Archer_01.atlas", 0);
		pArc->initWithJsonFile("Elf_Archer_01.json", atlas, 0.6);
#endif

        

		pArc->init_SkelAnimation();
		pArc->init_Cocos2dInfo(_pos);
		//pArc->init_ArcherState();
		pArc->autorelease();
		return pArc;		
	}
	CC_SAFE_DELETE(pArc);
	return nullptr;
#else
    Archer *sprite = new (std::nothrow) Archer();
    if (sprite && sprite->initWithFile("Archer_60.png"))
    {
        auto scene = Scene::createWithPhysics( );
        scene->getPhysicsWorld( )->setDebugDrawMask( PhysicsWorld::DEBUGDRAW_ALL );
        
        sprite->init_Cocos2dInfo(_pos);
        sprite->autorelease();
        //sprite->setPhysicsWorld( scene->getPhysicsWorld( ) );
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return nullptr;
#endif
    

}

void Archer::init_SkelAnimation()
{
#if SPINE2D == 1
	this->setStartListener([](spTrackEntry* entry) {
		//log("%d start: %s", entry->trackIndex, entry->animation->name);
	});
	this->setInterruptListener([](spTrackEntry* entry) {
		//log("%d interrupt", entry->trackIndex);
	});
	this->setEndListener([](spTrackEntry* entry) {
		//log("%d end", entry->trackIndex);
	});
	this->setCompleteListener([](spTrackEntry* entry) {
		//log("%d complete", entry->trackIndex);
	});
	this->setDisposeListener([](spTrackEntry* entry) {
		//log("%d dispose", entry->trackIndex);
	});
	this->setEventListener([](spTrackEntry* entry, spEvent* event) {
		//log("%d event: %s, %d, %f, %s", entry->trackIndex, event->data->name, event->intValue, event->floatValue, event->stringValue);
	});
    

    this->setMix("Run", "Idle", 2.5);
    this->setMix("Idle", "Run", 2.5);
    this->setMix("Idle", "WaitShooting", 0.5);
    this->setMix("WaitShooting", "Idle" , 1.5);
//    this->setMix("WaitShooting", "Shooting" , 0.3);
//    this->setMix("Shooting", "WaitShooting" , 0.3);
    //this->setAnimation(0, "Run", true);
    
    this->addAnimation(0, "WaitShooting", true);
    
    spTrackEntry* ShootingEntry = this->addAnimation(0, "Shooting", false, 1);
    this->setTrackStartListener(ShootingEntry, [](spTrackEntry* entry) {
        log("%d Shooting", entry->trackIndex);
    });
    
	// skeletonNode->addAnimation(1, "test", true);
	// skeletonNode->runAction(RepeatForever::create(Sequence::create(FadeOut::create(1), FadeIn::create(1), DelayTime::create(5), NULL)));

	//scheduleUpdate();
#endif
	
}

void Archer::init_Cocos2dInfo(cocos2d::Vec2 _pos)
{
	
	//Apply PhysicsBody
    this->update(0.0f);
	Rect skelRect = this->getBoundingBox();
    Vec2 pos(Vec2::ZERO);
    pos.y = skelRect.origin.y + skelRect.size.height * 0.45f;
    skelRect.size.height = skelRect.size.height * 0.9;
    skelRect.size.width = skelRect.size.width * 0.7;
	PhysicsBody* pbody = PhysicsBody::createBox(skelRect.size, BASIC_MATERIAL, pos);
	pbody->setDynamic(false);
	pbody->setContactTestBitmask(0xFFFFFFFF);

	this->setPhysicsBody(pbody);
	this->setTag(emNameTag::ARCHER);
	this->setPosition(_pos);
	this->setScale(1.0);
	this->getPhysicsBody()->setGravityEnable(false);  //Apply Grabvity 
}

void Archer::init_ArcherState()
{
	//Arrow init
	for (int ii = 0; ii < DEFALUT_ARR_CNT; ii++)
		Make_ArcherArrow(Vec2::ZERO);
	

	Arrow* pArr = m_listArrow.at(0);
	if (pArr != NULL) m_listArrow.at(0)->set_ArrowState(emArrowState::Arr_ShootWait);
}


bool Archer::Make_ArcherArrow(cocos2d::Vec2 _pos)
{
	Arrow* pArr = Arrow::create(_pos, this);
    pArr->setVisible(false);
	pArr->setScale(ARCHER_SCALE);
	this->addChild(pArr);
    
	m_listArrow.pushBack(pArr);

	return true;
}

int Archer::ShootArrow()
{
	int shooing = 0;
	for (int ii = 0; ii < m_listArrow.size(); ii++)
	{
		Arrow* pItArr = m_listArrow.at(ii);
		emArrowState CurState = m_listArrow.at(ii)->get_ArrowState();

		//Shooting State 2이상 시 Pass
		int nShooting_Cnt = 0;
		for (int jj = 0; jj < m_listArrow.size(); jj++)		
			if (m_listArrow.at(jj)->get_ArrowState() == emArrowState::Arr_Shooting) nShooting_Cnt += 1;
				

		
		if (nShooting_Cnt >= SCREEN_ARR_CNT || CurState == emArrowState::Arr_Shooting || CurState == emArrowState::Arr_Falling ) continue;

		//
		if (CurState == emArrowState::Arr_ShootWait)
		{
			//pItArr->ShootArrow();
			pItArr->ShootParabola();
			shooing = 1;
		}
		
	}
    

	//Wait상태 화살이 없다면 만든다.
	int nWaitCnt = 0;
	for (int ii = 0; ii < m_listArrow.size(); ii++)
		if (m_listArrow.at(ii)->get_ArrowState() == emArrowState::Arr_ShootWait)	nWaitCnt += 1;
    
    int nShooting_Cnt = 0;
    for (int jj = 0; jj < m_listArrow.size(); jj++)
        if (m_listArrow.at(jj)->get_ArrowState() == emArrowState::Arr_Shooting) nShooting_Cnt += 1;

	if (nWaitCnt == 0 && nShooting_Cnt < SCREEN_ARR_CNT)
	{
		for (int ii = 0; ii < m_listArrow.size(); ii++)
		{
			Arrow* pItArr = m_listArrow.at(ii);
			if (pItArr->get_ArrowState() == emArrowState::Arr_Preparing)
			{
				pItArr->set_ArrowState(emArrowState::Arr_ShootWait);
                //pItArr->setVisible(true);
				break;
			}
            else
            {
                //pItArr->setVisible(false);
                
            }
		}
	}

	return shooing;
	
}

void Archer::Set_TouchPosition(float _x, float _y)
{
	Vec2 _Pos(_x, _y);
	for (int ii = 0; ii < m_listArrow.size(); ii++)
	{
		Arrow* pItArr = m_listArrow.at(ii);
		if(pItArr->get_ArrowState() == emArrowState::Arr_Preparing || pItArr->get_ArrowState() == emArrowState::Arr_ShootWait
			|| pItArr->get_ArrowState() == emArrowState::Arr_Hit)
			pItArr->setPosition(Vec2::ZERO);
	}

	this->setPosition(_Pos);
}

emArcherState Archer::get_ArcState(void)
{
	return m_emArcherState;
}

void Archer::set_ArcState(emArcherState _emState)
{
	m_emArcherState = _emState;
}

Vec2 Archer::setArcherAnimation(int _emState)
{
    this->addAnimation(0, "Shooting", false);
    //this->addAnimation(0, "WaitShooting", false);
    
    spSkeleton* pSkel = getSkeleton();
    //spSkeleton_findBone(<#const spSkeleton *self#>, <#const char *boneName#>)
    
    //this->updateWorldTransform();
    spBone* pLArmBone = this->findBone("Left_Arm_HandPt");
    
    Vec2 Pos = Vec2::ZERO;
    
    Pos = this->convertToWorldSpace(Vec2(pLArmBone->worldX, pLArmBone->worldY));
    
    return Pos;
}

Vec2 Archer::get_ArcSpineBonePos(const char* pStr)
{
    spBone* pLArmBone = this->findBone(pStr);
    Vec2 Pos = Vec2::ZERO;
    Pos = this->convertToWorldSpace(Vec2(pLArmBone->worldX, pLArmBone->worldY));
    
    return Pos;
}
float Archer::get_ArcSpineBoneRotX(const char* pStr)
{
    spSkeleton* pSkel = getSkeleton();
    
    spBone* pLArmBone = this->findBone(pStr);
    float fRot = 0.0f;
    //pLArmBone->rotation
    fRot = spBone_getWorldRotationX(pLArmBone);
    
    return fRot;
}

float Archer::get_ArcSpineBoneRotY(const char* pStr)
{
    spSkeleton* pSkel = getSkeleton();
    
    spBone* pLArmBone = this->findBone(pStr);
    float fRot = 0.0f;
    fRot = spBone_getWorldRotationY(pLArmBone);
    
    return fRot;
}

void Archer::DyingAction()
{
	this->stopAllActions();
	auto action = cocos2d::RotateTo::create(0.4f, 100);
	auto action2 = cocos2d::MoveBy::create(2.5f,Vec2(50.0f, -200.0f));
	
	this->runAction(action);
	this->runAction(action2);
}
