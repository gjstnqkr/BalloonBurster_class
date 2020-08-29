//
//  TextInsertLayer.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 01/05/2019.
//

#include "TextInsertLayer.hpp"
#include "BB_MainScene.hpp"
#include "MC_FileUtils.hpp"

USING_NS_CC;


TextInsertLayer::TextInsertLayer()
: m_pTextField(nullptr)
, m_LayerState(emLayerState::Layer_None)
, m_Str("")
, m_sptPauseBG(NULL)
, m_pOkBtn(nullptr)
, m_pCancelBtn(nullptr)
, m_pEventListener(nullptr)
, m_pEvenLisn_TextField(nullptr)
, m_pEvenLisn_Touches(nullptr)
{
    
}

TextInsertLayer::~TextInsertLayer()
{
    
}

TextInsertLayer* TextInsertLayer::create(Node* pParent)
{
    //TextInsertLayer* pTextLayer = new (std::nothrow) TextInsertLayer();
    TextInsertLayer* pTextLayer = new TextInsertLayer();
    if(pTextLayer && pTextLayer->init())
    {
        //pTextLayer->setParent(pParent);
        pTextLayer->autorelease();
        pTextLayer->setLocalZOrder(Z_LAYER);
        return pTextLayer;
    }
    CC_SAFE_DELETE(pTextLayer);
    return nullptr;
}

bool TextInsertLayer::init()
{
    init_event();
    init_ui();
    
    
    return true;
}


bool TextInsertLayer::init_ui()
{
    //Menu BackGround
    const CCSize visibleSize    = Director::getInstance()->getVisibleSize();
    const Vec2 origin           = Director::getInstance()->getVisibleOrigin();
    auto Size                   = Director::getInstance()->getWinSize();
    const Vec2 ScreenCenter     = origin + Vec2(visibleSize * 0.5);
    
    auto sptPauseBG = Sprite::create("menu_Pause_mid.png");
    float fBtnPosY = visibleSize.height * 0.5;
    float fBtnPosX = visibleSize.width * 0.5;
    CCSize conSize_BG = sptPauseBG->getContentSize();
    sptPauseBG->setPosition(origin + Vec2(fBtnPosX, fBtnPosY) - conSize_BG/2);
    sptPauseBG->setAnchorPoint(Vec2(0,0));
    sptPauseBG->setScale(1.0);
    this->addChild(sptPauseBG);
    m_sptPauseBG = sptPauseBG;

    
    //Ok Btn
    m_pOkBtn = cocos2d::Sprite::create("button_Restart.png");
    float fBtnOffset            = visibleSize.height * UI_TOP_START_RATE;
    CCSize  orgSize_OkBtn       = m_pOkBtn->getContentSize();
    Vec2 Center = Vec2(orgSize_OkBtn.width*0.5f, (orgSize_OkBtn.height * 0.5) );
    m_pOkBtn->setPosition( ScreenCenter - Center + Vec2(-orgSize_OkBtn.width*0.7, -orgSize_OkBtn.height * 1.0f));
    m_pOkBtn->setAnchorPoint(Vec2(0.0,0.0));
    m_pOkBtn->setScale(1.0);

    this->addChild(m_pOkBtn, 1);
    
    //cancel Btn
    m_pCancelBtn = cocos2d::Sprite::create("button_Out.png");
    fBtnOffset                         = visibleSize.height * UI_TOP_START_RATE;
    CCSize orgSize_CancelBtn           = m_pCancelBtn->getContentSize();
    Center = Vec2(orgSize_CancelBtn.width*0.5f, (orgSize_CancelBtn.height * 0.5) );
    m_pCancelBtn->setPosition( ScreenCenter - Center + Vec2(orgSize_CancelBtn.width*0.7, -orgSize_CancelBtn.height * 1.0f));
    m_pCancelBtn->setAnchorPoint(Vec2(0.0,0.0));
    m_pCancelBtn->setScale(1.0);
    
    this->addChild(m_pCancelBtn, 1);
    
    
    if(m_pTextField == nullptr) return false;
    
    //label
    CCSize LabelSize = CCSize(300, 25);
    auto label = Label::createWithSystemFont("Insert your ID", "", 20,
                                             LabelSize, TextHAlignment::CENTER, TextVAlignment::CENTER);
    
    label->setColor(Color3B::BLACK);
    label->setPosition(ScreenCenter + Vec2(-LabelSize.width/2, m_pTextField->getContentSize().height));
    label->setAnchorPoint(Vec2(0.0,0.0));
    this->addChild(label);
    
    
    

    return true;
}

