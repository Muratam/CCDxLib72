#include "DxLib.h"
#include <stdarg.h>
#include <algorithm>
#include <iostream>
#pragma execution_character_set("utf-8")

//日本語が文字化けする場合はこれを定義して、UTF-8にしてください。
//#pragma execution_character_set("utf-8")
//http://rooms.pokotsun-labs.net/201/%E5%9F%BA%E6%9C%AC%E7%B7%A8/%E6%96%87%E5%AD%97%E5%88%97%E3%81%AE%E6%8F%8F%E7%94%BB

USING_NS_CC;

//インスタンス
CCDxLib *m_dxlib;
AppDelegate * m_AppDelegate;

Scene* CCDxLib::createScene(AppDelegate * app){
    auto scene = Scene::create();
	auto layer = CCDxLib::create();
    scene->addChild(layer);
	m_AppDelegate = app;
    return scene;
}

bool CCDxLib::init(){
    if ( !Layer::init() )return false;
	for (int i = 0; i < 100; i++)KeyPushed[i] = false;
	for (int i = 0; i < 8; i++)MousePushed[i] = false;
    this->visibleSize = Director::getInstance()->getWinSize();
	this->origin = Director::getInstance()->getVisibleOrigin();
	
	m_dxlib = this;
	simpleAudioEngine = SimpleAudioEngine::getInstance();
	textureCache = Director::getInstance()->getTextureCache();
	rendertexture = RenderTexture::create(visibleSize.width, visibleSize.height);
	rendertexture->setPosition(visibleSize.width/2,visibleSize.height/2);
	rendertexture->retain();
	this->addChild(rendertexture);
	rendertexture->beginWithClear((float)SetBackGroundColorR / 255.0, (float)SetBackGroundColorG / 255.0, (float)SetBackGroundColorB / 255.0, 1.0);
	RenderBegan = true;

	auto keylistener = EventListenerKeyboard::create();
	keylistener->onKeyPressed = CC_CALLBACK_2(CCDxLib::onKeyPressed, this);
	keylistener->onKeyReleased = CC_CALLBACK_2(CCDxLib::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keylistener, this);
	
	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(CCDxLib::onMouseDown, this);
	mouseListener->onMouseUp = CC_CALLBACK_1(CCDxLib::onMouseUp, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(CCDxLib::onMouseMove, this);
	mouseListener->onMouseScroll = CC_CALLBACK_1(CCDxLib::onMouseScroll, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener,this);
	
	auto touchlistener = EventListenerTouchAllAtOnce::create();
	touchlistener->onTouchesBegan = CC_CALLBACK_2(CCDxLib::onTouchesBegan, this);
	touchlistener->onTouchesMoved = CC_CALLBACK_2(CCDxLib::onTouchesMoved, this);
	touchlistener->onTouchesCancelled = CC_CALLBACK_2(CCDxLib::onTouchesCancelled, this);
	touchlistener->onTouchesEnded = CC_CALLBACK_2(CCDxLib::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchlistener, this);


	InitMembers();

	CCDxStart();
	this->scheduleUpdate();
    return true;
}

void CCDxLib::InitMembers(){
	drawNode = DrawNode::create();
	drawNode->retain();
	text = Label::createWithSystemFont("", "arial.ttf", 48);
	text->setAnchorPoint(Vec2(0, 1));
	text->retain();

	/*
	auto label2 = Label::createWithTTF("012345678", "fonts/Marker Felt.ttf", 32);
	label2->setPosition(Point(origin.x, origin.y + visibleSize.height - 80));
	label2->setAlignment(TextHAlignment::LEFT);
	label2->setAnchorPoint(Point(0.0f, 0.0f));
	label2->setString("ui0022");
	this->addChild(label2);
	*/
}

int DxLib_End(){
	Director::getInstance()->end(); 
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
	return 0; 
}


void CCDxLib::update(float delta){

	CCDxLoop(delta);
}


/////////////////////////////////////////////////
//////////////////CCDxLib////////////////////////
/////////////////////////////////////////////////

CCDxLib* Get_m_dxlib(){ return m_dxlib; }

int CCDxLib::SetBackgroundColor(int Red, int Green, int Blue){
	SetBackGroundColorR = SetBackGroundColorR = Red;
	SetBackGroundColorG = SetBackGroundColorG = Green;
	SetBackGroundColorB = SetBackGroundColorB = Blue;
	
	return 0;
}

int CCDxLib::LoadGraphScreen(int x, int y, char *GraphName, int TransFlag){
	int h = this->LoadGraph(GraphName);
	if (h == -1)return -1;
	if(DrawGraph(x,y,h,TransFlag) == -1)return -1 ;
	return ScreenFlip();

}

int CCDxLib::LoadGraph(char *FileName){	

	//SpriteHandles.push_back( Sprite::create(FileName));
	//SpriteHandles[SpriteHandles.size() - 1]->retain();
	//return SpriteHandles.size() - 1;

	
	textureCache->addImage(FileName);
	auto texture = textureCache->getTextureForKey(FileName);
	if (texture == nullptr) return -1;
	GraphicHandles.push_back(FileName);
	return GraphicHandles.size() - 1;
	
}

Sprite* CCDxLib::CheckGetSprite(int GrHandle){
	//if (GrHandle < 0 || (unsigned int)GrHandle > SpriteHandles.size()) return nullptr;
	//else return SpriteHandles[GrHandle];

	if (GrHandle < 0 || (unsigned int)GrHandle > GraphicHandles.size()) return nullptr;
	auto texture = Director::getInstance()->getTextureCache()->getTextureForKey(GraphicHandles[GrHandle]);
	
	if (sppindex >= (int)SpritePool.size()){
		SpritePOOL spr;
		SpritePool.push_back(spr);
	}
	SpritePool[sppindex].initWithTexture(texture);
	sppindex++;
	if (texture == nullptr) return nullptr;
	else return &SpritePool[sppindex-1];
	
	/*
	if (GrHandle < 0 || (unsigned int)GrHandle > GraphicHandles.size()) return nullptr;
	auto texture = Director::getInstance()->getTextureCache()->getTextureForKey(GraphicHandles[GrHandle]);
	if (texture == nullptr) return nullptr;
	else return Sprite::createWithTexture(texture);
	*/
}

int CCDxLib::ScreenFlip(){
	drawNode->visit();
	if (!RenderEnded){
		rendertexture->end();
	}
	for (auto& t : textTextureCache){ t.second->UsedBeforeScreenFlip = false; }
	//for (auto& t : TextCaches){ t.second->UsedBeforeScreenFlip = false; }
	RenderEnded = true;
	RenderBegan = false;
	return 0;
}

int CCDxLib::ClearDrawScreen(){
	if (!RenderBegan){
		drawNode->clear();
		rendertexture->beginWithClear((float)SetBackGroundColorR / 255.0, (float)SetBackGroundColorG / 255.0, (float)SetBackGroundColorB / 255.0, 1.0);
		sppindex=0;
		txtsppindex = 0;
	}
	RenderBegan = true;
	RenderEnded = false;
	return 0;
}

int CCDxLib::GetGraphSize(int GrHandle, int *SizeXBuf, int *SizeYBuf){
	auto texture = Director::getInstance()->getTextureCache()->getTextureForKey(GraphicHandles[GrHandle]);
	if (texture == nullptr) return -1;
	*SizeXBuf = texture->getContentSize() .width;
	*SizeYBuf = texture->getContentSize().height;
	return 0;
}

int CCDxLib::DrawGraph(int x, int y, int GrHandle, int TransFlag){
	return DrawRotaGraph3(x, y, 0, 0,1.0, 1.0, 0.0, GrHandle, TransFlag, false);
}

int CCDxLib::DrawTurnGraph(int x, int y, int GrHandle, int TransFlag){
	return DrawRotaGraph3(x, y, 0, 0, 1.0, 1.0, 0.0, GrHandle, TransFlag, true);
}

int CCDxLib::DrawExtendGraph(int x1, int y1, int x2, int y2, int GrHandle, int TransFlag){
	return DrawRotaGraph3(x1, y1, 0, 0,
		(float)(x2 - x1) / GetGraphSizeX(GrHandle), 
		(float)(y2 - y1) / GetGraphSizeY(GrHandle), 
		0, GrHandle, TransFlag, false);
}

int CCDxLib::DrawRotaGraph(int x, int y, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag){
	int w,h;
	GetGraphSize(GrHandle,&w,&h);
	return DrawRotaGraph3(x+w/2,y+h/2,w/2.0,h/2.0,ExtRate,ExtRate,Angle,GrHandle,TransFlag,TurnFlag);
}

int CCDxLib::DrawRotaGraph2(int x, int y, int cx, int cy, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag){
	return DrawRotaGraph3(x,y,cx,cy,ExtRate,ExtRate,Angle,GrHandle,TransFlag,TurnFlag);
}

int CCDxLib::DrawRotaGraph3(int x, int y, int cx, int cy, double ExtRateX, double ExtRateY, double Angle, int GrHandle, int TransFlag, int TurnFlag){
	auto image = CheckGetSprite(GrHandle);
	if (image == nullptr)return -1;
	auto size = image->getContentSize();
	//image->setTextureRect(Rect(0,0,size.width,size.height));
	image->setPosition(DxVec2(x, y));
	image->setAnchorPoint(Vec2((float)(cx) / size.width, (float)(size.height - cy) / size.height));
	image->setScale(ExtRateX,ExtRateY);
	image->setRotation(FromRadian(Angle));
	image->setFlippedX(TurnFlag == TRUE);
	image->visit();
	
	return 0;
}

int CCDxLib::DrawRectGraph(int DestX, int DestY, int SrcX, int SrcY, int Width, int Height, int GrHandle, int TransFlag, int TurnFlag){
	auto image = CheckGetSprite(GrHandle);
	if (image == nullptr)return -1;
	auto size = image->getContentSize();
	image->setTextureRect(Rect(SrcX, SrcY, Width, Height));
	image->setPosition(DxVec2(DestX,DestY));
	image->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	image->setScale(1,1);
	image->setRotation(0);
	image->setFlippedX(TurnFlag == TRUE);
	image->visit();
	return 0;
}


unsigned int CCDxLib::GetColor(int Red, int Green, int Blue){
	return Red + Green * 256 + Blue * 65536;
}

int CCDxLib::DrawLine(int x1, int y1, int x2, int y2, unsigned int Color){
	drawNode->drawSegment(DxVec2(x1,y1), DxVec2(x2,y2), 0.5f,DxColor(Color));
	return 0;
}
int CCDxLib::DrawBox(int x1, int y1, int x2, int y2, unsigned int Color, int FillFlag){
	int  _x1 = x1,_y1 = y1,  _x2 = x2 - 1, _y2 = y2 - 1;
	if (FillFlag == FALSE)
		drawNode->drawRect(DxVec2(_x1, _y1), DxVec2(_x2, _y2), DxColor(Color));
	else
		drawNode->drawSolidRect(DxVec2(_x1, _y1), DxVec2(_x2, _y2), DxColor(Color));
	return 0;
}
int CCDxLib::DrawCircle(int x, int y, int r, unsigned int Color, int FillFlag, unsigned int segment){
	if (FillFlag == FALSE)
		drawNode->drawCircle(DxVec2(x,y),r,__Pi*2,segment,false,DxColor(Color));
	else
		drawNode->drawSolidCircle(DxVec2(x, y), r, __Pi * 2, segment, DxColor(Color));
	return 0;
}
int CCDxLib::DrawOval(int x, int y, int rx, int ry, unsigned int Color, int FillFlag, unsigned int segment ){
	if (rx == 0) return 0;
	if (FillFlag == FALSE)
		drawNode->drawCircle(DxVec2(x, y), rx, __Pi * 2, segment,false, 1.0, ((float)ry) / ((float)rx), DxColor(Color));
	else
		drawNode->drawSolidCircle(DxVec2(x, y), rx, __Pi * 2, segment, 1.0, ((float)ry) / ((float)rx), DxColor(Color));
	return 0;
	
}
int CCDxLib::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, unsigned int Color, int FillFlag){
	Vec2 DxVecs[] = { DxVec2(x1, y1), DxVec2(x2, y2), DxVec2(x3, y3) };
	if (FillFlag == FALSE)
		drawNode->drawPoly(DxVecs, 3,true, DxColor(Color));
	else
		drawNode->drawSolidPoly(DxVecs,3, DxColor(Color));
	return 0;
}
int CCDxLib::DrawPixel(int x, int y, unsigned int Color){
	drawNode->drawDot(DxVec2(x,y),1,DxColor(Color));
	return 0;
}


