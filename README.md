# CCDxLib72
Cocos2dxで動くDxLibのラッパーライブラリだよ！  
Classesの中身を自分のCocos2dxのClassesの中身に追加して使ってね！  
同梱のButtonExample.png は、Resourcesの中に入れてね！  
更新履歴：2015/10/06 RectGraph のバグ修正と描画の高速化


##概要
DxLibの関数をCocos2d-xで動かすライブラリCCDxlib72です  
Dxlibで書いたコードを殆ど変更せずそのままAndroidやMacなどに移植出来るライブラリです  
導入の仕方については 後の段落に記述してあります。  

##CCDxLib72で実装した関数一覧
一部のマイナー関数を除いて殆どの関数を実装しています。  
2015/10/4 03:17現在 72個    
- ウインドウ関係(6)
	- ` SetGraphMode,ChangeWindowMode,SetMainWindowText,GetScreenState SetDrawArea ClearDrawScreen `
- 画像描画関係(14)
	- ` LoadGraphScreen,LoadGraph,InitGraph,DeleteGraph DrawGraph,DrawTurnGraph,DrawExtendGraph,DrawRotaGraph DrawRotaGraph2,DrawRotaGraph3,DrawRectGraph,GetGraphSize DerivationGraph LoadDivGraph `
- 図形描画関係(8)
	- ` GetColor,DrawLine,DrawBox,DrawCircle,DrawOval,DrawTriangle,DrawPixel,GetPixel `
- 文字描画関係(3)
	- ` DrawString,DrawFormatString ,SetFontSize `
- 時間計測/乱数(5)
	- ` GetNowCount GetNowHiPerformanceCount GetDateTime GetRand SRand `
- ユーザーの入力操作関係(9)
	- ` CheckHitKeyAll CheckHitKey GetHitKeyStateAll SetMouseDispFlag GetMousePoint GetMouseInput GetMouseWheelRotVol GetTouchInputNum GetTouchInput `
- CPUに読み込んで画像を扱う系の関数(10)
	- ` LoadSoftImage GetSoftImageSize GetPixelSoftImage DeleteSoftImage InitSoftImage DrawSoftImage DrawPixelSoftImage MakeARGB8ColorSoftImage MakeXRGB8ColorSoftImage FillSoftImage `
- アプリのファイルの読み込み関数(10)
	- ` FileRead_open FileRead_size FileRead_close FileRead_tell FileRead_seek FileRead_read FileRead_eof FileRead_gets FileRead_getc FileRead_scanf `
- 音利用関数(7)
	- ` SetCreateSoundDataType LoadSoundMem,PlaySoundMem,CheckSoundMem StopSoundMem,DeleteSoundMem,InitSoundMem	`

- 追記
	- 加えて、CCDxLib72の描画は専用のTextureRendererに描画しているだけなので、ui::Buttonなど、好きにthis->addChild()することで、Cocos2dxの機能を利用することも可能です。  
	- 一部のマルチプラットフォームに対応するための後述の関数を除いて本家と同じように動かすことが出来ます。

##仕様
マルチプラットフォーム対応のため、呼び出す関数が少し変化しています。  
###ゲームの開始・終了の関数
- ` DxLib_Init ProcessMessage ScreenFlip SetDrawScreen(DX_SCREEN_BACK) DxLib_End SetTransColor ` を呼ぶ必要はありません。代わりに下記の関数を使用してください。
	- ` AppDelegate::Dx72Init() `
		- DxLib_init()を呼ぶ前に設定する関数類はここで呼び出して下さい。
	- ` Dx72Start `
		- DxLib_init()をした後の状態で、ゲーム開始時に一回だけ呼ばれます。
		- 通常は初期設定(ファイルの読み込みなど)をここで行います。
	- ` Dx72Loop `
		- ゲームのループです。毎フレームこの関数が呼び出されます。
		- この関数を抜けたあと毎回DxLibで言うところのProcessMessage()やScreenFlip() が呼び出されます。
	- ` DxLib_End `
		- 仕様を変更しました。
		- DxLibの利用を終了する関数ではなく、ゲームループ(Dx72Loop)を終了して、
		- ゲームを終了する関数として使用してください。
	- ` SetTransColor ` 
		- 透過色を指定したりはしないので、透過PNGを利用してください。
		- http://www.gigafree.net/tool/retouch/ToukaPNG.html
		- http://toktakaomi.hatenablog.com/entry/2015/02/24/004729
		- このようなソフトウェアで透過PNGをちゃちゃっと作成してください。
             
