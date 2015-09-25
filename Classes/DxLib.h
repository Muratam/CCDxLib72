#ifndef __CCDxLib_SCENE_H__
#define __CCDxLib_SCENE_H__

#include "cocos2d.h"
#include "AppDelegate.h"

#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace cocos2d;

//Your code starts here.
void CCDxStart();
//This is your game loop.
void CCDxLoop(float deltaTime);


#define DX_PLAYTYPE_LOOP (3)
#define DX_PLAYTYPE_BACK (1)
//本当は(0)だが、コールバックを受け取るのも大変なので同値に
#define DX_PLAYTYPE_NORMAL DX_PLAYTYPE_BACK
const int DX_SOUNDDATATYPE_MEMNOPRESS = 0;//SE
#define DX_SOUNDDATATYPE_MEMPRESS 2  
//(本当は(3)だが、Cocosの都合上SimpleAudioEngineを使うので、同値に)
#define DX_SOUNDDATATYPE_FILE DX_SOUNDDATATYPE_MEMPRESS
#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
const long double __Pi = 3.14159265358979323846264338327950288L;
#ifndef LOBYTE
#define LOBYTE(w) ((unsigned char)(((unsigned long)(w)) & 0xff))
#endif

struct DATEDATA{
	
	int Year = 2000; 	// 年
	int Mon = 2; 	// 月
	int Day = 25; 	// 日
	int Hour = 7; 	// 時間
	int Min = 2; 	// 分
	int Sec = 7; 	// 秒
};


class CCDxLib : public cocos2d::Layer{
private:
	//typedef std::pair<bool, std::string> MusicHandle;
	//bool MusicHandle::* mpisBGM = &MusicHandle::first;
	//std::string MusicHandle::* mpFileName = &MusicHandle::second;
	struct MusicHandle{
		bool isBGM ;
		std::string FileName;
		unsigned int SE_ID = 0;
		bool SE_OncePlayed = false;
		MusicHandle(bool _isBGM,const char* _FileName){
			isBGM = _isBGM;
			FileName = _FileName;
		}
	};

public:
	static Scene* createScene(AppDelegate * app);
	virtual bool init();
	void update(float delta);
	void menuCloseCallback(Ref* pSender);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onMouseDown(Event* evt);
	void onMouseUp(Event* evt);
	void onMouseMove(Event* evt);
	void onMouseScroll(Event* evt);

	CREATE_FUNC(CCDxLib);

private:
	