bool TextInsertLayer::init_event()
{
    //Single
    if(m_pEvenLisn_Touches == nullptr)
    {
        m_pEvenLisn_Touches = EventListenerTouchOneByOne::create();
        m_pEvenLisn_Touches->onTouchBegan     = CC_CALLBACK_2(TextInsertLayer::onTouchBegan, this);
        m_pEvenLisn_Touches->onTouchEnded     = CC_CALLBACK_2(TextInsertLayer::onTouchEnded, this);
    }
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pEvenLisn_Touches, this);
    
    //Text Field
    const CCSize visibleSize    = Director::getInstance()->getVisibleSize();
    const Vec2 origin           = Director::getInstance()->getVisibleOrigin();
    auto Size                   = Director::getInstance()->getWinSize();
    const Vec2 ScreenCenter     = origin + Vec2(visibleSize * 0.5);
    
    if(m_pTextField == nullptr)
    {
        TextFieldTTF* textField = TextFieldTTF::textFieldWithPlaceHolder("Insert", "Arial", 30);
        textField->setPosition(visibleSize / 2);
        this->addChild(textField, Z_LAYER);
        m_pTextField = textField;
        m_pTextField->setDelegate(this);
    }
    
    if(m_pEvenLisn_TextField == nullptr)
    {
        m_pEvenLisn_TextField = EventListenerTouchOneByOne::create();
        m_pEvenLisn_TextField->onTouchBegan = CC_CALLBACK_2(TextInsertLayer::onTouchBegan_TextField, this);
    }
    _eventDispatcher->addEventListenerWithSceneGraphPriority(m_pEvenLisn_TextField, m_pTextField);
    

    
    return true;
}

bool TextInsertLayer::onTouchBegan(Touch* touch, Event* event)
{
    
    
    return true;
}
void TextInsertLayer::onTouchEnded(Touch* touch, Event* event)
{
    if(getLayerState() != emLayerState::Layer_Play) return;
    
    auto touchPoint = touch->getLocation();
    //log("onTouchBegan id = %d, x = %f, y = %f", touch->getID(), touchPoint.x, touchPoint.y);
    
    bool isTouch = m_pOkBtn->getBoundingBox().containsPoint(touchPoint);
    if(isTouch)
    {
        const std::string str = m_pTextField->getString();
        showLayer(false);
        m_pTextField->setString("");
        
        UserInformation* pUserInfo = UserInformation::GetInstance();
        pUserInfo->SetUserId(m_Str.c_str());
        MC_FileUtils::writeUserFile();
        m_Str.clear();
        
        CMainScene* pMainScene = dynamic_cast<CMainScene*>(this->getParent());
        if(pMainScene)
        {
            pMainScene->setSceneState(emSceneState::Scene_None);
            pMainScene->setStartMenuEnable(true);
        }
        
    }
    
    bool isTouch_Cancel = m_pCancelBtn->getBoundingBox().containsPoint(touchPoint);
    if(isTouch_Cancel)
    {
        this->setVisible(false);
        m_pTextField->setString("");
        m_Str.clear();
        showLayer(false);
        
        CMainScene* pMainScene = dynamic_cast<CMainScene*>(this->getParent());
        if(pMainScene)
        {
            pMainScene->setSceneState(emSceneState::Scene_None);
            pMainScene->setStartMenuEnable(true);
        }
        
    }
    
    
    log("onTouchBegan");
    log("onTouchEnded");
    //log("Multi touch end");
}

bool TextInsertLayer::onTouchBegan_TextField(Touch* touch, Event* event)
{
    if(getLayerState() != emLayerState::Layer_Play) return false;
    auto touchPoint = touch->getLocation();
    
    m_pTextField->enableOutline(Color4B(1.0, 1.0, 1.0, 1.0));
    
    bool isTouchOK = m_pTextField->getBoundingBox().containsPoint(touchPoint);
    if (isTouchOK) {
        m_pTextField->attachWithIME();
    }else{
        m_pTextField->detachWithIME();
    }
    
    log("x _ %f, y _ %f", touchPoint.x, touchPoint.y);
    log("onTouchBegan_TextField touch down");
    return true;
}


bool TextInsertLayer::showLayer(bool bShow)
{
    if(bShow)
    {
        init_event();
        this->setVisible(true);
        setLayerState(emLayerState::Layer_Play);
    }
    else
    {
        _eventDispatcher->removeEventListener(m_pEvenLisn_Touches);
        _eventDispatcher->removeEventListener(m_pEvenLisn_TextField);
        m_pEvenLisn_Touches = nullptr;
        m_pEvenLisn_TextField = nullptr;
        
        this->setVisible(false);
        setLayerState(emLayerState::Layer_Stop);
    }
    
    log("showLayer %d ",bShow);
    
    return true;
}


EventListenerTouchOneByOne* TextInsertLayer::getEventListener(void)
{
    if(m_pEventListener == nullptr)
        m_pEventListener = EventListenerTouchOneByOne::create();
        
    return m_pEventListener;
}

void TextInsertLayer::setLayerState(int nLayerState)
{
    m_LayerState = (emLayerState)nLayerState;
}

int TextInsertLayer::getLayerState()
{
    return (int)m_LayerState;
}


bool TextInsertLayer::onTextFieldInsertText(TextFieldTTF* sender, const char* text, size_t nLen)
{
    if(text == "\n")
    {
        m_pTextField->detachWithIME();
        return true;
    }
    
    
    int length = m_Str.length();
    if(length < 10)
    {
        m_Str = m_Str + text;
        m_pTextField->setString(m_Str);
    }
    
    return true;
}

/**
 *@brief    If the sender doesn't want to delete the delText, return true.
 */
bool TextInsertLayer::onTextFieldDeleteBackward(TextFieldTTF* sender, const char* delText, size_t nLen)
{
    int length = m_Str.length();
    if(length > 0)
        m_Str.erase(length - 1, 1);
    
    m_pTextField->setString(m_Str);
    return true;
}
