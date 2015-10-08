#ifndef __CCDxLib_SCENE_H__
#define __CCDxLib_SCENE_H__





#include "cocos2d.h"
#include "AppDelegate.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
using namespace cocos2d;

//Your code starts here.
void CCDxStart();
//This is your game loop.
void CCDxLoop(float deltaTime);


#define DX_PLAYTYPE_LOOP (3)
#define DX_PLAYTYPE_BACK (1)
//The true value is (0)
#define DX_PLAYTYPE_NORMAL DX_PLAYTYPE_BACK
const int DX_SOUNDDATATYPE_MEMNOPRESS = 0;//SE
#define DX_SOUNDDATATYPE_MEMPRESS 2  
//The true value is (3)
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

#ifndef SEEK_SET
#define SEEK_SET 0
#endif
#ifndef SEEK_CUR
#define SEEK_CUR 1
#endif
#ifndef SEEK_END
#define SEEK_END 2
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
	struct DxTouch{
		int X = 0,Y = 0,ID = 0;
		DxTouch(int x,int y,int id){
			X = x; Y = y; ID = id;
		}
	};
	class TextTexture2DCache : public Texture2D{
	public:
		TextTexture2DCache() : Texture2D(){};
		TextTexture2DCache(const TextTexture2DCache &){};// TextTexture2DCache();};
		static const int textTextureCacheSize = 255;
		int FontSize = 0;
		std::string FontName = "";
		bool UsedBeforeScreenFlip = false;
		inline bool isSame(//std::string _String,
			int _FontSize, std::string _FontName){
			return FontSize == _FontSize && FontName == _FontName;
		}
		inline bool isEmpty(){ return FontSize == 0; }
		bool initWithString(const char *text, const FontDefinition& textDefinition){
			FontSize = textDefinition._fontSize;
			FontName = textDefinition._fontName;
			return Texture2D::initWithString(text, textDefinition);
		}
	};
	class ImagePOOL : public Image {
	public://For DISARROW 
		ImagePOOL() :Image(){};
		ImagePOOL(const ImagePOOL &){};
		ImagePOOL &operator =(const ImagePOOL &){ return *this; };
	};
	struct AssetFILE72{
		unsigned char * p;
		unsigned char * data;
		unsigned char * startpos;
		unsigned char * endpos;
		ssize_t filesize;
		AssetFILE72(){
			AssetFILE72(nullptr, 0);
		}
		AssetFILE72(unsigned char * _data, ssize_t _filesize){
			startpos = data = p = _data;
			filesize = _filesize;
			endpos = startpos + filesize;
		}
		bool isValid(){
			return data != nullptr
				&&  startpos <= p && p <= endpos;
		};
	};
	/*
	class TextCache : public Label{
	public:
	TextCache(TextHAlignment hAlignment = TextHAlignment::LEFT,
	TextVAlignment vAlignment = TextVAlignment::TOP)
	: Label(hAlignment, vAlignment){};
	TextCache(const TextCache &){};
	static const int CacheSize = 255;
	bool UsedBeforeScreenFlip = false;
	bool Empty = true;
	inline bool isSame(	int _FontSize, std::string _FontName){
	return _systemFontSize == _FontSize && this->getSystemFontName() == _FontName;
	}
	static TextCache* createWithSystemFont(const std::string& text, const std::string& font, float fontSize){
	auto ret = new (std::nothrow) TextCache(TextHAlignment::LEFT, TextVAlignment::TOP);
	if (ret){
	ret->setSystemFontName(font);
	ret->setSystemFontSize(fontSize);
	ret->setDimensions(Size::ZERO.width, Size::ZERO.height);
	ret->setString(text);
	ret->autorelease();
	ret->retain();
	return ret;
	}
	delete ret;
	return nullptr;
	}
	};
	std::unordered_multimap <std::string, TextCache *>TextCaches;
	*/
	//For Experiment
	//void VISIT(Sprite* image);
	//std::vector<TrianglesCommand> TrianglesCommands;
	//TrianglesCommand _trianglesCommand;
	//TrianglesCommand trian[1000];
	//int trianIndex = 0;
	//Label* label2;

