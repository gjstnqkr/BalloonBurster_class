#pragma once


#include "cocos2d.h"
#include "interface.h"
#include <spine/spine-cocos2dx.h>
#include "Arrow.h"


#if SPINE2D == 1
class Archer : public spine::SkeletonAnimation
#else
class Archer : public cocos2d::Sprite
#endif
{
public:
	cocos2d::Vector<Arrow*>		m_listArrow;
	emArcherState				m_emArcherState;


public:
	static Archer * create(cocos2d::Vec2 _pos);

public:
	Archer();
	virtual ~Archer();

	
	void init_Cocos2dInfo(cocos2d::Vec2 _pos = cocos2d::Vec2::ZERO);
	void init_SkelAnimation();
	void init_ArcherState();

	void Timer(float _dt);
	
	bool Make_ArcherArrow(cocos2d::Vec2 _pos);
	int ShootArrow();
	void DyingAction();

		
	void Set_TouchPosition(float _x, float _y);
	
	emArcherState	get_ArcState(void);
    
	void			set_ArcState(emArcherState _emState);
    
    //about Spine2D 
    float           get_ArcSpineBoneRotX(const char* pStr);
    float           get_ArcSpineBoneRotY(const char* pStr);
    cocos2d::Vec2   get_ArcSpineBonePos(const char* pStr);
    
    cocos2d::Vec2            setArcherAnimation(int _nState);

	
};

