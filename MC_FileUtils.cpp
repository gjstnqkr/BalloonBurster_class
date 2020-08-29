//
//  MC_FileUtils.cpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 06/05/2019.
//

#include "MC_FileUtils.hpp"
#include "MC_GameOption.hpp"
#include "UserInformation.h"
#include "interface.h"

using namespace cocos2d;


MC_FileUtils::MC_FileUtils()
//: m_pUserInfo(NULL)
{
    //m_pFileUtils = nullptr;
}


MC_FileUtils::~MC_FileUtils()
{
}


MC_FileUtils* MC_FileUtils::create(void)
{
    MC_FileUtils* pFileUtils = new (std::nothrow) MC_FileUtils();
    if(pFileUtils && pFileUtils->init())
    {
        pFileUtils->autorelease();
        return pFileUtils;
    }
    CC_SAFE_DELETE(pFileUtils);
    return nullptr;
}

//UserInformation* MC_FileUtils::Get_Instance(void)
//{
//    return m_pUserInfo;
//}

bool MC_FileUtils::init()
{
    init_UserInfo();
    
    return true;
}

bool MC_FileUtils::init_UserInfo()
{
    std::string filePath = MC_FileUtils::getFilePath(USER_PATH);
    if(MC_FileUtils::isFile(USER_PATH))
    {
        readUserFile();
        return true;
    }
    else
    {
        makeUserFile();
        readUserFile();
        return false;
    }
    
    return true;
}

bool MC_FileUtils::init_OptionInfo()
{
    std::string filePath = MC_FileUtils::getFilePath(OPTION_PATH);
    if(MC_FileUtils::isFile(OPTION_PATH))
    {
        readOptionFile();
        return true;
    }
    else
    {
        makeOptionFile();
        readOptionFile();
        return false;
    }
    
    return true;
}

