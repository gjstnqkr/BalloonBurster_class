#include "Stage.h"
#include "BB_GameInfo.h"
#include "HelloWorldScene.h"
#include "BB_ActionTexture.hpp"
#include "Balloon.h"
#include "MC_SoundBox.hpp"


using namespace cocos2d;

#define EnemyTerm 6.5f
#define EachEnemyTerm 1.4f
#define StageChangeTerm 10.0f
#define ArcherRockTerm 6.5f

Stage::Stage()
	: m_unStage1st(0)
    ,m_unCurrentStage2nd(1)
    , m_unUserLife(LIFE_CNT)
    , m_unUserScore(0)
	, m_unEnemyCnt(0)
	, m_fTotalTime(0.0f)
	, m_fTempTime(0.0f)
	, m_nTempNum(0)
    , m_dRotValue(0)
	, m_nCloudTime(30)	
	, m_lbEnemy(NULL)
    , m_lbCenter(NULL)	
    , m_emTempState(emStageState::SState_None)
    , m_bStageEnable(false)
	, m_bTouchBallonMode(true)
    , m_fArcherRockTime(0.0f)
    , m_ArcherRock(nullptr)
	, m_LightningBtn(nullptr)
{
}


Stage::~Stage()
{
}

Stage* Stage::create(unsigned int _nStage1st, cocos2d::Node* pParent)
{
	
	Stage * ret = new (std::nothrow) Stage();
	if (ret && ret->init())
	{
		ret->autorelease();
        ret->set_Stage1stNum(_nStage1st);
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}
	return ret;
}

//when Stage Class is started, this func will begin
void Stage::onEnterTransitionDidFinish()
{
}

void Stage::initOnce()
{
    m_unCurrentStage2nd     = 1;
    m_unEnemyCnt            = 0;
    m_unMissedEnemyCnt      = 0;
    m_fTotalTime            = 0.0f;
    m_fTempTime             = 0.0f;
    m_nTempNum              = 0;
    m_unUserScore           = 0;
    m_unUserLife            = LIFE_CNT;
    
    initStageVal(m_unStage1st);
    initLabels();
    //Set_UserScoreLabel(0);
    init_UserlifeSprite();    
    init_Current();
	init_Sprite();

	//Single
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Stage::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Stage::onTouchMoved, this);
	touchListener->onTouchEnded = CC_CALLBACK_2(Stage::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
   
}

void Stage::init_Current(int _nStage2ndNum)
{
    HelloWorld* pHello = dynamic_cast<HelloWorld*>( getParent() );
    Clear_BMList();
    Clear_SpriteInScene();
    
    //To UI Setting
    BB_GameInfo* pGameInfo = BB_GameInfo::GetInstance();
    pGameInfo->setCurEnemyCnt(m_listStageValue.at(_nStage2ndNum - 1)->getEnemyTotalCnt());
    
    unsigned int unEnemyBnt = m_listStageValue.at(_nStage2ndNum - 1)->getEnemyTotalCnt();
    unsigned int Cnt = pHello->getEnemyCntFromUI();
    
    if(Cnt != 0 && Cnt != unEnemyBnt)
        unEnemyBnt = Cnt;
    
    BB_GameInfo::GetInstance()->setCurEnemyCnt(unEnemyBnt);
    
    //member value init
    m_unCurrentStage2nd    = _nStage2ndNum;
    
    
    m_unMissedEnemyCnt  = 0;
    m_fTotalTime        = 0.0f;
    m_fTempTime         = 0.0f;
    m_nTempNum          = 0;
    m_unUserScore       = m_unUserScore;
    
    Set_PosArcherfromParents();
    
    
    if(pHello != nullptr) pHello->set_GameSceneSprite(m_unStage1st, _nStage2ndNum);
}