	inline float DxY(int y){ return visibleSize.height - y; }
	inline Vec2 DxVec2(int x,int y){return Vec2(x,DxY(y));}
	inline float FromRadian(float Radian){
		return Radian * 180.0 / __Pi;
	}
	inline Color4F FromColor3B(int r,int g,int b){
		return Color4F((float)(r) / 255.0, (float)(g) / 255.0, (float)(b) / 255.0,1.0);
	}
	inline Color3B FromUnsignedIntColor(unsigned int color){
		int r = (LOBYTE(color));
		int g = (LOBYTE(((unsigned short)(color)) >> 8));
		int b = (LOBYTE((color) >> 16));
		return Color3B(r,g,b);
	}
	inline Color4F DxColor(int icolor){
		Color3B color = FromUnsignedIntColor(icolor);
		return FromColor3B(color.r,color.g,color.b);
	}
	void InitMembers();
	Sprite* CheckGetSprite(int GrHandle);
	bool CheckMusicHandle(int index);
	int KeyMap(cocos2d::EventKeyboard::KeyCode KeyCode);
	int KeyReverseMap(int Key);
	std::vector<std::string> GraphicHandles;
	std::vector <MusicHandle> MusicHandles;
	std::string CurrentPlayedBGMName;
	Size visibleSize;
	Vec2 origin;
	DrawNode* drawNode;
	int FontSize = 32;
	std::string FontName = "arial.ttf";
	int SoundDataType = DX_SOUNDDATATYPE_MEMNOPRESS ;
	SimpleAudioEngine* simpleAudioEngine;
	TextureCache* textureCache;
	bool KeyPushed[100];
	bool MousePushed[8];
	int MouseX = 0, MouseY = 0;
	float ScrollVal = 0;
	float CurrentScrollVal = 0;
public:
	int LoadGraphScreen(int x, int y, char *GraphName, int TransFlag = TRUE);
	int LoadGraph(char *FileName);
	int DrawGraph(int x, int y, int GrHandle, int TransFlag = TRUE);
	int ClearDrawScreen();
	int DrawTurnGraph(int x, int y,	int GrHandle, int TransFlag = TRUE);
	int DrawExtendGraph(int x1, int y1, int x2, int y2,	int GrHandle, int TransFlag = TRUE);
	int DrawRotaGraph(int x, int y, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);
	int DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);
	int DrawRotaGraph3(int x, int y,int cx, int cy,double ExtRateX, double ExtRateY,double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE);
	int DrawRectGraph(int DestX, int DestY,	int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag= TRUE, int TurnFlag = FALSE);
	unsigned int GetColor(int Red, int Green, int Blue);
	int DrawLine(int x1, int y1, int x2, int y2, unsigned int Color);
	int DrawBox(int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag);
	int DrawCircle(int x, int y, int r, unsigned int Color, int FillFlag,unsigned int segment = 72);
	int DrawOval(int x, int y, int rx, int ry, unsigned int Color, int FillFlag, unsigned int segment = 72);
	int DrawTriangle(int x1, int y1, int x2, int y2,int x3, int y3, unsigned int Color, int FillFlag);
	int DrawPixel(int x, int y, unsigned int Color);
	int DrawString(int x, int y, char *String, unsigned int Color);
	int SetFontSize(int FontSize); 
	int ChangeFont(char *FontName);
	
	///////////////////////////////////YET//////////////////////////
	int SetCreateSoundDataType(int SoundDataType);
	int LoadSoundMem(char *FileName);
	int PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag = TRUE);
	int CheckSoundMem(int SoundHandle);
	int StopSoundMem(int SoundHandle);
	int DeleteSoundMem(int SoundHandle);
	int InitSoundMem();
	int ChangePanSoundMem(int PanPal, int SoundHandle);//( -255 〜 255 ＋は左の音量を下げ、−は右の音量を下げる)
	int ChangeVolumeSoundMem(int VolumePal, int SoundHandle);//[0,256)
	int ChangeNextPlayPanSoundMem(int PanPal, int SoundHandle);
	int ChangeNextPlayVolumeSoundMem(int VolumePal, int SoundHandle);
	int SetFrequencySoundMem(int FrequencyPal, int SoundHandle);
	int SetLoopPosSoundMem(int LoopTime, int SoundHandle);
	int SetLoopSamplePosSoundMem(int LoopSamplePosition, int SoundHandle);
	int SetCurrentPositionSoundMem(int SamplePosition, int SoundHandle);
	int DuplicateSoundMem(int SrcSoundHandle);
	///////////////////////////////////YET//////////////////////////
	int GetNowCount();
	long long GetNowHiPerformanceCount();
	int GetDateTime(DATEDATA *DateBuf);
	int GetRand(int RandMax);
	int SRand(int Seed);
	int CheckHitKey(int KeyCode);
	int CheckHitKeyAll();
	int GetHitKeyStateAll(char *KeyStateBuf);
	
	int SetMouseDispFlag(int DispFlag);
	int GetMousePoint(int *XBuf, int *YBuf);
	int GetMouseInput(void);
	int GetMouseWheelRotVol(void);

	
	//int GetTouchInputNum(void);
	//int GetTouchInput(int InputNo, int *PositionX, int *PositionY, int *ID, int *Device);

	int GetMouseInputLog(int *Button, int *ClickX, int *ClickY, int LogDelete);
	//int LoadDivGraph(char *FileName, int AllNum,int XNum, int YNum,int XSize, int YSize, int *HandleBuf);
};


CCDxLib* Get_m_dxlib();

