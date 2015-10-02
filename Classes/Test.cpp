#include "DxLib.h"

//CCDxLib72のサンプル２

int HWhandle,BackGroundHandle;
int lh[12];
float xx = 0,yy = 0;
int MHandle ,SHandle;
int dd;

//DxLib_init()を呼ぶ前に宣言すべき関数郡はここで宣言する。
void AppDelegate::CCDxInit(){
	SetGraphMode(800, 600);             //画面サイズ設定
	ChangeWindowMode(TRUE);             //フルスクリーンにするか(TRUEなのでしない)
	SetMainWindowText("CCDxLib72");     //
	EMULATE_TOUCH_BY_MOUSEFUNCTIONS();  //タッチ操作でマウスの関数を反応できるように
	SetBackgroundColor(200, 100, 255);  //画面の初期色の設定
}

//ファイルの読み込みなどの初期処理はここで行う。
void CCDxStart(){
	
	//画像の読み込み
	HWhandle = LoadGraph("HelloWorld.png");
	BackGroundHandle = LoadGraph("wing.jpg");
	LoadDivGraph("HelloWorld.png",12,3,4,50,30,lh);
	
	
	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS);
	MHandle = Get_m_dxlib()->LoadSoundMem("res/twilightsky.mp3");
	Get_m_dxlib()->SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);
	SHandle = Get_m_dxlib()->LoadSoundMem("res/Attack.mp3");
	Get_m_dxlib()->PlaySoundMem(MHandle,DX_PLAYTYPE_BACK);

	//使用する仮想ボタンはここで宣言する
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_DOWN);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_UP);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_RIGHT);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_LEFT);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_T);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_R);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_A);
}


//毎フレーム呼ばれるゲームループ
void CCDxLoop(float deltaTime){
	//おなじみ
	ClearDrawScreen();
	
	//画像を描画したり、
	DrawExtendGraph(0, 0, 840, 600, BackGroundHandle);
	for (int h = 0,i = 0; h < 4; h++)
		for (int w = 0; w < 3; w++,i++)
			DrawGraph(xx * 5 + 53 * w, yy * 5 + 33 * h, lh[i]);
	
	//音楽を再生したり
	if (CheckHitKey(KEY_INPUT_A))PlaySoundMem(SHandle, DX_PLAYTYPE_BACK);
	if (CheckHitKey(KEY_INPUT_B))PlaySoundMem(MHandle, DX_PLAYTYPE_BACK);
	if (CheckHitKey(KEY_INPUT_C))StopSoundMem(MHandle);
	
	//Android ?
	if (CheckHitKey(KEY_INPUT_T))SetDrawArea(50, 100, 400, 200);
	if (CheckHitKey(KEY_INPUT_R))SetDrawArea(0, 0, 800, 600);

	//スマホの戻るボタン、もしくはESCキーで終了する
	if (CheckHitKey(KEY_INPUT_ESCAPE))DxLib_End();

	//キーボード操作も仮想ボタンでスマホで操作できる
	if (CheckHitKey(KEY_INPUT_RIGHT))xx += 1;
	if (CheckHitKey(KEY_INPUT_LEFT))xx -= 1;
	if (CheckHitKey(KEY_INPUT_DOWN))yy += 1;
	if (CheckHitKey(KEY_INPUT_UP))yy -= 1;
	if (CheckHitKey(KEY_INPUT_R)) DrawRectGraph(xx, yy, 0, 0, 72, 72, HWhandle);
	else DrawGraph(xx, yy, HWhandle);


	//マウス操作をスマホならタッチで操作できる
	int mx = 0, my = 0;
	GetMousePoint(&mx, &my);
	DrawRotaGraph(mx, my,1.0+mx/1000.0,my/3.14159, HWhandle);

}


//Music ::
//TopPositionFlag
//SoundEffect * DX_PLAYTYPE_LOOP 