###マルチプラットフォーム用のファイル入出力の関数
- ` FileRead_ooooの関数 `
	- 標準のfopenでは、スマホアプリが圧縮されている関係でファイルをそのまま読み書きすることができません。そこで、このFileRead_~~の名前のある関数を使います。
	- この関数を使用することで、(Resourcesにいれてあった)同梱のファイルを読み込むことができるようになります。
- ` UserDatafopen `
	- fopenはスマホアプリの保存場所の関係で上手く動かないですので、永続させたいデータを保存したい場合、これを利用してください。
	- 使い方は普通のfopenと同じです。
	- UserDatafopenで開いた以降は通常通りにfwrite,freadやfseek fcloseなどをそのまま使用できます。
- ` getUserDataPath() `
	- UserDatafopenで扱う読み書き可能なファイルがある場所を返します。
	- exeファイルのすぐ近くとかではないので注意してください。
	- またこれを使って得たパスには標準入出力関数、(例えばifstream) とかも使用出来ます。

###マルチプラットフォーム用の新規エミュレート関数
- ` EMULATE_TOUCH_BY_MOUSEFUNCTIONS() `
	- マウス操作関数を呼び出してタッチ操作を使えます。
	- スマホでも(タッチ操作関数だけでなく)マウス操作関数で動かすことが出来ます。
- ` EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON() `
	- 仮想的にボタンを作成し、キーボード操作をエミュレート出来ます。
	- CheckHitKeyなどの入力関数をエミュレートできます。
	- 第一引数：KEY_INPUT_A ~ KEY_INPUT_Z などのエミュレートしたいKEY_INPUT_~~。
	- 第二引数以降を省略可能です。その場合、いい感じにボタンが配置されます。
	- 第二引数：読み込むボタンの画像の名前(付属のボタンは"ButtonExample.png"です)
	- 第三,四引数：ボタンの位置
	- (注)ボタンの画像を読み込むので、CCDxInit()の中ではなく、CCDxStartで宣言してくださ

##ヘルプ
- Mp3の効果音とBGMはきちんとわけましょう。
  - BGMを読み込む前には SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMPRESS)
  - SE を読み込む前には SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS)
  - ということをしないと、うまくAndroidでは起動しない可能性があります。
  - 目安として、10秒以上ならBGMではないでしょうか。
- Midi,BMPの拡張子は対応していないです。mp3やpng,jpegを使用してください。
- リリースビルドをすると速くなる可能性があります。`cocos compile -p android -m release`のコマンドとかを使いましょう。
- LoadSoftImageは一度に一回までです！注意してください！
- ソースコード中に日本語をいれている場合、そのソースコードがUTF-8でないと文字化けします。UTF-8で保存してください。
- Visual Studio では、文頭に `#pragma execution_character_set("utf-8")`を宣言すればUTF-8を使用できます。
- cocos2dx v3.8で作成しています。バージョンアップで動かなくなることもあるかもしれないので、v3.8を推奨します。
- 一度作成してしまえば、プロジェクト名を変更すれば、他のアプリとしてビルド可能です。
- cocosにあるファイル名と重複しないように注意しましょう。特にMain.cppなどはすでに存在します。
- UserDatafopenではフォルダは作成できません。注意してください
- UserDatafopenはCCDxLib72の関数なので、init後にしか使用できないことに注意してください


