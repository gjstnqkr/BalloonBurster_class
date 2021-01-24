#pragma once


#include "cocos2d.h"
#include "interface.h"
#include "BalloonMaker.h"
#include "Archer.h"

#define STAGE_CNT 4
#define STAGE_WAVE 3
#define STAGE_LAST STAGE_CNT

#define STAGE_INFINITE_CNT 4
#define STAGE_INFINITE_WAVE 3


#define STAGE_UP 5
#define STAGE_DOWN 6
#define STAGE_BONUS 7


#define STAGE_VALUE_0 {1, 2, 4, 1, 1, 1, 2, 3, 3, 2, 1, 1, 3, 4, 1, 1, 3, 2}
#define STAGE_VALUE_1 {2, 1, 3, 2, 1, 1, 1, 4, 3, 2, 3, 1, 2, 4, 4, 1, 2, 1}
#define STAGE_VALUE_2 {0}

#define STAGE_VALUE(i) STAGE_VALUE_i


//1-x
static int StageOrder[STAGE_CNT][STAGE_WAVE] {
    {STAGE_DOWN, 1, 2 },
    {STAGE_UP, 1, 3 },
    {STAGE_DOWN, 1, 3 },
    {STAGE_BONUS, 1, 3 },
    //{0,}  // there is zero? so that stage is bonus stage
};

//2-x
static int StageOrder_2[STAGE_CNT][STAGE_WAVE] {
    {STAGE_UP, 1, 2 },
    {STAGE_DOWN, 1, 3 },  
    {STAGE_UP, 1, 3 },
    {STAGE_DOWN, 1, 3 },
    //{0,}  // there is zero? so that stage is bonus stage
};


//infinite Mode 1-x
static int StageOrder_Infinite_1[STAGE_INFINITE_CNT][STAGE_INFINITE_WAVE]{
	{ STAGE_UP, 1, 2 },
	{ STAGE_DOWN, 1, 3 },
	{ STAGE_UP, 1, 3 },
	{ STAGE_DOWN, 1, 3 },
	//{ STAGE_BONUS, 1, 3 },
	//{0,}  // there is zero? so that stage is bonus stage
};



struct StageValue : public cocos2d::Node
{
public:
    unsigned int                        m_nStageNumber;         // ex) 1-'1', 1-'2' -> '1' or '2'
    emStageType                         m_emStageType;          //bonus or Up/Down
    emStageState                        m_emStageState;         //not use now 20200105
    unsigned int                        m_nEnemyTotalCnt;       //not use now 20200105
    
    cocos2d::Vector<cocos2d::__Integer*> m_ListEnemyCnt;
    
public:
    StageValue(){}
    ~StageValue(){
    }
    
    static StageValue* create(unsigned int nStageNum, emStageType StageType)
    {
        StageValue* pStageVal = new (std::nothrow) StageValue();
        if(pStageVal && pStageVal->init())
        {
            pStageVal->m_nStageNumber   = nStageNum;
            pStageVal->m_emStageType    = StageType;
            pStageVal->m_emStageState   = emStageState::SState_Ready;
            pStageVal->m_nEnemyTotalCnt = 0;
            
            pStageVal->autorelease();
            return pStageVal;
        }
        CC_SAFE_DELETE(pStageVal);
        return nullptr;
    }
    
    virtual void SetStageNumber(const unsigned int _unStageNum) { m_nStageNumber = _unStageNum; }
    virtual void SetEnemyTotalCnt(const unsigned int _unTotal) { m_nEnemyTotalCnt = _unTotal; }
    virtual void SetStageType(const emStageType _emType) { m_emStageType = _emType; }
    virtual void SetStageState(const emStageState _emState) { m_emStageState = _emState; }
    
    virtual unsigned int getEnemyTotalCnt(){ return m_nEnemyTotalCnt; }
    virtual emStageState getStageState(){ return m_emStageState; }
    virtual emStageType getStageType(){ return m_emStageType; }
};