void MC_FileUtils::makeUserFile()
{
    std::string filePath = MC_FileUtils::getFilePath(USER_PATH);
    
    //C File I/O
    log("Save File Full-Path: %s", filePath.c_str());

    
    char Title[]    = GAME_TITLE;
    char Version[]  = GAME_VERSION;
    char item1[]    = "UserName_";
    char name[]     = "None";
    char item2[]    = "GameCash_";
    char item3[]    = "HighScore_";
    char item4[]    = "BestStage_";
    char newLine[]  = "$";
    int value       = 0;

    int cnt = strlen(Title);
    FILE* fp = fopen(filePath.c_str(), "w");
    fwrite(Title, sizeof(char)    ,strlen(Title)     ,fp);   //title
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(Version, sizeof(char)  ,strlen(Version)   ,fp);  //version
    //cnt = strlen(Version);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(item1,   sizeof(char)  ,strlen(item1)     ,fp);  //UserName
    fwrite(name,   sizeof(char)   ,strlen(name)      ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    
    fwrite(item2,   sizeof(char)  ,strlen(item2)     ,fp);  //GameCash
    fwrite(&value,   sizeof(int)  ,1                 ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
//    cnt = sizeof(item2);
    
    fwrite(item3,   sizeof(char)  ,strlen(item3)     ,fp);  //Hiscore
    fwrite(&value,   sizeof(int)  ,1                 ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
//    cnt = sizeof(item3);
    
    fwrite(item4,   sizeof(char)  ,strlen(item4)     ,fp);  //BestStage
    fwrite(&value,   sizeof(int)  ,1                 ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
//    cnt = sizeof(item4);

    fclose(fp);
}


bool MC_FileUtils::writeUserFile()
{
    
    std::string filePath = MC_FileUtils::getFilePath(USER_PATH);
    
    //C File I/O
    log("Save File Full-Path: %s", filePath.c_str());
    
    
    char Title[]    = GAME_TITLE;
    char Version[]  = GAME_VERSION;
    char item1[]    = "UserName_";
    char item2[]    = "GameCash_";
    char item3[]    = "HighScore_";
    char item4[]    = "BestStage_";
    char newLine[]  = "$";
    int value = 0;
    const char *name = NULL;
    
    UserInformation* pUserInfo = UserInformation::GetInstance();
    cocos2d::String strUser = pUserInfo->getUserId();
    name = strUser.getCString();
    
    int cnt = strlen(Title);
    FILE* fp = fopen(filePath.c_str(), "w");
    fwrite(Title, sizeof(char)    ,strlen(Title)     ,fp);   //title
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(Version, sizeof(char)  ,strlen(Version)   ,fp);  //version
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    
    fwrite(item1,   sizeof(char)  ,strlen(item1)     ,fp);  //UserName
    fwrite(name,   sizeof(char)   ,strlen(name)      ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    value = pUserInfo->getGameCash();
    fwrite(item2,   sizeof(char)  ,strlen(item2)     ,fp);  //GameCash
    fwrite(&value,   sizeof(int)  ,1                 ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    value = pUserInfo->getHighScore();
    fwrite(item3,   sizeof(char)  ,strlen(item3)     ,fp);  //Hiscore
    fwrite(&value,   sizeof(int)  ,1                 ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    value = pUserInfo->getBestStage();
    fwrite(item4,   sizeof(char)  ,strlen(item4)     ,fp);  //BestStage
    fwrite(&value,   sizeof(int)  ,1                 ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    
    fclose(fp);
    
    return true;
}

bool MC_FileUtils::readUserFile()
{
    if(!MC_FileUtils::isFile(USER_PATH)) return false;
    
    std::string filePath = MC_FileUtils::getFilePath(USER_PATH);
    
    char Title[50]      = "\0";
    char Version[50]    = "\0";
    char UserName[50]   = "\0";
	char pGameCash[50] = "\0";
	char pHighScore[50] = "\0";
	char pBestStage[50] = "\0";
    //int *pGameCash      = new int;
    //int *pHighScore     = new int;
    //int *pBestStage     = new int;
    
    FILE* fp = fopen(filePath.c_str(), "r");
    
    fgets(Title,    sizeof(GAME_TITLE), fp);
    if( strncmp(Title, GAME_TITLE, sizeof(GAME_TITLE)) ) return false;
    
    fseek(fp, strlen("$"), SEEK_CUR);
    fgets(Version,  sizeof(GAME_VERSION), fp);
    
    getStringFromFile(GAME_USERNAME, fp, UserName);
    getStringFromFile(GAME_GAMECASH, fp, pGameCash);
    getStringFromFile(GAME_HIGHSCORE, fp, pHighScore);
    getStringFromFile(GAME_BESTSTAGE, fp, pBestStage);
    
    UserInformation* pUserInfo = UserInformation::GetInstance();
    pUserInfo->SetUserId(UserName);
    pUserInfo->SetGameCash((*pGameCash));
    pUserInfo->SetHighScore((*pHighScore));
    pUserInfo->SetBestStage((*pBestStage));
    
    //delete pGameCash; delete pHighScore; delete pBestStage;
    
    fclose(fp);
    return true;
}
//
//void MC_FileUtils::makeRankFile()
//{
//    std::string filePath = MC_FileUtils::getFilePath(RANK_PATH);
//
//    //C File I/O
//    log("Save File Full-Path: %s", filePath.c_str());
//
//    char Title[]    = GAME_TITLE;
//    char Version[]  = GAME_VERSION;
//
//    std::string arrRankStr[RANK_COUNT] ={
//        "1_Murcuders_1000000_20",
//        "2_PHS_900000_19",
//        "3_BalloonBurster_800000_18",
//        "4_CoolArcher_700000_17",
//        "5_GunFire_600000_16",
//        "6_ILIDAN_500000_15",
//        "7_MoonPower_400000_14",
//        "8_None_300000_13",
//        "9_None2_200000_12"
//    };
//
//    char newLine[]  = "$";
//    int value = 0;
//
//    int cnt = strlen(Title);
//    FILE* fp = fopen(filePath.c_str(), "w");
//    fwrite(Title, sizeof(char)    ,strlen(Title)     ,fp);   //title
//    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
//
//    fwrite(Version, sizeof(char)  ,strlen(Version)   ,fp);  //version
//    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
//
//    for(int ii = 0; ii < RANK_COUNT; ii++)
//    {
//        fwrite(arrRankStr[ii].c_str(),   sizeof(char)  ,strlen(arrRankStr[ii].c_str())     ,fp);
//        fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
//    }
//
//    fclose(fp);
//}
//
//bool MC_FileUtils::readRankFile()
//{
//    if(!MC_FileUtils::isFile(RANK_PATH)) return false;
//
//    std::string filePath = MC_FileUtils::getFilePath(RANK_PATH);
//
//    char Title[50]      = "\0";
//    char Version[50]    = "\0";
//    char RankInfo[RANK_COUNT][50];
//    memset(RankInfo, 0, sizeof(char)*(RANK_COUNT*50) );
//
//    char UserName[50]   = "\0";
//
//    FILE* fp = fopen(filePath.c_str(), "r");
//
//    fgets(Title,    sizeof(GAME_TITLE), fp);
//    if( strncmp(Title, GAME_TITLE, sizeof(GAME_TITLE)) ) return false;
//
//    fseek(fp, strlen("$"), SEEK_CUR);
//    fgets(Version,  sizeof(GAME_VERSION), fp);
//
//    fseek(fp, strlen("$"), SEEK_CUR);
//
//    for(int ii = 1; ii < RANK_COUNT+1; ii++)
//    {
//        getRankInfoFromFile(std::to_string(ii)+"_", fp, RankInfo[ii-1]);
//        setRankInfoToUserInfo(RankInfo[ii-1], ii);
//
//    }
//
//
//    fclose(fp);
//    return true;
//}
//
//bool MC_FileUtils::writeRankFile()
//{
//    return true;
//}

void MC_FileUtils::getStringFromFile(std::string word, FILE* fp, void* pOutValue)
{
    fseek(fp, strlen(GAME_TITLE) + strlen("$") + strlen(GAME_VERSION) + strlen("$"), SEEK_SET);
    
    char buffer[255]; char* pbuf; pbuf = buffer; memset(buffer,0,sizeof(buffer));
    bool bClear = true; int c = 0; bool bIsRightData = false;
    char* pBuffer = (char*)pOutValue;
    do {
        c = fgetc(fp);
        if (c == '_' || c == '$' )
        {
            if( !strncmp(buffer, word.c_str(), strlen(buffer)) )
                bIsRightData = true;
            
            if(c == '$' && bIsRightData)
            {
                bClear = false;
                continue;
            }
            
            c = fgetc(fp);
            memset(buffer,0,sizeof(buffer));  pbuf = buffer;
            memset(pOutValue,0,sizeof(pOutValue));
            pBuffer = (char*)pOutValue;
        }
        
        (*pBuffer) = c;
        pBuffer++;
        (*pbuf) = c;
        pbuf++;
        
        if( c == EOF )
            bClear = false;
    } while (bClear);
    
}
//
//void MC_FileUtils::getRankInfoFromFile(std::string word, FILE* fp, void* pOutValue)
//{
//    fseek(fp, strlen(GAME_TITLE) + strlen("$") + strlen(GAME_VERSION) + strlen("$"), SEEK_SET);
//
//    char buffer[255]; char* pbuf; pbuf = buffer; memset(buffer,0,sizeof(buffer));
//    bool bClear = true; int c = 0; bool bIsRightData = false;
//    char* pBuffer = (char*)pOutValue;
//    do {
//        c = fgetc(fp);
//        if (c == '_' || c == '$' )
//        {
//            if(c == '_')
//            {
//                (*pbuf) = c; pbuf++;
//                (*pBuffer) = c; pBuffer++;
//            }
//
//            if( !strncmp(buffer, word.c_str(), strlen(buffer)) )
//                bIsRightData = true;
//
//            if(c == '$' && bIsRightData)
//            {
//                bClear = false;
//                continue;
//            }
//            c = fgetc(fp);
//
//
//            if(bIsRightData == false)
//            {
//                memset(buffer,0,sizeof(buffer)); pbuf = buffer;
//                memset(pOutValue,0,sizeof(pOutValue)); pBuffer = (char*)pOutValue;
//            }
//        }
//
//        (*pBuffer) = c; pBuffer++;
//        (*pbuf) = c; pbuf++;
//
//
//        if( c == EOF )
//            bClear = false;
//    } while (bClear);
//
//
//    log("complate read line");
//}
//
//void MC_FileUtils::setRankInfoToUserInfo(void* pInValue, int _rankNum)
//{
//    UserInformation* pUserInfo = UserInformation::GetInstance();
//
//    //rankInfo
//    char buffer[255];  char* pbuf; pbuf = buffer;
//    memset(buffer,0,sizeof(buffer));
//    char* c = (char*)pInValue;
//    bool bClear = true;
//    int ii = 0; int cate = 0;
//    do {
//        char p = c[ii];
//
//
//        if(p == '_' || p == NULL)
//        {
//            ii++;
//
//            switch (cate) {
//                case emRankCate::RANK_NO:
//                    for(int ii = 0; ii < pUserInfo->m_RankInfo.size(); ii++)
//                    {
//                        if(pUserInfo->m_RankInfo[ii]->getRank() == _rankNum)
//                        {
//                            int nRank = atoi(buffer);
//                            pUserInfo->m_RankInfo[ii]->setRank(nRank);
//                        }
//                    }
//                    cate++;
//                    break;
//                case emRankCate::RANK_NAME:
//                    for(int ii = 0; ii < pUserInfo->m_RankInfo.size(); ii++)
//                    {
//                        if(pUserInfo->m_RankInfo[ii]->getRank() == _rankNum)
//                        {
//                            std::string str(buffer);
//                            pUserInfo->m_RankInfo[ii]->m_strName = str;
//                        }
//                    }
//                    cate++;
//                    break;
//                case emRankCate::RANK_SCORE:
//                    for(int ii = 0; ii < pUserInfo->m_RankInfo.size(); ii++)
//                    {
//                        if(pUserInfo->m_RankInfo[ii]->getRank() == _rankNum)
//                        {
//                            int nScore = atoi(buffer);
//                            pUserInfo->m_RankInfo[ii]->setScore(nScore);
//                        }
//                    }
//                    cate++;
//                    break;
//                case emRankCate::RANK_STAGE:
//                    for(int ii = 0; ii < pUserInfo->m_RankInfo.size(); ii++)
//                    {
//                        if(pUserInfo->m_RankInfo[ii]->getRank() == _rankNum)
//                        {
//                            int nStage = atoi(buffer);
//                            pUserInfo->m_RankInfo[ii]->setStage(nStage);
//                        }
//                    }
//                    cate++;
//                    break;
//
//                default:
//                    break;
//            }
//
//            memset(buffer,0,sizeof(buffer)); pbuf = buffer;
//            if(cate > RANK_STAGE)
//                bClear = false;
//
//
//            continue;
//        }
//
//        (*pbuf) = p;
//        pbuf++;
//
//        ii++;
//    } while (bClear);
//
//    log("complate set rank");
//}


bool MC_FileUtils::isFile(unsigned int _filePath)
{
    std::string filePath = MC_FileUtils::getFilePath(_filePath);
    
    return FileUtils::getInstance()->isFileExist(filePath);
}

std::string MC_FileUtils::getFilePath(unsigned int _filePath)
{
    std::string WritablePath = FileUtils::getInstance()->getWritablePath();
    
	std::string filePath = "";
#if	USE_VISUALSTUDIO == 1		
	if (_filePath == USER_PATH)		
		filePath = WritablePath.append("UserInfo.dat");
	else if (_filePath == RANK_PATH)		
		filePath = WritablePath.append("RankInfo.dat");
	else if (_filePath == OPTION_PATH)		
		filePath = WritablePath.append("OptionInfo.dat");
#else	
	
	if (_filePath == USER_PATH)
		filePath = WritablePath + "UserInfo.dat";
	else if (_filePath == RANK_PATH)
		filePath = WritablePath + "RankInfo.dat";
	else if (_filePath == OPTION_PATH)
		filePath = WritablePath + "OptionInfo.dat";
#endif


    
    return filePath;
}


void MC_FileUtils::makeOptionFile()
{
    std::string filePath = MC_FileUtils::getFilePath(OPTION_PATH);
    
    //C File I/O
    log("Save File Full-Path: %s", filePath.c_str());
    
    
    char Title[]    = GAME_TITLE;
    char Version[]  = GAME_VERSION;
    char item1[]    = "Vibration_";
    char item2[]    = "Music_";
    char item3[]    = "Sound_";
    char newLine[]  = "$";
    char value[]    = "1";
    
    int cnt = strlen(Title);
    FILE* fp = fopen(filePath.c_str(), "wt");
    fwrite(Title, sizeof(char)    ,strlen(Title)     ,fp);   //title
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(Version, sizeof(char)  ,strlen(Version)   ,fp);  //version
    //cnt = strlen(Version);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(item1,   sizeof(char)  ,strlen(item1)     ,fp);  //Vibration
    fwrite(&value,   sizeof(char)  ,strlen(value)     ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(item2,   sizeof(char)  ,strlen(item2)     ,fp);  //Music
    fwrite(&value,   sizeof(char)  ,strlen(value)     ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(item3,   sizeof(char)  ,strlen(item3)     ,fp);  //Sound
    fwrite(&value,   sizeof(char)  ,strlen(value)     ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    //    cnt = sizeof(item4);
    
    fclose(fp);
}


bool MC_FileUtils::writeOptionFile()
{
    
    std::string filePath = MC_FileUtils::getFilePath(OPTION_PATH);
    
    //C File I/O
    log("Save File Full-Path: %s", filePath.c_str());
    
    
    char Title[]    = GAME_TITLE;
    char Version[]  = GAME_VERSION;
    char item1[]    = "Vibration_";
    char item2[]    = "Music_";
    char item3[]    = "Sound_";
    char newLine[]  = "$";
    char value[256];
    const char *name = NULL;
    
    MC_GameOption* pGameOption = MC_GameOption::GetInstance();
    
    int cnt = strlen(Title);
    FILE* fp = fopen(filePath.c_str(), "wt");
    fwrite(Title, sizeof(char)    ,strlen(Title)     ,fp);   //title
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    fwrite(Version, sizeof(char)  ,strlen(Version)   ,fp);  //version
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    sprintf(value,"%d",(int)pGameOption->getVibration());
    fwrite(item1,   sizeof(char)  ,strlen(item1)     ,fp);  //Vibration
    fwrite(&value,  sizeof(char)  ,strlen(value)     ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    sprintf(value,"%d",(int)pGameOption->getMusic());
    fwrite(item2,   sizeof(char)  ,strlen(item2)     ,fp);  //Music
    fwrite(&value,  sizeof(char)  ,strlen(value)     ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    sprintf(value,"%d",(int)pGameOption->getSound());
    fwrite(item3,   sizeof(char)  ,strlen(item3)     ,fp);  //Sound
    fwrite(&value,  sizeof(char)  ,strlen(value)     ,fp);
    fwrite(newLine, sizeof(char)  ,strlen(newLine)   ,fp);
    
    
    fclose(fp);
    
    return true;
}

bool MC_FileUtils::readOptionFile()
{
    if(!MC_FileUtils::isFile(USER_PATH)) return false;
    
    std::string filePath = MC_FileUtils::getFilePath(OPTION_PATH);
    
    char Title[50]      = "\0";
    char Version[50]    = "\0";
	char pVibration[50] = "\0";
	char pSound[50]		= "\0";
	char pMusic[50]		= "\0";
    //char *pVibration     = new char;
    //char *pSound         = new char;
    //char *pMusic         = new char;
	
    
    FILE* fp = fopen(filePath.c_str(), "r");
    
    fgets(Title,    sizeof(GAME_TITLE), fp);
    if( strncmp(Title, GAME_TITLE, sizeof(GAME_TITLE)) ) return false;
    
    fseek(fp, strlen("$"), SEEK_CUR);
    fgets(Version,  sizeof(GAME_VERSION), fp);
    
    getStringFromFile(GAME_OP_VIBRATION, fp, pVibration);
	//delete pVibration;
    getStringFromFile(GAME_OP_MUSIC, fp, pSound);
    getStringFromFile(GAME_OP_SOUND, fp, pMusic);
    
    MC_GameOption* pGameOption = MC_GameOption::GetInstance();
    
    pGameOption->setVibration(atoi(pVibration));
    pGameOption->setMusic(atoi(pSound));
    pGameOption->setSound(atoi(pMusic));

    
	//delete pVibration; 
	//delete pSound; 
	//delete pMusic;
    
    fclose(fp);
    return true;
}
