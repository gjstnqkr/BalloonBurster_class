//
//  GameUILayer.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 25/04/2019.
//

#ifndef GameUILayer_hpp
#define GameUILayer_hpp

#include <stdio.h>
#include <vector>

#include "interface.h"
#include "PauseLayer.hpp"

#define UI_H_OFFSET 20.0f
#define UI_W_OFFSET 15.0f

//using namespace std;

enum emUiPosition;
struct UI_Tile //: public cocos2d::Node
{
public:
    int nX;
    int nY;
    float fLeft;
    float fTop;
    float fWidth;
    float fHeight;
    std::string strName;
public:
    UI_Tile(){
        nX = 0; nY = 0;
        fLeft = 0.0f; fTop = 0.0f;
        fWidth  = 0.0f; fHeight = 0.0f;
        strName = "";
    };

    void setTileValue(int _x, int _y, float _left, float _top, float _width, float _height, std::string _name)
    {
        this->nX        = _x;
        this->nY        = _y;
        this->fLeft     = _left;
        this->fTop      = _top;
        this->fWidth    = _width;
        this->fHeight   = _height;
        this->strName   = _name;
    }
    void initTile()
    {
        setTileValue( 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, "" );
    }


    void operator=(const UI_Tile& ut)
    {
        UI_Tile temp;
        this->nX = ut.nX;
        this->nY = ut.nY;
        this->fLeft     = ut.fLeft;
        this->fTop      = ut.fTop;
        this->fWidth    = ut.fWidth;
        this->fHeight   = ut.fHeight;
        this->strName   = ut.strName;
    }
};

class UI_TileManager
{
public:
    std::vector<UI_Tile>        m_listUiTile;

public:

    bool getUiTile(int _iX, int _iY,UI_Tile &_outTile){
        bool bFind = false;
        for(int ii = 0; ii < m_listUiTile.size(); ii++)
        {
            if(m_listUiTile.at(ii).nX == _iX && m_listUiTile.at(ii).nY == _iY){
                _outTile = m_listUiTile.at(ii);
                bFind = true;
            }
        }
        return bFind;
    };
    bool getUiTile(std::string _name, UI_Tile &_outTile){
        bool bFind = false;
        for(int ii = 0; ii < m_listUiTile.size(); ii++)
        {
            if(m_listUiTile.at(ii).strName == _name){
                _outTile = m_listUiTile.at(ii);
                bFind = true;
            }
        }
        return bFind;
    };
    bool isUiTile(int _iX, int _iY){
        bool bFind = false;
        for(int ii = 0; ii < m_listUiTile.size(); ii++)
            if(m_listUiTile.at(ii).nX == _iX && m_listUiTile.at(ii).nY == _iY) bFind = true;
        return bFind;
    };
    //Modify Tile value
    bool setUiTile(int _x, int _y, float _left, float _top, float _width, float _height, std::string _name){
        if(isUiTile(_x, _y) == false) return false;

        for(int ii = 0; ii < m_listUiTile.size(); ii++){
            if(m_listUiTile.at(ii).nX == _x && m_listUiTile.at(ii).nY == _y)
            {
                m_listUiTile.at(ii).setTileValue(_x, _y, _left, _top, _width, _height, _name);
            }
        }

        return true;
    };

    bool addUiTile(int _x, int _y, float _left, float _top , float _width, float _height, std::string _name){
        if(isUiTile(_x, _y) == true) return false;

        UI_Tile newTile;
        newTile.setTileValue(_x, _y, _left, _top, _width, _height, _name);
        m_listUiTile.push_back(newTile);

        return true;
    };

    bool calculateTilePos(int _x, int _y, int _direction, UI_Tile &_out){
        if(isUiTile(_x, _y) == false) return false;
        UI_Tile existTile;
        getUiTile(_x,_y, existTile);
        
        float nextLeft  = 0.0f;
        float nextTop   = 0.0f;
        switch(_direction)
        {
            case emLeft :
            case emTop :
                return false;
                break;
            case emRight :
                nextLeft = existTile.fLeft + existTile.fWidth + UI_W_OFFSET;
                nextTop = existTile.fTop;
                break;
            
            case emBottom :
                nextLeft = existTile.fLeft;
                nextTop = existTile.fTop - existTile.fHeight - UI_H_OFFSET;
                break;
        }
        
        
        _out.fLeft = nextLeft;
        _out.fTop = nextTop;
        return true;
    };

};


class HelloWorld;
class GameUILayer : public cocos2d::Layer
{
public:
    
    //
    PauseLayer*                     m_PauseLayer;
    UI_TileManager                  m_TileManager;
    
    //touch ui
    cocos2d::Menu*                  m_pPauseImg;
    cocos2d::Sprite*                m_pArrBtn;
    cocos2d::Sprite*                m_pMoveObject;
    
    //
    cocos2d::Sprite*                m_sptLifeBG;
    cocos2d::Sprite*                m_sptCashBG;
    cocos2d::Sprite*                m_sptEnemyBG;
    cocos2d::Sprite*                m_spScoreG;
    
    //
    cocos2d::Vec2                   m_MoveObjPosOld;        //This is a object about Pulley
    cocos2d::Vec2                   m_touchStart;           //
    cocos2d::Vec2                   m_ArcPos;               //
    
    cocos2d::Label*                 m_lbEnemyCnt;
    cocos2d::Label*                 m_lbLifeCnt;
    cocos2d::Label*                 m_lbUserScore;
    
    unsigned int                    m_oldLife;
    unsigned int                    m_oldScore;
    unsigned int                    m_oldEnemyCnt;
    
    bool                            m_bIsMoveTouch;
    
    
public:
    GameUILayer();
    ~GameUILayer();
    
    static GameUILayer * create(Node* pParent);
    
    void            setMoveObjPos(cocos2d::Vec2 _arcPos);
    cocos2d::Vec2   getMoveObjPos();
    
public:
    bool init();
    bool Init_UI();
    void Init_Value();
    void Init_Labels();
    bool Init_PauseLayer();
    //bool Init_Sprite();
    
    //Show/Hide ui
    void HideGameUI();
    void ShowGameUI();
    
    
    
    void SetEnemyCntLabel(unsigned int _unScore);
    void SetLifeCntLabel(unsigned int _unScore);
    bool Set_UserScoreLabel(unsigned int _unScore);
    
    unsigned int GetEnemyCntFromLabel(void);
    
public:
    void menuPauseCallback(Ref* sender);
    
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    //Schedular
    void updateData(float _dt);
    
    //Common Func
    void Call_FuncAfterFewTime(void(Ref::*SEL_SCHEDULE)(float), float _fSec);
    
private:
    void BB_GamePause(float _dt);
    void BB_GameResume(float _dt);

};



#endif /* GameUILayer_hpp */