class Stage : public cocos2d::Node
{
public:
	cocos2d::Vector<BalloonMaker*>		m_listBalloonMaker;
    cocos2d::Vector<StageValue*>        m_listStageValue;
    cocos2d::Vector<cocos2d::Sprite*>   m_listUserLifeSprite;
    cocos2d::Vector<cocos2d::Sprite*>   m_listBird;
	cocos2d::Vector<cocos2d::Sprite*>   m_vecCloud;	
    Archer*                             m_pArcher;
	cocos2d::Label*						m_lbEnemy;
    cocos2d::Label*                     m_lbCenter;
    cocos2d::Label*                     m_lbUserScore;
    
    cocos2d::Sprite*                    m_ArcherRock;
	cocos2d::Sprite*                    m_LightningBtn; //all kill
    

    unsigned int                        m_unStage1st; // ex) '1'-1, '1'-2 -> '1' or '1'
	unsigned int						m_unCurrentStage2nd;
    unsigned int                        m_unUserLife;
    unsigned int                        m_unUserScore;

	unsigned int						m_unEnemyCnt;
    unsigned int                        m_unMissedEnemyCnt;
    bool                                m_bStageEnable;
	bool                                m_bTouchBallonMode;


private:
	float								m_fTotalTime;
	float								m_fTempTime;
	int									m_nTempNum;
    emStageState                        m_emTempState;
    
    double                              m_dRotValue;
    float                               m_fArcherRockTime;

	//cloud appear time
	int									m_nCloudTime;
	

public:
	static Stage* create(unsigned int _nStage1st, cocos2d::Node* pParent = NULL );

	virtual void onEnterTransitionDidFinish();  //first call function after init

public:	
	Stage();
	virtual ~Stage();
	
    //init
    void initOnce();
	void init_Sprite();
    void init_Current(int _nStageNum = 1);
    void initStageVal(unsigned int _nStage1st);
    void initLabels();
    void init_Event(int _nStageNum);
    //void Init_Archer(bool bPositionOK = true);
    
    //Stage Control
    void Start_Stage();
    void Stop_Stage();
    void Delete_Stage();

    //Schedular
    void Stage_Schedular(float _dt);
	void AppearBM_Scheduler(float _dt);
    void Bonus_Scheduler(float _dt);
	void AppearanceTimer(float _dt);
    void lbCenter_InvisibleScheduler(float _dt);
    void Start_GameOverScheduler(float _dt);

	void Start_CloudSchedular(float _dt);
    

    //Run Game time
	void Appear_Group(unsigned int _unEnemyCnt);
	void Appear_BalloonMaker(emMovingType _mType);
	void DecreaseEnemyNum();
    
    void AddBird(cocos2d::Vec2 Pos);
	void appearCloud();
    
    void Start_StageTextAndAppearBM();
    void ShowStageText(int _n1st ,int _n2nd);
    void ShowStageClear();
    void ShowStageEnd();
    void ShowGameEnd();
    
    void StopAction_BalloonMaker();
    
    
    //UserLife
    void Add_UserLifeSpriteAtList();    //1 Life add
    void Remove_UserLifeSpriteAtList(float dt = 0.0f); //1 Life count out
    void Clear_LifeList();
    
    //Get
    unsigned int    get_UserScore();
    ssize_t         get_UserLifeCnt(){ return m_listUserLifeSprite.size(); }
    unsigned int    get_BM_AchivedAttackCnt();
    unsigned int    get_BM_AchivedPushCnt();
    unsigned int    get_Stage1stNum();
    
    
    //Set
    void            Set_UserScore(unsigned int _unScore);
    void            set_Stage1stNum(unsigned int _unStage1st);
    
    //
    unsigned int    moveAchivedBMPos(cocos2d::Vec2 _Move);
    
    
    //Collision Func
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    
    //Single touch
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchCancelled(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
	void SetEnemyLabel(int _Value);
    void SetStageTextLabel(int _n1st ,int _n2nd);
    
    void NextStage(float _float = 0.0f);
    void Clear_BMList();
    void Clear_SpriteInScene();
    
    void init_UserlifeSprite();

    void Align_UserLifeInScreen();
    
    
    
    void Change_CurrentStageState(float _dt);
    void Set_PosArcherfromParents();
    
    //Common Func 
    void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);
    
public:
    StageValue* getCurrentStageVal();

};