#define MKCCDxFN(T,FN,PARAMS,ARGS)inline T FN PARAMS { return Get_m_dxlib()-> FN ARGS;}
MKCCDxFN(int, LoadGraphScreen, (int x, int y, char *GraphName, int TransFlag = TRUE), (x, y, GraphName, TransFlag))
MKCCDxFN(int, LoadGraph, (char *FileName), (FileName))
MKCCDxFN(int, DrawGraph, (int x, int y, int GrHandle, int TransFlag = TRUE), (x, y, GrHandle, TransFlag))
MKCCDxFN(int, ClearDrawScreen, (), ())
MKCCDxFN(int, DrawTurnGraph, (int x, int y, int GrHandle, int TransFlag = TRUE), (x, y, GrHandle, TransFlag))
MKCCDxFN(int, DrawExtendGraph, (int x1, int y1, int x2, int y2, int GrHandle, int TransFlag = TRUE), (x1, y1, x2, y2, GrHandle, TransFlag))
MKCCDxFN(int, DrawRotaGraph, (int x, int y, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE), (x, y, ExtRate, Angle, GrHandle, TransFlag, TurnFlag))
MKCCDxFN(int, DrawRotaGraph2, (int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE), (x, y, cx, cy, ExtRate, Angle, GrHandle, TransFlag, TurnFlag))
MKCCDxFN(int, DrawRotaGraph3, (int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag = TRUE, int TurnFlag = FALSE), (x, y, cx, cy, ExtRateX, ExtRateY, Angle, GrHandle, TransFlag, TurnFlag))
MKCCDxFN(int, DrawRectGraph, (int DestX, int DestY, int SrcX, int SrcY, int Width, int Height, int GraphHandle, int TransFlag = TRUE, int TurnFlag = FALSE), (DestX, DestY, SrcX, SrcY, Width, Height, GraphHandle, TransFlag, TurnFlag))
MKCCDxFN(unsigned int, GetColor, (int Red, int Green, int Blue), (Red, Green, Blue))
MKCCDxFN(int, DrawLine, (int x1, int y1, int x2, int y2, unsigned int Color), (x1, y1, x2, y2, Color))
MKCCDxFN(int, DrawBox, (int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag), (x1, y1, x2, y2, Color, FillFlag))
MKCCDxFN(int, DrawCircle, (int x, int y, int r, unsigned int Color, int FillFlag, unsigned int segment = 72), (x, y, r, Color, FillFlag, segment))
MKCCDxFN(int, DrawOval, (int x, int y, int rx, int ry, unsigned int Color, int FillFlag, unsigned int segment = 72), (x, y, rx, ry, Color, FillFlag, segment))
MKCCDxFN(int, DrawTriangle, (int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag), (x1, y1, x2, y2, x3, y3, Color, FillFlag))
MKCCDxFN(int, DrawPixel, (int x, int y, unsigned int Color), (x, y, Color))
MKCCDxFN(int, DrawString, (int x, int y, char *String, unsigned int Color), (x, y, String, Color))
int DrawFormatString(int x, int y, unsigned int Color, char *FormatString, ...);
MKCCDxFN(int, SetFontSize, (int FontSize), (FontSize))
MKCCDxFN(int, ChangeFont, (char *FontName), (FontName))
MKCCDxFN(int ,GetNowCount,(),())
MKCCDxFN(long long ,GetNowHiPerformanceCount,(),())
MKCCDxFN(int, GetDateTime,(DATEDATA *DateBuf),(DateBuf))
MKCCDxFN(int, GetRand,(int RandMax),(RandMax))
MKCCDxFN(int, SRand,(int Seed),(Seed))
MKCCDxFN(int, CheckHitKey,(int KeyCode),(KeyCode))
MKCCDxFN(int, CheckHitKeyAll, (), ())
MKCCDxFN(int, GetHitKeyStateAll,(char *KeyStateBuf),(KeyStateBuf))
MKCCDxFN(int, SetMouseDispFlag, (int DispFlag), (DispFlag))
MKCCDxFN(int, GetMousePoint,(int *XBuf, int *YBuf), (XBuf,YBuf))
MKCCDxFN(int, GetMouseInput, (), ())
MKCCDxFN(int, GetMouseWheelRotVol, (), ())
int DxLib_End();