public:
	//Should fclose or release?
	~CCDxLib(){
		for (auto& ds : DxSprites)free(ds) ;
		DxSprites.clear();
		for (auto& ds : textDxSprites)free(ds);
		textDxSprites.clear();
		dxSpriteDirector.initQuadCommands();
	}
	static Scene* createScene(AppDelegate * app);
	virtual bool init();
	void update(float delta);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
	void onMouseDown(Event* evt);
	void onMouseUp(Event* evt);
	void onMouseMove(Event* evt);
	void onMouseScroll(Event* evt);
	void onTouchesBegan    (const std::vector<Touch*>& touches, Event *pEvent);
	void onTouchesMoved    (const std::vector<Touch*>& touches, Event *pEvent);
	void onTouchesEnded    (const std::vector<Touch*>& touches, Event *pEvent);
	void onTouchesCancelled(const std::vector<Touch*>& touches, Event *pEvent);
	void onAcceleration(cocos2d::Acceleration *acc, cocos2d::Event *event);
	void EmulateButtonEnterCallback(int KeyCode);
	void EmulateButtonExitCallback(int KeyCode);
	CREATE_FUNC(CCDxLib);

private:
	
	inline float DxY(int y){return visibleSize.height - y - DrawAreaLeftBottom.y;}
	inline float DxX(int x){ return x - DrawAreaLeftBottom.x; }
	inline Vec2 DxVec2(int x, int y){ return Vec2(DxX(x), DxY(y)); }
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
	int GetGraphSizeX(int GrHandle){
		int w, h;
		GetGraphSize(GrHandle,&w,&h);
		return w;
	}
	int GetGraphSizeY(int GrHandle){
		int w, h;
		GetGraphSize(GrHandle, &w, &h);
		return h;
	}

	class DxSpriteDirector {
	private:
		std::vector<QuadCommand*> _quadCommands;
		std::vector<V3F_C4B_T2F_Quad*> _quads;
		BlendFunc _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
		GLProgramState* _glProgramState = GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR_NO_MVP);
		Director* _director;
		int _quadindex = 0;
	public:
		DxSpriteDirector(){
			_director = Director::getInstance();
		};

		void initQuadCommands(){		
			_quadindex = 0;

			//遅い原因はpush_backのところ
			/*
			for (auto qc : _quadCommands)free(qc);
			_quadCommands.clear();
			for (auto q : _quads)free(q);
			_quads.clear();			
			*/
		}

		void DxDraw(GLuint TextureName,Mat4 transform, V3F_C4B_T2F_Quad quad){
			
			auto renderer = _director->getRenderer();
			auto& parentTransform = _director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
			transform = transform * parentTransform;

			QuadCommand* quadCmd ;
			V3F_C4B_T2F_Quad* _quad ;

			if (_quadindex >= _quads.size()){
				quadCmd = new QuadCommand;
				_quad = new V3F_C4B_T2F_Quad(quad);
				_quadCommands.push_back(quadCmd);
				_quads.push_back(_quad);
			}else{
				quadCmd = _quadCommands[_quadindex];
				_quad = _quads[_quadindex];
				_quad->bl = quad.bl;
				_quad->br = quad.br;
				_quad->tl = quad.tl;
				_quad->tr = quad.tr;
			}
			_quadindex++;
			quadCmd->init(0, TextureName, _glProgramState, _blendFunc, _quad, 1, transform);
			renderer->addCommand(quadCmd);
			
		}
	};
	DxSpriteDirector dxSpriteDirector;

	class DxSprite : public Sprite {
	public:
		V3F_C4B_T2F_Quad GetDxQuad(){ return _quad; }

		std::string FileName;
		bool isDivedGraph = false;
		Rect DivedRect;

		DxSprite* DivedBaseDxSprite;
		DxSprite() :Sprite(){};
		DxSprite(const DxSprite &){};
		DxSprite &operator =(const DxSprite &){ return *this; };
	};

	
	std::vector<DxSprite*> DxSprites;
	std::vector<DxSprite*> textDxSprites;
	int txtsppindex = 0;
	TextTexture2DCache textTextureCacheVec[TextTexture2DCache::textTextureCacheSize];
	std::unordered_multimap <std::string, TextTexture2DCache *> textTextureCache;	


	void InitMembers();
	DxSprite* CheckGetSprite(int GrHandle);
	bool CheckMusicHandle(int index);
	int KeyMap(cocos2d::EventKeyboard::KeyCode KeyCode);
	int KeyReverseMap(int Key);
	std::string KeyName(cocos2d::EventKeyboard::KeyCode KeyCode);
	std::vector<ImagePOOL> SoftImageHandles;
	std::vector <MusicHandle> MusicHandles;
	std::string CurrentPlayedBGMName;
	std::list <DxTouch> DxTouches;
	AssetFILE72 dummyFILE72;
	std::vector<AssetFILE72> fpvec;
	Size visibleSize;
	Size DrawAreaSize;
	Vec2 DrawAreaLeftBottom;
	Vec2 origin;
	DrawNode* drawNode;
	Label* text;
	int SoundDataType = DX_SOUNDDATATYPE_MEMNOPRESS ;
	SimpleAudioEngine* simpleAudioEngine;
	TextureCache* textureCache;
	RenderTexture* rendertexture;
	bool RenderEnded = false;
	bool RenderBegan = false;
	bool KeyPushed[100];
	bool MousePushed[8];
	int MouseX = 0, MouseY = 0;
	int SetBackGroundColorR = 0, SetBackGroundColorB = 0, SetBackGroundColorG = 0;
	int LastTouchX = 0, LastTouchY = 0;
	float ScrollVal = 0;
	float CurrentScrollVal = 0;
	bool EmulateTouchByMouseFunctions = false;
	int EmulateButtonNum = 0;
	bool EmulateKeyBordArrowsByAccelerometer = false;
	int EmulateKeyBoardBySingleTouchKeyCode = 0;
	bool EmulateKeyBoardArrowsBySwipe = false;
	std::unordered_map<int, ui::Button*> ImaginaryButton;
	int EmulateKeyboardBySingleTouchKeyCode = 0;
	bool EmulateKeyboardBySingleTouchEnabled = false;

