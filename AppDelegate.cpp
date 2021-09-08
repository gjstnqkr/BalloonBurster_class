/****************************************************************************
Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "BB_LogoScene.hpp"
#include "BB_MainScene.hpp"
#include "HelloWorldScene.h"
#include "interface.h"


// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif


#if USE_ADMOB == 1
#include "firebase/app.h"
#include "firebase/admob.h"
#endif

USING_NS_CC;

//static cocos2d::Size designResolutionSize = cocos2d::Size(480, 270);
static cocos2d::Size designResolutionSize = cocos2d::Size(1136, 640);
static cocos2d::Size smallResolutionSize = cocos2d::Size(1136, 640);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1334, 750);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1920, 1080);

//480 x 270    // 16:9
//1136 x 640   // 16:9 -> iPhone 5
//1334 x 750   // 16:9 -> iPhone 6, 6s, 7, 8
//1920 x 1080  // 16:9 -> iPhone 6+, 6s+, 7+, 8+
//2688 x 1242  // 16:7.4 -> XS Max
//아이폰 6 1334 x 750 16:9기준으로


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
	AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {

#if USE_ADMOB == 1
	//------------------------------------------------
	//Initialze Fire base
	//------------------------------------------------
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// Initialize Firebase for Android.
	firebase::App* app = firebase::App::Create(
		firebase::AppOptions(), JniHelper::getEnv(), JniHelper::getActivity());
	// Initialize AdMob.
	firebase::admob::Initialize(*app, "INSERT_YOUR_ADMOB_ANDROID_APP_ID");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	// Initialize Firebase for iOS.
	firebase::App* app = firebase::App::Create(firebase::AppOptions());
	// Initialize AdMob.
	firebase::admob::Initialize(*app, "INSERT_YOUR_ADMOB_IOS_APP_ID");
#endif

	// Initialize AdMob.
	firebase::admob::Initialize(*app);
#endif

	//------------------------------------------------


	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("BalloonBurster", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("BalloonBurster");
#endif
		director->setOpenGLView(glview);

	}

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	// Set the design resolution
	//glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);






	cocos2d::Size ResolutionSize = mediumResolutionSize;
	glview->setDesignResolutionSize(ResolutionSize.width, ResolutionSize.height, ResolutionPolicy::EXACT_FIT);
	//glview->setDesignResolutionSize(largeResolutionSize.width, largeResolutionSize.height, ResolutionPolicy::NO_BORDER);
	//SHOW_ALL, NO_BORDER, EXACT_FIT
	auto frameSize = glview->getFrameSize();
	float Percentage_16_9 = 56.25; // 16 = 100, 9 = 56.25
	float height_9Ratio = (frameSize.width * Percentage_16_9) / 100;
	float fFactor = ResolutionSize.height / height_9Ratio;
	fFactor = 1.0;
	//float fFactor = height_9Ratio/frameSize.height;

	//test
	float dd = MIN(frameSize.height / designResolutionSize.height, frameSize.width / designResolutionSize.width);
	float hh = frameSize.height / designResolutionSize.height;
	float ww = frameSize.width / designResolutionSize.width;

	// if the frame's height is larger than the height of medium size.
	//    if (frameSize.height > mediumResolutionSize.height)
	//    {
	//        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
	//    }
	//    // if the frame's height is larger than the height of small size.
	//    else if (frameSize.height > smallResolutionSize.height)
	//    {
	//        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
	//    }
	//    // if the frame's height is smaller than the height of medium size.
	//    else
	//    {
	//        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
	//    }

	//director->setContentScaleFactor(MIN(frameSize.height/designResolutionSize.height, frameSize.width/designResolutionSize.width));
	//director->setContentScaleFactor(MIN(frameSize.height/designResolutionSize.height, frameSize.width/designResolutionSize.width));

	//director->setContentScaleFactor(MIN(hh,ww));
	//16:9 ratio





	director->setContentScaleFactor(fFactor);

	//float fFactor = director->getCo ntentScalseFactor();

	register_all_packages();



	//https://hyunity3d.tistory.com/755
	// ------------------------------------------------
	// Screen rate
	// ------------------------------------------------
	//director->getOpenGLView()->setFrameSize(TARGET_WIDTH, TARGET_HEIGHT);
	//director->getOpenGLView()->setFrameZoomFactor(0.45);
	//CCSize curSize = CCSize(TARGET_WIDTH, TARGET_HEIGHT);
	//director->getOpenGLView()->setDesignResolutionSize(curSize.width, curSize.height, ResolutionPolicy::NO_BORDER);


	// ------------------------------------------------
	// BalloonBurster Scene Start
	// ------------------------------------------------
	// create a scene. it's an autorelease object
	//auto HelloScene = HelloWorld::scene();
	auto LogoScene = CLogoScene::createScene();
	auto MainScene = CMainScene::createScene();


	// run
	director->runWithScene(MainScene);
	director->pushScene(LogoScene);
	//    director->popScene();
	//    director->pushScene(HelloScene);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
