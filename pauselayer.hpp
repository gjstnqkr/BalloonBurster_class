//
//  PauseLayer.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2019. 3. 3..
//

#ifndef PauseLayer_hpp
#define PauseLayer_hpp

#include <stdio.h>

#if USE_ADMOB == 1
#include "firebase/admob.h"
#endif
#include "cocos2d.h"
#include "interface.h"

#define BANNERVIEW_ONLYLOADING 99.0

class GameUILayer;
class PauseLayer : public cocos2d::Layer
{
public:
	unsigned int                    m_UserOpType;
#if USE_ADMOB == 1
	firebase::admob::BannerView*    banner_view;
#endif
	int                             m_LayerState;

	//ui
	cocos2d::Sprite                 *m_sptPauseBG;
	cocos2d::MenuItemImage          *m_closeItem;
	cocos2d::MenuItemImage          *m_musicItem;
	cocos2d::MenuItemImage          *m_soundItem;
	cocos2d::MenuItemImage          *m_vibrationItem;
	cocos2d::MenuItemImage          *m_reStartItem;
	cocos2d::MenuItemImage          *m_outItem;

	//parent
	GameUILayer*                    m_pGameUI;




public:
	PauseLayer();
	~PauseLayer();

	static PauseLayer * create(unsigned int _UserOpType);
	//virtual void update(float dt);
	bool Loading_Banner(void);

	void moveAdmobView(int nX, int nY);
	void VisiblePauseLayer(bool _bOnOff);

	void ShowActionUI(void);
	void HideActionUI(void);
	void HideUI(float _f);


	void                setUserOpType(int nType);
	void                setLayerState(int nState);

	unsigned int        getUserOpType();
	unsigned int        getLayerState();


public:
	bool init(unsigned int _UserOpType);
	bool Init_UI();
	bool Init_Sprite();
	bool Init_Firebase();

	bool SetOnOff(int optionn, bool bOnOff);
	bool updateUI();
public:
	void menuCloseCallback(Ref* sender);
	void menuSoundCallback(Ref* sender);
	void menuMusicCallback(Ref* sender);
	void menuVibrationCallback(Ref* sender);
	void menuRestartCallback(Ref* sender);
	void menuOutCallback(Ref* sender);

	//Common Func
	void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);
};


#endif /* PauseLayer_hpp */