#define USE_EMULATE_BUTTON_DEFAULT_POSITION -7272

public:
	int SetBackgroundColor(int Red, int Green, int Blue);
	int GetScreenState(int *SizeX, int *SizeY, int *ColorBitDepth);
	int SetDrawArea(int x1, int y1, int x2, int y2);
	int LoadGraphScreen(int x, int y, char *GraphName, int TransFlag = TRUE);
	int LoadDivGraph(char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf);
	int DerivationGraph(int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle);

	int LoadGraph(char *FileName);
	int DeleteGraph(int GrHandle);
	int InitGraph();
	int ScreenFlip();
	int ClearDrawScreen();
	int GetGraphSize(int GrHandle, int *SizeXBuf, int *SizeYBuf);
	int DrawGraph(int x, int y, int GrHandle, int TransFlag = TRUE);
	int DrawTurnGraph(int x, int y, int GrHandle, int TransFlag = TRUE);
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
	int DrawString(int x, int y, const char *String, unsigned int Color);
	int SetFontSize(int FontSize); 
	//int ChangeFont(char *FontName);
	
	int SetCreateSoundDataType(int SoundDataType);
	int LoadSoundMem(char *FileName);
	int PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag = TRUE);
	int CheckSoundMem(int SoundHandle);
	int StopSoundMem(int SoundHandle);
	int DeleteSoundMem(int SoundHandle);
	int InitSoundMem();
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
	int GetMouseInput();
	int GetMouseWheelRotVol();
	int GetTouchInputNum();
	int GetTouchInput(int InputNo, int *PositionX, int *PositionY, int *ID, int *Device);
	void EMULATE_TOUCH_BY_MOUSEFUNCTIONS(bool Emulate = true);
	void EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(int KeyCode, char* ButtonFileName, int PositionX = USE_EMULATE_BUTTON_DEFAULT_POSITION, int PositionY = USE_EMULATE_BUTTON_DEFAULT_POSITION,bool WriteKeyName = false);
	void EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(int KeyCode);
	void EMULATE_KEYBOARD_ARROWS_BY_ACCELEROMETER(bool Emulate = true);
	
	void EMULATE_KEYBOARD_ARROWS_BY_SWIPE(bool Emulate = true);
	void EMULATE_KEYBOARD_BY_SINGLETOUCH(int KeyCode,bool Emulate = true);
	void SET_VISIBLE_IMAGINARY_BUTTON(int KeyCode,bool Visible);
	
	int LoadSoftImage(char *FileName);
	int GetSoftImageSize(int SIHandle, int *Width, int *Height);
	int GetPixelSoftImage(int SIHandle, int x, int y, int *r, int *g, int *b, int *a);
	int DrawSoftImage(int x, int y, int SIHandle);
	int DrawPixelSoftImage(int SIHandle, int x, int y, int r, int g, int b, int a);
	int MakeARGB8ColorSoftImage(int SizeX, int SizeY);
	int MakeXRGB8ColorSoftImage(int SizeX, int SizeY);
	int FillSoftImage(int SIHandle, int r, int g, int b, int a);
	int DeleteSoftImage(int SIHandle);
	int InitSoftImage();

	FILE* UserDatafopen(const char* Filename, const char* Mode);
	inline std::string getUserDataPath(){ return FileUtils::getInstance()->getWritablePath(); }
	//int CopyFileToUserDataPath(const char * AssetFilename);

	AssetFILE72& __getFilePointer(int FileHandle);
	int FileRead_open(char *FilePath, int ASync = FALSE);
	int FileRead_close(int FileHandle);
	int FileRead_seek(int FileHandle, long long Offset, int Origin);
	int FileRead_read(void *Buffer, int ReadSize, int FileHandle);
	int FileRead_eof(int FileHandle);
	int FileRead_gets(char *Buffer, int Num, int FileHandle);
	int FileRead_getc(int FileHandle);
	long long FileRead_size(char *FilePath);
	long long FileRead_tell(int FileHandle);
	
};