##CCDxLib72を利用してAndroidビルドする方法
- 入門サイトを見ながらcocosを入れる  
  http://wiki.xsqi.net/index.php?%A5%D7%A5%ED%A5%B0%A5%E9%A5%E0%2FCocos2d-x%2FCocos2d-x%B3%AB%C8%AF%B4%C4%B6%AD%A4%CE%B9%BD%C3%DB  
  このライブラリを利用するにはcocos2dx v3.8を利用してください。  

- cocos のプロジェクトを作成する。
	- ` cocos new -p com.samplegame -l cpp samplegame ` (AndroidのNDKやSDKのパスをセットしていれば、Eclipseを利用しなくて大丈夫です。)
	- Classesフォルダにコードを追加
	- Resourcesに画像や音楽ファイルを追加 (仮想ボタンとして使うButtonExample.pngもここに)
	- ClassesフォルダにいれたコードをAndroidで読み込む設定をします。proj.android のjniのAndroid.mkで  
~~~~    
LOCAL_SRC_FILES := hellocpp/main.cpp \
                 ../../Classes/AppDelegate.cpp \
                  ../../Classes/HelloWorldScene.cpp
~~~~    
  を  
~~~~
FILE_LIST := $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)
LOCAL_SRC_FILES := hellocpp/main.cpp
LOCAL_SRC_FILES += $(FILE_LIST:$(LOCAL_PATH)/%=%)                   
~~~~
  へ、変更してください。


- コードを少しリファクタリング  
~~~~
int WINAPI WinMain(HINSTANCE,HINSTANCE,LPSTR,int){
	
	SetWindowSize(800 , 600 );
	ChangeWindowMode(TRUE);
	DxLib_Init();
	SetDrawScreen( DX_SCREEN_BACK );
	
	Awake();
	
	while( ScreenFlip()==0 && ProcessMessage()==0 && ClearDrawScreen()==0 ){
		GameLoop();
	}
	
	DxLib_End();
	return 0;
} 
~~~~
  というロジックのコードを  
~~~~
void AppDelegate::CCDxInit(){
	SetGraphMode(800 , 600);
	ChangeWindowMode(TRUE);           
	//タッチ操作でマウスの関数を反応できるように  
	EMULATE_TOUCH_BY_MOUSEFUNCTIONS();
}

void CCDxStart(){
	Awake();
	//使用する仮想ボタンはここで宣言する
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_LEFT);
	EMULATE_KEYBOARD_BY_IMAGINARY_BUTTON(KEY_INPUT_Z);
}

void CCDxLoop(float deltaTime){
	ClearDrawScreen();
	GameLoop();
	if (CheckHitKey(KEY_INPUT_ESCAPE))DxLib_End();
}
~~~~
  に変更したら準備完了です！

- コンパイルします。  
  ` cocos compile -p android `

- これであなたのDxLibで書いたコードがAndroidでも動きます！

- 一度作成してしまえば、プロジェクト名を変更すれば、他のアプリとしてビルド可能です！  
~~~~    
proj.androidの
    build.xml の二行目 project name = ""
    AndroidManufest.xml の三行目 package= ""
~~~~
  を com.hogehoge みたいに変えましょう



##実装は可能なのでまた今度ので実装する予定の関数リスト
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
memo
#fontの種類変更したいなあ
#画面端をどうする…？
#TopPositionFlag
#oncaptureScreen may be good !!!

・EMULATE_KEYBOARD_ARROWS_BY_ACCELEROMETER ()
・ChangeFont （なぜかうまく動かない…）
・GetMouseInputLog	  (ややこしい)
・CreateGraphFromSoftImage     CPUで扱うイメージからグラフィックハンドルを作成する
  CreateDivGraphFromSoftImage	 →Texture->Sprite がうまくいかない。