int CCDxLib::DrawString(int x, int y, char *String, unsigned int Color){
	if (x >= visibleSize.width || y >= visibleSize.height || String == "") return 0;
	

	/*
	Label* TargetText;
	bool NeedUpdate = true;
	int count = TextCaches.count(String);
	auto itr = TextCaches.find(String);
	
	for (int i = 0; i < count; i++){//SearchCache
		if (itr->second->isSame(text->getSystemFontSize(), text->getSystemFontName())){
			TargetText = itr->second;
			NeedUpdate = false;
			itr->second->UsedBeforeScreenFlip = true;
			break;
		}
		itr++;
	}
	if (NeedUpdate){//MakeCache
		if (TextCaches.size() < TextCache::CacheSize - 1){
			auto T = TextCache::createWithSystemFont(String,text->getSystemFontName(),text->getSystemFontSize());
			T->UsedBeforeScreenFlip = true;
			TextCaches.insert(std::make_pair(String, T));
			TargetText = T;
		}
		else {//UpdateCache
			for (auto itr = TextCaches.begin(); itr != TextCaches.end(); itr++){
				if (!itr->second->UsedBeforeScreenFlip){
					itr->second->release();
					TextCaches.erase(itr);
					auto T = TextCache::createWithSystemFont(String, text->getSystemFontName(), text->getSystemFontSize());
					T->UsedBeforeScreenFlip = true;
					TextCaches.insert(std::make_pair(String, T));
					TargetText = T; 
					break;
				}
			}
		}
	}
	if (TargetText == nullptr)return -1;
	TargetText->setPosition(DxVec2(x, y));
	TargetText->setColor(FromUnsignedIntColor(Color));
	TargetText->visit(_director->getRenderer(), this->getNodeToWorldTransform(), true);
	*/
	/*
	text->setPosition(DxVec2(x,y));
	text->setColor(FromUnsignedIntColor(Color));
	text->setString(String);
	text->visit(_director->getRenderer(), this->getNodeToWorldTransform(), true);
	*/
	
	if (txtsppindex >= (int)textSpritePool.size()){
		SpritePOOL spr;
		textSpritePool.push_back(spr);
	}
	
	
	auto& textSprite = textSpritePool[txtsppindex];
	bool NeedUpdate = true;
	
	//HashMapSearch
	int count = textTextureCache.count(String);
	auto itr = textTextureCache.find(String);
	for (int i = 0; i < count; i++){//SearchCache
		if (itr->second->isSame(text->getSystemFontSize(), text->getSystemFontName())){
			textSprite.initWithTexture(itr->second);
			NeedUpdate = false;
			itr->second->UsedBeforeScreenFlip = true;
			break;
		}
		itr++;
	}
	if (NeedUpdate){//MakeCache
		if (textTextureCache.size() < TextTexture2DCache::textTextureCacheSize-1){
			auto& T = textTextureCacheVec[textTextureCache.size()];
			T.initWithString(String, text->getFontDefinition());
			textSprite.initWithTexture(&T);
			T.UsedBeforeScreenFlip = true;
			textTextureCache.insert(std::make_pair(String, &T));
		}else {//UpdateCache
			for (auto itr = textTextureCache.begin(); itr != textTextureCache.end();itr++){
				if (!itr->second->UsedBeforeScreenFlip){
					itr->second->initWithString(String, text->getFontDefinition());
					itr->second->UsedBeforeScreenFlip = true;
					textSprite.initWithTexture(itr->second);
					textTextureCache.insert(std::make_pair(String ,itr->second));
					textTextureCache.erase(itr);
					break;
				}
			}
		}
	}
	
	
	textSprite.setPosition(DxVec2(x, y));
	textSprite.setCameraMask(text->getCameraMask());
	textSprite.setGlobalZOrder(text->getGlobalZOrder());
	textSprite.setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	textSprite.setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED );
	textSprite.updateDisplayedColor(FromUnsignedIntColor(Color));
	//textSprite.setColor(FromUnsignedIntColor(Color));
	textSprite.updateDisplayedOpacity(255);
	textSprite.draw( _director->getRenderer(), textSprite.getNodeToWorldTransform(), true);
	txtsppindex++;
	
	return 0;
}

