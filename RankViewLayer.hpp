//
//  RankViewLayer.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 27/10/2019.
//

#ifndef RankViewLayer_hpp
#define RankViewLayer_hpp

#include <stdio.h>
#include "interface.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

class RankViewLayer
    : public cocos2d::Layer
    , public cocos2d::extension::TableViewDataSource
    , public cocos2d::extension::TableViewDelegate
{
public:
    static RankViewLayer*           m_pRankViewLayer;
    //ui
    cocos2d::Sprite*                m_sptPauseBG;
    
public:
    RankViewLayer();
    ~RankViewLayer();
    
    static RankViewLayer * create(Node* pParent);
    
public:
    bool init();
    bool Init_UI();
    bool Init_Table();
    
public:
    virtual void scrollViewDidScroll(cocos2d::extension::ScrollView* view){};
    virtual void scrollViewDidZoom(cocos2d::extension::ScrollView* view){};
    virtual void tableCellTouched(TableView* table, TableViewCell* cell);
    virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx);
    virtual TableViewCell* tableCellAtIndex(TableView* table, ssize_t idx);
    virtual ssize_t numberOfCellsInTableView(TableView* table);
    
private:
    void fillTableviewItem(TableViewCell* _pCell, ssize_t idx);
    
};


#endif /* RankViewLayer_hpp */
