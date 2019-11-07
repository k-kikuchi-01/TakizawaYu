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

#define    CDMAX        5
GsTEXTURE	CD;
GsTEXTURE	YUBI_SENTAKU;//指
GsTEXTURE   MUSICNAME;//曲名
GsTEXTURE	SENTAKU_MOZI;
GsTEXTURE	SENTAKU_YAZIRUSI;
GsTEXTURE	SENTAKU_YOMIKOMI;
GsTEXTURE   MUSIC_S;
GsTEXTURE   YOMIKOMI_SEN;
GsSOUND     HASIRE;//走れ
GsSOUND     ROSUTOWAN;//ロストワン
GsSOUND     PLASTICSKY;//Plastic Sky
GsSOUND     IALFUNCLUB;//いーあるふぁんくらぶ
GsSOUND     SENBONZAKURA;//千本桜
GsSOUND     BAAN;//baan
GsSOUND     BB_SE;//戻る
GsSOUND     SENTAKU_SE3;//選択
GsSOUND     KETTEI;//選択
//****************************************************************************
//	■	内部使用　変数
//
//****************************************************************************

short alp;
bool  alp_Flg;

typedef struct{
	bool   alp_Flg[320];//テキスト切り替え時エフェクト
	short  alp[320];
	short  Cnt;//切り替えエフェクト時のカウント
	short  Fps;
}CONTROLSEN;

typedef struct{
	short  Cnt;//CDが動く
	short  Num;//CD番号の管理
	bool   Move_LFlg,Move_RFlg;
	bool   M_Flg;
	bool   Back;
	bool   B_Flg,HIDARI,MIGI;
	short  B_Cnt;
	bool   S_Flg;
	bool   Z_Flg;
	short  S_Cnt;
	short  S_alp;//スタートを押すと作動
}CONTROL4;

typedef struct{
	float  Px,Py;
	float  Dx,Dy;
	short  Num;
	short  Ang,D_Ang;
	short  Cnt;
	bool   Flg;
	float  Scl;//CDの拡大値
	short  S_alp;
}CDD;

short Select_Music;
short F_alp;

CONTROLSEN  Controlsen;
CDD  Cd[CDMAX];
CONTROL4    Control;

//--------------------------------------------------------------------------//
//	●	初期化		：シーンに移る時一回、変数の初期化
//
//--------------------------------------------------------------------------//
void	SentakuInit(void)
{
	F_alp = 0;
	for(short i=0;i<320;i++){
		Controlsen.alp_Flg[i] = false;
		Controlsen.alp[i] = 255;
	}
	Controlsen.Cnt = 0;
	Controlsen.Fps = 0;
	alp = 254;
	alp_Flg = false;

	for(short i=0;i<CDMAX;i++){
		Cd[i].Px = F(WINW/2 + i * 320);
		Cd[i].Py = 480;
		Cd[i].Ang = 0;
		Cd[i].D_Ang = 0;
		Cd[i].Scl = 1.25f;
		Cd[i].S_alp = 0;
	}
	Cd[4].Px = WINW/2 - 320;
	Control.Num = 0;
	Control.Cnt = 0;
	Control.Move_LFlg = false;
	Control.Move_RFlg = false;
	Control.M_Flg = false;
	Control.Back = false;
	Control.B_Flg = false;
	Control.B_Cnt = 0;
	Control.S_Flg = false;
	Control.Z_Flg = false;
	Control.S_Cnt = 0;
	Control.S_alp = 0;
	Gs_PlayBGM(HASIRE);
}