int DrawFormatString(int x, int y, unsigned int Color, char *FormatString, ...){
	va_list ap;
	va_start(ap, FormatString);
	char* buf = (char*)malloc(102400);
	if (buf == nullptr){va_end(ap);return -1;}
	vsnprintf(buf, 102400, FormatString, ap);
	va_end(ap);
	int res = DrawString(x, y, buf, Color);
	free(buf);
	return res;
}

int CCDxLib::SetFontSize(int FontSize){
	text->setSystemFontSize(FontSize);
	return 0;
}

/*int CCDxLib::ChangeFont(char *FontName){
	text->setSystemFontName(FontName);
	return 0;
}*/

int CCDxLib::SetCreateSoundDataType(int SoundDataType){
	switch (SoundDataType){
	case DX_SOUNDDATATYPE_MEMNOPRESS:
	case DX_SOUNDDATATYPE_MEMPRESS:
		this->SoundDataType = SoundDataType;
		return 0;
	default: return -1;
	}
}
int CCDxLib::LoadSoundMem(char *FileName){
	switch (SoundDataType){
//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 
	//default:
	case DX_SOUNDDATATYPE_MEMPRESS:
		simpleAudioEngine->preloadBackgroundMusic(FileName);
		MusicHandles.push_back(MusicHandle(true, FileName));
		return MusicHandles.size()-1;
	case DX_SOUNDDATATYPE_MEMNOPRESS:
		simpleAudioEngine->preloadEffect(FileName);
		MusicHandles.push_back(MusicHandle(false, FileName));
		return MusicHandles.size() - 1;
	}
	return -1;
}
bool CCDxLib::CheckMusicHandle(int index){
	return  !(index < 0 || (unsigned int)index > MusicHandles.size());
}
int CCDxLib::PlaySoundMem(int SoundHandle, int PlayType, int TopPositionFlag){
	if (CheckMusicHandle(SoundHandle)){
		bool loop = PlayType == DX_PLAYTYPE_LOOP;
		if (MusicHandles[SoundHandle].isBGM){			
			simpleAudioEngine->playBackgroundMusic(MusicHandles[SoundHandle].FileName.c_str(),loop);
			CurrentPlayedBGMName = MusicHandles[SoundHandle].FileName;
		}else{
			if (!MusicHandles[SoundHandle].SE_OncePlayed){
				MusicHandles[SoundHandle].SE_OncePlayed = true;
			}
			auto ID = simpleAudioEngine->playEffect(MusicHandles[SoundHandle].FileName.c_str(), loop);
			MusicHandles[SoundHandle].SE_ID = ID;
		}
	}else return -1;
	return 0;
}
int CCDxLib::CheckSoundMem(int SoundHandle){
	
	if (CheckMusicHandle(SoundHandle)){
		if (MusicHandles[SoundHandle].isBGM){
			if (CurrentPlayedBGMName != MusicHandles[SoundHandle].FileName)return 0;
			else return simpleAudioEngine->isBackgroundMusicPlaying();
		}else{
			//Sorry!!!!Cocos whether SE is playing can't be checked !
			return -1;
		}
	}else return -1;
}
int CCDxLib::StopSoundMem(int SoundHandle){
	if (CheckMusicHandle(SoundHandle)){
		if (MusicHandles[SoundHandle].isBGM){
			if (CurrentPlayedBGMName != MusicHandles[SoundHandle].FileName)return 0;
			else simpleAudioEngine->pauseBackgroundMusic();
		}else{
			simpleAudioEngine->stopEffect(MusicHandles[SoundHandle].SE_ID);
			return -1;
		}
	}else return -1;
	return 0;
}

