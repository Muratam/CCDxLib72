#include "DxLib.h"

int Handle;
float xx = 0,yy = 0;
int BackHandle;
int MHandle = 0;
int SHandle;
int lsih,lsihcocos;
void AppDelegate::CCDxInit(){
	SetGraphMode(800, 600);// 640, 480, 32);
	ChangeWindowMode(TRUE);
	SetMainWindowText("Chihaya is God");
	EMULATE_TOUCH_BY_MOUSEFUNCTIONS();
	//EMULATE_KEYBOARD_ARROWS_BY_ACCELEROMETER();
	SetBackgroundColor(200, 100, 255);

}


void CCDxStart(){
	
	lsihcocos = LoadSoftImage("HelloWorld.png");
	lsih = MakeARGB8ColorSoftImage(100,200);
	
	Handle = LoadGraph("HelloWorld.png");
	BackHandle = LoadGraph("wing.jpg");


	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
	MHandle = Get_m_dxlib()->LoadSoundMem("res/twilightsky.mp3");
	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	SHandle = Get_m_dxlib()->LoadSoundMem("res/Attack.mp3");
	//Get_m_dxlib()->PlaySoundMem(MHandle,DX_PLAYTYPE_BACK);
	/*
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_DOWN);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_UP);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_RIGHT);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_LEFT);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_R);
	*/
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_T);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_R);
}

FILE* CCDxfopen(const char* Filename,const char* Mode){
	auto fileutils = FileUtils::getInstance();
	return fopen(fileutils->getSuitableFOpen(
		fileutils->fullPathForFilename(Filename)).c_str(), Mode);
}


void CCDxLoop(float deltaTime){
	CCEAGLView d;
	ClearDrawScreen();
	DrawExtendGraph(0,0, 840, 600, BackHandle);
	if (CheckHitKey(KEY_INPUT_RIGHT))xx +=1;
	if (CheckHitKey(KEY_INPUT_LEFT))xx -= 1;
	if (CheckHitKey(KEY_INPUT_DOWN))yy += 1;
	if (CheckHitKey(KEY_INPUT_UP))yy -= 1;
	//if (CheckHitKey(KEY_INPUT_A))Get_m_dxlib()->PlaySoundMem(SHandle, DX_PLAYTYPE_LOOP);
	if (CheckHitKey(KEY_INPUT_ESCAPE))DxLib_End();
	if (CheckHitKey(KEY_INPUT_R))SetFontSize(16);
	if (CheckHitKey(KEY_INPUT_E))SetFontSize(32);
	if (CheckHitKey(KEY_INPUT_T))FillSoftImage(lsih,0,72,172,72);
	if (CheckHitKey(KEY_INPUT_R)) DrawRectGraph(xx, yy,0,0,22,72, Handle);
	else DrawGraph(xx * 3, yy * 3, Handle);
	DrawPixelSoftImage(lsih,xx,yy,0,255,111,255);
	DrawSoftImage(0, 0, lsih);
	
	int mx = 0, my = 0;
	GetMousePoint(&mx,&my);
	
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			//DrawGraph(100 * i, 100 * j, Handle);
			//DrawGraph(100 * i, 100 * j + 50, Handle);
			//DrawFormatString(100 * i, 100 * j, GetColor(222, 0, 0), "%d", mx+i);
			//DrawFormatString(100 * i, 100 * j + 50, GetColor(0, 222, 0), "%d", my+j);
		}
	} 
	//DrawFormatString(100, 100, 0, "x : %f", Get_m_dxlib()->accx);
	//DrawFormatString(100, 200, 0, "z : %f", Get_m_dxlib()->accz);
	for (int i = 0; i < 120; i++){
		mx += 2; my += 2;
		//DrawRotaGraph(mx, my,1.0,i/10.0,Handle);
		//DrawRotaGraph(mx, my,1.0+i/100.0,i/3.14159, Handle);
		DrawBox(mx -20,my -20,mx + 20 ,my + 20,GetColor(0,100,100),TRUE);
		//if (GetMouseInput() & MOUSE_INPUT_LEFT) DrawString(mx, my + 60, "Touch", GetColor(222, 0, 0));
	}
	Get_m_dxlib()->ScreenFlip();
}


//Font はGLCallも多く、よくない。
//Graphicは、GLCallは少ないが、毎回生成しているので遅い
//TopPositionFlag
//SoundEffect * DX_PLAYTYPE_LOOP 