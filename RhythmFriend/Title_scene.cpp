//======================================================================================//
//
//							ゲームプログラム
//
//======================================================================================//
#include		<windows.h>
#include		<stdio.h>
#include		<math.h>
#include		"MASTER/Ci-Lib.H"
#include		"Game.H"

//****************************************************************************
//	■	定義
//
//****************************************************************************

#define    MONSTERMAX    44
#define    TEXTMAX        4

GsTEXTURE	TITLE;
GsTEXTURE   STARt;
GsTEXTURE	MONSTER_T;
GsTEXTURE	SENTAKU;//選択画面
GsTEXTURE	SENTAKU2;//選択画面
GsTEXTURE	SENTAKU3;//選択画面
GsTEXTURE   YUBI;
GsTEXTURE   UI_MOZI;
GsTEXTURE   M_YAZIRUSI;
GsTEXTURE   YOMIKOMI_TITLE;

//GsTEXTURE	CLOCK03;
GsSOUND     T_BGM;
GsSOUND     OK_SE;
GsSOUND     SENTAKU_SE;
GsSOUND     SEfe;
GsSOUND     BACK_SE;

//****************************************************************************
//	■	内部使用　変数
//
//****************************************************************************


typedef struct{
	float  Px,Py;
	short    Alp;
	float    Scl;
	bool     Flg;
}TITL;

typedef struct{
	short    Alp;
	float    Scl;
	bool     Flg;
}PUSH;

typedef struct{
	float    Px,Py;
}TEKI;

bool S_Flg;//スタートフラグ
short S_Cnt;
bool Lo_Flg;
bool M_Flg;//ミュージックフラグ
float S_Py;

TITL Title;
PUSH Push;
TEKI Teki[MONSTERMAX];


typedef struct{
	short  Fps;
	short  MiniFps;
	bool   Start;
	bool   Flg;
	bool  alp_Flg[320];//テキスト切り替え時エフェクト
	short alp[320];
	bool   G_Flg,T_Flg,S_Flg,Se_Flg,Back;
	short  Cnt;//切り替えエフェクト時のカウント
}CONTROL2;

typedef struct{
	short  Ang;
	short  Scl;
	bool   Start;
}UNKO;

typedef struct{
	float  Px,Py;
	float  Dx,Dy;
	bool   Flg;
	bool   S_Flg;
}TEXT;

CONTROL2 Control2;
UNKO  Titl;
TEXT  Text[TEXTMAX];


//--------------------------------------------------------------------------//
//	●	初期化		：シーンに移る時一回、変数の初期化
//
//--------------------------------------------------------------------------//

void    Control2Init(void)
{
	Control2.Flg = false;
	Control2.Fps  =  0;
	if(S_Flg){
		Control2.Fps = 59;
	}
	for(short i=0;i<TEXTMAX;i++){
		Text[i].Flg = false;
		if(!Text[i].Flg){
			Text[i].Px  = 640;
		}
	}
	for(short i=0;i<320;i++){
		Control2.alp_Flg[i] = false;
		Control2.alp[i] = NULL;
	}
	Control2.G_Flg = false;
	Control2.T_Flg = false;
	Control2.S_Flg = false;
	Control2.Se_Flg = false;
	Control2.Cnt = NULL;
}

void    JP_Init(void)
{
	Push.Alp = 255;
	Push.Scl = 0.5f;
	Push.Flg = false;
//	S_Flg = false;
	S_Cnt = NULL;
	S_Py = NULL;
	M_Flg = false;
	Lo_Flg = false;

}

void    TekiInit(void)
{
	for(short i=0;i<MONSTERMAX;i++){
		if(i%2 == 0){
			Teki[i].Px = 160;
		}
		if(i%2 == 1){
			Teki[i].Px = 480;
		}
		Teki[i].Py = F(128+i*512);
	}
}

void	TitleInit(void)
{
	Titl.Ang  =  -11;

	Control2Init();
	JP_Init();
	TekiInit();
}

//--------------------------------------------------------------------------//
//	●	内部処理	：シーン中の処理、現在のシーンで行う処理
//
//--------------------------------------------------------------------------//