int CCDxLib::GetNowCount(){
	struct timeval t;
	gettimeofday(&t, nullptr);
	return t.tv_sec * 1000 + t.tv_usec / 1000;	
}
long long CCDxLib::GetNowHiPerformanceCount(){
	struct timeval t;
	gettimeofday(&t, nullptr);
	return t.tv_sec * 1000000ll + t.tv_usec;
}
int  CCDxLib::GetDateTime(DATEDATA *DateBuf){
	struct tm *nowDate;
	time_t now;
	time(&now);
	nowDate = localtime(&now);
	DateBuf->Sec = nowDate->tm_sec;
	DateBuf->Min = nowDate->tm_min;
	DateBuf->Hour = nowDate->tm_hour;
	DateBuf->Day = nowDate->tm_mday;
	DateBuf->Mon = nowDate->tm_mon + 1;
	DateBuf->Year = nowDate->tm_year + 1900;
	return 0;
}

int CCDxLib::GetRand(int RandMax){
	return rand() % (RandMax+1);
}
int CCDxLib::SRand(int Seed){
	srand((unsigned int)Seed);
	return 0;
}
int CCDxLib::KeyMap(cocos2d::EventKeyboard::KeyCode KeyCode){
	switch ((int)KeyCode){
	case KEY_INPUT_BACK		  :return 0;
	case KEY_INPUT_TAB		  :return 1;
	case KEY_INPUT_RETURN	  :return 2;
	case KEY_INPUT_LSHIFT	  :return 3;
	case KEY_INPUT_RSHIFT	  :return 4;
	case KEY_INPUT_LCONTROL	  :return 5;
	case KEY_INPUT_RCONTROL	  :return 6;
	case KEY_INPUT_ESCAPE	  :return 7;
	case KEY_INPUT_SPACE	  :return 8;
	case KEY_INPUT_PGUP		  :return 9;
	case KEY_INPUT_PGDN		  :return 10;
	case KEY_INPUT_END		  :return 11;
	case KEY_INPUT_HOME		  :return 12;
	case KEY_INPUT_LEFT		  :return 13;
	case KEY_INPUT_UP		  :return 14;
	case KEY_INPUT_RIGHT	  :return 15;
	case KEY_INPUT_DOWN		  :return 16;
	case KEY_INPUT_INSERT	  :return 17;
	case KEY_INPUT_DELETE	  :return 18;
	case KEY_INPUT_MINUS	  :return 19;
	case KEY_INPUT_YEN		  :return 20;
	case KEY_INPUT_PREVTRACK  :return 21;
	case KEY_INPUT_PERIOD	  :return 22;
	case KEY_INPUT_SLASH	  :return 23;
	case KEY_INPUT_LALT		  :return 24;
	case KEY_INPUT_RALT		  :return 25;
	case KEY_INPUT_SCROLL	  :return 26;
	case KEY_INPUT_SEMICOLON  :return 27;
	case KEY_INPUT_COLON	  :return 28;
	case KEY_INPUT_LBRACKET	  :return 29;
	case KEY_INPUT_RBRACKET	  :return 30;
	case KEY_INPUT_AT		  :return 31;
	case KEY_INPUT_BACKSLASH  :return 32;
	case KEY_INPUT_COMMA	  :return 33;
	case KEY_INPUT_CAPSLOCK	  :return 34;
	case KEY_INPUT_PAUSE	  :return 35;
//	case KEY_INPUT_NUMPAD0	  :return 36;
//	case KEY_INPUT_NUMPAD1	  :return 37;
//	case KEY_INPUT_NUMPAD2	  :return 38;
//	case KEY_INPUT_NUMPAD3	  :return 39;
//	case KEY_INPUT_NUMPAD4	  :return 40;
//	case KEY_INPUT_NUMPAD5	  :return 41;
//	case KEY_INPUT_NUMPAD6	  :return 42;
//	case KEY_INPUT_NUMPAD7	  :return 43;
//	case KEY_INPUT_NUMPAD8	  :return 44;
//	case KEY_INPUT_NUMPAD9	  :return 45;
	case KEY_INPUT_MULTIPLY	  :return 46;
	case KEY_INPUT_ADD		  :return 47;
	case KEY_INPUT_SUBTRACT	  :return 48;
//	case KEY_INPUT_DECIMAL	  :return 49;
	case KEY_INPUT_DIVIDE	  :return 50;
	case KEY_INPUT_NUMPADENTER:return 51;
	case KEY_INPUT_F1		  :return 52;
	case KEY_INPUT_F2		  :return 53;
	case KEY_INPUT_F3		  :return 54;
	case KEY_INPUT_F4		  :return 55;
	case KEY_INPUT_F5		  :return 56;
	case KEY_INPUT_F6		  :return 57;
	case KEY_INPUT_F7		  :return 58;
	case KEY_INPUT_F8		  :return 59;
	case KEY_INPUT_F9		  :return 60;
	case KEY_INPUT_F10		  :return 61;
	case KEY_INPUT_F11		  :return 62;
	case KEY_INPUT_F12		  :return 63;
	case KEY_INPUT_A		  :return 64;
	case KEY_INPUT_B		  :return 65;
	case KEY_INPUT_C		  :return 66;
	case KEY_INPUT_D		  :return 67;
	case KEY_INPUT_E		  :return 68;
	case KEY_INPUT_F		  :return 69;
	case KEY_INPUT_G		  :return 70;
	case KEY_INPUT_H		  :return 71;
	case KEY_INPUT_I		  :return 72;
	case KEY_INPUT_J		  :return 73;
	case KEY_INPUT_K		  :return 74;
	case KEY_INPUT_L		  :return 75;
	case KEY_INPUT_M		  :return 76;
	case KEY_INPUT_N		  :return 77;
	case KEY_INPUT_O		  :return 78;
	case KEY_INPUT_P		  :return 79;
	case KEY_INPUT_Q		  :return 80;
	case KEY_INPUT_R		  :return 81;
	case KEY_INPUT_S		  :return 82;
	case KEY_INPUT_T		  :return 83;
	case KEY_INPUT_U		  :return 84;
	case KEY_INPUT_V		  :return 85;
	case KEY_INPUT_W		  :return 86;
	case KEY_INPUT_X		  :return 87;
	case KEY_INPUT_Y		  :return 88;
	case KEY_INPUT_Z		  :return 89;
	case KEY_INPUT_0		  :return 90;
	case KEY_INPUT_1		  :return 91;
	case KEY_INPUT_2		  :return 92;
	case KEY_INPUT_3		  :return 93;
	case KEY_INPUT_4		  :return 94;
	case KEY_INPUT_5		  :return 95;
	case KEY_INPUT_6		  :return 96;
	case KEY_INPUT_7		  :return 97;
	case KEY_INPUT_8		  :return 98;
	case KEY_INPUT_9          :return 99;
	}
	return 0;
}
int CCDxLib::KeyReverseMap(int Key){
	switch (Key){
	case 0:return KEY_INPUT_BACK;
	case 1:return KEY_INPUT_TAB;
	case 2:return KEY_INPUT_RETURN;
	case 3:return KEY_INPUT_LSHIFT;
	case 4:return KEY_INPUT_RSHIFT;
	case 5:return KEY_INPUT_LCONTROL;
	case 6:return KEY_INPUT_RCONTROL;
	case 7:return KEY_INPUT_ESCAPE;
	case 8:return KEY_INPUT_SPACE;
	case 9:return KEY_INPUT_PGUP;
	case 10:return KEY_INPUT_PGDN;
	case 11:return KEY_INPUT_END;
	case 12:return KEY_INPUT_HOME;
	case 13:return KEY_INPUT_LEFT;
	case 14:return KEY_INPUT_UP;
	case 15:return KEY_INPUT_RIGHT;
	case 16:return KEY_INPUT_DOWN;
	case 17:return KEY_INPUT_INSERT;
	case 18:return KEY_INPUT_DELETE;
	case 19:return KEY_INPUT_MINUS;
	case 20:return KEY_INPUT_YEN;
	case 21:return KEY_INPUT_PREVTRACK;
	case 22:return KEY_INPUT_PERIOD;
	case 23:return KEY_INPUT_SLASH;
	case 24:return KEY_INPUT_LALT;
	case 25:return KEY_INPUT_RALT;
	case 26:return KEY_INPUT_SCROLL;
	case 27:return KEY_INPUT_SEMICOLON;
	case 28:return KEY_INPUT_COLON;
	case 29:return KEY_INPUT_LBRACKET;
	case 30:return KEY_INPUT_RBRACKET;
	case 31:return KEY_INPUT_AT;
	case 32:return KEY_INPUT_BACKSLASH;
	case 33:return KEY_INPUT_COMMA;
	case 34:return KEY_INPUT_CAPSLOCK;
	case 35:return KEY_INPUT_PAUSE;
	case 36:return KEY_INPUT_NUMPAD0;
	case 37:return KEY_INPUT_NUMPAD1;
	case 38:return KEY_INPUT_NUMPAD2;
	case 39:return KEY_INPUT_NUMPAD3;
	case 40:return KEY_INPUT_NUMPAD4;
	case 41:return KEY_INPUT_NUMPAD5;
	case 42:return KEY_INPUT_NUMPAD6;
	case 43:return KEY_INPUT_NUMPAD7;
	case 44:return KEY_INPUT_NUMPAD8;
	case 45:return KEY_INPUT_NUMPAD9;
	case 46:return KEY_INPUT_MULTIPLY;
	case 47:return KEY_INPUT_ADD;
	case 48:return KEY_INPUT_SUBTRACT;
	case 49:return KEY_INPUT_DECIMAL;
	case 50:return KEY_INPUT_DIVIDE;
	case 51:return KEY_INPUT_NUMPADENTER;
	case 52:return KEY_INPUT_F1;
	case 53:return KEY_INPUT_F2;
	case 54:return KEY_INPUT_F3;
	case 55:return KEY_INPUT_F4;
	case 56:return KEY_INPUT_F5;
	case 57:return KEY_INPUT_F6;
	case 58:return KEY_INPUT_F7;
	case 59:return KEY_INPUT_F8;
	case 60:return KEY_INPUT_F9;
	case 61:return KEY_INPUT_F10;
	case 62:return KEY_INPUT_F11;
	case 63:return KEY_INPUT_F12;
	case 64:return KEY_INPUT_A;
	case 65:return KEY_INPUT_B;
	case 66:return KEY_INPUT_C;
	case 67:return KEY_INPUT_D;
	case 68:return KEY_INPUT_E;
	case 69:return KEY_INPUT_F;
	case 70:return KEY_INPUT_G;
	case 71:return KEY_INPUT_H;
	case 72:return KEY_INPUT_I;
	case 73:return KEY_INPUT_J;
	case 74:return KEY_INPUT_K;
	case 75:return KEY_INPUT_L;
	case 76:return KEY_INPUT_M;
	case 77:return KEY_INPUT_N;
	case 78:return KEY_INPUT_O;
	case 79:return KEY_INPUT_P;
	case 80:return KEY_INPUT_Q;
	case 81:return KEY_INPUT_R;
	case 82:return KEY_INPUT_S;
	case 83:return KEY_INPUT_T;
	case 84:return KEY_INPUT_U;
	case 85:return KEY_INPUT_V;
	case 86:return KEY_INPUT_W;
	case 87:return KEY_INPUT_X;
	case 88:return KEY_INPUT_Y;
	case 89:return KEY_INPUT_Z;
	case 90:return KEY_INPUT_0;
	case 91:return KEY_INPUT_1;
	case 92:return KEY_INPUT_2;
	case 93:return KEY_INPUT_3;
	case 94:return KEY_INPUT_4;
	case 95:return KEY_INPUT_5;
	case 96:return KEY_INPUT_6;
	case 97:return KEY_INPUT_7;
	case 98:return KEY_INPUT_8;
	case 99:return KEY_INPUT_9;
	}
	return 0;
}
std::string CCDxLib::KeyName(cocos2d::EventKeyboard::KeyCode KeyCode){
	switch ((int)KeyCode){
	case KEY_INPUT_BACK		  :return "BACK";
	case KEY_INPUT_TAB		  :return "TAB";
	case KEY_INPUT_RETURN	  :return "ENTER";
	case KEY_INPUT_LSHIFT	  :return "SHIFT";
	case KEY_INPUT_RSHIFT	  :return "RSHIFT";
	case KEY_INPUT_LCONTROL	  :return "CTRL";
	case KEY_INPUT_RCONTROL	  :return "RCTRL";
	case KEY_INPUT_ESCAPE	  :return "ESC";
	case KEY_INPUT_SPACE	  :return "SPACE";
	case KEY_INPUT_PGUP		  :return "PGUP";
	case KEY_INPUT_PGDN		  :return "PGDN";
	case KEY_INPUT_END		  :return "END";
	case KEY_INPUT_HOME		  :return "HOME";
	case KEY_INPUT_LEFT		  :return "←";
	case KEY_INPUT_UP		  :return "↑";
	case KEY_INPUT_RIGHT	  :return "→";
	case KEY_INPUT_DOWN		  :return "↓";
	case KEY_INPUT_INSERT	  :return "INSERT";
	case KEY_INPUT_DELETE	  :return "DEL";
	case KEY_INPUT_MINUS	  :return "-";
	case KEY_INPUT_YEN		  :return "￥";
	case KEY_INPUT_PREVTRACK  :return "^";
	case KEY_INPUT_PERIOD	  :return ".";
	case KEY_INPUT_SLASH	  :return "/";
	case KEY_INPUT_LALT		  :return "ALT";
	case KEY_INPUT_RALT		  :return "ALT";
	case KEY_INPUT_SCROLL	  :return "SCROLL";
	case KEY_INPUT_SEMICOLON  :return ";";
	case KEY_INPUT_COLON	  :return ":";
	case KEY_INPUT_LBRACKET	  :return "[";
	case KEY_INPUT_RBRACKET	  :return "]";
	case KEY_INPUT_AT		  :return "@";
	case KEY_INPUT_BACKSLASH  :return "\\";
	case KEY_INPUT_COMMA	  :return ".";
	case KEY_INPUT_CAPSLOCK	  :return "CAPS";
	case KEY_INPUT_PAUSE	  :return "PAUSE";
	case KEY_INPUT_MULTIPLY	  :return "×";
	case KEY_INPUT_ADD		  :return "+";
	case KEY_INPUT_SUBTRACT	  :return "-";
	case KEY_INPUT_DIVIDE	  :return "÷";
	case KEY_INPUT_NUMPADENTER:return "ENTER";
	case KEY_INPUT_F1		  :return "F1";
	case KEY_INPUT_F2		  :return "F2";
	case KEY_INPUT_F3		  :return "F3";
	case KEY_INPUT_F4		  :return "F4";
	case KEY_INPUT_F5		  :return "F5";
	case KEY_INPUT_F6		  :return "F6";
	case KEY_INPUT_F7		  :return "F7";
	case KEY_INPUT_F8		  :return "F8";
	case KEY_INPUT_F9		  :return "F9";
	case KEY_INPUT_F10		  :return "F10";
	case KEY_INPUT_F11		  :return "F11";
	case KEY_INPUT_F12		  :return "F12";
	case KEY_INPUT_A		  :return "A";
	case KEY_INPUT_B		  :return "B";
	case KEY_INPUT_C		  :return "C";
	case KEY_INPUT_D		  :return "D";
	case KEY_INPUT_E		  :return "E";
	case KEY_INPUT_F		  :return "F";
	case KEY_INPUT_G		  :return "G";
	case KEY_INPUT_H		  :return "H";
	case KEY_INPUT_I		  :return "I";
	case KEY_INPUT_J		  :return "J";
	case KEY_INPUT_K		  :return "K";
	case KEY_INPUT_L		  :return "L";
	case KEY_INPUT_M		  :return "M";
	case KEY_INPUT_N		  :return "N";
	case KEY_INPUT_O		  :return "O";
	case KEY_INPUT_P		  :return "P";
	case KEY_INPUT_Q		  :return "Q";
	case KEY_INPUT_R		  :return "R";
	case KEY_INPUT_S		  :return "S";
	case KEY_INPUT_T		  :return "T";
	case KEY_INPUT_U		  :return "U";
	case KEY_INPUT_V		  :return "V";
	case KEY_INPUT_W		  :return "W";
	case KEY_INPUT_X		  :return "X";
	case KEY_INPUT_Y		  :return "Y";
	case KEY_INPUT_Z		  :return "Z";
	case KEY_INPUT_0		  :return "0";
	case KEY_INPUT_1		  :return "1";
	case KEY_INPUT_2		  :return "2";
	case KEY_INPUT_3		  :return "3";
	case KEY_INPUT_4		  :return "4";
	case KEY_INPUT_5		  :return "5";
	case KEY_INPUT_6		  :return "6";
	case KEY_INPUT_7		  :return "7";
	case KEY_INPUT_8		  :return "8";
	case KEY_INPUT_9          :return "9";
	}
	return "";
}

