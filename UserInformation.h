//
//  UserInformation.h
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 9. 29..
//
#pragma once

#ifndef UserInformation_h
#define UserInformation_h

#include "cocos2d.h"
//#include ""<vector.h>""

struct rankInfo
{
    unsigned int                        m_unRank;
    std::string                         m_strName;
    unsigned int                        m_unScore;
    unsigned int                        m_unStage;
    bool                                m_bIsUser;
    
    rankInfo()
    : m_unRank(0)
    //, m_strName("")
    , m_unScore(0)
    , m_bIsUser(false)
    {

    }
    ~rankInfo();
    
    std::string getName(){ return m_strName; }
    virtual unsigned int getRank(){ return m_unRank; }
    virtual unsigned int getScore(){ return m_unScore; }
    virtual unsigned int getStage(){ return m_unStage; }
    virtual bool IsUser(){ return m_bIsUser; }
    
    void setName(std::string pStr)
    {
        m_strName = "";
        m_strName = pStr;
    }

    virtual void setRank(const unsigned int _unRank) { m_unRank = _unRank; }
    virtual void setScore(const unsigned int _unScore) { m_unScore = _unScore; }
    virtual void setStage(const unsigned int _unStage) { m_unStage = _unStage; }
    virtual void setIsUser(bool _bIsUser) { m_bIsUser = _bIsUser; }
    
    void setRankinfo(const unsigned int _unRank, std::string _strName,
                     const unsigned int _unScore, const unsigned int _unStage, bool _bIsUser = false )
    {
        setRank(_unRank);
        setName(_strName);
        setScore(_unScore);
        setStage(_unStage);
        setIsUser(_bIsUser);
    }
};



class UserInformation //: public cocos2d::Node
{
public:
    
public:
    cocos2d::String                     m_strUserID;
    unsigned int                        m_unGameCash;
    unsigned int                        m_unHighScore;
    unsigned int                        m_unBestStage;
    std::vector<rankInfo*>              m_RankInfo;
    
public:
    UserInformation();
    ~UserInformation();
    
    static UserInformation* GetInstance();
    static void ReleaseInstance();
    

public:
    bool init();
    void init_UserInfo();
    //void init();
    
    void SetUserId(const char* pStr);
    virtual void SetGameCash(const unsigned int _unCash) { m_unGameCash = _unCash; }
    virtual void SetHighScore(const unsigned int _unHighScore) { m_unHighScore = _unHighScore; }
    virtual void SetBestStage(const unsigned int _unBestStage) { m_unBestStage = _unBestStage; }
    
    cocos2d::String getUserId(){ return m_strUserID; }
    virtual unsigned int getGameCash(){ return m_unGameCash; }
    virtual unsigned int getHighScore(){ return m_unHighScore; }
    virtual unsigned int getBestStage(){ return m_unBestStage; }
};

static UserInformation* g_pUserInfo;


#endif /* UserInformation_h */
