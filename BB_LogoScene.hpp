//
//  BB_LogoScene.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 7. 3..
//

#ifndef BB_LogoScene_hpp
#define BB_LogoScene_hpp

//#include <stdio.h>

#include "cocos2d.h"


class CLogoScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    cocos2d::Sprite*  m_pSptLogoBatman;
    
    virtual bool init();
    virtual void onEnterTransitionDidFinish();

    void PopScene(float _fSec);
    
    // a selector callback
    //void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(CLogoScene);
    
public:
    void ActionLogoScene();
    

};


#endif /* BB_LogoScene_hpp */