void Stage::initStageVal(unsigned int _nStage1st)
{
    //Clear StageValue
    m_listStageValue.clear();    
	
	
    int (*ppArr)[STAGE_WAVE];
	int(*ppArrInfi)[STAGE_INFINITE_WAVE];
    switch (_nStage1st) {
        case STAGE_1st_ONE:
            ppArr = StageOrder;
            break;
        case STAGE_1st_TWO:
            ppArr = StageOrder_2;
            break;
		case STAGE_Infinite_01:
			ppArrInfi = StageOrder_Infinite_1;
			break;
			
        default:
            ppArr = StageOrder;
            break;
    }

    for (int ii = 0; ii < (_nStage1st > 10 ? STAGE_INFINITE_CNT : STAGE_CNT); ii++) {
        emStageType emStageType = emStageType::SType_FlyingUp;
		int nType = (_nStage1st > 10 ? ppArrInfi[ii][0] : ppArr[ii][0]);
        if(nType == STAGE_UP)
			emStageType = emStageType::SType_FlyingUp;
        else if(nType == STAGE_DOWN)
			emStageType = emStageType::SType_FlyingDown;
        else if(nType == STAGE_BONUS)
			emStageType = emStageType::SType_Bonus;
        
        StageValue* pSV = StageValue::create(ii+1, emStageType);		
        int enemyTotalCnt = 0;
        for (int jj = 1; jj < (_nStage1st > 10 ? STAGE_INFINITE_WAVE : STAGE_WAVE); jj++) {
            pSV->m_ListEnemyCnt.pushBack(__Integer::create(_nStage1st > 10 ? ppArrInfi[ii][jj] : ppArr[ii][jj]) );
            enemyTotalCnt = enemyTotalCnt + _nStage1st > 10 ? ppArrInfi[ii][jj] : ppArr[ii][jj];
        }

        pSV->SetEnemyTotalCnt(enemyTotalCnt);
        m_listStageValue.pushBack(pSV);
    }	
}

void Stage::initLabels()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Node* pParent = this->getParent();
    if (pParent != NULL)
    {
        //Stage Center Label init
        if (m_lbCenter == NULL)
        {
            m_lbCenter = Label::createWithBMFont("fonts/bitmapFontTest4.fnt", "0");
            m_lbCenter->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
            m_lbCenter->setVisible(false);
            pParent->addChild(m_lbCenter);
        }
    }
}


