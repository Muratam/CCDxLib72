#include "DxLib.h"

int Handle;
int xx = 0,yy = 0;
int BackHandle;
int MHandle = 0;
int SHandle;

void AppDelegate::CCDxInit(){
	SetGraphMode(720, 540, 32);
	ChangeWindowMode(TRUE);
	SetMainWindowText("Chihaya is God");
}
void CCDxStart(){
	Handle = LoadGraph("HelloWorld.png");
	BackHandle = LoadGraph("wing.jpg");
	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
	MHandle = Get_m_dxlib()->LoadSoundMem("res/twilightsky.mp3");
	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	SHandle = Get_m_dxlib()->LoadSoundMem("res/Attack.mp3");
	Get_m_dxlib()->PlaySoundMem(MHandle,DX_PLAYTYPE_BACK);

}
void CCDxLoop(float deltaTime){
	ClearDrawScreen();
	DrawExtendGraph(0, 0, 720, 540, BackHandle);
	if (CheckHitKey(KEY_INPUT_RIGHT))xx +=3;
	if (CheckHitKey(KEY_INPUT_LEFT ))xx -= 3;
	if (CheckHitKey(KEY_INPUT_DOWN ))yy += 3;
	if (CheckHitKey(KEY_INPUT_UP   ))yy -= 3;
	int mx, my;
	Get_m_dxlib()->GetMousePoint(&mx, &my);
	if (CheckHitKey(KEY_INPUT_A))Get_m_dxlib()->PlaySoundMem(SHandle, DX_PLAYTYPE_LOOP);
	if (CheckHitKey(KEY_INPUT_S))SetMouseDispFlag(FALSE);
	if (CheckHitKey(KEY_INPUT_D))SetMouseDispFlag(TRUE);

	if (CheckHitKey(KEY_INPUT_ESCAPE))DxLib_End();
	DrawRectGraph(xx, yy, 50, 50, 100, 100, Handle);
	DrawRectGraph(mx, my, 50, 50, 100, 100, Handle);

	DrawFormatString(0, 0, GetColor(0, 222, 222), "%d",mx);
	DrawFormatString(0, 30, GetColor(0, 222, 222), "%d", my);
	DrawFormatString(0, 60, GetColor(0, 222, 222), "%d", GetMouseInput());
}

//Font はGLCallも多く、よくない。
//Graphicは、GLCallは少ないが、毎回生成しているので遅い
//TopPositionFlag