//--------------------------------------------------------------------------//
//	●	内部処理	：シーン中の処理、現在のシーンで行う処理
//
//--------------------------------------------------------------------------//
// ゲームメインループ
void	SentakuLoop(void)
{

	//CD関連
	for(short i=0;i<CDMAX;i++){
		if(Cd[0].Px >= 1600){
			 Cd[i].Px = F(WINW/2-320 + i * 320);
		}
		Cd[i].Px += Cd[i].Dx;

		if(Control.Num == i){
			Cd[i].Flg = true;
		}else{
			Cd[i].Flg = false;
		}
		if(Cd[i].Flg){
			Cd[i].Ang += Cd[i].D_Ang;
			Cd[i].D_Ang ++;//CDを加速させる

			if(!Control.S_Flg){
				Cd[i].Scl = 1.25f;
			}
			if(!Control.S_Flg){
				LIMIT(Cd[i].D_Ang,0,40);
			}else{
				LIMIT(Cd[i].D_Ang,0,100);
			}
		}else{
			if(!Control.S_Flg){
				Cd[i].Scl = 0.75f;
			}
			Cd[i].Ang = 0;
			Cd[i].D_Ang = 0;

		}
	}

	if(!Control.S_Flg){
		//入り初めのエフェクト
		Controlsen.Fps += 8;
		for(short i=0;i<320;i++){
			if(Controlsen.Fps >= 320 - i){
				Controlsen.alp_Flg[i] = true;
			}
			if(Controlsen.alp_Flg[i]){
				Controlsen.alp[i] -= 10;
			}
			LIMIT(Controlsen.alp[i],0,255);
		}

		//矢印
		if(MsAREA(0,300,175,960-620)){
			Control.HIDARI = true;
		}else{
			Control.HIDARI = false;
		}
		if(MsAREA(480,300,190,960-620)){
			Control.MIGI = true;
		}else{
			Control.MIGI = false;
		}
		if(MsAREA(0,660,640,300)){
			if(!Control.Z_Flg){
				Gs_PlaySE(SENTAKU_SE3);
				Gs_PlaySE(SENTAKU_SE3);
				Control.Z_Flg = true;
			}
		}else{
			Control.Z_Flg = false;
			alp = 254;
			alp_Flg = false;
		}
		if(Control.Z_Flg && !alp_Flg){
			alp -= 4;
		}
		if(alp <= 128){
			alp_Flg = true;
		}
		if(Control.Z_Flg && alp_Flg){
			alp += 4;
		}
		if(alp >= 254){
			alp_Flg = false;
		}


		if(KEY_LEFTtrg){
			if(MsAREA(0,300,175,960-620)){
				if(!Control.M_Flg){
				Gs_PlaySE(BAAN);
				switch(Control.Num){
					case(1):{
						Gs_PlayBGM(HASIRE);
					}break;
					case(2):{
						Gs_PlayBGM(PLASTICSKY);
					}break;
					case(3):{
						Gs_PlayBGM(ROSUTOWAN);
					}break;
					case(4):{
						Gs_PlayBGM(IALFUNCLUB);
					}break;
					case(0):{
						Gs_PlayBGM(SENBONZAKURA);
					}break;
				}
				Control.Num --;
				Control.Move_LFlg = true;
				Control.M_Flg = true;
				}
			}

			if(MsAREA(480,300,190,960-620)){
				if(!Control.M_Flg){
				Gs_PlaySE(BAAN);
				switch(Control.Num){
					case(4):{
						Gs_PlayBGM(HASIRE);
					}break;
					case(0):{
						Gs_PlayBGM(PLASTICSKY);
					}break;
					case(1):{
						Gs_PlayBGM(ROSUTOWAN);
					}break;
					case(2):{
						Gs_PlayBGM(IALFUNCLUB);
					}break;
					case(3):{
						Gs_PlayBGM(SENBONZAKURA);
					}break;
				}
				Control.Num ++;
				Control.Move_RFlg = true;
				Control.M_Flg = true;
			}
			}
			if(MsAREA(0,640,640,320)){
				Control.S_Flg = true;//曲が始まるフラグが立つ
			}
		}
		if(Control.Move_LFlg || Control.Move_RFlg){
			Control.Cnt ++;
		}
		for(short i=0;i<CDMAX;i++){
			//左側へ移動
			if(Control.Move_LFlg){

				if(Control.Cnt >= 1 && Control.Cnt < 16){
					Cd[i].Dx += 1.422f;
				}
				if(Control.Cnt >= 16 && Control.Cnt < 31){
					Cd[i].Dx -= 1.422f;
				}
			}

			//右側へ移動
			if(Control.Move_RFlg){
				if(Control.Cnt >= 1 && Control.Cnt < 16){
					Cd[i].Dx -= 1.422f;
				}
				if(Control.Cnt >= 16 && Control.Cnt < 31){
					Cd[i].Dx += 1.422f;
				}
			}

			if(Control.Cnt >= 31){
				for(short i=0;i<CDMAX;i++){
					Cd[i].Px = F(WINW/2 + i*320 + -Control.Num*320);
				}
				Control.Move_LFlg = false;
				Control.Move_RFlg = false;
				Control.Cnt = 0;
				Control.M_Flg = false;
			}
		}

		if(Control.Num >= 5){
			Control.Num = 0;
		}
		if(Control.Num <= -1){
			Control.Num = 4;
		}


		//戻るボタン
		if(MsAREA(0,0,200,50)){
			Control.Back = true;
			if(Mouse.Trg & _lMOUSE){
				Control.B_Flg = true;
			}
		}else{
			Control.Back = false;
		}
		if(Control.B_Flg){
			Control.B_Cnt ++;
		}
		if(Control.B_Cnt == 1){
			Gs_PlaySE(BB_SE);
			Gs_StopMUSIC(HASIRE);
			Gs_StopMUSIC(PLASTICSKY);
			Gs_StopMUSIC(ROSUTOWAN);
			Gs_StopMUSIC(IALFUNCLUB);
			Gs_StopMUSIC(SENBONZAKURA);
		}
		if(Control.B_Cnt >= 20){
			SceneChange(TITLE_SCENE);
		}
	}
	
	LIMIT(Control.S_alp,0,255);
	for(short i=0;i<CDMAX;i++){
		if(Control.S_Flg && !(i == Select_Music)){
			Cd[i].S_alp += 5;
			LIMIT(Cd[i].S_alp,0,255);
		}
		
		if(Control.S_Cnt >= 60){
			Cd[i].Scl += 0.5f;
			F_alp += 2;
			LIMIT(F_alp,0,255);
			if(i == Select_Music){
				Cd[i].S_alp += 15;
				LIMIT(Cd[i].S_alp,0,255);
			}
		}


	}

	if(Control.S_Cnt == 1){
		Gs_PlayBGM(KETTEI);
		Gs_PlaySE(KETTEI);
		Gs_PlaySE(KETTEI);
	}

	if(Control.S_Flg){
		Control.S_Cnt ++;
		Control.S_alp += 5;
		LIMIT(Control.S_alp,0,255);
	}

	if(Control.S_Cnt >= 140){
		SceneChange(GAME_SCENE);
	}

	/////////////////////////////////
	Select_Music = Control.Num;
	////////////////////////////////

	//DEB_TEXT("Control.Num = %d",Control.Num); 

}              
//--------------------------------------------------------------------------//
//	●	描画		：シーン中の描画、現在のシーンで表示するBMPはここで
//
//--------------------------------------------------------------------------//
// ゲーム画面　描画
void	SentakuDraw(void){
	Gs_DrawBox(0,0,WINW,WINH,ARGB(255,255,255,255),true);//背景
	Gs_DrawBox(0,0,WINW,WINH,ARGB(96,221,153,137),true);//背景
	//Gs_DrawBox(0,0,WINW,WINH,ARGB(255,0,0,0),true);//背景
	Gs_DrawBox(0,300,640,660,ARGB(255-Control.S_alp,140,140,140),true);//赤
	for(short i=0;i<CDMAX;i++){
		Gs_DrawLayer(I(Cd[i].Px-1600),I(Cd[i].Py),CD,256*i,0,256,256,false,ARGB(255-Cd[i].S_alp,255,255,255),true,Cd[i].Ang,Cd[i].Scl,Cd[i].Scl);//走れ
		Gs_DrawLayer(I(Cd[i].Px),I(Cd[i].Py),CD,256*i,0,256,256,false,ARGB(255-Cd[i].S_alp,255,255,255),true,Cd[i].Ang,Cd[i].Scl,Cd[i].Scl);//走れ
		Gs_DrawLayer(I(Cd[i].Px+1600),I(Cd[i].Py),CD,256*i,0,256,256,false,ARGB(255-Cd[i].S_alp,255,255,255),true,Cd[i].Ang,Cd[i].Scl,Cd[i].Scl);//走れ
	}

	if(!Control.S_Flg){
		if(!Control.Z_Flg){
			Gs_DrawBox(0,660,640,960,ARGB(128,255,255,255),true);//裏地
			Gs_DrawLayer(WINW/2,810,MUSIC_S,0,0,1024,128,false,ARGB(128,255,255,255),true,0,0.55f,0.55f);//ミュージックスタート
		}else{
			Gs_DrawBox(0,660,640,960,ARGB(128,255,255,204),true);//裏地
			Gs_DrawLayer(WINW/2,810,MUSIC_S,0,0,1024,128,false,ARGB(alp,255,255,255),true,0,0.55f+alp*0.0002f,0.55f+alp*0.0002f);//ミュージックスタート
		}
	}

	Gs_DrawLayer(0,50,MUSICNAME,0,Control.Num * 250,640,250,false,ARGB(255-Control.S_alp,255,255,255),false,0,1,1);//曲名


	if(!Control.S_Flg){
		if(!Control.Back){
			Gs_DrawLayer(32,14,SENTAKU_MOZI,0,50,512,50,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//目次へ戻る
			Gs_DrawLayer(18,26,SENTAKU_YAZIRUSI,0,0,128,128,true,ARGB(192,255,255,255),true,0,0.25f,0.25f);//矢印
		}else{
			Gs_DrawLayer(32,14,SENTAKU_MOZI,0,50,512,50,false,ARGB(192,255,255,255),false,0,0.55f,0.55f);//目次へ戻る
			Gs_DrawLayer(18,26,SENTAKU_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//矢印
		}//UI(戻るボタン)

		if(Control.HIDARI){
			Gs_DrawLayer(50,480,SENTAKU_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.6f,0.6f);//左矢印
		}else{
			Gs_DrawLayer(50,480,SENTAKU_YAZIRUSI,0,0,128,128,true,ARGB(150,255,255,255),true,0,0.4f,0.4f);//左矢印
		}

		if(Control.MIGI){
			Gs_DrawLayer(590,480,SENTAKU_YAZIRUSI,0,0,128,128,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//右矢印
		}else{
			Gs_DrawLayer(590,480,SENTAKU_YAZIRUSI,0,0,128,128,false,ARGB(150,255,255,255),true,0,0.4f,0.4f);//右矢印
		}
	}

	if(Control.B_Flg){
		Gs_DrawLayer(0,0,SENTAKU_YOMIKOMI,0,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//目次へ戻る
	}

	for(short i=0;i<320;i++){
		Gs_DrawBox(i,0,i+1,WINH,ARGB(Controlsen.alp[i],255,240,240),true);//赤
		Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(Controlsen.alp[i],255,240,240),true);//赤
	}

	if(Control.S_Flg){
		Gs_DrawBox(0,0,WINW,WINH,ARGB(F_alp,255,255,255),true);//背景
		if(Control.S_Cnt >= 120){
			Gs_DrawLayer(0,0,YOMIKOMI_SEN,0,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//読み込み
		}
	}

	if(Mouse.Px >= 20 && Mouse.Px <= 620 && Mouse.Py >= 20 && Mouse.Py <= 940){
		Gs_DrawLayer(Mouse.Px,Mouse.Py+32,YUBI_SENTAKU,0,0,256,256,false,ARGB(255,255,255,255),true,0,0.25f,0.25f);//指
	}
}
//--------------------------------------------------------------------------//
//	●	ロード		：シーンに入る時、そのシーンで必要なデータの読み込み
//
//--------------------------------------------------------------------------//
short	SentakuLoad(void)
{

	CD	 = Gs_LoadBMP("DATA/BMP/cd.PNG");
	YUBI_SENTAKU  = Gs_LoadBMP("DATA/BMP/yubi.PNG");
	MUSICNAME  = Gs_LoadBMP("DATA/BMP/曲選択.PNG");
	SENTAKU_MOZI  = Gs_LoadBMP("DATA/BMP/UImozi.PNG");
	SENTAKU_YAZIRUSI  = Gs_LoadBMP("DATA/BMP/矢印.PNG");
	SENTAKU_YOMIKOMI  = Gs_LoadBMP("DATA/BMP/yomikomi.PNG");
	MUSIC_S  = Gs_LoadBMP("DATA/BMP/music_s.PNG");
	YOMIKOMI_SEN  = Gs_LoadBMP("DATA/BMP/yomikomi.PNG");
	HASIRE    = Gs_LoadWAVE("DATA/SOUND/走れmini.wav",true);
	ROSUTOWAN    = Gs_LoadWAVE("DATA/SOUND/ロストワンの号哭mini.wav",true);
	PLASTICSKY    = Gs_LoadWAVE("DATA/SOUND/Plastic Skymini.wav",true);
	IALFUNCLUB    = Gs_LoadWAVE("DATA/SOUND/いーあるふぁんくらぶmini.wav",true);
	SENBONZAKURA    = Gs_LoadWAVE("DATA/SOUND/千本桜mini.wav",true);
	BAAN    = Gs_LoadWAVE("DATA/SOUND/pan.wav",false);
	BB_SE = Gs_LoadWAVE("DATA/SOUND/back_se.wav",false);//戻るボタン選択時の効果音
	SENTAKU_SE3 = Gs_LoadWAVE("DATA/SOUND/sentaku_se.wav",false);
	KETTEI = Gs_LoadWAVE("DATA/SOUND/piano.wav",false);

	return		SENTAKU_SCENE;
}

//--------------------------------------------------------------------------//
//	●	後処理		：現在のシーンから抜ける時、データの破棄
//
//--------------------------------------------------------------------------//
void	SentakuExit(void)
{
	Gs_ReleaseBMP(CD);
	Gs_ReleaseBMP(YUBI_SENTAKU);
	Gs_ReleaseBMP(MUSICNAME);
	Gs_ReleaseBMP(SENTAKU_MOZI);
	Gs_ReleaseBMP(SENTAKU_YAZIRUSI);
	Gs_ReleaseBMP(SENTAKU_YOMIKOMI);
	Gs_ReleaseBMP(MUSIC_S);
	Gs_ReleaseBMP(YOMIKOMI_SEN);
	Gs_ReleaseSOUND(ROSUTOWAN);
	Gs_ReleaseSOUND(PLASTICSKY);
	Gs_ReleaseSOUND(HASIRE);
	Gs_ReleaseSOUND(IALFUNCLUB);
	Gs_ReleaseSOUND(SENBONZAKURA);
	Gs_ReleaseSOUND(BAAN);
	Gs_ReleaseSOUND(BB_SE);
	Gs_ReleaseSOUND(SENTAKU_SE3);
	Gs_ReleaseSOUND(KETTEI);
}

//****************************************************************************
//	★　シーンテーブル：各シーンで呼び出される関数の登録
//****************************************************************************

SCENE_TBL	SentakuSceneTbl = {
		SentakuLoad,
		SentakuInit,
		SentakuLoop,
		SentakuDraw,
		SentakuExit
};

//======================================================================================//
//							EOP															//
//============================================================