・音利用関数の拡張
  PlayMovie
  ChangePanSoundMem,ChangeVolumeSoundMem
  ChangeNextPlayPanSoundMem,ChangeNextPlayVolumeSoundMem
  SetFrequencySoundMem,SetLoopPosSoundMem,SetLoopSamplePosSoundMem
  SetCurrentPositionSoundMem,DuplicateSoundMem
  PlaySoundFile,CheckSoundFile,StopSoundFile
  PlayMusic CheckMusic StopMusic SetVolumeMusic
・非同期読み込み
  SetUseASyncLoadFlag CheckHandleASyncLoad GetASyncLoadNum
・画像処理の拡張
  SetDrawMode SetDrawBlendMode SetDrawBright SaveDrawScreen
・デバッグ用
  SetOutApplicationLogValidFlag printfDx clsDx
・マスク関係関数
  CreateMaskScreen DeleteMaskScreen LoadMask LoadDivMask
  DrawMask DrawFillMask	DeleteMask InitMask	FillMaskScreen
  SetUseMaskScreenFlag MakeMask	GetMaskSize	SetDataToMask
  DrawMaskToDirectData DrawFillMaskToDirectData	

・日本語入力関連関数
  KeyInputString KeyInputSingleCharString KeyInputNumber
  SetKeyInputStringColor MakeKeyInput DeleteKeyInput 
  InitKeyInput SetActiveKeyInput CheckKeyInput
  DrawKeyInputString DrawKeyInputModeString	SetKeyInputString 
  SetKeyInputNumber	GetKeyInputString GetKeyInputNumber	

・CopyFileToUserDataPath()
  :同梱のファイルはスマホではFileRead_~~で読み込むことしかできません。
   しかし、UserDatafopenができる場所にファイルをコピーすれば、
   UserDatafopenができるようになります。
   この関数でUserDatafopenができる場所に同梱のファイルをコピーできます

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

##CCDxLib72で実装していない関数一覧
- (実装してくれる方を募集しています！！！！)
- 3D音楽関数,３D関数
- CPUで扱うイメージのパレット系関数
- 通信関係の関数（cocos本来のhttpの通信機能を利用した方が楽です。）
- 半角文字入力関連関数 (GetInputCharooooとかの関数)
- フルスクリーン関係の関数 (SetFullScreenooooとかの関数)
- Wait系関数 (Wait~~とかの関数)
- フォントハンドル(フォントデータ)関連の関数 (CreateFontToHandleなど)
- グラフィックデータの生成関連の関数
	- MakeGraph MakeScreen SetCreateDrawValidGraphMultiSample SetCreateGraphColorBitDepth SetDrawValidFloatTypeGraphCreateFlag SetCreateDrawValidGraphChannelNum SetUsePremulAlphaConvertLoad GetDrawScreenGraph	
- 画像のフィルター関連の関数 (GraphFilter GraphFilterBlt GraphFilterRectBlt)
- 画像のブレンド関連の関数 (GraphBlend LoadBlendGraph DrawBlendGraphなど)
- 画像として動画を扱う関数 (~MovieToGraph とかの関数)
- その他
	- LoadSoftImageToMem  BltSoftImage DrawModiGraph ScreenCopy SetMousePoint GetPixel SetWindowSizeChangeEnableFlag SetWindowSizeExtendRate SetWindowIconID GetColorBitDepth SetDrawScreen SetWindowIconID
- その他マイナー関数
	- SetUse3DFlag SetEmulation320x240 SetUseDXArchiveFlag SetDXArchiveExtension SetDXArchiveKeyString EnumFontName SetAlwaysRunFlag,LoadPauseGraph SelectMidiMode GetDrawStringWidth GetDrawFormatStringWidth SetFontThickness	CreateGraphFromMem LoadSoundMemByMemImage ReCreateGraphFromMem ReloadFileGraphAll SetRestoreGraphCallback SetUseDivGraphFlag
DrawVString	DrawVStringToHandle 