void    ControlT_Loop(void){

	Control2.Fps ++;
	if(Control2.Fps == 60){
		Gs_PlayBGM(T_BGM);
		Control2.Flg = true;
	}//始め


	if(!Control2.G_Flg && !Control2.T_Flg && !Control2.S_Flg && !Control2.Se_Flg){
		if(S_Flg && S_Cnt >= 15){
			if(MsAREA(55,50,542,480)){
				if(!Text[0].S_Flg){
					Gs_PlaySE(SENTAKU_SE);
					Text[0].S_Flg = true;
				}
				if(KEY_LEFTtrg){
					Control2.G_Flg = true;//育成ゲームフラグ起動
				}
			}else{
				Text[0].S_Flg = false;
			}

			if(MsAREA(55,570,542,350)){
				if(!Text[1].S_Flg){
					Gs_PlaySE(SENTAKU_SE);
					Text[1].S_Flg = true;
				}
				if(KEY_LEFTtrg){
					Control2.T_Flg = true;//ふれんど手帳フラグ起動
				}
			}else{
				Text[1].S_Flg = false;
			}

			//if(MsAREA(55,760,342,150)){
			//	if(!Text[2].S_Flg){
			//		Gs_PlaySE(SENTAKU_SE);
			//		Text[2].S_Flg = true;
			//	}
			//	if(KEY_LEFTtrg){
			//		Control2.S_Flg = true;//サウンドテストフラグ起動
			//	}
			//}else{
			//	Text[2].S_Flg = false;
			//}

			//if(MsAREA(424,760,174,150)){
			//	if(!Text[3].S_Flg){
			//		Gs_PlaySE(SENTAKU_SE);
			//		Text[3].S_Flg = true;
			//	}
			//	if(KEY_LEFTtrg){
			//		Control2.Se_Flg = true;//設定フラグ起動
			//	}
			//}else{
			//	Text[3].S_Flg = false;
			//}
		}
	}

	if(KEY_DOWNtrg){
		S_Flg = true;
	}

	if(MsAREA(0,0,250,50)){
		if(Mouse.Trg & _lMOUSE){
			if(Text[1].Px == 0){
				Gs_PlaySE(BACK_SE);
				S_Flg = false;
				S_Cnt = NULL;
			}
		}
		if(Text[1].Px == 0){
			if(!Control2.Back){
				Gs_PlaySE(SENTAKU_SE);
				Control2.Back = true;
			}
		}
	}else{//戻るボタン
		Control2.Back = false;
	}


	if(S_Flg){
		S_Cnt ++;
	}

	if(Control2.G_Flg || Control2.T_Flg || Control2.S_Flg || Control2.Se_Flg){
		if(KEY_LEFTtrg){
			Gs_PlaySE(SEfe);
			Gs_StopMUSIC(T_BGM);
		}
		Control2.Cnt += 8;
	}
	for(short i=0;i<320;i++){
		if(i <= Control2.Cnt){
			Control2.alp_Flg[i] = true;
			Control2.alp[i] += 10;
		}
	}
	if(Control2.Cnt >= 960){
		if(Control2.G_Flg){
			SceneChange(SENTAKU_SCENE);
		}
		if(Control2.T_Flg){
			SceneChange(TETYOU_SCENE);
		}
		if(Control2.Se_Flg){
			SceneChange(SETTEI_SCENE);
		}
	}

	for(short i=0;i<320;i++){
		LIMIT(Control2.alp[i],0,255);
	}

	//DEB_TEXT("Px = %d",Mouse.Px);
	//DEB_TEXT("Py = %d",Mouse.Py);
	//DEB_TEXT("Flg = %d",S_Flg);
}

void    JP_Loop(void)
{

	if(!Push.Flg){
		Push.Alp -= 5;
		Push.Scl -= 0.0004f;
	}else{
		Push.Alp += 5;
		Push.Scl += 0.0004f;
	}

	if(Push.Alp == 75){
		Push.Flg = true;
	}
	if(Push.Alp == 255){
		Push.Flg = false;
	}

	if(!S_Flg && (Mouse.Trg & _lMOUSE)){
		S_Flg = true;
		Gs_PlaySE(OK_SE);
	}
	if(S_Flg && !Lo_Flg){
		Lo_Flg = true;
	}

}

