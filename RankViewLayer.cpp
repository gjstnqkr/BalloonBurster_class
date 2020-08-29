//
//  RankViewLayer.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 27/10/2019.
//

#include "RankViewLayer.hpp"
#include "CustomTableViewCell.h"
#include "MC_FileUtils.hpp"
#include "UserInformation.h"

RankViewLayer::RankViewLayer()
: m_sptPauseBG(NULL)
{
}

RankViewLayer::~RankViewLayer()
{
    
}

RankViewLayer* RankViewLayer::create(Node* pParent)
{
    RankViewLayer* pRankView = new (std::nothrow) RankViewLayer();
    if(pRankView && pRankView->init())
    {
        pRankView->autorelease();
        pRankView->setLocalZOrder(Z_LAYER);
        //pRankView->setParent(pParent);
        
        return pRankView;
    }
    CC_SAFE_DELETE(pRankView);
    return nullptr;
}

bool RankViewLayer::init()
{
    Init_UI();
//    MC_FileUtils::makeRankFile();
//    if(!MC_FileUtils::readRankFile()) return false;
    
    Init_Table();
    
    
    
    
    
    
    
    return true;
}

bool RankViewLayer::Init_UI(void)
{

    

    return true;
}

bool RankViewLayer::Init_Table(void)
{
    //995x635
    Node* pParent = this->getParent();
    if(pParent == nullptr) return false;
    
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
    
    //Table
    CCSize winSize = Director::getInstance()->getWinSize();
    
    TableView* tableView = TableView::create(this, CCSize(conSize_BG.width-conSize_BG.height*0.1, conSize_BG.height-conSize_BG.height*0.2));
    tableView->setDirection(ScrollView::Direction::VERTICAL);
    tableView->setPosition(origin + Vec2(fBtnPosX, fBtnPosY) - conSize_BG/2 + Vec2(conSize_BG.height*0.12, conSize_BG.height*0.1));
    tableView->setDelegate(this);
    this->addChild(tableView, Z_LAYER);
    tableView->reloadData();
    
    
    return true;
}


void RankViewLayer::tableCellTouched(TableView* table, TableViewCell* cell)
{
    CCLOG("cell touched at index: %ld", static_cast<long>(cell->getIdx()));
}

Size RankViewLayer::tableCellSizeForIndex(TableView *table, ssize_t idx)
{
    //if (idx == 2) {
    //    return Size(100, 100);
    //}
    return Size(60, RANK_COLUMN_GAP);
}

TableViewCell* RankViewLayer::tableCellAtIndex(TableView *table, ssize_t idx)
{
    auto string = StringUtils::format("%ld", static_cast<long>(idx));
    TableViewCell *cell = table->dequeueCell();
    
    
    if (!cell) {
        
        cell = new (std::nothrow) CustomTableViewCell();
        cell->autorelease();
        
        Size winSize = Director::getInstance()->getWinSize();
        
        fillTableviewItem(cell, idx);
    }
    else
    {
        cell->removeAllChildren();
        
        fillTableviewItem(cell, idx);
    }
    
    return cell;
}

ssize_t RankViewLayer::numberOfCellsInTableView(TableView *table)
{
    return RANK_COUNT;
}

void RankViewLayer::fillTableviewItem(TableViewCell* _pCell, ssize_t idx)
{
//    UserInformation* pUserInfo = UserInformation::GetInstance();
//    
//    rankInfo* pRank = nullptr;
//    for(int ii = 0; ii < pUserInfo->m_RankInfo.size(); ii++)
//    {
//        //if(idx == 0) continue;
//        if(pUserInfo->m_RankInfo[ii]->getRank() == RANK_COUNT - (idx+1))
//             pRank = pUserInfo->m_RankInfo[ii];
//    }
//    
//    std::string arrCategory[5] = { "num", "name", "score", "stage", "class" };
//    int arrCateWidth[5] = { 20+10, 60+10, 130+20, 200+30, 220+40 };
//    
//    //120
//    //0 + 10, 120 + 40, 240 + 90, 360 + 100, 480 + 110,
//    //int arrCateWidth[5] = { 10, 30, 50, 50, 10 };
//
//    
//    for(int ii = 0; ii < RANK_ROW_COUNT; ii++)
//    {
//        if(idx == RANK_COUNT - 1)
//        {
//            auto label = Label::createWithSystemFont(arrCategory[ii], "Helvetica", 20.0);
//            label->setPosition(Vec2(RANK_ROW_GAP*ii + arrCateWidth[ii], 0));
//            label->setAnchorPoint(Vec2::ZERO);
//            label->setTag(123);
//            _pCell->addChild(label);
//        }
//        else
//        {
//            if(pRank == nullptr)
//                return;
//
//            std::string itStr;
//            switch (ii) {
//                case RANK_NO:
//                    itStr = std::to_string(pRank->getRank());
//                    break;
//                case RANK_NAME:
//                    itStr = pRank->getName();
//                    break;
//                case RANK_SCORE:
//                    itStr = std::to_string(pRank->getScore());
//                    break;
//                case RANK_STAGE:
//                    itStr = std::to_string(pRank->getStage());
//                    break;
//                    
//                default:
//                    itStr = "777";
//                    break;
//            }
//            
//            auto label = Label::createWithSystemFont(itStr, "Helvetica", 20.0);
//            label->setPosition(Vec2(RANK_ROW_GAP*ii + arrCateWidth[ii], 0));
//            label->setAnchorPoint(Vec2::ZERO);
//            label->setTag(123);
//            _pCell->addChild(label);
//        }
//    }
}