void CCDxLib::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event){
	KeyPushed[KeyMap(keyCode)] = true;
}
void CCDxLib::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event){
	KeyPushed[KeyMap(keyCode)] = false;
}
int CCDxLib::CheckHitKey(int KeyCode){
	return KeyPushed[KeyMap((EventKeyboard::KeyCode)KeyCode)] ? 1 : 0; 
}
int CCDxLib::CheckHitKeyAll(){
	for (int i = 0; i < 100; i++){
		if (KeyPushed[i]) return 0;
	}
	return -1;
}
int CCDxLib::GetHitKeyStateAll(char *KeyStateBuf){
	for (int i = 0; i < 100; i++){
		KeyStateBuf[KeyReverseMap(i)] = CheckHitKey(KeyReverseMap(i));
	}
	return 0;
}

void CCDxLib::onMouseDown(Event* evt){
	EventMouse* e = (EventMouse*)evt;
	MousePushed[e->getMouseButton()] = true;
}
void CCDxLib::onMouseUp(Event* evt){
	EventMouse* e = (EventMouse*)evt;
	MousePushed[e->getMouseButton()] = false;

}
void CCDxLib::onMouseMove(Event* evt){
	EventMouse* e = (EventMouse*)evt;
	MouseX = e->getCursorX();
	MouseY =DxY( e->getCursorY());
}
void CCDxLib::onMouseScroll(Event* evt){
	EventMouse* e = (EventMouse*)evt;
	ScrollVal += e->getScrollY();
}
int CCDxLib::SetMouseDispFlag(int DispFlag){
	auto glview = Director::getInstance()->getOpenGLView();
	glview->setCursorVisible(DispFlag == TRUE);
	return 0;
}
int CCDxLib::GetMousePoint(int *XBuf, int *YBuf){ 
	if (EmulateTouchByMouseFunctions){
		if (GetTouchInputNum() > 0){
			GetTouchInput(0,XBuf,YBuf,NULL,NULL);
		}else{
			*XBuf = LastTouchX;
			*YBuf = DxY(LastTouchY);
		}
		return 0;
	}else{
		*XBuf = MouseX;
		*YBuf = MouseY;
		return 0;
	}
}
int CCDxLib::GetMouseInput(){ 
	if (EmulateTouchByMouseFunctions){
		if (GetTouchInputNum() > 0) 
			return MOUSE_INPUT_LEFT;
		else return 0;
	}else return 
		MousePushed[0] * MOUSE_INPUT_LEFT   |
		MousePushed[1] * MOUSE_INPUT_RIGHT  |
		MousePushed[2] * MOUSE_INPUT_MIDDLE |
		MousePushed[3] * MOUSE_INPUT_4      |
		MousePushed[4] * MOUSE_INPUT_5      |
		MousePushed[5] * MOUSE_INPUT_6      |
		MousePushed[6] * MOUSE_INPUT_7      |
		MousePushed[7] * MOUSE_INPUT_8      ;
		
}
int CCDxLib::GetMouseWheelRotVol(){
	int res = CurrentScrollVal - ScrollVal;
	CurrentScrollVal = ScrollVal;
	return res;
}

