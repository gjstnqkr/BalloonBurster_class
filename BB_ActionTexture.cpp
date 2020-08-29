//
//  BB_ActionTexture.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 09/05/2020.
//

#include "BB_ActionTexture.hpp"
USING_NS_CC;

ActionTexture::ActionTexture()
: m_nType(emActionTextureType::AT_Number)
, m_lbNum(nullptr)
, m_pSprite(nullptr)
, m_pParentNode(nullptr)
{
}

ActionTexture::~ActionTexture()
{
}

ActionTexture* ActionTexture::create(cocos2d::Vec2 _Pos, int type, cocos2d::Node* pParent)
{
    ActionTexture * ret = new (std::nothrow) ActionTexture();
    if (ret && ret->init())
    {
        ret->autorelease();
        ret->m_pParentNode = pParent;
        ret->init_Cocos2dInfo(_Pos, type);
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

void ActionTexture::init_Cocos2dInfo(cocos2d::Vec2 _Pos, int _type) {
    set_Type(_type);
    if(get_Type() == emActionTextureType::AT_Number)
    {
        m_lbNum = cocos2d::Label::createWithBMFont("fonts/bitmapFontTest4.fnt", "0");
        m_lbNum->setPosition(_Pos);
        m_lbNum->setColor(Color3B(255.0,240.0,230.0));
        m_lbNum->setScale(1.5f);
        m_lbNum->setVisible(false);
        this->addChild(m_lbNum, Z_SCENE_UI_VALUE);
        
        if(m_pParentNode != nullptr)
            m_pParentNode->addChild(this, Z_SCENE_UI_VALUE);
    }
}


void ActionTexture::Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec)
{
    this->scheduleOnce(SEL_SCHEDULE, _fSec);
}

void ActionTexture::disappearTexture(float _dt)
{
    m_lbNum->stopAllActions();
    FadeOut* ActionFade      = FadeOut::create(0.6f);
    auto moveAction = cocos2d::MoveBy::create(0.6f,Vec2(4.0f, 0.0f));
    m_lbNum->runAction(ActionFade);
    m_lbNum->runAction(moveAction);
    Call_FuncAfterFewTime(schedule_selector(ActionTexture::deleteActionTexture), 1.0);
}

void ActionTexture::deleteActionTexture(float _dt)
{
    removeFromParentAndCleanup(true);
}

void ActionTexture::set_Type(int _type)
{
    m_nType = _type;
}

int ActionTexture::get_Type()
{
    return m_nType;
}

void ActionTexture::SetNumLabel(unsigned int _Value)
{
    char str[10] = { 0 };
    sprintf(str, "%d", _Value);
    m_lbNum->setString(str);
}

void ActionTexture::SetNumLabel(unsigned int _value, float _scale, Color3B _color)
{
    SetNumLabel(_value);
    m_lbNum->setScale(_scale);
    m_lbNum->setColor(_color);
}

void ActionTexture::showActionTexture() {
    m_lbNum->setVisible(true);
    m_lbNum->setOpacity(0.0f);
    m_lbNum->stopAllActions();
    FadeIn* ActionFade      = FadeIn::create(0.3f);
    auto moveAction = cocos2d::MoveBy::create(0.3f,Vec2(3.0f, 0.0f));
    m_lbNum->runAction(ActionFade);
    m_lbNum->runAction(moveAction);
    
    Call_FuncAfterFewTime(schedule_selector(ActionTexture::disappearTexture), 0.3);
}