CCDxLib* Get_m_dxlib();

#define MKCCDxFN(T,FN,PARAMS,ARGS)inline T FN PARAMS { return Get_m_dxlib()-> FN ARGS;}
MKCCDxFN(int, SetBackgroundColor, (int Red, int Green, int Blue), (Red, Green, Blue))
MKCCDxFN(int, GetScreenState, (int *SizeX, int *SizeY, int *ColorBitDepth), (SizeX,SizeY,ColorBitDepth))
MKCCDxFN(int, SetDrawArea, (int x1, int y1, int x2, int y2), (x1,y1,x2,y2))
MKCCDxFN(int, LoadGraphScreen, (int x, int y, char *GraphName, int TransFlag = TRUE), (x, y, GraphName, TransFlag))
MKCCDxFN(int, LoadGraph, (char *FileName), (FileName))
MKCCDxFN(int, LoadDivGraph, (char *FileName, int AllNum, int XNum, int YNum, int XSize, int YSize, int *HandleBuf), (FileName, AllNum, XNum, YNum, XSize, YSize, HandleBuf))
MKCCDxFN(int, DerivationGraph, (int SrcX, int SrcY, int Width, int Height, int SrcGraphHandle), (SrcX, SrcY, Width, Height, SrcGraphHandle))

MKCCDxFN(int, DeleteGraph, (int GrHandle), (GrHandle))
MKCCDxFN(int, InitGraph, (), ())
MKCCDxFN(int, ClearDrawScreen, (), ())
MKCCDxFN(int, GetGraphSize, (int GrHandle, int *SizeXBuf, int *SizeYBuf), (GrHandle, SizeXBuf, SizeYBuf))
MKCCDxFN(int, DrawGraph, (int x, int y, int GrHandle, int TransFlag = TRUE), (x, y, GrHandle, TransFlag))
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
MKCCDxFN(int, DrawString, (int x, int y,const char *String, unsigned int Color), (x, y, String, Color))
int DrawFormatString(int x, int y, unsigned int Color, const char *FormatString, ...);
MKCCDxFN(int, SetFontSize, (int FontSize), (FontSize))
//MKCCDxFN(int, ChangeFont, (char *FontName), (FontName))
MKCCDxFN(int, SetCreateSoundDataType, (int SoundDataType), (SoundDataType))
MKCCDxFN(int, LoadSoundMem, (char *FileName), (FileName))
MKCCDxFN(int, PlaySoundMem, (int SoundHandle, int PlayType, int TopPositionFlag = TRUE), (SoundHandle, PlayType, TopPositionFlag))
MKCCDxFN(int, CheckSoundMem, (int SoundHandle), (SoundHandle))
MKCCDxFN(int, StopSoundMem, (int SoundHandle), (SoundHandle))
MKCCDxFN(int, DeleteSoundMem, (int SoundHandle), (SoundHandle))
MKCCDxFN(int, InitSoundMem,(),())


