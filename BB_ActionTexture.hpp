//
//  BB_ActionTexture.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 09/05/2020.
//

#ifndef BB_ActionTexture_hpp
#define BB_ActionTexture_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "interface.h"



class ActionTexture : public cocos2d::Node
{
public:
    int                     m_nType;
    cocos2d::Sprite*        m_pSprite;
    cocos2d::Label*         m_lbNum;
    
    cocos2d::Node*          m_pParentNode;
    
public:
    static ActionTexture* create( cocos2d::Vec2 _Pos, int type, cocos2d::Node* pParent = NULL );

public:
    //Common Func
    void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);
    
public:
    ActionTexture();
    virtual ~ActionTexture();
    
    void init_Cocos2dInfo( cocos2d::Vec2 _Pos = cocos2d::Vec2::ZERO, int _type = emActionTextureType::AT_Number );
    
    void showActionTexture();
    void disappearTexture(float _dt);
    void deleteActionTexture(float _dt);
    
    void set_Type(int _type);
    int get_Type();
    
    void SetNumLabel(unsigned int _Value);
    void SetNumLabel(unsigned int _value, float _scale, cocos2d::Color3B _color);
};

#endif /* BB_ActionTexture_hpp */