void Stage::init_Event(int _nStageNum)
{
    //adds Contact event listener
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(Stage::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void Stage::Start_Stage()
{
    initOnce();
    this->schedule(schedule_selector(Stage::Stage_Schedular), 0.5f);

	//if ((get_Stage1stNum() >= 3 && get_Stage1stNum() < 5) || (get_Stage1stNum() > 11))
	{
		int nRanVal = RandomHelper::random_int(30, 60);
		m_nCloudTime = nRanVal;

		this->schedule(schedule_selector(Stage::Start_CloudSchedular), 5);
	}
}

void Stage::Delete_Stage()
{
    for (int jj = 0; jj < m_listBalloonMaker.size(); jj++) {
        if(m_listBalloonMaker.at(jj) != nullptr) delete m_listBalloonMaker.at(jj);
    }
    
    for (int jj = 0; jj < m_listBalloonMaker.size(); jj++) {
        if(m_listBalloonMaker.at(jj) != nullptr) delete m_listStageValue.at(jj);
    }
    
    for (int jj = 0; jj < m_listBalloonMaker.size(); jj++) {
        if(m_listBalloonMaker.at(jj) != nullptr) delete m_listUserLifeSprite.at(jj);
    }
    
    for (int jj = 0; jj < m_listBird.size(); jj++) {
        if(m_listBird.at(jj) != nullptr) delete m_listBird.at(jj);		
    }

	for (int jj = 0; jj < m_vecCloud.size(); jj++) {
		if (m_vecCloud.at(jj) != nullptr) delete m_vecCloud.at(jj);
	}
}

void Stage::Start_StageTextAndAppearBM() //Start Point
{
    ShowStageText(m_unStage1st, m_unCurrentStage2nd);
    
    StageValue* pCurStageV = getCurrentStageVal();
    if(pCurStageV->getStageType() == emStageType::SType_Bonus)
        this->schedule(schedule_selector(Stage::Bonus_Scheduler), 0.1f);
    else
    {
        this->schedule(schedule_selector(Stage::AppearBM_Scheduler), 0.1f);
        
        
        if(pCurStageV->getStageType() == emStageType::SType_FlyingUp)
        {
            HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
            pHello->setBMRockGravity(false);
            auto visibleSize = Director::getInstance()->getVisibleSize();
            pHello->m_pBMrock->setPosition(Vec2( (visibleSize.width * 0.5 ) , (visibleSize.height * 0.88) ));
        }
    }
    
}


//Once every 0.5 seconds Call this Function again again
//to manage Stage All
void Stage::Stage_Schedular(float _dt)
{
    m_fArcherRockTime++;
    if(m_fArcherRockTime > ArcherRockTerm)
    {
        m_fArcherRockTime = 0.0f;
    }
    
    StageValue* pCurStageV = getCurrentStageVal();
    if(pCurStageV->getStageState() == emStageState::SState_Ready)
    {
        init_Current(m_unCurrentStage2nd);

        pCurStageV->SetStageState(emStageState::SState_Fighting);
        HelloWorld* pHello = dynamic_cast<HelloWorld*>( getParent() );
        
        if(m_unCurrentStage2nd > STAGE_LAST) return;
        Start_StageTextAndAppearBM();
    }
    else if(pCurStageV->getStageState() == emStageState::SState_Fighting)
    {
        
    }
    else if(pCurStageV->getStageState() == emStageState::SState_GameOver)
    {
        HelloWorld* pHello = dynamic_cast<HelloWorld*>( getParent() );
        if(pHello != NULL)
        {
            pHello->Show_GameOver();
        }
        pCurStageV->SetStageState(emStageState::SState_None);
        
        
        this->scheduleOnce(schedule_selector(Stage::Start_GameOverScheduler), 6.0f);
    }
    else if(pCurStageV->getStageState() == emStageState::SState_UserDie)
    {
        StopAction_BalloonMaker();
        m_emTempState = emStageState::SState_Ready;
        Call_FuncAfterFewTime(schedule_selector(Stage::Change_CurrentStageState), 4.0f);
        Call_FuncAfterFewTime(schedule_selector(Stage::Remove_UserLifeSpriteAtList), 4.1f);
        

        pCurStageV->SetStageState(emStageState::SState_None);
    }
    else if(pCurStageV->getStageState() == emStageState::SState_Clear)
    {
        Clear_BMList();
        this->unschedule(schedule_selector(Stage::AppearBM_Scheduler));
        this->scheduleOnce(schedule_selector(Stage::NextStage), 3.0f);
        pCurStageV->SetStageState(emStageState::SState_End); //temp
        ShowStageClear();
        
        HelloWorld* pHello = dynamic_cast<HelloWorld*>( getParent() );
        if(pHello != NULL)
        {
            pHello->erase_childRock();
            pHello->erase_childBalloon();
        }
    }
    else if(pCurStageV->getStageState() == emStageState::SState_End)
    {
        if(m_unCurrentStage2nd == STAGE_LAST)
        {
            HelloWorld* pHello = dynamic_cast<HelloWorld*>( getParent() );
            if(pHello != NULL)
            {
                if(m_unStage1st == STAGE_1st_ONE)       // Stage's 1-x is ended.
                    pHello->set_AnimationScene(ANIMATION_1_END);
                else if(m_unStage1st == STAGE_1st_TWO) // Stage's 2-x is ended.
                    pHello->set_AnimationScene(ANIMATION_2_END);
            }
            //Start Point
            Director::getInstance()->popScene();
        }
    }
    
    if(get_BM_AchivedPushCnt() > 6 && getCurrentStageVal()->getStageType() == emStageType::SType_FlyingUp)
    {
        HelloWorld* pHello = dynamic_cast<HelloWorld*>( getParent() );
        Vec2 RockPos = pHello->m_pBMrock->getPosition();
        Vec2 Archer = pHello->m_pArcher->getPosition();
        
        Vec2 velocity = pHello->m_pBMrock->getPhysicsBody()->getVelocity();
        if(RockPos.x - Archer.x  < 70 && Vec2(0, 0) == velocity )
        {
            pHello->setBMRockGravity(true);
        }
    }
    
}

//appear Balloon Maker
void Stage::AppearBM_Scheduler(float _dt)
{
	m_fTotalTime += _dt;

	if (m_fTotalTime > EnemyTerm)
	{
        int nCnt = 0;
        StageValue* pSV = getCurrentStageVal();
        ssize_t size = pSV->m_ListEnemyCnt.size();
        __Integer* pInt = nullptr;        
        
        if( size > 0 )
        {
			pInt = pSV->m_ListEnemyCnt.front();
            nCnt = pInt->getValue();
            pSV->m_ListEnemyCnt.eraseObject(pInt);
        }
        else
        {
            int nCurEnemyCnt = pSV->getEnemyTotalCnt();
            if(nCurEnemyCnt > 4) nCurEnemyCnt = 4;
            nCnt = RandomHelper::random_int(0, nCurEnemyCnt);            
        }        
        
        if(pSV->getStageState() == emStageState::SState_Fighting && nCnt > 0)
            Appear_Group(nCnt);
        else if(pSV->getStageState() == emStageState::SState_Clear)
            Clear_BMList();            
        
		m_fTotalTime = 0.0f;
	}
	
}

//appear Balloon Maker
void Stage::Bonus_Scheduler(float _dt)
{
    if(m_listBird.size() < 1)
    {
        auto visibleSize = Director::getInstance()->getVisibleSize();
        AddBird(Vec2(visibleSize.width, visibleSize.height * 0.5 + 200));
    }
    
    for (int jj = 0; jj < m_listBird.size(); jj++) {
        float CurActStat = m_listBird.at(jj)->getNumberOfRunningActions();
        Vec2 pos = m_listBird.at(jj)->getPosition();
        m_listBird.at(jj)->setPositionX(pos.x - 2.2);
        pos = m_listBird.at(jj)->getPosition();
        
        if(CurActStat == 0)
        {
            auto visibleSize = Director::getInstance()->getVisibleSize();
            
            
            int destH = 300;
            
            if(sqrt(visibleSize.height * 0.5 - pos.y) < 10.0) destH = 125;
            if(visibleSize.height * 0.5 < pos.y) destH = -destH;
            
            if(visibleSize.width * 0.5 > pos.x) destH = 300;
            
            auto move = MoveBy::create(1.2f, Vec2(0.0f, destH));
            auto ease_out = EaseInOut::create(move->clone(), 2.0f);
            m_listBird.at(jj)->runAction(ease_out);
        }
        
    }
    
}

void Stage::AddBird(Vec2 Pos)
{
    Node* pParent = this->getParent();
    if (pParent == NULL) return;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    Sprite* pSprite = Sprite::create("bird1.png");  //여기서부터20201104
    pSprite->setPosition(Pos);
    pSprite->setScale(1.5);
    m_listBird.pushBack(pSprite);
    pParent->addChild(pSprite);
}

void Stage::appearCloud()
{
	int makeCnt = RandomHelper::random_int(2, 10);
	auto visibleSize = Director::getInstance()->getVisibleSize();

	int cntCurCloud = 0;
	for (int ii = 0; ii < m_vecCloud.size(); ii++)
	{
		Sprite* itor = m_vecCloud.at(ii);
		if(itor->getOpacity() == 254)
			cntCurCloud++;
	}


	cntCurCloud = makeCnt - cntCurCloud;
	makeCnt = 0;
	for (int ii = 0; ii < m_vecCloud.size(); ii++)
	{
		if (makeCnt >= cntCurCloud) break;
		Sprite* itor = m_vecCloud.at(ii); 
		if (itor->getOpacity() != 255) continue;
			
		int valueX = RandomHelper::random_real(visibleSize.width * 0.2f, visibleSize.width * 0.9f);
		int valueY = RandomHelper::random_real(visibleSize.height * 0.2f, visibleSize.height * 0.9f);
			
		itor->setPosition(Vec2(valueX, valueY));
		itor->setOpacity(254);
		makeCnt++;
	}

}

void Stage::Appear_Group(unsigned int _unEnemyCnt)
{
    unsigned int unCnt = _unEnemyCnt;
    if (_unEnemyCnt > 4) unCnt = 4;
    
    m_nTempNum = unCnt;
    
    this->schedule(schedule_selector(Stage::AppearanceTimer), 0.1f);
}

void Stage::AppearanceTimer(float _dt)
{
	if (m_fTempTime == 0.0f && m_nTempNum > 0)
		Appear_BalloonMaker((emMovingType)m_nTempNum);

	m_fTempTime += _dt;


	if (m_fTempTime > EachEnemyTerm)
	{
		m_fTempTime = 0.0f;	
		
		int nRanVal = RandomHelper::random_int(0, 1);
		
		//
		nRanVal = nRanVal == 0 ? 1 : -1;

		m_nTempNum -= 1;
		if(m_nTempNum < 0)
		{
			m_nTempNum = 0;
			this->unschedule(schedule_selector(Stage::AppearanceTimer));
			return;
		}
	}
	
}

void Stage::lbCenter_InvisibleScheduler(float _dt)
{
    this->stopAllActions();
    FadeOut* ActionFade      = FadeOut::create(3.0f);
    this->runAction(ActionFade);
    
    m_lbCenter->setVisible(false);
}

void Stage::Start_GameOverScheduler(float _dt)
{
    HelloWorld* pHello = dynamic_cast<HelloWorld*>( getParent() );
    if(pHello != NULL)
    {
        pHello->Hide_GameOver();
    }
    
    Director::getInstance()->popToRootScene();
}

void Stage::Start_CloudSchedular(float _dt)
{
	appearCloud();	
}

void Stage::Appear_BalloonMaker(emMovingType _mType)
{
	Node* pParent = this->getParent();
    StageValue* pCurStage = getCurrentStageVal();
	if (pParent == NULL && pCurStage->getStageState() == emStageState::SState_Clear) return;

    
	auto visibleSize = Director::getInstance()->getVisibleSize();
    float fAppearPosH = visibleSize.height / 2;
    
    if(pCurStage->getStageType() == emStageType::SType_FlyingDown) //This Code will be Changed to other way
        fAppearPosH = visibleSize.height * 0.85; //Current Height 80%
    else
        fAppearPosH = visibleSize.height * 0.15; //Current Height 20%
    
    int type = RandomHelper::random_int(0, (int)emMakerType::ORC);
	Vec2 pos = Vec2(visibleSize.width, fAppearPosH);
    BalloonMaker* pBM = BalloonMaker::create(pos, (emMakerType)type, _mType);
    
	pBM->setScale(GOBLIN_SCALE);
    pBM->set_MaxHigh(visibleSize.height * 0.90);
    pBM->set_MaxLow(visibleSize.height * 0.15);
    
	m_listBalloonMaker.pushBack(pBM);
	
	pParent->addChild(pBM);
}

void Stage::DecreaseEnemyNum()
{
    BB_GameInfo* pGameInfo = BB_GameInfo::GetInstance();
    if(pGameInfo->getCurEnemyCnt() > 0)
    {
        unsigned int unEnemyCnt = pGameInfo->getCurEnemyCnt()-1;
        BB_GameInfo::GetInstance()->setCurEnemyCnt(unEnemyCnt);
    }

    if(pGameInfo->getCurEnemyCnt() == 0)
    {
        StageValue* pCurrentStage = m_listStageValue.at(m_unCurrentStage2nd - 1);
        pCurrentStage->SetStageState(emStageState::SState_Clear);
    }
}

unsigned int Stage::get_UserScore()
{
    return m_unUserScore;
}

unsigned int Stage::get_BM_AchivedAttackCnt()
{
    unsigned int AchivedCnt = 0;
    for(int ii = 0; ii < m_listBalloonMaker.size(); ii++)
    {
        BalloonMaker* pBM = m_listBalloonMaker.at(ii);
        if(pBM->get_BMakerState() == emMakerState::BM_Achieved_Attack)
            AchivedCnt++;
    }
    
    return AchivedCnt;
}

unsigned int Stage::get_BM_AchivedPushCnt()
{
    unsigned int AchivedCnt = 0;
    for(int ii = 0; ii < m_listBalloonMaker.size(); ii++)
    {
        BalloonMaker* pBM = m_listBalloonMaker.at(ii);
        if(pBM->get_BMakerState() == emMakerState::BM_Achieved_Push)
            AchivedCnt++;
    }
    
    return AchivedCnt;
}

unsigned int Stage::moveAchivedBMPos(Vec2 _Move)
{
    unsigned int AchivedCnt = 0;
    for(int ii = 0; ii < m_listBalloonMaker.size(); ii++)
    {
        BalloonMaker* pBM = m_listBalloonMaker.at(ii);
        if(pBM->get_BMakerState() == emMakerState::BM_Achieved_Push)
        {
            Vec2 Pos = pBM->getPosition();
            pBM->setPosition(Pos + _Move);
        }
    }
    
    return AchivedCnt;
}

unsigned int Stage::get_Stage1stNum()
{
    return m_unStage1st;
}


void Stage::SetEnemyLabel(int _Value)
{
	char str[10] = { 0 };
	sprintf(str, "%d", _Value);
	//sprintf(str, "%2.2f", m_fTime);
	m_lbEnemy->setString(str);
}

void Stage::SetStageTextLabel(int _n1st ,int _n2nd)
{
    char str[50] = { 0 };
    sprintf(str, "Stage %d - %d", _n1st, _n2nd);

    m_lbCenter->setString(str);
}

void Stage::Set_UserScore(unsigned int _unScore)
{
    m_unUserScore = _unScore;
}

void Stage::set_Stage1stNum(unsigned int _unStage1st)
{
    m_unStage1st = _unStage1st;
}



void Stage::NextStage(float _float)
{
    getCurrentStageVal()->SetStageState(emStageState::SState_End);
    if(m_unCurrentStage2nd < STAGE_LAST)
        m_unCurrentStage2nd = m_unCurrentStage2nd + 1;
}

void Stage::ShowStageText(int _n1st ,int _n2nd)
{
    m_lbCenter->setVisible(true);
    
    SetStageTextLabel(_n1st, _n2nd);
    this->scheduleOnce(schedule_selector(Stage::lbCenter_InvisibleScheduler), 3.0f);
}

void Stage::ShowStageClear()
{
    m_lbCenter->setVisible(true);
    char str[20] = { 0 };
    sprintf(str, "Stage Clear!!");
    
    m_lbCenter->setString(str);

    this->scheduleOnce(schedule_selector(Stage::lbCenter_InvisibleScheduler), 3.0f);
    //log("ShowStageClear()__");
    
    Clear_BMList();
    StopAction_BalloonMaker();
}

void Stage::ShowGameEnd()
{
    m_lbCenter->setVisible(true);
    char str[20] = { 0 };
    sprintf(str, "Stage Clear!!");
    
    m_lbCenter->setString(str);
    
    this->scheduleOnce(schedule_selector(Stage::lbCenter_InvisibleScheduler), 3.0f);
    //log("ShowStageClear()__");
    
    Clear_BMList();
    StopAction_BalloonMaker();
}

void Stage::StopAction_BalloonMaker()
{
    for(int ii = 0 ; ii < m_listBalloonMaker.size(); ii++)
    {
        m_listBalloonMaker.at(ii)->set_BMakerState(emMakerState::BM_None);
    }
}

StageValue* Stage::getCurrentStageVal()
{
    StageValue* pCurrentStage = m_listStageValue.at(m_unCurrentStage2nd - 1);
    
    return pCurrentStage;
}

void Stage::Clear_BMList()
{
    Node* pParent = this->getParent();
    if (pParent != NULL)
    {
        //MB Remove
        for (int ii = 0; ii < m_listBalloonMaker.size(); ii++)
        {
            m_listBalloonMaker.at(ii)->eraseBalloon();
            m_listBalloonMaker.at(ii)->eraseRock();
            pParent->removeChild(m_listBalloonMaker.at(ii));
        }
    }
    m_listBalloonMaker.clear();
}

void Stage::Clear_SpriteInScene()
{
    HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
    if (pHello != NULL)
        pHello->erase_childSprite();
}


void Stage::Add_UserLifeSpriteAtList()
{
    Node* pParent = this->getParent();
    
    if (pParent == NULL) return;
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    Sprite* pSprite = Sprite::create("tomato.png");
    pSprite->setPosition(Vec2(visibleSize) - Vec2(visibleSize.height/10+10,20));
    
    m_listUserLifeSprite.pushBack(pSprite);
    pParent->addChild(pSprite);

    Align_UserLifeInScreen();
}

void Stage::Remove_UserLifeSpriteAtList(float dt)
{
    Node* pParent = this->getParent();
    //unsigned int nLifeCnt = m_listUserLifeSprite.size();
    BB_GameInfo* pGameInfo = BB_GameInfo::GetInstance();
    int nLifeCnt = pGameInfo->getLifeCnt();
    nLifeCnt = nLifeCnt -1;
    
    if(nLifeCnt >= 0)
        BB_GameInfo::GetInstance()->setLifeCnt(nLifeCnt);
    
    if (pParent == NULL || nLifeCnt < 1) return;
    
    m_unUserLife = m_unUserLife -1;
    Sprite* pSpt = m_listUserLifeSprite.at(nLifeCnt-1);
    
    pParent->removeChild(pSpt);
    m_listUserLifeSprite.eraseObject(pSpt);
    
    Align_UserLifeInScreen();
    
}

//void Stage::Minus_UserLife(float dt)
//{
//    Node* pParent = this->getParent();
//    unsigned int nLifeCnt = m_listUserLifeSprite.size();
//
//    if(nLifeCnt > 0)
//        BB_GameInfo::GetInstance()->setLifeCnt(nLifeCnt);
//
//
//    BB_GameInfo* pGameInfo = BB_GameInfo::GetInstance();
//    int LifeCnt = pGameInfo->getLifeCnt();
//
//    if(nLifeCnt > 0)
//    pGameInfo->setLifeCnt(LIFE_CNT);
//}

void Stage::Align_UserLifeInScreen()
{
    unsigned int nLifeCnt = m_listUserLifeSprite.size();
    auto visibleSize = Director::getInstance()->getVisibleSize();
    
    int Pos_Y = 20;
    for(int ii = 0; ii < nLifeCnt; ii++)
    {
        Sprite* pSprite = m_listUserLifeSprite.at(ii);
        Rect rec = pSprite->getTextureRect();
        int sptWidth = rec.size.width;
        int Start_Pos = sptWidth * nLifeCnt;
        pSprite->setPosition(Vec2(visibleSize) - Vec2(RIGHT_OFFSET+(Start_Pos - (sptWidth*ii)), Pos_Y));
    }
}

void Stage::init_UserlifeSprite()
{
    Clear_LifeList();
    
    for(int ii = 0; ii < m_unUserLife; ii++)
        Add_UserLifeSpriteAtList();
}

void Stage::init_Sprite()
{
	HelloWorld* pHello = dynamic_cast<HelloWorld*>(getParent());

	//m_ArcherRock
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto origin = Director::getInstance()->getVisibleOrigin();

	Sprite* pSprtRock = Sprite::create("rock.png");
	const PhysicsMaterial PHYSICSBODY_MATERIAL_DEFAULT(0.1f, 0.5f, 0.5f);
	PhysicsMaterial PhyMat(1.0f, 0.0f, 1.0f);
	PhysicsBody* pbody2 = PhysicsBody::createBox(pSprtRock->getContentSize(), PhyMat);

	PhysicsBody* pbody_Rock = PhysicsBody::createBox(pSprtRock->getContentSize(), ROCK_MATERIAL);
	pbody2->setDynamic(false);
	pbody_Rock->setContactTestBitmask(0xFFFFFFFF);
	pSprtRock->setPhysicsBody(pbody_Rock);
	m_ArcherRock = pSprtRock;

	pSprtRock->setPosition(Vec2((visibleSize.width / 2) - 160, (visibleSize.height / 2) - 80));
	pSprtRock->getPhysicsBody()->setGravityEnable(true);
	this->addChild(pSprtRock);

	//--------------------------------------start
	//Lightning
	m_LightningBtn = cocos2d::Sprite::create("button_SoundOn.png");
	m_LightningBtn->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + m_LightningBtn->getContentSize().height / 2));
	m_LightningBtn->setVisible(true);
	//this->addChild(m_LightningBtn, Z_SCENE_UI);
	pHello->addChild(m_LightningBtn, Z_SCENE_UI);
	
	//--------------------------------------
	//insert Cluod Batch Node

	
#if	USE_VISUALSTUDIO == 1
	auto batch = SpriteBatchNode::create("image/clowdlarge.png");
#else
	auto batch = SpriteBatchNode::create("clowdlarge.png");
#endif

	pHello->addChild(batch, 0, Z_SCENE_SPRITE_BG);

	for (int ii = 0; ii < 10; ii++)
	{
		auto spt1 = Sprite::createWithTexture(batch->getTexture());
		m_vecCloud.pushBack(spt1);
	}

	for (int ii = 0; ii < m_vecCloud.size(); ii++)
	{
		Sprite* itor = m_vecCloud.at(ii);
		batch->addChild(itor, 0, Z_SCENE_SPRITE_BG);
	}
}

