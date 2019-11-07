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

GsTEXTURE	YUBI_T;
GsTEXTURE	MEISYOU;
GsTEXTURE	SETUMEI;
GsTEXTURE	SETUMEI2;
GsTEXTURE	SETUMEI3;
GsTEXTURE	MOKUZI;
GsTEXTURE	HATENA;
GsTEXTURE	SITAZI;
GsTEXTURE	UI_MOZI_T;
GsTEXTURE	T_YAZIRUSI;
GsTEXTURE	MONSTERs;
GsTEXTURE	YOMIKOMI_T;
GsSOUND	TETYOU_BGM;
GsSOUND	PERA;
GsSOUND	SENTAKU_SE2;
GsSOUND B_SE;

//****************************************************************************
//	■	内部使用　変数
//
//****************************************************************************

typedef struct{
	short  Fps;
	short  MiniFps;
	bool   Start;
	bool   alp_Flg[320];//テキスト切り替え時エフェクト
	short  alp[320];
	short  Cnt;//切り替えエフェクト時のカウント
	bool   Flg;
	bool   Back;
	short  B_Time;//戻る時間
	bool   B_Flg;
}CONTROLT;

typedef struct{
	bool   S_Flg;//このフラグが立つと項目が光る
	bool   Flg;
}CONTROLM;

typedef struct{
	short  Px,Py;
}FRIEND2;

typedef struct{
	short  type;//ふれんどの種類
	short  page;//現在のページ
}BOOK;

CONTROLT    ControlT;
CONTROLM    ControlM[11];
BOOK        Book;
FRIEND2      FRiend;

//--------------------------------------------------------------------------//
//	●	初期化		：シーンに移る時一回、変数の初期化
//
//--------------------------------------------------------------------------//
void	TetyouInit(void)
{
	Gs_PlayBGM(TETYOU_BGM);
	for(short i=0;i<320;i++){
		ControlT.alp_Flg[i] = false;
		ControlT.alp[i] = 255;
	}
	for(short i=0;i<11;i++){
		ControlT.Flg = false;
		ControlM[i].S_Flg = false;
		ControlM[i].Flg = false;
	}
	ControlT.Fps = 0;
	ControlT.Back = false;
	ControlT.B_Flg = false;
	ControlT.B_Time = 0;

	Book.type = 0;
	Book.page = 0;

	FRiend.Px = WINW/2;
	FRiend.Py = 150;


}