void    TekiLoop(void)
{
	for(short i=0;i<MONSTERMAX;i++){
		Teki[i].Py -= 1;
	}
	if(Titl.Ang >= 9){
		Titl.Start = true;
	}
	if(Titl.Ang <= -9){
		Titl.Start = false;
	}
	if(Control2.Fps %2 == 0){
	if(Titl.Start){
			Titl.Ang --;
		}else{
			Titl.Ang ++;
		}
	}
}

void    TextLoop(void){

	for(short i=0;i<TEXTMAX;i++){
		Text[i].Px += Text[i].Dx;
		Text[i].Py += Text[i].Dy;
		if(S_Flg){
			Text[i].Flg = true;
		}else{
			Text[i].Flg = false;
		}
	
		if(Text[i].Flg){
			Text[i].Dx = -40;
		}else{
			Text[i].Dx = 40;
		}

		LIMIT(Text[i].Px,0,640);
	}
}

// ゲームメインループ
void	TitleLoop(void)
{
	if(Control2.Flg){
		JP_Loop();
		TekiLoop();
	}

	ControlT_Loop();
	TextLoop();
	//DEB_TEXT("w = %f",Text[0].Px);
	
}
                       
//--------------------------------------------------------------------------//
//	●	描画		：シーン中の描画、現在のシーンで表示するBMPはここで
//
//--------------------------------------------------------------------------//
// ゲーム画面　描画
void	TitleDraw(void)
{
	Gs_DrawBox(0,0,WINW,WINH,ARGB(255,255,255,204),true);//背景
	//Gs_DrawBox(0,0,WINW,WINH,ARGB(255,0,0,0),true);//デバッグ用背景
	for(short i=0;i<MONSTERMAX;i++){
		if(Teki[i].Py >= -512 && Teki[i].Py <= 960 + 512){
			Gs_DrawLayer(I(Teki[i].Px),I(Teki[i].Py),MONSTER_T,512*(i/11),512*(i%11),512,512,false,ARGB(50,255,255,255),true,0,1.2f,1.2f);//ふれんど
		}
	}



	if(!S_Flg){
		if(Text[0].Px == 640){
			Gs_DrawLayer(WINW/2,WINH/4,TITLE,0,0,1024,512,false,ARGB(255,255,255,255),true,Titl.Ang,0.6f,0.6f);//タイトル
			Gs_DrawLayer(WINW/2,WINH/4,TITLE,0,0,1024,512,false,ARGB(255,255,255,255),true,Titl.Ang,0.6f,0.6f);
			Gs_DrawLayer(WINW/2,I(780-S_Py),STARt,0,0,1024,256,false,ARGB(Push.Alp,255,255,255),true,0,Push.Scl,Push.Scl);//プッシュスタート
		}
	}

	if(Control2.Fps <= 44 && !S_Flg){
		Gs_DrawBox(0,0,WINW,WINH,ARGB(255,255,255,255),true);//初回起動時、画面を隠す
	}



	Gs_DrawLayer(I(0-Text[0].Px),0,SENTAKU3,0,0,640,550,false,ARGB(255,255,255,255),false,0,1,1);//育成ゲーム
	if(Text[0].S_Flg){
		Gs_DrawLayer(I(0-Text[0].Px),0,SENTAKU3,0,0,640,550,false,ARGB(40,30,0,0),false,0,1,1);//育成ゲーム
		//Gs_DrawBox(57,57,583,423,ARGB(64,255,255,255),true);//背景
	}

	Gs_DrawLayer(I(0+Text[1].Px),570,SENTAKU3,0,560,640,480,false,ARGB(255,255,255,255),false,0,1,1);//ふれんど手帳
	if(Text[1].S_Flg){
		Gs_DrawLayer(I(0+Text[1].Px),570,SENTAKU3,0,560,640,480,false,ARGB(40,30,0,0),false,0,1,1);//ふれんど手帳
	}

	//Gs_DrawLayer(I(0-Text[2].Px),740,SENTAKU3,0,740,420,220,false,ARGB(255,255,255,255),false,0,1,1);//サウンドテスト
	//if(Text[2].S_Flg){
	//	Gs_DrawLayer(I(0-Text[2].Px),740,SENTAKU3,0,740,420,220,false,ARGB(40,30,0,0),false,0,1,1);//サウンドテスト
	//}

	//Gs_DrawLayer(I(420+Text[3].Px),740,SENTAKU3,420,740,220,220,false,ARGB(255,255,255,255),false,0,1,1);//設定
	//if(Text[3].S_Flg){
	//	Gs_DrawLayer(I(420+Text[3].Px),740,SENTAKU3,420,740,220,220,false,ARGB(40,30,0,0),false,0,1,1);//設定
	//}

	if(!Text[0].S_Flg){
		Gs_DrawLayer(I(0-Text[0].Px),0,SENTAKU,0,0,640,450,false,ARGB(255,255,255,255),false,0,1,1);//育成ゲーム
	}else{
		Gs_DrawLayer(I(0-Text[0].Px),0,SENTAKU2,0,0,640,450,false,ARGB(255,255,255,255),false,0,1,1);//育成ゲーム
	}

	if(!Text[1].S_Flg){
		Gs_DrawLayer(I(0+Text[1].Px),475,SENTAKU,0,460,640,480,false,ARGB(255,255,255,255),false,0,1,1);//ふれんど手帳
	}else{
		Gs_DrawLayer(I(0+Text[1].Px),475,SENTAKU2,0,460,640,480,false,ARGB(255,255,255,255),false,0,1,1);//ふれんど手帳
	}

	//if(!Text[2].S_Flg){
	//	Gs_DrawLayer(I(0-Text[2].Px),740,SENTAKU,0,740,420,220,false,ARGB(255,255,255,255),false,0,1,1);//サウンドテスト
	//}else{
	//	Gs_DrawLayer(I(0-Text[2].Px),740,SENTAKU2,0,740,420,220,false,ARGB(255,255,255,255),false,0,1,1);//サウンドテスト
	//}

	//if(!Text[3].S_Flg){
	//	Gs_DrawLayer(I(420+Text[3].Px),740,SENTAKU,420,740,220,220,false,ARGB(255,255,255,255),false,0,1,1);//設定
	//}else{
	//	Gs_DrawLayer(I(420+Text[3].Px),740,SENTAKU2,420,740,220,220,false,ARGB(255,255,255,255),false,0,1,1);//設定
	//}
	if(S_Flg){
		if(!Control2.Back){
			Gs_DrawLayer(32,14,UI_MOZI,0,0,512,50,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//タイトル画面へ戻る
			Gs_DrawLayer(18,26,M_YAZIRUSI,0,0,128,128,true,ARGB(192,255,255,255),true,0,0.25f,0.25f);//矢印
		}else{
			Gs_DrawLayer(32,14,UI_MOZI,0,0,512,50,false,ARGB(192,255,255,255),false,0,0.55f,0.55f);//タイトル画面へ戻る
			Gs_DrawLayer(18,26,M_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//矢印
		}
	}


	//画面切り替え時のエフェクト
	for(short i=0;i<320;i++){
		if(Control2.alp_Flg[i]){
			if(Control2.G_Flg){
				Gs_DrawBox(i,0,i+1,WINH,ARGB(Control2.alp[i],255,240,240),true);//赤
				Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(Control2.alp[i],255,240,240),true);//赤
			}
			if(Control2.T_Flg){
				Gs_DrawBox(i,0,i+1,WINH,ARGB(Control2.alp[i],240,255,240),true);//緑
				Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(Control2.alp[i],240,255,240),true);//緑
			}
			if(Control2.S_Flg){
				Gs_DrawBox(i,0,i+1,WINH,ARGB(Control2.alp[i],240,240,255),true);//青
				Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(Control2.alp[i],240,240,255),true);//青
			}
			if(Control2.Se_Flg){
				Gs_DrawBox(i,0,i+1,WINH,ARGB(Control2.alp[i],240,240,240),true);//灰
				Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(Control2.alp[i],240,240,240),true);//灰
			}
		}
	}



	if(Mouse.Px >= 20 && Mouse.Px <= 620 && Mouse.Py >= 20 && Mouse.Py <= 940){
		Gs_DrawLayer(Mouse.Px,Mouse.Py+32,YUBI,0,0,256,256,false,ARGB(255,255,255,255),true,0,0.25f,0.25f);//指
	}


	if(Control2.Cnt >= 820){
		Gs_DrawLayer(0,0,YOMIKOMI_TITLE,0,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//読み込み
	}
}
//--------------------------------------------------------------------------//
//	●	ロード		：シーンに入る時、そのシーンで必要なデータの読み込み
//
//--------------------------------------------------------------------------//
short	TitleLoad(void)
{

	TITLE	 = Gs_LoadBMP("DATA/BMP/title.PNG");
	MONSTER_T	 = Gs_LoadBMP("DATA/BMP/monster.PNG");
	YUBI  = Gs_LoadBMP("DATA/BMP/yubi.PNG");
	//CLOCK02  = Gs_LoadBMP("DATA/BMP/clock2.PNG");
	//CLOCK03  = Gs_LoadBMP("DATA/BMP/clock3.PNG");
	T_BGM      = Gs_LoadWAVE("DATA/SOUND/TITLEBGM.wav",true);
	OK_SE      = Gs_LoadWAVE("DATA/SOUND/ok_se.wav",false);
	SENTAKU_SE = Gs_LoadWAVE("DATA/SOUND/sentaku_se.wav",false);
	SEfe = Gs_LoadWAVE("DATA/SOUND/se.wav",false);
	BACK_SE = Gs_LoadWAVE("DATA/SOUND/back_se.wav",false);
	STARt	= Gs_LoadBMP("DATA/BMP/gamestart.PNG");
	SENTAKU	 = Gs_LoadBMP("DATA/BMP/sentakuc.PNG");
	SENTAKU2 = Gs_LoadBMP("DATA/BMP/sentakuc2.PNG");
	SENTAKU3 = Gs_LoadBMP("DATA/BMP/sentakuc3.PNG");
	UI_MOZI  = Gs_LoadBMP("DATA/BMP/UImozi.PNG");
	M_YAZIRUSI  = Gs_LoadBMP("DATA/BMP/矢印.PNG");
	YOMIKOMI_TITLE  = Gs_LoadBMP("DATA/BMP/yomikomi.PNG");


	return		TITLE_SCENE;
}

