//
//  PauseLayer.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2019. 3. 3..
//

#include "PauseLayer.hpp"
#include "BB_GameInfo.h"
#include "GameUILayer.hpp"
#include "MC_GameOption.hpp"
#include "MC_FileUtils.hpp"
#include "MC_SoundBox.hpp"

#include "SimpleAudioEngine.h"

#if USE_ADMOB == 1
#include "FirebaseHelper.hpp"

#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"
#include "firebase/admob/banner_view.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <android/log.h>
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

PauseLayer::PauseLayer()
	: m_sptPauseBG(NULL)
#if USE_ADMOB == 1
	, banner_view(NULL)
#endif
	, m_UserOpType(emUserOption::INGAME)
	, m_LayerState(emLayerState::Layer_None)
	, m_closeItem(NULL)
	, m_pGameUI(nullptr)
{
}

#define MENU_COLOR_DISABLE Color3B(158,158,158)
#define MENU_COLOR_ENABLE Color3B(255,255,255)

PauseLayer::~PauseLayer()
{

}

PauseLayer* PauseLayer::create(unsigned int _UserOpType)
{
	PauseLayer* pPauseLayer = new (std::nothrow) PauseLayer();
	if (pPauseLayer && pPauseLayer->init(_UserOpType))
	{
		pPauseLayer->autorelease();
		pPauseLayer->setLocalZOrder(Z_LAYER);

		return pPauseLayer;
	}
	CC_SAFE_DELETE(pPauseLayer);
	return nullptr;
}

bool PauseLayer::init(unsigned int _UserOpType)
{
	setUserOpType(_UserOpType);
	Init_UI();
	Init_Sprite();
	if (getUserOpType() == emUserOption::INGAME)
		Init_Firebase();

	return true;
}

