#pragma once



#include "cocos2d.h"
#include "interface.h"
#include <spine/spine-cocos2dx.h>
//#include "Balloon.h"
class Balloon;
class Rock;

#if SPINE2D_BallonMaker == 1
class BalloonMaker : public spine::SkeletonAnimation
#else
class BalloonMaker : public cocos2d::Sprite
#endif
{
public:
	emMakerType			m_emType;
	emMakerState		m_emState;
	emMovingType		m_emMovingType;
	int					m_uId;

	float				m_fMoveSpeed;		//	
	unsigned int		m_nMaxHP;
	unsigned int		m_nCurrentHP;
    float               m_fMaxHighPos;       // When BM arrive this Pos, Stop fly and try next work.
    float               m_fMaxLowPos;        // When BM arrive this Pos, Stop fly and try next work.

    
	cocos2d::Vector<Balloon*>		m_listBalloon;
	cocos2d::Vector<Rock*>			m_listRock;
private:
	float		m_fTime;			//	
	float		m_fTempTime;		//
public:
	

public:
	static BalloonMaker * create(
		cocos2d::Vec2 _pos = cocos2d::Vec2::ZERO, 		
		emMakerType _emType = RED,
		emMovingType _emMoveType = BM_Random
	);

	virtual void onEnterTransitionDidFinish();

public:	
	BalloonMaker();	
	virtual ~BalloonMaker();

	
	void init_Cocos2dInfo(
		cocos2d::Vec2 _pos = cocos2d::Vec2::ZERO, 
		emMakerType _emType = RED,
		emMovingType _emMoveType = BM_Random		
	);
	void init_BMakerState(emMakerType _emType = RED, emMovingType _emMoveType = BM_Random);
	void init_SkelAnimation();

    //Scheduler Function
	void Start_Scheduler(float _dt);
	void UpdateBMPos(float _dt);
	
	void ChangeType(emMakerType _emType);
	void ChangeAction();
	
	void RiseUpDown_BMaker(float _dt);
	void Fall_BalloonMaker();
	void Walk_Ground(emMovingType _MovingType);


	bool Make_Rock(cocos2d::Vec2 _pos);
	bool Make_Balloon(cocos2d::Vec2 _pos, float fSpeedVal);


	bool eraseBalloonInList(cocos2d::Sprite* _pSprite);	
	bool eraseRockInList(cocos2d::Sprite* _pSprite);
    
    void eraseBalloon();
    void eraseRock();
    bool remove_BalloonMaker(bool bDecreaseEnemy = false);
    
    void updateSpeedAndMove(int balloonCnt);

	
	emMakerState	get_BMakerState();
    unsigned int    get_BalloonCount();
	int				get_BMakerType();
    
	void			set_BMakerState(emMakerState _emState);
	void			set_BMakerFlyingUp();
	void			set_BMakerFlyingDown();
    void            set_StateBalloon(emBalloonState _emState);
    void            set_MaxLow(float _fLowVal);
    void            set_MaxHigh(float _fHighVal);
    
    
    //about Spine2D
    bool            setBallonMakerAnimation(int _emState, bool _bLoop);
    
    cocos2d::Vec2   get_BMSpineBonePos(const char* pStr);
    
    bool            setBMSpineSpear(bool bSpear);
	


};

