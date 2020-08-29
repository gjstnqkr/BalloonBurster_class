//
//  MC_FileUtils.hpp
//  BalloonBurster-mobile
//
//  Created by 박헌수 on 06/05/2019.
//

#ifndef MC_FileUtils_hpp
#define MC_FileUtils_hpp

#include <stdio.h>
#include "cocos2d.h"

class UserInformation;
class MC_FileUtils : public cocos2d::Node
{
public:
    //UserInformation*        m_pUserInfo;
    //static MC_FileUtils*    m_pFileUtils;
    
public:
    MC_FileUtils();
    ~MC_FileUtils();
    
    static MC_FileUtils * create(void);
//    static MC_FileUtils * getInstance(){
//        if(m_pFileUtils == nullptr){
//            m_pFileUtils = create();
//        }
//        return m_pFileUtils;
//    }
    
public:
    bool init();
    static bool init_UserInfo();
    static bool init_OptionInfo();
    static bool isFile(unsigned int _filePath);
    
    static void makeUserFile();
    static bool readUserFile();
    static bool writeUserFile();
    
//    static void makeRankFile();
//    static bool readRankFile();
//    static bool writeRankFile();
    
    static void makeOptionFile();
    static bool readOptionFile();
    static bool writeOptionFile();
    
    static std::string getFilePath(unsigned int _filePath);
    static void getStringFromFile(std::string word, FILE* fp, void* pOutValue);
    static void getRankInfoFromFile(std::string word, FILE* fp, void* pOutValue);
    static void setRankInfoToUserInfo(void* pInValue, int _rankNum);
    
public:
    UserInformation * Get_Instance(void);
    
};

#endif /* MC_FileUtils_hpp */