bool PauseLayer::Init_UI(void)
{
	//Menu BackGround
	const CCSize visibleSize = Director::getInstance()->getVisibleSize();
	const Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto Size = Director::getInstance()->getWinSize();
	const Vec2 ScreenCenter = origin + Vec2(visibleSize * 0.5);

	auto sptPauseBG = Sprite::create("menu_Pause_mid.png");
	float fBtnPosY = visibleSize.height * 0.5;
	float fBtnPosX = visibleSize.width * 0.5;
	CCSize conSize_BG = sptPauseBG->getContentSize();
	sptPauseBG->setPosition(origin + Vec2(fBtnPosX, fBtnPosY) - conSize_BG / 2);
	sptPauseBG->setAnchorPoint(Vec2(0, 0));
	sptPauseBG->setScale(1.0);
	this->addChild(sptPauseBG);
	m_sptPauseBG = sptPauseBG;

	float HeightOffset = conSize_BG.height*0.05;


	//Close btn
	auto closeItem = MenuItemImage::create(
		"button_Close.png",
		"button_Close_push.png",
		CC_CALLBACK_1(PauseLayer::menuCloseCallback, this));

	CCSize  conSize_CloseBtn = closeItem->getContentSize();
	float   fBtnOffset = conSize_BG.width * UI_RIGHT_START_RATE;
	closeItem->setPosition(ScreenCenter + (conSize_BG * 0.5) - Vec2(fBtnOffset, fBtnOffset) - (conSize_CloseBtn * 0.5));
	closeItem->setAnchorPoint(Vec2(0.0, 0.0));
	closeItem->setScale(0.5);

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
	m_closeItem = closeItem;

	//Music btn
	auto MusicItem = MenuItemImage::create(
		"button_Check.png",
		"button_Check_push.png",
		CC_CALLBACK_1(PauseLayer::menuMusicCallback, this));

	CCSize  conSize_MusicBtn = MusicItem->getContentSize();
	Vec2 UI_Center1 = Vec2(conSize_MusicBtn.width*0.5f, (conSize_MusicBtn.height * 0.5));
	MusicItem->setPosition(ScreenCenter - UI_Center1 + Vec2(-conSize_MusicBtn.width*1.2, conSize_MusicBtn.height * 0.5f + HeightOffset));
	MusicItem->setAnchorPoint(Vec2(0.0, 0.0));
	MusicItem->setScale(1.0);

	// create menu, it's an autorelease object
	auto menu_0 = Menu::create(MusicItem, nullptr);
	menu_0->setPosition(Vec2::ZERO);
	this->addChild(menu_0, 1);
	m_musicItem = MusicItem;


	//Sound btn
	auto SoundItem = MenuItemImage::create(
		"button_SoundOn.png",
		"button_SoundOn_push.png",
		CC_CALLBACK_1(PauseLayer::menuSoundCallback, this));

	CCSize  conSize_SoundBtn = SoundItem->getContentSize();
	Vec2 UI_Center2 = Vec2(conSize_SoundBtn.width*0.5f, (conSize_SoundBtn.height * 0.5));
	SoundItem->setPosition(ScreenCenter - UI_Center2 + Vec2(0.0f, conSize_MusicBtn.height * 0.5f + HeightOffset));
	SoundItem->setAnchorPoint(Vec2(0.0, 0.0));
	SoundItem->setScale(1.0);

	// create menu, it's an autorelease object
	auto menu_1 = Menu::create(SoundItem, nullptr);
	menu_1->setPosition(Vec2::ZERO);
	this->addChild(menu_1, 1);
	m_soundItem = SoundItem;


	//Vibration Btn
	auto VibrationItem = MenuItemImage::create(
		"button_Play.png",
		"button_Play_push.png",
		CC_CALLBACK_1(PauseLayer::menuVibrationCallback, this));

	CCSize  conSize_VibBtn = VibrationItem->getContentSize();
	Vec2 UI_Center3 = Vec2(conSize_VibBtn.width*0.5f, (conSize_VibBtn.height * 0.5));
	VibrationItem->setPosition(ScreenCenter - UI_Center3 + Vec2(conSize_VibBtn.width*1.2, conSize_VibBtn.height * 0.5f + HeightOffset));
	VibrationItem->setAnchorPoint(Vec2(0.0, 0.0));
	VibrationItem->setScale(1.0);

	// create menu, it's an autorelease object
	auto menu_2 = Menu::create(VibrationItem, nullptr);
	menu_2->setPosition(Vec2::ZERO);
	this->addChild(menu_2, 1);
	m_vibrationItem = VibrationItem;


	if (getUserOpType() == emUserOption::INGAME)
	{
		//restart Btn
		auto RestartItem = MenuItemImage::create(
			"button_Restart.png",
			"button_Restart.png",
			CC_CALLBACK_1(PauseLayer::menuRestartCallback, this));

		CCSize  conSize_RestartBtn = RestartItem->getContentSize();
		Vec2 Center = Vec2(conSize_RestartBtn.width*0.5f, (conSize_RestartBtn.height * 0.5));
		RestartItem->setPosition(ScreenCenter - Center + Vec2(-conSize_RestartBtn.width*0.7, -conSize_RestartBtn.height * 0.5f + HeightOffset));
		RestartItem->setAnchorPoint(Vec2(0.0, 0.0));
		RestartItem->setScale(1.0);

		// create menu, it's an autorelease object
		auto menu_3 = Menu::create(RestartItem, nullptr);
		menu_3->setPosition(Vec2::ZERO);
		this->addChild(menu_3, 1);
		m_reStartItem = RestartItem;

		//Out Btn
		auto OutItem = MenuItemImage::create(
			"button_Out.png",
			"button_Out.png",
			CC_CALLBACK_1(PauseLayer::menuOutCallback, this));

		CCSize  conSize_OutBtn = OutItem->getContentSize();
		Vec2 Center2 = Vec2(conSize_OutBtn.width*0.5f, (conSize_OutBtn.height * 0.5));
		OutItem->setPosition(ScreenCenter - Center2 + Vec2(conSize_RestartBtn.width*0.7, -conSize_RestartBtn.height * 0.5f + HeightOffset));
		OutItem->setAnchorPoint(Vec2(0.0, 0.0));
		OutItem->setScale(1.0);

		// create menu, it's an autorelease object
		auto menu_4 = Menu::create(OutItem, nullptr);
		menu_4->setPosition(Vec2::ZERO);
		this->addChild(menu_4, 1);
		m_outItem = OutItem;
	}
	else if (getUserOpType() == emUserOption::MAINPAGE)
	{

	}

	MC_GameOption* pGameOp = MC_GameOption::GetInstance();
	SetOnOff(emOption::SOUND, pGameOp->getSound());
	SetOnOff(emOption::VIBRATION, pGameOp->getVibration());
	SetOnOff(emOption::MUSIC, pGameOp->getMusic());
	return true;
}

bool PauseLayer::Init_Sprite(void)
{
	return false;
}

bool PauseLayer::Init_Firebase(void)
{
#if USE_ADMOB == 1
	//----------------------------------------------------------------
	// General scene setup ... (firebase admob)
	//----------------------------------------------------------------

#if defined(__ANDROID__)
	// Android ad unit IDs.
	const char* kBannerAdUnit = "ca-app-pub-3940256099942544/6300978111";
#else
	// iOS ad unit IDs.
	const char* kBannerAdUnit = "ca-app-pub-3940256099942544/2934735716";
#endif

	// Create and initialize banner view.
	//firebase::admob::BannerView* banner_view;
	banner_view = new firebase::admob::BannerView();
	firebase::admob::AdSize ad_size;
	ad_size.ad_size_type = firebase::admob::kAdSizeStandard;
	ad_size.width = ADD_WIDTH;
	ad_size.height = ADD_HEIGHT;
	banner_view->Initialize(getAdParent(), kBannerAdUnit, ad_size);
	//banner_view->Initialize(this, kBannerAdUnit, ad_size);

	Loading_Banner();
	//VisiblePauseLayer(false);
#endif
	return true;
}

