//
//  BB_LogoScene.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 7. 3..
//

#include "BB_LogoScene.hpp"

#define Height_Gap 35.00

USING_NS_CC;

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}


Scene* CLogoScene::createScene()
{
    return CLogoScene::create();
}

// on "init" you need to initialize your instance
bool CLogoScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    
    //Set BackGround Color
    Director::getInstance()->setClearColor(Color4F(0.0, 0.0, 0.0, 0));
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
   
    // add "HelloWorld" splash screen"
    m_pSptLogoBatman = Sprite::create("Log_Batman.png");
    if (m_pSptLogoBatman == nullptr)
    {
        problemLoading("'Log_Batman.png'");
    }
    else
    {
        //BackGround rate (full Screen)
        m_pSptLogoBatman->setPosition(Vec2(origin.x,origin.y));
        m_pSptLogoBatman->setAnchorPoint(Vec2(0,0));

        CCSize tSizeOrig = m_pSptLogoBatman->getContentSize();

        float ScaleX = (visibleSize.width)/tSizeOrig.width;
        float ScaleY = (visibleSize.height)/tSizeOrig.height;

        m_pSptLogoBatman->setScaleX(ScaleX);
        m_pSptLogoBatman->setScaleY(ScaleY);
        
        // add the sprite as a child to this layer
        this->addChild(m_pSptLogoBatman, 0);
        
        //Display Test
        auto spt2688 = Sprite::create("2688_1242.png");
        spt2688->setPosition(Vec2(origin.x,origin.y));
        spt2688->setAnchorPoint(Vec2(0,0));
        
        auto spt1920 = Sprite::create("1920_1080.png");
        spt1920->setPosition(Vec2(origin.x,origin.y));
        spt1920->setAnchorPoint(Vec2(0,0));
        
        auto spt1334 = Sprite::create("1334_750.png");
        spt1334->setPosition(Vec2(origin.x,origin.y));
        spt1334->setAnchorPoint(Vec2(0,0));
        
        auto spt480 = Sprite::create("480_270.png");
        spt480->setPosition(Vec2(origin.x,origin.y));
        spt480->setAnchorPoint(Vec2(0,0));
        
//        this->addChild(spt2688, 1);
//        this->addChild(spt1920, 2);
//        this->addChild(spt1334, 2);
//        this->addChild(spt480, 3);
    
        
    }
    
    
    return true;
}

//When this Scene push in Director, Call this Func
void CLogoScene::onEnterTransitionDidFinish()
{
    ActionLogoScene();
    return;
}

void CLogoScene::PopScene(float _fSec)
{
    Director::getInstance()->popScene();
}

void CLogoScene::ActionLogoScene()
{
    if (m_pSptLogoBatman == nullptr)
    {
        problemLoading("'Log_Batman.png' Loading Failed");
        return;
    }
    
    m_pSptLogoBatman->setOpacity(0.0f);
    m_pSptLogoBatman->stopAllActions();
    auto Action = cocos2d::FadeIn::create(5.0f);
    m_pSptLogoBatman->runAction(Action);
    this->scheduleOnce(schedule_selector(CLogoScene::PopScene), 5.0f);
}


