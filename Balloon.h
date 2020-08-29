#pragma once



#include "cocos2d.h"
#include "interface.h"

class BalloonMaker;
class Balloon : public cocos2d::Sprite
{
public:	
	BalloonMaker*		m_pParentBM;
    int                 m_emBalloonState;
    
    float               m_fUpSpeed;
    float               m_fMoveDistance;

public:
	static Balloon * create(cocos2d::Vec2 _Pos, float fSpeedVal);
    
    //Common Func
    void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);

public:
	Balloon();	
	virtual ~Balloon();
    
    void init_Balloon(float fSpeedVal);
	void init_Cocos2dInfo(cocos2d::Vec2 _Pos);
    
	void Fly_Balloon(float _dt);
    void Free_fromBalloonMaker();
    void cleanupBallon(float _fVal);

	void			set_BalloonMaker(BalloonMaker* pBM);
    void            set_BalloonState(emBalloonState _State);
    void            set_UpSpeed(float _fVal);
    void            set_MoveDistance(float _fVal);
    
    
	BalloonMaker*	get_BalloonMaker();
    int             get_BalloonState();
};