#define MOUSE_INPUT_LEFT   (1)
#define MOUSE_INPUT_RIGHT  (2)
#define MOUSE_INPUT_MIDDLE (4)
#define MOUSE_INPUT_4	   (8)
#define MOUSE_INPUT_5	   (16)
#define MOUSE_INPUT_6	   (32)
#define MOUSE_INPUT_7 	   (64)
#define MOUSE_INPUT_8	   (128)

#define KEY_INPUT_BACK 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_BACKSPACE// バックスペースキー
#define KEY_INPUT_TAB 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_TAB// タブキー
#define KEY_INPUT_RETURN 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_RETURN// エンターキー
#define KEY_INPUT_LSHIFT 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_LEFT_SHIFT// 左シフトキー
#define KEY_INPUT_RSHIFT 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_SHIFT// 右シフトキー
#define KEY_INPUT_LCONTROL 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_LEFT_CTRL// 左コントロールキー
#define KEY_INPUT_RCONTROL 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_CTRL// 右コントロールキー
#define KEY_INPUT_ESCAPE 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE// エスケープキー
#define KEY_INPUT_SPACE 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_SPACE// スペースキー
#define KEY_INPUT_PGUP 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_PG_UP// ＰａｇｅＵＰキー
#define KEY_INPUT_PGDN 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_PG_DOWN// ＰａｇｅＤｏｗｎキー
#define KEY_INPUT_END 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_END// エンドキー
#define KEY_INPUT_HOME 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_HOME// ホームキー
#define KEY_INPUT_LEFT 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ARROW// 左キー
#define KEY_INPUT_UP 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_UP_ARROW// 上キー
#define KEY_INPUT_RIGHT 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ARROW// 右キー
#define KEY_INPUT_DOWN 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_DOWN_ARROW// 下キー
#define KEY_INPUT_INSERT 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_INSERT// インサートキー
#define KEY_INPUT_DELETE 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_DELETE// デリートキー
#define KEY_INPUT_MINUS 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_MINUS// −キー
#define KEY_INPUT_YEN 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_YEN// ￥キー
#define KEY_INPUT_PREVTRACK     (int)cocos2d::EventKeyboard::KeyCode::KEY_CIRCUMFLEX// ＾キー
#define KEY_INPUT_PERIOD 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_PERIOD// ．キー
#define KEY_INPUT_SLASH 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_SLASH// ／キー
#define KEY_INPUT_LALT 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_LEFT_ALT// 左ＡＬＴキー
#define KEY_INPUT_RALT          (int)cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_ALT// 右ＡＬＴキー
#define KEY_INPUT_SCROLL 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_SCROLL_LOCK// ScrollLockキー
#define KEY_INPUT_SEMICOLON     (int)cocos2d::EventKeyboard::KeyCode::KEY_SEMICOLON// ；キー
#define KEY_INPUT_COLON 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_COLON// ：キー
#define KEY_INPUT_LBRACKET 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_LEFT_BRACKET// ［キー
#define KEY_INPUT_RBRACKET 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_RIGHT_BRACKET// ］キー
#define KEY_INPUT_AT            (int)cocos2d::EventKeyboard::KeyCode::KEY_AT// ＠キー
#define KEY_INPUT_BACKSLASH     (int)cocos2d::EventKeyboard::KeyCode::KEY_BACK_SLASH// ＼キー
#define KEY_INPUT_COMMA 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_COMMA// ，キー
#define KEY_INPUT_CAPSLOCK 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_CAPS_LOCK// CaspLockキー
#define KEY_INPUT_PAUSE 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_PAUSE// PauseBreakキー
#define KEY_INPUT_NUMPAD0 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_0// テンキー０
#define KEY_INPUT_NUMPAD1 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_1// テンキー１
#define KEY_INPUT_NUMPAD2 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_2// テンキー２
#define KEY_INPUT_NUMPAD3 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_3// テンキー３
#define KEY_INPUT_NUMPAD4 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_4// テンキー４
#define KEY_INPUT_NUMPAD5 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_5// テンキー５
#define KEY_INPUT_NUMPAD6 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_6// テンキー６
#define KEY_INPUT_NUMPAD7 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_7// テンキー７
#define KEY_INPUT_NUMPAD8 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_8// テンキー８
#define KEY_INPUT_NUMPAD9 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_9// テンキー９
#define KEY_INPUT_MULTIPLY 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_KP_MULTIPLY// テンキー＊キー
#define KEY_INPUT_ADD           (int)cocos2d::EventKeyboard::KeyCode::KEY_KP_PLUS// テンキー＋キー
#define KEY_INPUT_SUBTRACT 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_KP_MINUS// テンキー−キー
#define KEY_INPUT_DECIMAL       (int)cocos2d::EventKeyboard::KeyCode::KEY_PERIOD// テンキー．キー
#define KEY_INPUT_DIVIDE        (int)cocos2d::EventKeyboard::KeyCode::KEY_KP_DIVIDE// テンキー／キー
#define KEY_INPUT_NUMPADENTER 	(int)cocos2d::EventKeyboard::KeyCode::KEY_KP_ENTER// テンキーのエンターキー
#define KEY_INPUT_F1 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F1// Ｆ１キー
#define KEY_INPUT_F2 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F2// Ｆ２キー
#define KEY_INPUT_F3 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F3// Ｆ３キー
#define KEY_INPUT_F4 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F4// Ｆ４キー
#define KEY_INPUT_F5 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F5// Ｆ５キー
#define KEY_INPUT_F6 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F6// Ｆ６キー
#define KEY_INPUT_F7 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F7// Ｆ７キー
#define KEY_INPUT_F8 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F8// Ｆ８キー
#define KEY_INPUT_F9 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F9// Ｆ９キー
#define KEY_INPUT_F10 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F10// Ｆ１０キー
#define KEY_INPUT_F11 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F11// Ｆ１１キー
#define KEY_INPUT_F12 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F12// Ｆ１２キー
#define KEY_INPUT_A 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_A// Ａキー
#define KEY_INPUT_B 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_B// Ｂキー
#define KEY_INPUT_C 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_C// Ｃキー
#define KEY_INPUT_D 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_D// Ｄキー
#define KEY_INPUT_E 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_E// Ｅキー
#define KEY_INPUT_F 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_F// Ｆキー
#define KEY_INPUT_G 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_G// Ｇキー
#define KEY_INPUT_H 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_H// Ｈキー
#define KEY_INPUT_I 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_I// Ｉキー
#define KEY_INPUT_J 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_J// Ｊキー
#define KEY_INPUT_K 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_K// Ｋキー
#define KEY_INPUT_L 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_L// Ｌキー
#define KEY_INPUT_M 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_M// Ｍキー
#define KEY_INPUT_N 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_N// Ｎキー
#define KEY_INPUT_O 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_O// Ｏキー
#define KEY_INPUT_P 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_P// Ｐキー
#define KEY_INPUT_Q 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_Q// Ｑキー
#define KEY_INPUT_R 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_R// Ｒキー
#define KEY_INPUT_S 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_S// Ｓキー
#define KEY_INPUT_T 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_T// Ｔキー
#define KEY_INPUT_U 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_U// Ｕキー
#define KEY_INPUT_V 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_V// Ｖキー
#define KEY_INPUT_W 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_W// Ｗキー
#define KEY_INPUT_X 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_X// Ｘキー
#define KEY_INPUT_Y 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_Y// Ｙキー
#define KEY_INPUT_Z 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_Z// Ｚキー
#define KEY_INPUT_0 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_0// ０キー
#define KEY_INPUT_1 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_1// １キー
#define KEY_INPUT_2 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_2// ２キー
#define KEY_INPUT_3 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_3// ３キー
#define KEY_INPUT_4 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_4// ４キー
#define KEY_INPUT_5 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_5// ５キー
#define KEY_INPUT_6 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_6// ６キー
#define KEY_INPUT_7 	        (int)cocos2d::EventKeyboard::KeyCode::KEY_7// ７キー
#define KEY_INPUT_8             (int)cocos2d::EventKeyboard::KeyCode::KEY_8// ８キー
#define KEY_INPUT_9             (int)cocos2d::EventKeyboard::KeyCode::KEY_9	// ９キー

#endif // __CCDxLib_SCENE_H__
