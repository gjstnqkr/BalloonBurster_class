#pragma once



#include "cocos2d.h"
#include "interface.h"

class Archer;
class Arrow : public cocos2d::Sprite
{


private:
	emArrowState	m_emState;
	Archer*			m_pParentArcher;

	float angle;
	bool bArrowShooting;

public:
	static Arrow * create(cocos2d::Vec2 _Pos, Archer* _parent = nullptr);

	//Common Func
	void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);

public:
	Arrow();
	virtual ~Arrow();

    void init_Cocos2dInfo(cocos2d::Vec2 _Pos = cocos2d::Vec2::ZERO);


	void ShootArrow();
	void ShootParabola();
	void FallingArrow();
	void HitArrow();

	//update
	void UpdateArrowPos(float f);
	void StopArrowUpdate(float f);
	void update(float delta);


	emArrowState	get_ArrowState(void);
	void			set_ArrowState(emArrowState _emState);
	void			set_Archer(Archer* pArcher);

};


