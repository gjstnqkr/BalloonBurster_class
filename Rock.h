#pragma once

#include "cocos2d.h"
#include "interface.h"

class BalloonMaker;
class Rock : public cocos2d::Sprite
{
public:
	emRockState			m_RockState;
	BalloonMaker*		m_pParentBM;
	

public:
	static Rock * create(cocos2d::Vec2 _Pos);
    
    //Common Func
    void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);

public:
	Rock();
	virtual ~Rock();

	void init_Cocos2dInfo(cocos2d::Vec2 _Pos);
	void Thrown_Rock(float fDist = 0.0f);
	void explode_Rock(float _dt = NOTHING);

	void			set_BalloonMaker(BalloonMaker* pArcher);
	BalloonMaker*	get_BalloonMaker();

	void			set_RockState(emRockState _RockState);
	emRockState		get_RockState();


};

