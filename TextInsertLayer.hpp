//
//  TextInsertLayer.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 01/05/2019.
//

#ifndef TextInsertLayer_hpp
#define TextInsertLayer_hpp

#include <stdio.h>
#include "interface.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"


USING_NS_CC;
using namespace ui;

class TextInsertLayer : public cocos2d::Layer, cocos2d::TextFieldDelegate
{
public:
    cocos2d::TextFieldTTF*          m_pTextField;
    int                             m_LayerState;
    std::string                     m_Str;
    
    //ui
    cocos2d::Sprite*                m_sptPauseBG;
    cocos2d::Sprite*                m_pOkBtn;
    cocos2d::Sprite*                m_pCancelBtn;
    
    cocos2d::EventListenerTouchOneByOne*     m_pEventListener;
    cocos2d::EventListenerTouchOneByOne*     m_pEvenLisn_TextField;
    cocos2d::EventListenerTouchOneByOne*     m_pEvenLisn_Touches;
    
    
public:
    TextInsertLayer();
    ~TextInsertLayer();
    
    static TextInsertLayer * create(Node* pParent);
    
    
public:
    bool init();
    bool init_ui();
    bool init_event();
    bool init_sprite();
    
    bool showLayer(bool bShow);
    
    void setLayerState(int nLayerState);
    
    int getLayerState();
    
    cocos2d::EventListenerTouchOneByOne* getEventListener(void);
    
public:
    void menuPauseCallback(Ref* sender);
    
    //Single
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    bool onTouchBegan_TextField(cocos2d::Touch* touch, cocos2d::Event* event);
//    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    
//TextFieldDelegate
public:
    virtual bool onTextFieldInsertText(TextFieldTTF* sender, const char* text, size_t nLen);
    virtual bool onTextFieldDeleteBackward(TextFieldTTF* sender, const char* delText, size_t nLen);
    
};


#endif /* TextInsertLayer_hpp */