void CCDxLib::onTouchesBegan    (const std::vector<Touch*>& touches, Event *pEvent){
	for (auto t : touches){
		DxTouches.push_back(DxTouch(
			t->getLocation().x,
			t->getLocation().y,
			t->getID()));
		LastTouchX = t->getLocation().x;
		LastTouchY = t->getLocation().y;
	}
}
void CCDxLib::onTouchesMoved    (const std::vector<Touch*>& touches, Event *pEvent){
	int i = 0;
	for (auto t : touches){
		for (auto idxt = DxTouches.begin(); idxt != DxTouches.end();){
			if ((*idxt).ID == t->getID()) {
				(*idxt).X = t->getLocation().x;
				(*idxt).Y = t->getLocation().y;
				break;
			}
			idxt++;
		}
		LastTouchX = t->getLocation().x;
		LastTouchY = t->getLocation().y;
	}
}
void CCDxLib::onTouchesEnded    (const std::vector<Touch*>& touches, Event *pEvent){
	for (auto t : touches){
		for (auto idxt = DxTouches.begin();idxt !=DxTouches.end();){
			if ((*idxt).ID == t->getID()) {
				idxt = DxTouches.erase(idxt);
				break;
			}
			idxt++;
		}
		LastTouchX = t->getLocation().x;
		LastTouchY = t->getLocation().y;
	}
}
void CCDxLib::onTouchesCancelled(const std::vector<Touch*>& touches, Event *pEvent){
	this->onTouchesEnded(touches, pEvent);
}
int CCDxLib::GetTouchInputNum(){
	return DxTouches.size();
}
int CCDxLib::GetTouchInput(int InputNo, int *PositionX, int *PositionY, int *ID, int *Device){
	if (InputNo < 0 || (unsigned int)InputNo >= DxTouches.size())return -1;
	int i = 0;
	for (auto idxt = DxTouches.begin(); idxt != DxTouches.end();){
		if (i == InputNo){
			if (PositionX != nullptr)*PositionX = idxt->X;
			if (PositionY != nullptr)*PositionY = DxY(idxt->Y);
			if (ID != nullptr)*ID = idxt->ID;
			return 0;
		}
		idxt++; i++;
	}
	
	return -1;
}

