#include "DxLib.h"

int Handle;
float xx = 0,yy = 0;
int BackHandle;
int MHandle = 0;
int SHandle;

void AppDelegate::CCDxInit(){
	SetGraphMode(800, 600);// 640, 480, 32);
	ChangeWindowMode(TRUE);
	SetMainWindowText("Chihaya is God");
	EMULATE_TOUCH_BY_MOUSEFUNCTIONS();
	SetBackgroundColor(200, 100, 255);

}
void CCDxStart(){
	

	Handle = LoadGraph("HelloWorld.png");
	BackHandle = LoadGraph("wing.jpg");
	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
	MHandle = Get_m_dxlib()->LoadSoundMem("res/twilightsky.mp3");
	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	SHandle = Get_m_dxlib()->LoadSoundMem("res/Attack.mp3");
	Get_m_dxlib()->PlaySoundMem(MHandle,DX_PLAYTYPE_BACK);
	Get_m_dxlib()->EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_DOWN);
	Get_m_dxlib()->EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_UP);
	Get_m_dxlib()->EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_RIGHT);
	Get_m_dxlib()->EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_LEFT);
	Get_m_dxlib()->EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_R);
	Get_m_dxlib()->EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_T);
	Get_m_dxlib()->EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_R);
}

void CCDxLoop(float deltaTime){

	ClearDrawScreen();
	DrawExtendGraph(0,0, 840, 600, BackHandle);
	if (CheckHitKey(KEY_INPUT_RIGHT))xx +=3;
	if (CheckHitKey(KEY_INPUT_LEFT))xx -= 3;
	if (CheckHitKey(KEY_INPUT_DOWN))yy += 3;
	if (CheckHitKey(KEY_INPUT_UP))yy -= 3;
	if (CheckHitKey(KEY_INPUT_A))Get_m_dxlib()->PlaySoundMem(SHandle, DX_PLAYTYPE_LOOP);
	if (CheckHitKey(KEY_INPUT_ESCAPE))DxLib_End();
	if (CheckHitKey(KEY_INPUT_R))SetFontSize(16);
	if (CheckHitKey(KEY_INPUT_E))SetFontSize(32);
	if (CheckHitKey(KEY_INPUT_R)) DrawRectGraph(xx, yy,0,0,22,72, Handle);
	else DrawGraph(xx, yy, Handle);
	DrawFormatString(0,0,0,"%d",(int)yy);
	int mx = 0, my = 0;
	GetMousePoint(&mx,&my);
	for (int i = 0; i < 10; i++){
		for (int j = 0; j < 10; j++){
			//DrawGraph(100 * i, 100 * j, Handle);
			//DrawGraph(100 * i, 100 * j + 50, Handle);
			DrawFormatString(100 * i, 100 * j, GetColor(222, 0, 0), "%d", mx+i);
			DrawFormatString(100 * i, 100 * j + 50, GetColor(0, 222, 0), "%d", my+j);
		}
	}
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