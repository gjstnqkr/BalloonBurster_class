#pragma once


//---------------------------------------------------------------
//define
//---------------------------------------------------------------

//AdMob
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    #define USE_ADMOB 0
#else
	#define USE_ADMOB 0
#endif
#define USE_VISUALSTUDIO 1

//Sound
#if USE_VISUALSTUDIO == 1
#define SOUND_PATH_ARRSHOTING       "music/smw_kick1.wav"
#define MUSIC_PATH_MAIN             "music/JingleBells1.mp3"
#else
#define SOUND_PATH_ARRSHOTING       "smw_kick1.wav"
#define MUSIC_PATH_MAIN             "JingleBells1.mp3"
#endif

//
#define GAME_TITLE      "BalloonBurster_Counterattack"
#define GAME_VERSION    "BalloonBurster ver 1.0"
#define GAME_USERNAME   "UserName"
#define GAME_GAMECASH   "GameCash"
#define GAME_HIGHSCORE  "HighScore"
#define GAME_BESTSTAGE  "BestStage"

#define GAME_OP_VIBRATION   "Vibration"
#define GAME_OP_MUSIC       "Music"
#define GAME_OP_SOUND       "Sound"


// -- Layer Z-Order --
#define Z_SCENE             0
#define Z_SCENE_BACKGROUND  1
#define Z_SCENE_BUILD       2
#define Z_SCENE_SPRITE      3
#define Z_SCENE_UI          9
#define Z_SCENE_UI_VALUE    10
#define Z_LAYER             11


#define SPINE2D 1
#define SPINE2D_BallonMaker 1


//MATH
#define PI 3.141592653
#define RadiToAngle(radi) (radi * 180) / PI;
#define AngleToRadian(Ang) Ang * (PI / 180);


//Balloon Burster
#define BASIC_MATERIAL PhysicsMaterial(5.0f, 0.0f, 1.0f)
#define ROCK_MATERIAL PhysicsMaterial(500.0f, 0.0f, 1.0f)
#define FLYING_TIME 10.0f
#define FLYING_DIST 100.0f


#define OUTOFSCREEN Vec2(2000.0f, 2000.0f)


//Score
#define SCORE_BM 200
#define SCORE_ROCK 50
#define SCORE_BALLOON 50


//
#define RIGHT_OFFSET 20


//Screen Rate
#define TARGET_WIDTH        480 // 1920
#define TARGET_HEIGHT       270 // 1080
//static cocos2d::Size mediumResolutionSize = cocos2d::Size(1334, 750);
#define SCREEN_WIDTH        1334
#define SCREEN_HEIGHT        750

#define UI_TOP_START_RATE       0.045
#define UI_BUTTOM_START_RATE    0.05
#define UI_RIGHT_START_RATE     0.045
#define UI_LEFT_START_RATE      0.055
#define UI_W_OFFSET 10
#define UI_H_OFFSET 15

#define ADD_WIDTH       320
#define ADD_HEIGHT      50


#define LIFE_CNT 3
#define UI_ACTION_TIME 0.1f

#define RANK_COUNT 9
#define RANK_ROW_COUNT 5
#define RANK_ROW_GAP 120
#define RANK_COLUMN_GAP 60

//stage

#define STAGE_1st_ONE   1   // One Of 1st Stage ex)1-1
#define STAGE_1st_TWO   2   // '2' of 2-1
#define STAGE_1st_THREE 3   // '3' of 3-1
#define STAGE_1st_FOUR  4   // '4' of 4-1
#define STAGE_1st_MAX STAGE_1st_TWO

// Infinite Mode
#define STAGE_Infinite_01   11   
#define STAGE_Infinite_max STAGE_Infinite_01   

#define STAGE_BOUNUS_BALOON 5


//animation

#define ANIMATION_1_START   11
#define ANIMATION_1_END     10
#define ANIMATION_2_END     20
#define ANIMATION_3_END     30
#define ANIMATION_4_START   41
#define ANIMATION_4_End     40
#define ANIMATION_START     ANIMATION_1_START

#define NOTHING 0.0f

//ActionTexture
#define SMALL_SIZE 0.8f
#define NORMAL_SIZE 1.2f


//---------------------------------------------------------------
//enum
//---------------------------------------------------------------

enum emLayerTag
{
	TagPause,
	TagPause1,
	TagPause2,
};

enum  emNameTag
{
	NONE,
	ARROW,
	BALLOON,
	BALLOONMAKER,
	ARCHER,			//
	TERRAIN,		//지형
	PROJECTTILE,	//발사체
};

enum emMovingType
{
	BM_Furthermost,
	BM_Far,
	BM_NormalDist,
	BM_Near,
	BM_Nearest,
	BM_Random,
	BM_NoFlying,
};

enum emMakerType
{
	RED,
	WHITE,
	BLACK,
	GREEN,
	BOSS_GOBLIN,
	TotalCNT
};

enum emMakerState
{
	BM_Walking,
	BM_FlyingUp,
	BM_FlyingDown,
	BM_Falling,
	BM_Died,
	BM_Achieved_Up,
	BM_Achieved_Down,
	BM_Achieved_Attack,
	BM_Achieved_Push,
	BM_None
};

enum emArrowState
{
	Arr_Preparing,		//arrow on Not Shooting, Falling
	Arr_ShootWait,		//Ready To Shoot arrow
	Arr_Shooting,		//shooting Arrow
	Arr_Falling,		//떨어지는중
	Arr_Hit,			//떨어지는중	
};

enum emArcherState
{
	Arc_Idle,
	Arc_Shooting,
	Arc_Runing,
	Arc_Dying,
};

enum emStageType
{
	SType_FlyingUp,
	SType_FlyingDown,
	SType_Bonus,
};

enum emStageState
{
	SState_Ready,
	SState_Fighting,
	SState_GameOver,
	SState_UserDie,
	SState_Clear,
	SState_End,
	SState_None,
};

enum emRockState
{
	Rock_Standby,
	Rock_Throwing,
	Rock_explode,
};

enum emBalloonState
{
	Balloon_Up,
	Balloon_Down,
	Balloon_Free,
	Balloon_none,
};


enum emSceneState
{
	Scene_Init,
	Scene_Play,
	Scene_Stop,
	Scene_Pause,
	Scene_None
};

enum emLayerState
{
	Layer_Init,
	Layer_Play,
	Layer_Stop,
	Layer_Pause,
	Layer_None
};

enum emUiPosition
{
	emLeft,
	emRight,
	emTop,
	emBottom
};

enum emBB_State
{
	emLogScene,
	emMainScene,
	emGameOn,
	emGameOff
};


enum emFilePath
{
	USER_PATH,
	RANK_PATH,
	OPTION_PATH,
};

enum emRankCate
{
	RANK_NO,
	RANK_NAME,
	RANK_SCORE,
	RANK_STAGE,
};

enum emUserOption
{
	INGAME,
	MAINPAGE
};

enum emOption
{
	VIBRATION,
	MUSIC,
	SOUND,
};

enum emActionTextureType
{
	AT_Number,
	AT_Image
};