MKCCDxFN(int, GetNowCount, (), ())
MKCCDxFN(long long, GetNowHiPerformanceCount, (), ())
MKCCDxFN(int, GetDateTime, (DATEDATA *DateBuf), (DateBuf))
MKCCDxFN(int, GetRand, (int RandMax), (RandMax))
MKCCDxFN(int, SRand, (int Seed), (Seed))
MKCCDxFN(int, CheckHitKey, (int KeyCode), (KeyCode))
MKCCDxFN(int, CheckHitKeyAll, (), ())
MKCCDxFN(int, GetHitKeyStateAll, (char *KeyStateBuf), (KeyStateBuf))
MKCCDxFN(int, SetMouseDispFlag, (int DispFlag), (DispFlag))
MKCCDxFN(int, GetMousePoint, (int *XBuf, int *YBuf), (XBuf, YBuf))
MKCCDxFN(int, GetMouseInput, (), ())
MKCCDxFN(int, GetMouseWheelRotVol, (), ())
MKCCDxFN(int, GetTouchInputNum, (), ())
MKCCDxFN(int, GetTouchInput, (int InputNo, int *PositionX, int *PositionY, int *ID, int *Device), (InputNo, PositionX, PositionY, ID, Device))
MKCCDxFN(void, EMULATE_TOUCH_BY_MOUSEFUNCTIONS, (bool Emulate = true), (Emulate))
MKCCDxFN(void, EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON, (int KeyCode, char* ButtonFileName, int PositionX = USE_EMULATE_BUTTON_DEFAULT_POSITION, int PositionY = USE_EMULATE_BUTTON_DEFAULT_POSITION, bool WriteKeyName = false), (KeyCode, ButtonFileName, PositionX, PositionY, WriteKeyName))
MKCCDxFN(void, EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON, (int KeyCode), (KeyCode))
MKCCDxFN(void, EMULATE_KEYBOARD_ARROWS_BY_SWIPE, (bool Emulate = true), (Emulate))
MKCCDxFN(void, EMULATE_KEYBOARD_BY_SINGLETOUCH, (int KeyCode, bool Emulate = true), (KeyCode, Emulate ))
MKCCDxFN(void, SET_VISIBLE_IMAGINARY_BUTTON, (int KeyCode, bool Visible),(KeyCode, Visible))

MKCCDxFN(int, LoadSoftImage, (char *FileName), (FileName))
MKCCDxFN(int, GetSoftImageSize, (int SIHandle, int *Width, int *Height), (SIHandle, Width, Height))
MKCCDxFN(int, GetPixelSoftImage, (int SIHandle, int x, int y, int *r, int *g, int *b, int *a), (SIHandle, x, y, r, g, b, a))
MKCCDxFN(int, DrawSoftImage, (int x, int y, int SIHandle), (x, y, SIHandle))
MKCCDxFN(int, DrawPixelSoftImage, (int SIHandle, int x, int y, int r, int g, int b, int a), (SIHandle, x, y, r, g, b, a))
MKCCDxFN(int, MakeARGB8ColorSoftImage, (int SizeX, int SizeY), (SizeX, SizeY))
MKCCDxFN(int, MakeXRGB8ColorSoftImage, (int SizeX, int SizeY), (SizeX, SizeY))
MKCCDxFN(int, FillSoftImage, (int SIHandle, int r, int g, int b, int a), (SIHandle, r, g, b, a))
MKCCDxFN(int, DeleteSoftImage, (int SIHandle), (SIHandle))
MKCCDxFN(int, InitSoftImage, (), ())
MKCCDxFN(FILE*, UserDatafopen, (const char* Filename, const char* Mode), (Filename, Mode))
MKCCDxFN(std::string, getUserDataPath, (), ())
MKCCDxFN(int, FileRead_open, (char *FilePath, int ASync = FALSE), (FilePath, ASync))
MKCCDxFN(int, FileRead_close, (int FileHandle), (FileHandle))
MKCCDxFN(int, FileRead_seek, (int FileHandle, long long Offset, int Origin), (FileHandle, Offset, Origin))
MKCCDxFN(int, FileRead_read, (void *Buffer, int ReadSize, int FileHandle), (Buffer, ReadSize, FileHandle))
MKCCDxFN(int, FileRead_eof, (int FileHandle), (FileHandle))
MKCCDxFN(int, FileRead_gets, (char *Buffer, int Num, int FileHandle), (Buffer, Num, FileHandle))
MKCCDxFN(int, FileRead_getc, (int FileHandle), (FileHandle))
MKCCDxFN(long long, FileRead_size, (char *FilePath), (FilePath))
MKCCDxFN(long long, FileRead_tell, (int FileHandle), (FileHandle))
int FileRead_scanf(int FileHandle, char *Format, ...);
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
#define KEY_INPUT_MINUS 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_MINUS// －キー
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
#define KEY_INPUT_SUBTRACT 	    (int)cocos2d::EventKeyboard::KeyCode::KEY_KP_MINUS// テンキー－キー
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