void PauseLayer::VisiblePauseLayer(bool _bOnOff)
{
	if (_bOnOff == true) this->setVisible(true);
	//    else
	//    {
	//        if(BB_GameInfo::GetInstance()->getGameState() != emBB_State::emMainScene){
	//            Call_FuncAfterFewTime(schedule_selector(PauseLayer::HideUI), 0.2f);
	//            HideActionUI();
	//        }
	//        else this->setVisible(false);
	//    }


	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size gl_Size = Director::getInstance()->getOpenGLView()->getFrameSize();

	const Vec2 ScreenCenter = origin + Vec2(visibleSize * 0.5);
	const Vec2 ScreenSize = origin + visibleSize;
	const Vec2 glCenter = Vec2(gl_Size.width*0.5, gl_Size.height*0.5);

	if (_bOnOff)
	{
		ShowActionUI();
		//this->resumeSchedulerAndActions();
		//this->schedule(schedule_selector(PauseLayer::update));

		if (getUserOpType() == emUserOption::INGAME)
		{
			Loading_Banner();

#if USE_ADMOB == 1
			// Check that the banner has been initialized.
			if (banner_view->InitializeLastResult().status() == firebase::kFutureStatusComplete)
			{
				banner_view->Show();

				if (m_sptPauseBG == NULL) return;

				//Set banner Position
				const firebase::admob::BoundingBox Box = banner_view->bounding_box();
				CCSize conSize_BG = m_sptPauseBG->getContentSize();

				float percent = (conSize_BG.height / visibleSize.height) * 100;
				float Hvalue = percent * (gl_Size.height / 100);

				float offsetH = (Hvalue * 0.5) - (Hvalue * 0.1) - (Box.height);

				banner_view->MoveTo(glCenter.x - (Box.width*0.5), glCenter.y + offsetH);
			}
#endif
		}

	}
	else
	{
		if (BB_GameInfo::GetInstance()->getGameState() == emBB_State::emGameOn && getLayerState() == emLayerState::Layer_Play) {
			Call_FuncAfterFewTime(schedule_selector(PauseLayer::HideUI), UI_ACTION_TIME + 0.02);
			HideActionUI();
		}
		else
			this->setVisible(false);

		if (getUserOpType() == emUserOption::INGAME)
		{
#if USE_ADMOB == 1
			// Check that the banner has been initialized.
			if (banner_view->InitializeLastResult().status() ==
				firebase::kFutureStatusComplete) {
				banner_view->Hide();

				const firebase::admob::BoundingBox Box = banner_view->bounding_box();
				int aa = 0;

				banner_view->MoveTo(340, 740);
			}
#endif
		}
	}
}

void PauseLayer::moveAdmobView(int nX, int nY)
{
#if USE_ADMOB == 1
	banner_view->MoveTo(nX, nY);
#endif
}


bool PauseLayer::Loading_Banner(void)
{
#if USE_ADMOB == 1
	// Check that the banner has been initialized.
	if (banner_view->InitializeLastResult().status() ==
		firebase::kFutureStatusComplete) {
		// Check that the banner hasn't started loading.
		if (banner_view->LoadAdLastResult().status() ==
			firebase::kFutureStatusInvalid) {
			// Make the banner load an ad.
			CCLOG("Loading a banner.");
			//banner_view->Show();
			firebase::admob::AdRequest my_ad_request = {};
			banner_view->LoadAd(my_ad_request);
			return true;
		}
	}
#endif
	log("[  BannerLoading  ]");
	return false;
}


void PauseLayer::menuCloseCallback(Ref* sender)
{
	//Pause
	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
	Director::getInstance()->resume();
	this->getParent()->resume();

	if (m_pGameUI != nullptr) m_pGameUI->ShowGameUI();

	this->VisiblePauseLayer(false);
	MC_FileUtils::writeOptionFile();
	this->setLayerState(emLayerState::Layer_Stop);
}

void PauseLayer::menuSoundCallback(Ref* sender)
{
	//Sound
	MC_GameOption* pGameOp = MC_GameOption::GetInstance();
	SetOnOff(emOption::SOUND, !pGameOp->getSound());
	MC_FileUtils::writeOptionFile();
}

void PauseLayer::menuMusicCallback(Ref* sender)
{
	//Music
	MC_GameOption* pGameOp = MC_GameOption::GetInstance();
	SetOnOff(emOption::MUSIC, !pGameOp->getMusic());
	MC_FileUtils::writeOptionFile();
}