void Stage::Clear_LifeList()
{
    Node* pParent = this->getParent();
    unsigned int nLifeCnt = m_listUserLifeSprite.size();
    
    if (pParent == NULL || nLifeCnt < 1) return;
    
    for(int ii = 0; ii < nLifeCnt; ii++)
    {
        Sprite* pSpt = m_listUserLifeSprite.at(ii);
        pParent->removeChild(pSpt);
        m_listUserLifeSprite.eraseObject(pSpt);
    }
    m_listUserLifeSprite.clear();
}

void Stage::Change_CurrentStageState(float _dt)
{
    getCurrentStageVal()->SetStageState(m_emTempState);
}

void Stage::Set_PosArcherfromParents()
{
    HelloWorld* pHello = dynamic_cast<HelloWorld*>(this->getParent());
    if(pHello == NULL) return;
    
    pHello->Init_Archer();
}

void Stage::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
    this->scheduleOnce(SEL_SCHEDULE, _fSec);
}


//Sprite Collision
bool Stage::onContactBegin(PhysicsContact& contact)
{
	return true;
}

bool Stage::onTouchBegan(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
    return true;
}



void Stage::onTouchMoved(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
}


void Stage::onTouchEnded(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
    if(touch->getID() != 0) return;
	

	auto visibleSize = Director::getInstance()->getVisibleSize();

	for (int ii = 0; ii < m_vecCloud.size(); ii++)
	{
		cocos2d::Sprite* pCloud = m_vecCloud.at(ii);
		bool isTouch = pCloud->getBoundingBox().containsPoint(touchPoint);
		if (isTouch)
		{
			int Alpha = pCloud->getOpacity();
			Alpha = Alpha - 100;
			if (Alpha < 0)
			{
				pCloud->setPosition(Vec2(-900, -900));
				pCloud->setOpacity(255);
			}
			else
				pCloud->setOpacity(Alpha);

		}
	}
	
	bool isTouch = m_LightningBtn->getBoundingBox().containsPoint(touchPoint);
	if (isTouch)
	{
		log("m_LightningBtn()__");
		HelloWorld* pHello = dynamic_cast<HelloWorld*>(getParent());
		for (int ii = 0; ii < m_listBalloonMaker.size(); ii++)
		{
			BalloonMaker* pBM = m_listBalloonMaker.at(ii);

			if (!(pBM->get_BMakerState() == BM_FlyingUp || pBM->get_BMakerState() == BM_FlyingDown)) continue;

			//int size = pBM->m_listBalloon.size();
			for (int jj = 0; jj < pBM->m_listBalloon.size(); jj++)
			{	
				Balloon* pBalloon = pBM->m_listBalloon.at(jj);
				
				Vec2 pos = pBalloon->getPosition();
				ActionTexture* ActTexture = nullptr;
				BalloonMaker* itBM = pBalloon->get_BalloonMaker();
				if (itBM == nullptr || itBM->getReferenceCount() < 1) 
					continue;
				
				itBM->eraseBalloonInList(pBalloon);
				jj = jj - 1;
				pHello->removeChild(pBalloon, true);

				if (itBM->m_emType == emMakerType::ORC)
					itBM->updateSpeedAndMove(itBM->get_BalloonCount());
				
				if (itBM->get_BalloonCount() > 0) 
					continue;

				ActTexture = ActionTexture::create(pos, emActionTextureType::AT_Number, this);
				if (pBalloon->get_BalloonState() == emBalloonState::Balloon_Free) {
					ActTexture->SetNumLabel(SCORE_BALLOON, SMALL_SIZE, Color3B(255, 130, 100));
					pHello->Plus_StageScore(SCORE_BALLOON);
				}
				else {
					ActTexture->SetNumLabel(SCORE_BM, NORMAL_SIZE, Color3B(255, 130, 100));
					pHello->Plus_StageScore(SCORE_BM);
				}
				ActTexture->showActionTexture();

				itBM->Fall_BalloonMaker();
				MC_SoundBox::getInstance()->tryVibration(0.1);				
				
			}
		}
	}

	if (m_bTouchBallonMode)
	{
		//printf_s("ddd");
		HelloWorld* pHello = dynamic_cast<HelloWorld*>(getParent());
		for (int ii = 0; ii < m_listBalloonMaker.size(); ii++)
		{
			BalloonMaker* pBM = m_listBalloonMaker.at(ii);

			int bPass = false;
			for (int jj = 0; jj < pBM->m_listBalloon.size(); jj++)
			{
				if (bPass) break;
				Balloon* pBalloon = pBM->m_listBalloon.at(jj);
				bool isTouch = pBalloon->getBoundingBox().containsPoint(touchPoint);
				if (isTouch)
				{
					Vec2 pos = pBalloon->getPosition();
					ActionTexture* ActTexture = nullptr;
					BalloonMaker* itBM = pBalloon->get_BalloonMaker();
					if (itBM == nullptr || itBM->getReferenceCount() < 1) continue;
					bPass = true;
					itBM->eraseBalloonInList(pBalloon);
					pHello->removeChild(pBalloon, true);

					if (itBM->m_emType == emMakerType::ORC)
						itBM->updateSpeedAndMove(itBM->get_BalloonCount());

					if (itBM->get_BalloonCount() > 0) continue;

					ActTexture = ActionTexture::create(pos, emActionTextureType::AT_Number, this);
					if (pBalloon->get_BalloonState() == emBalloonState::Balloon_Free) {
						ActTexture->SetNumLabel(SCORE_BALLOON, SMALL_SIZE, Color3B(255, 130, 100));
						pHello->Plus_StageScore(SCORE_BALLOON);
					}
					else {
						ActTexture->SetNumLabel(SCORE_BM, NORMAL_SIZE, Color3B(255, 130, 100));
						pHello->Plus_StageScore(SCORE_BM);
					}
					ActTexture->showActionTexture();

					itBM->Fall_BalloonMaker();
					MC_SoundBox::getInstance()->tryVibration(0.1);
					break;
				}
			}
		}
	}
}

void Stage::onTouchCancelled(Touch* touch, Event* event)
{
    auto touchPoint = touch->getLocation();
    if(touch->getID() != 0) return;
}