void CCDxLib::EMULATE_TOUCH_BY_MOUSEFUNCTIONS(bool Emulate){
	EmulateTouchByMouseFunctions = Emulate;
}
void CCDxLib::EmulateButtonEnterCallback(int KeyCode){
	onKeyPressed((EventKeyboard::KeyCode) KeyCode, nullptr);
}
void CCDxLib::EmulateButtonExitCallback(int KeyCode){
	onKeyReleased((EventKeyboard::KeyCode) KeyCode, nullptr);
}

void CCDxLib::EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(int KeyCode){
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KeyCode, "ButtonExample.png", USE_EMULATE_BUTTON_DEFAULT_POSITION, USE_EMULATE_BUTTON_DEFAULT_POSITION,true);
}
void CCDxLib::EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(int KeyCode, char* ButtonFileName, int PositionX, int PositionY, bool WriteKeyName){
	
	auto button = ui::Button::create(ButtonFileName, "");
	button->addTouchEventListener([KeyCode,button](Ref* pSender, cocos2d::ui::Widget::TouchEventType type){
		switch (type){
		case ui::Widget::TouchEventType::BEGAN:
			Get_m_dxlib()->EmulateButtonEnterCallback(KeyCode);
			button->setOpacity(255);
			break;
		case ui::Widget::TouchEventType::ENDED:
		case ui::Widget::TouchEventType::CANCELED:
			Get_m_dxlib()->EmulateButtonExitCallback(KeyCode);
			button->setOpacity(100);
			break;		
		}
	});
	if (WriteKeyName){
		button->setTitleText(KeyName((EventKeyboard::KeyCode)KeyCode));
		button->setTitleColor(FromUnsignedIntColor(0));
		button->setTitleFontSize(visibleSize.height /18.0);
	}

	button->setOpacity(100);
	if (PositionX = USE_EMULATE_BUTTON_DEFAULT_POSITION || PositionY == USE_EMULATE_BUTTON_DEFAULT_POSITION){
		switch (KeyCode){
		case KEY_INPUT_LEFT	:
			PositionX = visibleSize.width - button->getSize().width * 1.1 *3;
			PositionY = visibleSize.height - button->getSize().height;
			break;
		case KEY_INPUT_RIGHT:
			PositionX = visibleSize.width - button->getSize().width * 1.1 * 1;
			PositionY = visibleSize.height - button->getSize().height;
			break;
		case KEY_INPUT_UP:
			PositionX = visibleSize.width - button->getSize().width * 1.1 * 2;
			PositionY = visibleSize.height - button->getSize().height *1.1 *  2;
			break;
		case KEY_INPUT_DOWN:
			PositionX = visibleSize.width - button->getSize().width * 1.1 * 2;
			PositionY = visibleSize.height - button->getSize().height;
			break;
		default:
			PositionX = button->getSize().width * 1.1 *(0.7 + EmulateButtonNum);
			PositionY = visibleSize.height - button->getSize().height;
			EmulateButtonNum++;
			break;

		}
	}
	button->setPosition(DxVec2(PositionX, PositionY));
	this->addChild(button);
	

}