void PauseLayer::menuVibrationCallback(Ref* sender)
{
	//Vibration
	MC_GameOption* pGameOp = MC_GameOption::GetInstance();
	SetOnOff(emOption::VIBRATION, !pGameOp->getVibration());
	MC_FileUtils::writeOptionFile();
}

void PauseLayer::menuRestartCallback(Ref* sender)
{
	//Restart

}

void PauseLayer::menuOutCallback(Ref* sender)
{
	BB_GameInfo::GetInstance()->setGameState(emMainScene);
	//Out  PHS_start_point
	//Director::getInstance()->popScene();
	Director::getInstance()->popToRootScene();


	Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(1);
	Director::getInstance()->resume();

	this->VisiblePauseLayer(false);
}

void PauseLayer::ShowActionUI()
{
	float MoveTime = 0.2;
	Vec2 Move = Vec2(0.0f, 150.0f);
	Vec2 Move2 = Vec2(0.0f, 100.0f);

	m_sptPauseBG->runAction(FadeIn::create(UI_ACTION_TIME));
	m_closeItem->runAction(FadeIn::create(UI_ACTION_TIME));
	m_musicItem->runAction(FadeIn::create(UI_ACTION_TIME));
	m_soundItem->runAction(FadeIn::create(UI_ACTION_TIME));
	m_vibrationItem->runAction(FadeIn::create(UI_ACTION_TIME));


	if (getUserOpType() == emUserOption::INGAME)
	{
		m_reStartItem->runAction(FadeIn::create(UI_ACTION_TIME));
		m_outItem->runAction(FadeIn::create(UI_ACTION_TIME));
	}
}


void PauseLayer::HideActionUI()
{
	float MoveTime = 0.2;
	Vec2 Move = Vec2(0.0f, -150.0f);
	Vec2 Move2 = Vec2(0.0f, -100.0f);

	m_sptPauseBG->runAction(FadeOut::create(UI_ACTION_TIME));
	m_closeItem->runAction(FadeOut::create(UI_ACTION_TIME));
	m_musicItem->runAction(FadeOut::create(UI_ACTION_TIME));
	m_soundItem->runAction(FadeOut::create(UI_ACTION_TIME));
	m_vibrationItem->runAction(FadeOut::create(UI_ACTION_TIME));

	if (getUserOpType() == emUserOption::INGAME)
	{
		m_reStartItem->runAction(FadeOut::create(UI_ACTION_TIME));
		m_outItem->runAction(FadeOut::create(UI_ACTION_TIME));
	}
}


void PauseLayer::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
	this->scheduleOnce(SEL_SCHEDULE, _fSec);
}


void PauseLayer::HideUI(float _f)
{
	this->setVisible(false);
}


void PauseLayer::setUserOpType(int nType)
{
	m_UserOpType = nType;
}

void PauseLayer::setLayerState(int nState)
{
	m_LayerState = nState;
}

unsigned int PauseLayer::getUserOpType()
{
	return m_UserOpType;
}

unsigned int PauseLayer::getLayerState()
{
	return m_LayerState;
}

bool PauseLayer::updateUI() {
	MC_GameOption* pOption = MC_GameOption::GetInstance();

	//    m_soundItem->setEnabled(pOption->getSound());
	//    m_musicItem->setEnabled(pOption->getMusic());
	//    m_vibrationItem->setEnabled(pOption->getVibration());

	SetOnOff(emOption::VIBRATION, pOption->getVibration());
	SetOnOff(emOption::MUSIC, pOption->getMusic());
	SetOnOff(emOption::SOUND, pOption->getSound());
	return true;
}


bool PauseLayer::SetOnOff(int option, bool bOnOff)
{
	MC_GameOption* pGameOp = MC_GameOption::GetInstance();
	if (option == emOption::VIBRATION)
	{
		if (bOnOff)
		{
			pGameOp->setVibration(true);
			m_vibrationItem->setColor(MENU_COLOR_ENABLE);
		}
		else
		{
			pGameOp->setVibration(false);
			m_vibrationItem->setColor(MENU_COLOR_DISABLE);
		}
	}
	else if (option == emOption::MUSIC)
	{
		if (bOnOff)
		{
			pGameOp->setMusic(true);
			m_musicItem->setColor(MENU_COLOR_ENABLE);
			MC_SoundBox::getInstance()->resumeMusic();
		}
		else
		{
			pGameOp->setMusic(false);
			m_musicItem->setColor(MENU_COLOR_DISABLE);
			MC_SoundBox::getInstance()->pauseMusic();
		}
	}
	else if (option == emOption::SOUND)
	{
		if (bOnOff)
		{
			pGameOp->setSound(true);
			m_soundItem->setColor(MENU_COLOR_ENABLE);
		}
		else
		{
			pGameOp->setSound(false);
			m_soundItem->setColor(MENU_COLOR_DISABLE);
		}
	}
	else
	{

	}

	return true;

}