//--------------------------------------------------------------------------//
//	●	後処理		：現在のシーンから抜ける時、データの破棄
//
//--------------------------------------------------------------------------//
void	TitleExit(void)
{
	Gs_ReleaseBMP(STARt);
	Gs_ReleaseBMP(MONSTER_T);
	Gs_ReleaseBMP(YUBI);
	Gs_ReleaseBMP(SENTAKU);
	Gs_ReleaseBMP(SENTAKU2);
	Gs_ReleaseBMP(SENTAKU3);
	Gs_ReleaseBMP(UI_MOZI);
	Gs_ReleaseBMP(M_YAZIRUSI);
	Gs_ReleaseBMP(YOMIKOMI_TITLE);
	Gs_ReleaseSOUND(T_BGM);
	Gs_ReleaseSOUND(OK_SE);
	Gs_ReleaseSOUND(SEfe);
	Gs_ReleaseSOUND(SENTAKU_SE);
	Gs_ReleaseSOUND(BACK_SE);
}

//****************************************************************************
//	★　シーンテーブル：各シーンで呼び出される関数の登録
//****************************************************************************

SCENE_TBL	TitleSceneTbl = {
		TitleLoad,
		TitleInit,
		TitleLoop,
		TitleDraw,
		TitleExit
};

//======================================================================================//
//							EOP															//
//======================================================================================//