//--------------------------------------------------------------------------//
//	●	内部処理	：シーン中の処理、現在のシーンで行う処理
//
//--------------------------------------------------------------------------//
// ゲームメインループ
void	TetyouLoop(void)
{
	//DEB_TEXT("Px = %d",Mouse.Px);
	//DEB_TEXT("Py = %d",Mouse.Py);


	if(ControlT.Flg){
		//ページめくり(仮)
		if(KEY_LEFTtrg){
			Book.page ++;
			Gs_PlaySE(PERA);
			Gs_PlaySE(PERA);
		}
		if(Book.page >= 4){
			Book.page = 0;
			//Book.type ++;
		}
	}//補足：この処理を先に行わないとおかしくなる



	//入り初めのエフェクト
	ControlT.Fps += 8;
	for(short i=0;i<320;i++){
		if(ControlT.Fps >= 320 - i){
			ControlT.alp_Flg[i] = true;
		}
		if(ControlT.alp_Flg[i]){
			ControlT.alp[i] -= 10;
		}
		LIMIT(ControlT.alp[i],0,255);
	}


	//戻る時の処理
	if(MsAREA(0,0,200,50)){
		ControlT.Back = true;
		if(Mouse.Trg & _lMOUSE){
			Book.page = 0;
			FRiend.Py = 150;
			if(!ControlT.Flg){
				ControlT.B_Flg = true;
			}else{
				ControlT.Flg = false;
				for(short i=0;i<11;i++){
					ControlM[i].Flg = false;
				}
			}
		}
	}else{
		ControlT.Back = false;
	}


	if(ControlT.B_Flg){
		ControlT.B_Time ++;
	}
	if(ControlT.B_Time == 1){
		Gs_PlaySE(B_SE);
		Gs_StopMUSIC(TETYOU_BGM);
	}
	if(ControlT.B_Time == 20){
		SceneChange(TITLE_SCENE);
	}

	


	//目次
	if(!ControlT.Flg){
		for(short i=0;i<11;i++){
			if(MsAREA(0,80+80*i,640,80)){
				if(!ControlM[i].S_Flg){
					Gs_PlaySE(SENTAKU_SE2);
				}
				ControlM[i].S_Flg = true;
			}else{
				ControlM[i].S_Flg = false;
			}
		}
	}


	for(short i=0;i<11;i++){
		if(ControlM[i].S_Flg){//カーソルが何らかの目次の上に置いてある状態
			if(KEY_LEFTtrg){
				Gs_PlaySE(PERA);
				Gs_PlaySE(PERA);
				ControlM[i].Flg = true;
				ControlT.Flg = true;//何らかのページを開いたフラグ
			}
			if(ControlM[i].Flg){
			//ふれんど
				if(FRiend.Py <= WINH/2-50){
					FRiend.Py += 20;
				}
				Book.type = i;
			}
		}
	}//もしもボタンを押したら対応したモンスターの説明文開始

	for(short i=0;i<11;i++){
		//DEB_TEXT("ControlT.Flg[i] = %d",ControlM[i].Flg);
	}

}              
//--------------------------------------------------------------------------//
//	●	描画		：シーン中の描画、現在のシーンで表示するBMPはここで
//
//--------------------------------------------------------------------------//
// ゲーム画面　描
void	TetyouDraw(void){

	Gs_DrawBox(0,0,WINW,WINH,ARGB(255,255,255,204),true);//背景
	//Gs_DrawBox(0,0,WINW,WINH,ARGB(255,0,0,0),true);//背景

	if(ControlT.Flg){
		Gs_DrawLayer(0,50,MEISYOU,Book.type*640,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//ふれんどの名称、裏地
		Gs_DrawLayer(0,50,SETUMEI,640*Book.page,Book.type*960,640,960,false,ARGB(255,255,255,255),false,0,1,1);//説明文
		if(Book.type >= 4){
			Gs_DrawLayer(0,50,SETUMEI2,640*Book.page,(Book.type-4)*960,640,960,false,ARGB(255,255,255,255),false,0,1,1);//説明文
		}
		if(Book.type >= 8){
			Gs_DrawLayer(0,50,SETUMEI3,640*Book.page,(Book.type-8)*960,640,960,false,ARGB(255,255,255,255),false,0,1,1);//説明文
		}
	}



	//if(!ControlT.Flg[0] && !ControlT.Flg[1] && !ControlT.Flg[2] && !ControlT.Flg[3] && !ControlT.Flg[4] && !ControlT.Flg[5] && !ControlT.Flg[6] && !ControlT.Flg[7] && !ControlT.Flg[8] && !ControlT.Flg[9] && !ControlT.Flg[10]){
	for(short i=0;i<11;i++){
		if(!ControlT.Flg){
			Gs_DrawLayer(0,50,HATENA,Book.type*640,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//ふれんどの名称、裏地
			Gs_DrawLayer(0,0,SITAZI,0,0,640,960,false,ARGB(255,245,245,245),false,0,1,1);//説明文

			for(short j=0;j<11;j++){
				if(ControlM[j].S_Flg){
					Gs_DrawBox(0,80+j*80,640,158+j*80,ARGB(255,255,255,170),true);
				}
			}//選択時光るやつ
			Gs_DrawLayer(0,0,MOKUZI,0,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//説明文
		}
	}


	if(!ControlT.Flg){
		if(!ControlT.Back){
			Gs_DrawLayer(32,14,UI_MOZI_T,0,50,512,50,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//メインメニューへ戻る
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(192,255,255,255),true,0,0.25f,0.25f);//矢印
		}else{
			Gs_DrawLayer(32,14,UI_MOZI_T,0,50,512,50,false,ARGB(192,255,255,255),false,0,0.55f,0.55f);//メインメニューへ戻る
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//矢印
		}
	}
	if(ControlT.Flg){
		if(!ControlT.Back){
			Gs_DrawLayer(32,14,UI_MOZI_T,0,100,512,50,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//目次へ戻る
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(192,255,255,255),true,0,0.25f,0.25f);//矢印
		}else{
			Gs_DrawLayer(32,14,UI_MOZI_T,0,100,512,50,false,ARGB(192,255,255,255),false,0,0.55f,0.55f);//目次へ戻る
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//矢印
		}

		if(Book.type == 0){
			Gs_DrawLayer(FRiend.Px-10,FRiend.Py,MONSTERs,512*Book.page,0,512,512,false,ARGB(255,255,255,255),true,0,0.4f,0.4f);//説明文
		}
		if(Book.type == 1){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-24,MONSTERs,512*Book.page,512*2,512,512,false,ARGB(255,255,255,255),true,0,0.5f,0.5f);//説明文
		}
		if(Book.type == 2){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-24,MONSTERs,512*Book.page,512*1,512,512,false,ARGB(255,255,255,255),true,0,0.5f,0.5f);//説明文
		}
		if(Book.type == 3){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*5,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//説明文
		}
		if(Book.type == 4){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*6,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//説明文
		}
		if(Book.type == 5){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*3,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//説明文
		}
		if(Book.type == 6){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*4,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//説明文
		}
		if(Book.type == 7){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*9,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//説明文
		}
		if(Book.type == 8){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*10,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//説明文
		}
		if(Book.type == 9){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*7,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//説明文
		}
		if(Book.type == 10){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*8,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//説明文
		}

		//Gs_DrawLayer(450,580,T_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//矢印
		//Gs_DrawLayer(600,580,T_YAZIRUSI,0,0,128,128,false,ARGB(255,255,255,255),true,0,0.3f,0.3f);//矢印
	}



	for(short i=0;i<320;i++){
		Gs_DrawBox(i,0,i+1,WINH,ARGB(ControlT.alp[i],240,255,240),true);//緑
		Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(ControlT.alp[i],240,255,240),true);//緑
	}

	if(Mouse.Px >= 20 && Mouse.Px <= 620 && Mouse.Py >= 20 && Mouse.Py <= 940){
		Gs_DrawLayer(Mouse.Px,Mouse.Py+32,YUBI_T,0,0,256,256,false,ARGB(255,255,255,255),true,0,0.25f,0.25f);//指
	}

	if(ControlT.B_Flg){
		Gs_DrawLayer(0,0,YOMIKOMI_T,0,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//読み込み
	}
}
//--------------------------------------------------------------------------//
//	●	ロード		：シーンに入る時、そのシーンで必要なデータの読み込み
//
//--------------------------------------------------------------------------//
short	TetyouLoad(void)
{
	YUBI_T  = Gs_LoadBMP("DATA/BMP/yubi.PNG");
	MEISYOU  = Gs_LoadBMP("DATA/BMP/meisyo.PNG");
	SETUMEI  = Gs_LoadBMP("DATA/BMP/setumei.PNG");
	SETUMEI2  = Gs_LoadBMP("DATA/BMP/setumei2.PNG");
	SETUMEI3  = Gs_LoadBMP("DATA/BMP/setumei3.PNG");
	MOKUZI  = Gs_LoadBMP("DATA/BMP/図鑑.PNG");
	HATENA  = Gs_LoadBMP("DATA/BMP/図鑑？.PNG");
	SITAZI  = Gs_LoadBMP("DATA/BMP/図鑑下地.PNG");

	MONSTERs  =  Gs_LoadBMP("DATA/BMP/monster.PNG");

	UI_MOZI_T  = Gs_LoadBMP("DATA/BMP/UImozi.PNG");
	T_YAZIRUSI  = Gs_LoadBMP("DATA/BMP/矢印.PNG");

	YOMIKOMI_T  = Gs_LoadBMP("DATA/BMP/yomikomi.PNG");
	TETYOU_BGM      = Gs_LoadWAVE("DATA/SOUND/t_bgm.wav",true);
	PERA      = Gs_LoadWAVE("DATA/SOUND/pera.wav",false);
	SENTAKU_SE2 = Gs_LoadWAVE("DATA/SOUND/sentaku_se.wav",false);
	B_SE = Gs_LoadWAVE("DATA/SOUND/back_se.wav",false);
	return		TETYOU_SCENE;
}

//--------------------------------------------------------------------------//
//	●	後処理		：現在のシーンから抜ける時、データの破棄
//
//--------------------------------------------------------------------------//
void	TetyouExit(void)
{	
	Gs_ReleaseBMP(YUBI_T);
	Gs_ReleaseBMP(MEISYOU);
	Gs_ReleaseBMP(SETUMEI);
	Gs_ReleaseBMP(SETUMEI2);
	Gs_ReleaseBMP(SETUMEI3);
	Gs_ReleaseBMP(MOKUZI);
	Gs_ReleaseBMP(HATENA);
	Gs_ReleaseBMP(SITAZI);
	Gs_ReleaseBMP(UI_MOZI_T);
	Gs_ReleaseBMP(T_YAZIRUSI);
	Gs_ReleaseBMP(MONSTERs);
	Gs_ReleaseBMP(YOMIKOMI_T);
	Gs_ReleaseSOUND(TETYOU_BGM);
	Gs_ReleaseSOUND(PERA);
	Gs_ReleaseSOUND(SENTAKU_SE2);
	Gs_ReleaseSOUND(B_SE);
}

//****************************************************************************
//	★　シーンテーブル：各シーンで呼び出される関数の登録
//****************************************************************************

SCENE_TBL	TetyouSceneTbl = {
		TetyouLoad,
		TetyouInit,
		TetyouLoop,
		TetyouDraw,
		TetyouExit
};

//======================================================================================//
//							EOP															//
//============================================================