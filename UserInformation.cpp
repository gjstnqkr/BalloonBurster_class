//
//  UserInformation.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 2018. 9. 29..
//

#include "UserInformation.h"
#include "interface.h"

using namespace cocos2d;

UserInformation::UserInformation()
:
//m_strUserID("None")
//, m_unGameCash(0)
//, m_unHighScore(0)
//, m_unBestStage(0)
m_strUserID("Over")
, m_unGameCash(78)
, m_unHighScore(99)
, m_unBestStage(2020)
{
//    for(int ii = 0; ii < RANK_COUNT; ii++)
//    {
//        rankInfo* pRankinfo = new (std::nothrow)rankInfo;
//        pRankinfo->setRank(ii+1);
//        m_RankInfo.push_back(pRankinfo);
//    }
    
}

UserInformation::~UserInformation()
{
//    for(int ii = 0; ii < m_RankInfo.size(); ii++)
//    {
//        delete m_RankInfo[ii];
//
//    }
}


UserInformation* UserInformation::GetInstance()
{
    if(g_pUserInfo == nullptr)
        g_pUserInfo = new (std::nothrow) UserInformation();

    return g_pUserInfo;
}

void UserInformation::ReleaseInstance()
{
    if(g_pUserInfo != nullptr)
        delete g_pUserInfo;
    g_pUserInfo = nullptr;
}


bool UserInformation::init()
{
    init_UserInfo();
    
    return true;
}

void UserInformation::init_UserInfo()
{
    m_strUserID._string.clear();
    m_unGameCash    = 0;
    m_unHighScore   = 0;
    m_unBestStage   = 0;
    
    SetUserId("None");
}

void UserInformation::SetUserId(const char* pStr)
{
    m_strUserID._string.clear();
    
    cocos2d::String cocosStr(pStr);
    m_strUserID = cocosStr;
}


