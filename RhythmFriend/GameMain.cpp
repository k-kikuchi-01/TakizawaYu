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
//#include		"Alldefine.H"

//****************************************************************************
//	■	定義
//
//****************************************************************************

#define    NULL         0
#define    TAMAMAX    300

//#define    ZURE       282//大きいほど出現速度があがる
#define    SCLSPEED   0.015f//玉が大きくなるスピード 
//#define    ZURE       250
//#define    ZURE       258//大きいほど出現速度があがる
#define    EFEMAX     36
#define    EFE_X      WINW/2
#define    EFE_Y      824
#define    RED          0
#define    BLUE         1
#define    YELLOW       2
#define    GREEN        3
#define    GAGEMAX    200
//#define    PLAY_BGM   BGM_02

//****************************************************************************
//	■	内部使用　変数
//
//****************************************************************************

GsTEXTURE	HAIKEI;
GsTEXTURE	TAMA;
GsTEXTURE	SEN;
GsTEXTURE	TAMA2;
GsTEXTURE	GAGE;
GsTEXTURE   GAGE2;
GsTEXTURE	UI_01;
GsTEXTURE	MONSTER;
GsTEXTURE	STAR;
GsTEXTURE   T_GAGEH;
GsTEXTURE   T_GAGEP;
GsTEXTURE   T_GAGE;
GsTEXTURE   YUBI_M;
GsTEXTURE   MOZIZI;//数字とか

GsSOUND   SUZU;
GsSOUND   BGM_01;
GsSOUND   BGM_02;
GsSOUND   BGM_03;
GsSOUND   BGM_04;
GsSOUND   BGM_05;
GsSOUND   BGM_06;
GsSOUND   SE1;

bool   HANA;//進化フラグ
short   HANACnt;
short  Palp;
short  Muzu;
float  PPx;

short  ZURE;
bool   End;//曲の終わり
short  End_Cnt;//カウンター
short  Black;//リザルト後


typedef struct{
	float    Px,Py;//x,y値の座標
	float    Dx,Dy;//おそらく使わない
	float    Scl,D_Scl;//リズム玉の大きさ
	bool     Flg;//このフラグが立つとリズム玉が出現する
	bool     Good,Bad;
	bool     Eat,Enjoy,Drill;
	bool     Miss_Flg;//このフラグが立つとミス
	short    Color;
	bool     TamaP_Flg;
	bool     E_Flg;
}OBJ;

typedef struct{
	short    Fps;//シーン全体の時間
	short    In_Fps;//ゲームが始まってからの時間
	bool     S_Flg;//このフラグがONになるとIn_Fpsが作動する
	float    Eat,Enjoy,Drill;//
	bool     Eat_Flg,Enjoy_Flg,Drill_Flg;
	short    Red,Blue,Yellow,Green;
	short    You,In;//You(陽)が赤黄、In(陰)が青緑
	short    TypeA,TypeB;//TypeAが赤青、TypeBが黄緑
}CONTROL;

typedef struct{
	float    Px,Py;
	float    Dx,Dy;
	float    Ang;//回転率
	float    DAng;
	short    Ang_Fps;
	short    Ang_Cnt;
	short    Move_Cnt;//動き(アニメーション)のカウンター
	bool     T_Flg/*食べる*/,K_Flg/*鍛える*/,A_Flg/*遊ぶ*/;
	short    Level;//モンスターの進化具合、0~3
	short    Type;//モンスターの系統
	short    Color;//カラーバリエーション
}FRIEND;

typedef struct{
	float    Px,Py;
	float    Dx,Dy;
	bool     Flg;//このフラグがONの間エフェクト作動
	float    Scl;//スケール値
	short    Ang;//角度
	short    alp;//透明値
}EFE;

typedef struct{
	float    Px,Py;
	short    gage;
}GAG;

typedef struct{
	float    Scl;
}UI;

typedef struct{
	bool    Flg;
	short   Cnt;
}DEB;


short   Cnter;
bool     Red,Blue,Green;
float   PPy;


/////////////
extern short Select_Music;//前のシーンで選んだ曲


//時計
OBJ Tama[TAMAMAX];//リズム玉
FRIEND Friend;
CONTROL Control;
EFE Efe[EFEMAX];
GAG Gage[GAGEMAX];
UI Ui[3];
EFE Plus;

DEB Deb[100];
//--------------------------------------------------------------------------//
//	●	初期化		：シーンに移る時一回、変数の初期化
//
//--------------------------------------------------------------------------//

void    ControlInit(void)
{
	PPx = 960;
	Plus.Px = 0;
	Plus.Py = 0;

	End = false;
	End_Cnt = 0;
	Black = 0;

	if(Select_Music == 0){
		ZURE = 250;
		Muzu =   8;
	}
	if(Select_Music == 1){
		ZURE = 282;
		Muzu =  21;
	}
	if(Select_Music == 2){
		ZURE = 250;
		Muzu =  30;
	}
	if(Select_Music == 3){
		ZURE = 250;
		Muzu =  22;
	}
	if(Select_Music == 4){
		ZURE = 250;
		Muzu = 24;
	}

	Palp = 255;
	timeGetTime();
	srand(timeGetTime());
	Control.Fps    = NULL;
	Control.In_Fps = NULL;
	Control.S_Flg  = false;
	Control.Eat    = NULL;
	Control.Drill  = NULL;
	Control.Enjoy  = 256;
	Control.You    = NULL;
	Control.In     = NULL;
	Control.TypeA  = NULL;
	Control.TypeB  = NULL;
	Control.Red    = NULL;
	Control.Blue   = NULL;
	Control.Yellow = NULL;
	Control.Green  = NULL;
	Control.Eat_Flg = false;
	Control.Drill_Flg = false;
	Control.Enjoy_Flg = false;
	Cnter  =  NULL;
}

void    TamaInit(void)
{
	for(short i=0;i<TAMAMAX;i++){
		Tama[i].Px    = WINW/2;
		Tama[i].Py    = WINH-134;
		Tama[i].Dx    = NULL;
		Tama[i].Dy    = NULL;
		Tama[i].Scl   = NULL;
		Tama[i].D_Scl = NULL;
		Tama[i].Flg   = false;
		Tama[i].Color = i%4;//赤0　青1　黄2　緑3
		Tama[i].Miss_Flg = false;
		Tama[i].TamaP_Flg = false;
		Tama[i].Eat  =  false;
		Tama[i].Enjoy = false;
		Tama[i].E_Flg = false;
	}
	Tama[0].E_Flg = true;
}	

void    FriendInit(void)
{
	Friend.Px    =   310;
	Friend.Py    =   420;
	Friend.Dx    =  NULL;
	Friend.Dy    =  NULL;
	Friend.Level =  NULL;
	//Friend.Type  =  NULL;
	Friend.Type  =  NULL;
	Friend.Color =  NULL;
	Friend.Ang   =  NULL;
	Friend.Ang_Fps =  13;
	Friend.Ang_Cnt = NULL;
	Friend.Move_Cnt = NULL;
	Friend.T_Flg = false;
	Friend.K_Flg = false;
	Friend.A_Flg = false;
	Friend.DAng  = NULL;
	PPy = -128;
}
void    EfeInit(void)
{

	for(short i=0;i<EFEMAX;i++){
		Efe[i].Px  = WINW/2;
		Efe[i].Py  = 820;
		Efe[i].Dx  = NULL;
		Efe[i].Dy  = NULL;
		Efe[i].Scl = NULL;
		Efe[i].Ang = i*10;
		Efe[i].Flg = false;
		Efe[i].alp = 255;
	}
}
void    GageInit(void)
{
	for(short i=0;i<GAGEMAX;i++){
		Gage[i].gage = 4;
	}
}
void	GameInit(void)
{
	ControlInit();
	FriendInit();
	TamaInit();
	EfeInit();
	GageInit();
}


//　ゲームメインループ

void    ControlLoop(void)
{
	if(Gs_GetKEYtrg(kKEY_B)){
		SceneChange(GAME_SCENE);
	}
	Control.Fps ++;
	if(Control.In_Fps %(35-Muzu) == 1){
		Control.Enjoy ++;
	}
	if(Control.Fps == 180){//Fpsが180を超えたら曲を始めるFlgがtrueになる
		Control.S_Flg = true;
		if(Select_Music == 0){
			Gs_PlayBGM(BGM_04);
		}
		if(Select_Music == 1){
			Gs_PlayBGM(BGM_02);
		}
		if(Select_Music == 2){
			Gs_PlayBGM(BGM_03);
		}
		if(Select_Music == 3){
			Gs_PlayBGM(BGM_05);
		}
		if(Select_Music == 4){
			Gs_PlayBGM(BGM_06);
		}
	}
	if(Control.S_Flg){//もしもこのFlgがtrueだったら曲が再生される
		Control.In_Fps ++;//ついでにインゲームのカウンターが作動
	}

}

void    FriendLoop(void)
{
	Friend.Ang += Friend.DAng;
	Friend.Px += Friend.Dx;
	Friend.Py += Friend.Dy;
	Control.You   = Control.Red + Control.Yellow;
	Control.In    = Control.Blue + Control.Green;
	Control.TypeA = Control.Red + Control.Blue;
	Control.TypeB = Control.Yellow + Control.Green;
	if(Control.S_Flg){
		Friend.Ang_Fps ++;
		if(Friend.Ang_Fps >= 26){
			Friend.Ang_Cnt ++;
			Friend.Ang_Fps = NULL;
		}
		if(Friend.Ang_Cnt %2 == 0){
			Friend.DAng = -1;
		}else{
			Friend.DAng = 1;
		}
	}


	//進化
	if(Control.Drill >= 512){
		HANA = true;
	}
	if(HANA){
		HANACnt ++;
		Palp  -= 22;
	}
	if(HANACnt >= 10){
		HANA = false;
		HANACnt = 0;
		Palp  =  255;
	}


	if(Control.Drill >= 512){


		Friend.Level ++;

		switch(Friend.Type){
			case(0):{
				if(Control.In > Control.You){
					Friend.Type = 1;
				}
				if(Control.You >= Control.In){
					Friend.Type = 2;
				}
					}break;
			case(1):{
				if(Control.TypeA >= Control.TypeB){
					Friend.Type = 3;
				}
				if(Control.TypeB > Control.TypeA){
					Friend.Type = 4;
				}
					}break;
			case(2):{
				if(Control.TypeA >= Control.TypeB){
					Friend.Type = 5;
				}
				if(Control.TypeB > Control.TypeA){
					Friend.Type = 6;
				}
					}break;
			case(3):{
				Friend.Type = 7;
					}break;
			case(4):{
				Friend.Type = 8;
					}break;
			case(5):{
				Friend.Type = 9;
					}break;
			case(6):{
				Friend.Type = 10;
					}break;
		}
	
		Control.Drill = NULL;
	}

	if(Friend.T_Flg){
		Friend.Move_Cnt ++;
		if(Friend.Move_Cnt < 5){
			Friend.Dy = -5;
		}
		if(Friend.Move_Cnt >= 5){
			Friend.Dy = 5;
		}
		if(Friend.Move_Cnt == 9){
			Friend.T_Flg = false;
			Friend.Dy = NULL;
			Friend.Move_Cnt = NULL;
		}
	}
	if(Friend.K_Flg && !Friend.Type == 0){
		Friend.Move_Cnt ++;
		if(Friend.Move_Cnt < 4){
			Friend.Dx = -10;
		}
		if(Friend.Move_Cnt >= 4){
			Friend.Dx = 5;
		}
		if(Friend.Move_Cnt == 10){
			Friend.K_Flg = false;
			Friend.Dx = NULL;
			Friend.Move_Cnt = NULL;
		}
	}
	if(Friend.A_Flg){
		Friend.Move_Cnt ++;
		if(Friend.Move_Cnt < 11){
			Friend.Ang += 36;
		}
		if(Friend.Move_Cnt == 11){
			Friend.A_Flg = false;
			Friend.Move_Cnt = NULL;
		}
	}

}


void    TamaLoop(void)
{
	for(short i=TAMAMAX-1;i>-1;i--){
		Tama[i].Px  += Tama[i].Dx;
		Tama[i].Py  += Tama[i].Dy;
		Tama[i].Scl += Tama[i].D_Scl;

		if(Tama[i].Flg){
			Tama[i].D_Scl = SCLSPEED;//もしもリズム玉フラグが真になったら中央からこのスピードでスケール拡大
		}
		if(Tama[i].Scl >= 1.061f && Tama[i].Flg){//もしもリズム玉をスルーしたらリズム玉が消滅
			Control.Enjoy   +=    5;
			Tama[i].Flg      = false;
			Tama[i].Miss_Flg = true;
			Tama[i].D_Scl    = NULL;
			Tama[i].E_Flg = true;
		}
		if(KEY_ANYtrg){
			if(Tama[i].Scl >= 0.7f && Tama[i].Scl < 0.84f && !Tama[i-1].Flg && !Tama[i].Miss_Flg){//もしも間違ったタイミングで押してしまうとリズム玉が消滅
				Control.Enjoy   += 10;  
				Tama[i].Miss_Flg = true;
				Tama[i].Flg      = false;
				Control.Enjoy += 10;
				Tama[i].E_Flg = true;
			}
		}

		if(KEY_RIGHT  || KEY_WRIGHT){
			Control.Enjoy_Flg = true;
		}else{
			Control.Enjoy_Flg = false;
		};
		

		if(KEY_UP){
			Control.Eat_Flg = true;
		}else{
			Control.Eat_Flg = false;
		}

		if(KEY_LEFT || KEY_WLEFT){
			Control.Drill_Flg = true;
		}else{
			Control.Drill_Flg = false;
		}
	

		if(Tama[i].Scl >= 0.84f && Tama[i].Scl <= 1.06f && !Tama[i].TamaP_Flg && !Tama[i].Miss_Flg && Tama[i-1].E_Flg){//成功した時
			if(KEY_RIGHTtrg || KEY_WRIGHTtrg){
			//if(Tama[i].Scl >= 0.95f){
				Gs_PlaySE(SUZU);
				Tama[i].Enjoy = true;//このフラグが立つことによってキャッチしたリズム玉が動く
				Control.Enjoy -= 10;//ストレスを下げる
				Tama[i].TamaP_Flg = true;
				Tama[i].E_Flg = true;
			}


			if(KEY_UPtrg){
				Gs_PlaySE(SUZU);
				Control.Drill += 3;
				Control.Enjoy -= 3;
				Tama[i].Eat   = true;
				for(short j=0;j<GAGEMAX;j++){
					if(j == Control.Eat){
						if(Tama[i].Color == RED){
							Gage[j].gage = RED;
							Control.Red  ++;
						}
						if(Tama[i].Color == BLUE){
							Gage[j].gage = BLUE;
							Control.Blue  ++;
						}
						if(Tama[i].Color == YELLOW){
							Gage[j].gage = YELLOW;
							Control.Yellow  ++;
						}
						if(Tama[i].Color == GREEN){
							Gage[j].gage = GREEN;
							Control.Green  ++;
						}
					}
				}
				Control.Eat ++;
				Tama[i].TamaP_Flg = true;
				Tama[i].E_Flg = true;
			}
			if(KEY_LEFTtrg || KEY_WLEFTtrg){
				Gs_PlaySE(SUZU);
				Tama[i].Drill = true;
				//Control.Drill += 20.0f/(2.0f+(Control.Enjoy*0.01f));
				Control.Drill += (30.0f - (Control.Enjoy * 0.04f))/(Friend.Level+1);
				Tama[i].TamaP_Flg = true;
				Tama[i].E_Flg = true;
			}
	
		}

		if(Tama[i].Enjoy && !Tama[i].Eat && !Tama[i].Drill){//遊ぶ
			Tama[i].Dx    = 20;
			Tama[i].D_Scl = NULL;
		}
		if(!Tama[i].Enjoy && Tama[i].Eat && !Tama[i].Drill){//食べる
			Tama[i].Dy    = -30;
			Tama[i].D_Scl = NULL;
		}
		if(!Tama[i].Enjoy && !Tama[i].Eat && Tama[i].Drill){//鍛える
			Tama[i].Dx    = -20;
			Tama[i].D_Scl = NULL;
		}
	}

	//左キーを押すと鍛えるの文字が大きくなる
	if(KEY_LEFT || KEY_WLEFT){
		Ui[0].Scl = 0.05f;
	}else{
		Ui[0].Scl = NULL;
	}


	//上キーを押すと鍛えるの文字が大きくなる
	if(KEY_UP){
		Ui[1].Scl = 0.05f;
	}else{
		Ui[1].Scl = NULL;
	}

	//右キーを押すと鍛えるの文字が大きくなる
	if(KEY_RIGHT  || KEY_WRIGHT){
		Ui[2].Scl = 0.05f;
	}else{
		Ui[2].Scl = NULL;
	}

	LIMIT(Control.Eat,0,256)
	LIMIT(Control.Drill,0,512)
	LIMIT(Control.Enjoy,0,512)
}

void    EfeLoop(void)
{
	for(short i=0;i<EFEMAX;i++){

		if(KEY_RIGHTtrg  || KEY_WRIGHTtrg || KEY_LEFTtrg || KEY_WLEFTtrg || KEY_UPtrg){//もしいずれかのキーを押したらエフェクトが初期位置にリセットされる
			Red        = false;
			Blue       = false;
			Green      = false;
			Efe[i].Flg = false;
			Efe[i].Scl = NULL;
			Efe[i].Px  = WINW/2;
			Efe[i].Py  = 820;
			Efe[i].Dx  = NULL;
			Efe[i].Dy  = NULL;
			Efe[i].alp = 255;
		}

		Efe[i].Px  =  ROUND_X(Efe[i].Ang,140,EFE_X);
		Efe[i].Py  =  ROUND_Y(Efe[i].Ang,140,EFE_Y);
		Efe[i].Px += Efe[i].Dx;
		Efe[i].Py += Efe[i].Dy;

	for(short j=0;j<TAMAMAX;j++){
		if(Tama[j].TamaP_Flg && Tama[j].Px == WINW/2 && Tama[j].Py == WINH-134){
			if(KEY_UPtrg){
				Red        = true;
				Efe[i].Flg = true;
				if(!Friend.K_Flg && !Friend.A_Flg){
					Friend.T_Flg = true;
				}

			}
			if(KEY_LEFTtrg || KEY_WLEFTtrg){
				Green      = true;
				Efe[i].Flg = true;
				if(!Friend.T_Flg && !Friend.A_Flg){
					Friend.K_Flg = true;
				}
			}

			if(KEY_RIGHTtrg || KEY_WRIGHTtrg){
				Blue       = true;
				Efe[i].Flg = true;
				if(!Friend.T_Flg && !Friend.K_Flg){
					Friend.A_Flg = true;
				}
			}
		}
	}

		if(Efe[i].Flg){
			Efe[i].Scl += 10.0f;
			Efe[i].Dx   =  ROUND_X(Efe[i].Ang,Efe[i].Scl,0);
			Efe[i].Dy   =  ROUND_Y(Efe[i].Ang,Efe[i].Scl,0);
			Efe[i].alp -= 15;
			Efe[i].Ang += 9;
		}
		if(Efe[i].alp <= 0){
			Efe[i].Flg = false;
			Efe[i].Scl = 0;
			Efe[i].Px  = WINW/2;
			Efe[i].Py  = 820;
			Efe[i].Dx  = NULL;
			Efe[i].Dy  = NULL;
			Efe[i].alp = 255;
			Red        = false;
			Blue       = false;
			Green      = false;
		}
	}

	//リザルト後
	if(End){
		End_Cnt ++;
		Black += 3;
	}
	if(Black >= 200 && Black <= 252){
		Plus.Px += 10;
		Plus.Py += 9;
		Plus.Scl += 0.01f;
	}
	if(End_Cnt >= 120 && End_Cnt <= 140){
		PPy += 20.0f;
	}
	if(End_Cnt >= 140 && End_Cnt <= 160){
		PPx -= 20.0f;
	}

	//おわり
	if(End_Cnt >= 300){
		SceneChange(SENTAKU_SCENE);
	}
	LIMIT(Black,0,255);
	if(KEY_B){
		End = true;
	}
}

void    Gakuhu_01Loop(void)
{
	if(Control.In_Fps == 427-ZURE){//145
		Tama[1].Flg = true;
	}
	if(Control.In_Fps == 486-ZURE){
		Tama[2].Flg = true;
	}
	if(Control.In_Fps == 546-ZURE){
		Tama[3].Flg = true;
	}
	if(Control.In_Fps == 606-ZURE){
		Tama[4].Flg = true;
	}
	if(Control.In_Fps == 908-ZURE){
		Tama[5].Flg = true;
	}
	if(Control.In_Fps == 966-ZURE){
		Tama[6].Flg = true;
	}
	if(Control.In_Fps == 1024-ZURE){
		Tama[7].Flg = true;
	}
	if(Control.In_Fps == 1082-ZURE){
		Tama[8].Flg = true;
	}
	if(Control.In_Fps == 1142-ZURE){//145
		Tama[9].Flg = true;
	}
	if(Control.In_Fps == 1172-ZURE){
		Tama[10].Flg = true;
	}
	if(Control.In_Fps == 1202-ZURE){
		Tama[11].Flg = true;
	}
	if(Control.In_Fps == 1232-ZURE){
		Tama[12].Flg = true;
	}
	if(Control.In_Fps == 1262-ZURE){
		Tama[13].Flg = true;
	}
	if(Control.In_Fps == 1292-ZURE){
		Tama[14].Flg = true;
	}
	if(Control.In_Fps == 1322-ZURE){
		Tama[15].Flg = true;
	}
	if(Control.In_Fps == 1352-ZURE){
		Tama[16].Flg = true;
	}
}


//プラスチック
void    Gakuhu_02Loop(void)
{
	if(Control.In_Fps >= 4171){
		//Gs_StopMUSIC(BGM_02);
	}


	if(Control.In_Fps == 446-ZURE){//145
		Tama[1].Flg = true;
	}
	if(Control.In_Fps == 472-ZURE){
		Tama[2].Flg = true;
	}
	if(Control.In_Fps == 498-ZURE){
		Tama[3].Flg = true;
	}
	if(Control.In_Fps == 511-ZURE){
		Tama[4].Flg = true;
	}
	if(Control.In_Fps == 525-ZURE){
		Tama[5].Flg = true;
	}
	if(Control.In_Fps == 550-ZURE){
		Tama[6].Flg = true;
	}
	if(Control.In_Fps == 575-ZURE){
		Tama[7].Flg = true;
	}
	if(Control.In_Fps == 602-ZURE){
		Tama[8].Flg = true;
	}
	if(Control.In_Fps == 615-ZURE){//145
		Tama[9].Flg = true;
	}
	if(Control.In_Fps == 628-ZURE){
		Tama[10].Flg = true;
	}
	if(Control.In_Fps == 652-ZURE){
		Tama[11].Flg = true;
	}
	if(Control.In_Fps == 678-ZURE){
		Tama[12].Flg = true;
	}
	if(Control.In_Fps == 704-ZURE){
		Tama[13].Flg = true;
	}
	if(Control.In_Fps == 715-ZURE){
		Tama[14].Flg = true;
	}
	if(Control.In_Fps == 729-ZURE){
		Tama[15].Flg = true;
	}
	if(Control.In_Fps == 755-ZURE){
		Tama[16].Flg = true;
	}
	if(Control.In_Fps == 781-ZURE){//145
		Tama[17].Flg = true;
	}
	if(Control.In_Fps == 808-ZURE){
		Tama[18].Flg = true;
	}
	if(Control.In_Fps == 820-ZURE){
		Tama[19].Flg = true;
	}
	if(Control.In_Fps == 835-ZURE){
		Tama[20].Flg = true;
	}
	if(Control.In_Fps == 859-ZURE){
		Tama[21].Flg = true;
	}
	if(Control.In_Fps == 886-ZURE){
		Tama[22].Flg = true;
	}
	if(Control.In_Fps == 911-ZURE){
		Tama[23].Flg = true;
	}
	if(Control.In_Fps == 924-ZURE){
		Tama[24].Flg = true;
	}
	if(Control.In_Fps == 936-ZURE){//145
		Tama[25].Flg = true;
	}
	if(Control.In_Fps == 962-ZURE){
		Tama[26].Flg = true;
	}
	if(Control.In_Fps == 988-ZURE){
		Tama[27].Flg = true;
	}
	if(Control.In_Fps == 1013-ZURE){
		Tama[28].Flg = true;
	}
	if(Control.In_Fps == 1025-ZURE){
		Tama[29].Flg = true;
	}
	if(Control.In_Fps == 1038-ZURE){
		Tama[30].Flg = true;
	}


	if(Control.In_Fps == 1061-ZURE){
		Tama[31].Flg = true;
	}
	if(Control.In_Fps == 1074-ZURE){
		Tama[32].Flg = true;
	}
	if(Control.In_Fps == 1087-ZURE){//145
		Tama[33].Flg = true;
	}
	if(Control.In_Fps == 1100-ZURE){
		Tama[34].Flg = true;
	}
	if(Control.In_Fps == 1113-ZURE){
		Tama[35].Flg = true;
	}
	if(Control.In_Fps == 1126-ZURE){
		Tama[36].Flg = true;
	}
	if(Control.In_Fps == 1139-ZURE){
		Tama[37].Flg = true;
	}


	if(Control.In_Fps == 1166-ZURE){
		Tama[38].Flg = true;
	}
	if(Control.In_Fps == 1179-ZURE){
		Tama[39].Flg = true;
	}
	if(Control.In_Fps == 1192-ZURE){
		Tama[40].Flg = true;
	}
	if(Control.In_Fps == 1205-ZURE){
		Tama[41].Flg = true;
	}
	if(Control.In_Fps == 1218-ZURE){
		Tama[42].Flg = true;
	}
	if(Control.In_Fps == 1231-ZURE){
		Tama[43].Flg = true;
	}
	if(Control.In_Fps == 1244-ZURE){
		Tama[44].Flg = true;
	}

	if(Control.In_Fps == 1269-ZURE){
		Tama[45].Flg = true;
	}
	if(Control.In_Fps == 1282-ZURE){
		Tama[46].Flg = true;
	}
	if(Control.In_Fps == 1295-ZURE){
		Tama[47].Flg = true;
	}
	if(Control.In_Fps == 1308-ZURE){
		Tama[48].Flg = true;
	}
	if(Control.In_Fps == 1321-ZURE){//145
		Tama[49].Flg = true;
	}
	if(Control.In_Fps == 1334-ZURE){
		Tama[50].Flg = true;
	}
	if(Control.In_Fps == 1347-ZURE){
		Tama[51].Flg = true;
	}

	if(Control.In_Fps == 1372-ZURE){
		Tama[52].Flg = true;
	}
	if(Control.In_Fps == 1385-ZURE){
		Tama[53].Flg = true;
	}
	if(Control.In_Fps == 1398-ZURE){
		Tama[54].Flg = true;
	}
	if(Control.In_Fps == 1411-ZURE){
		Tama[55].Flg = true;
	}
	if(Control.In_Fps == 1424-ZURE){
		Tama[56].Flg = true;
	}
	if(Control.In_Fps == 1437-ZURE){//145
		Tama[57].Flg = true;
	}
	if(Control.In_Fps == 1450-ZURE){
		Tama[58].Flg = true;
	}

	if(Control.In_Fps == 1475-ZURE){
		Tama[59].Flg = true;
	}
	if(Control.In_Fps == 1488-ZURE){
		Tama[60].Flg = true;
	}
	if(Control.In_Fps == 1501-ZURE){
		Tama[61].Flg = true;
	}
	if(Control.In_Fps == 1514-ZURE){
		Tama[62].Flg = true;
	}
	if(Control.In_Fps == 1527-ZURE){//145
		Tama[63].Flg = true;
	}
	if(Control.In_Fps == 1540-ZURE){
		Tama[64].Flg = true;
	}
	if(Control.In_Fps == 1553-ZURE){
		Tama[65].Flg = true;
	}

	if(Control.In_Fps == 1578-ZURE){
		Tama[66].Flg = true;
	}
	if(Control.In_Fps == 1591-ZURE){
		Tama[67].Flg = true;
	}
	if(Control.In_Fps == 1604-ZURE){
		Tama[68].Flg = true;
	}
	if(Control.In_Fps == 1617-ZURE){
		Tama[69].Flg = true;
	}
	if(Control.In_Fps == 1630-ZURE){
		Tama[70].Flg = true;
	}
	if(Control.In_Fps == 1643-ZURE){//145
		Tama[71].Flg = true;
	}
	if(Control.In_Fps == 1656-ZURE){
		Tama[72].Flg = true;
	}

	if(Control.In_Fps == 1681-ZURE){
		Tama[73].Flg = true;
	}
	if(Control.In_Fps == 1694-ZURE){
		Tama[74].Flg = true;
	}
	if(Control.In_Fps == 1707-ZURE){
		Tama[75].Flg = true;
	}
	if(Control.In_Fps == 1720-ZURE){
		Tama[76].Flg = true;
	}
	if(Control.In_Fps == 1733-ZURE){
		Tama[77].Flg = true;
	}
	if(Control.In_Fps == 1746-ZURE){
		Tama[78].Flg = true;
	}
	if(Control.In_Fps == 1759-ZURE){
		Tama[79].Flg = true;
	}

	if(Control.In_Fps == 1784-ZURE){
		Tama[80].Flg = true;
	}
	if(Control.In_Fps == 1797-ZURE){
		Tama[81].Flg = true;
	}
	if(Control.In_Fps == 1810-ZURE){
		Tama[82].Flg = true;
	}
	if(Control.In_Fps == 1823-ZURE){
		Tama[83].Flg = true;
	}
	if(Control.In_Fps == 1836-ZURE){
		Tama[84].Flg = true;
	}
	if(Control.In_Fps == 1849-ZURE){
		Tama[85].Flg = true;
	}
	if(Control.In_Fps == 1862-ZURE){
		Tama[86].Flg = true;
	}




	if(Control.In_Fps == 1889-ZURE){//1
		Tama[87].Flg = true;
	}
	if(Control.In_Fps == 1914-ZURE){
		Tama[88].Flg = true;
	}
	if(Control.In_Fps == 1939-ZURE){
		Tama[89].Flg = true;
	}
	if(Control.In_Fps == 1951-ZURE){
		Tama[90].Flg = true;
	}
	if(Control.In_Fps == 1963-ZURE){
		Tama[91].Flg = true;
	}
	if(Control.In_Fps == 1988-ZURE){//2
		Tama[92].Flg = true;
	}
	if(Control.In_Fps == 2016-ZURE){
		Tama[93].Flg = true;
	}
	if(Control.In_Fps == 2042-ZURE){
		Tama[94].Flg = true;
	}
	if(Control.In_Fps == 2053-ZURE){
		Tama[95].Flg = true;
	}
	if(Control.In_Fps == 2065-ZURE){
		Tama[96].Flg = true;
	}
	if(Control.In_Fps == 2092-ZURE){//3
		Tama[97].Flg = true;
	}
	if(Control.In_Fps == 2117-ZURE){
		Tama[98].Flg = true;
	}
	if(Control.In_Fps == 2144-ZURE){
		Tama[99].Flg = true;
	}
	if(Control.In_Fps == 2157-ZURE){
		Tama[100].Flg = true;
	}
	if(Control.In_Fps == 2170-ZURE){
		Tama[101].Flg = true;
	}
	if(Control.In_Fps == 2196-ZURE){
		Tama[102].Flg = true;
	}
	if(Control.In_Fps == 2220-ZURE){//145
		Tama[103].Flg = true;
	}
	if(Control.In_Fps == 2247-ZURE){
		Tama[104].Flg = true;
	}
	if(Control.In_Fps == 2259-ZURE){
		Tama[105].Flg = true;
	}
	if(Control.In_Fps == 2272-ZURE){
		Tama[106].Flg = true;
	}
	if(Control.In_Fps == 2299-ZURE){
		Tama[107].Flg = true;
	}
	if(Control.In_Fps == 2324-ZURE){
		Tama[108].Flg = true;
	}
	if(Control.In_Fps == 2349-ZURE){
		Tama[109].Flg = true;
	}
	if(Control.In_Fps == 2361-ZURE){
		Tama[110].Flg = true;
	}
	if(Control.In_Fps == 2377-ZURE){//145
		Tama[111].Flg = true;
	}
	if(Control.In_Fps == 2401-ZURE){
		Tama[112].Flg = true;
	}
	if(Control.In_Fps == 2425-ZURE){
		Tama[113].Flg = true;
	}
	if(Control.In_Fps == 2453-ZURE){
		Tama[114].Flg = true;
	}
	if(Control.In_Fps == 2464-ZURE){
		Tama[115].Flg = true;
	}
	if(Control.In_Fps == 2479-ZURE){
		Tama[116].Flg = true;
	}
	if(Control.In_Fps == 2503-ZURE){
		Tama[117].Flg = true;
	}
	if(Control.In_Fps == 2530-ZURE){
		Tama[118].Flg = true;
	}
	if(Control.In_Fps == 2556-ZURE){
		Tama[119].Flg = true;
	}
	if(Control.In_Fps == 2580-ZURE){
		Tama[120].Flg = true;
	}
	if(Control.In_Fps == 2607-ZURE){//2607
		Tama[121].Flg = true;
	}
	if(Control.In_Fps == 2607+13-ZURE){
		Tama[122].Flg = true;
	}
	if(Control.In_Fps == 2633-ZURE){//2633
		Tama[123].Flg = true;
	}
	if(Control.In_Fps == 2633+13-ZURE){
		Tama[124].Flg = true;
	}
	if(Control.In_Fps == 2658-ZURE){//2658
		Tama[125].Flg = true;
	}
	if(Control.In_Fps == 2658+13-ZURE){
		Tama[126].Flg = true;
	}
	if(Control.In_Fps == 2682-ZURE){//2682
		Tama[127].Flg = true;
	}
	if(Control.In_Fps == 2682+13-ZURE){
		Tama[128].Flg = true;
	}


	if(Control.In_Fps == 2706-ZURE){
		Tama[129].Flg = true;
	}
	if(Control.In_Fps == 2733-ZURE){
		Tama[130].Flg = true;
	}
	if(Control.In_Fps == 2760-ZURE){
		Tama[131].Flg = true;
	}
	if(Control.In_Fps == 2785-ZURE){
		Tama[132].Flg = true;
	}
	if(Control.In_Fps == 2812-ZURE){//145
		Tama[133].Flg = true;
	}
	if(Control.In_Fps == 2837-ZURE){
		Tama[134].Flg = true;
	}
	if(Control.In_Fps == 2864-ZURE){
		Tama[135].Flg = true;
	}
	if(Control.In_Fps == 2874-ZURE){
		Tama[136].Flg = true;
	}
	if(Control.In_Fps == 2888-ZURE){
		Tama[137].Flg = true;
	}

	if(Control.In_Fps == 2914-ZURE){
		Tama[138].Flg = true;
	}
	if(Control.In_Fps == 2941-ZURE){
		Tama[139].Flg = true;
	}
	if(Control.In_Fps == 2967-ZURE){
		Tama[140].Flg = true;
	}
	if(Control.In_Fps == 2992-ZURE){
		Tama[141].Flg = true;
	}
	if(Control.In_Fps == 3017-ZURE){
		Tama[142].Flg = true;
	}
	if(Control.In_Fps == 3043-ZURE){
		Tama[143].Flg = true;
	}
	if(Control.In_Fps == 3069-ZURE){
		Tama[144].Flg = true;
	}

	if(Control.In_Fps == 3080-ZURE){
		Tama[145].Flg = true;
	}
	if(Control.In_Fps == 3094-ZURE){
		Tama[146].Flg = true;
	}
	if(Control.In_Fps == 3120-ZURE){
		Tama[147].Flg = true;
	}
	if(Control.In_Fps == 3146-ZURE){
		Tama[148].Flg = true;
	}
	if(Control.In_Fps == 3171-ZURE){//145
		Tama[149].Flg = true;
	}
	if(Control.In_Fps == 3198-ZURE){
		Tama[150].Flg = true;
	}
	if(Control.In_Fps == 3225-ZURE){
		Tama[151].Flg = true;
	}

	if(Control.In_Fps == 3250-ZURE){
		Tama[152].Flg = true;
	}
	if(Control.In_Fps == 3275-ZURE){
		Tama[153].Flg = true;
	}
	if(Control.In_Fps == 3286-ZURE){
		Tama[154].Flg = true;
	}
	if(Control.In_Fps == 3299-ZURE){
		Tama[155].Flg = true;
	}
	if(Control.In_Fps == 3326-ZURE){
		Tama[156].Flg = true;
	}
	if(Control.In_Fps == 3351-ZURE){//145
		Tama[157].Flg = true;
	}
	if(Control.In_Fps == 3380-ZURE){
		Tama[158].Flg = true;
	}

	if(Control.In_Fps == 3405-ZURE){
		Tama[159].Flg = true;
	}
	if(Control.In_Fps == 3429-ZURE){
		Tama[160].Flg = true;
	}
	if(Control.In_Fps == 3442-ZURE){
		Tama[161].Flg = true;
	}
	if(Control.In_Fps == 3454-ZURE){
		Tama[162].Flg = true;
	}
	if(Control.In_Fps == 3468-ZURE){//145
		Tama[163].Flg = true;
	}
	if(Control.In_Fps == 3480-ZURE){
		Tama[164].Flg = true;
	}
	if(Control.In_Fps == 3494-ZURE){
		Tama[165].Flg = true;
	}
	if(Control.In_Fps == 3507-ZURE){
		Tama[166].Flg = true;
	}
	if(Control.In_Fps == 3520-ZURE){
		Tama[167].Flg = true;
	}


	if(Control.In_Fps == 3532-ZURE){
		Tama[168].Flg = true;
	}
	if(Control.In_Fps == 3558-ZURE){
		Tama[169].Flg = true;
	}
	if(Control.In_Fps == 3585-ZURE){
		Tama[170].Flg = true;
	}
	if(Control.In_Fps == 3596-ZURE){
		Tama[171].Flg = true;
	}
	if(Control.In_Fps == 3612-ZURE){
		Tama[172].Flg = true;
	}

	if(Control.In_Fps == 3638-ZURE){
		Tama[173].Flg = true;
	}
	if(Control.In_Fps == 3650-ZURE){
		Tama[174].Flg = true;
	}
	if(Control.In_Fps == 3663-ZURE){
		Tama[175].Flg = true;
	}
	if(Control.In_Fps == 3675-ZURE){
		Tama[176].Flg = true;
	}
	if(Control.In_Fps == 3687-ZURE){
		Tama[177].Flg = true;
	}
	if(Control.In_Fps == 3699-ZURE){
		Tama[178].Flg = true;
	}
	if(Control.In_Fps == 3713-ZURE){
		Tama[179].Flg = true;
	}

	if(Control.In_Fps == 3738-ZURE){
		Tama[180].Flg = true;
	}
	if(Control.In_Fps == 3764-ZURE){
		Tama[181].Flg = true;
	}
	if(Control.In_Fps == 3789-ZURE){
		Tama[182].Flg = true;
	}
	if(Control.In_Fps == 3802-ZURE){
		Tama[183].Flg = true;
	}
	if(Control.In_Fps == 3816-ZURE){
		Tama[184].Flg = true;
	}
	if(Control.In_Fps == 3842-ZURE){
		Tama[185].Flg = true;
	}
	if(Control.In_Fps == 3854-ZURE){
		Tama[186].Flg = true;
	}




	if(Control.In_Fps == 3866-ZURE){//1
		Tama[187].Flg = true;
	}
	if(Control.In_Fps == 3879-ZURE){
		Tama[188].Flg = true;
	}
	if(Control.In_Fps == 3893-ZURE){
		Tama[189].Flg = true;
	}
	if(Control.In_Fps == 3904-ZURE){
		Tama[190].Flg = true;
	}
	if(Control.In_Fps == 3919-ZURE){
		Tama[191].Flg = true;
	}
	if(Control.In_Fps == 3943-ZURE){//2
		Tama[192].Flg = true;
	}
	if(Control.In_Fps == 3968-ZURE){
		Tama[193].Flg = true;
	}
	if(Control.In_Fps == 3995-ZURE){
		Tama[194].Flg = true;
	}
	if(Control.In_Fps == 4008-ZURE){
		Tama[195].Flg = true;
	}
	if(Control.In_Fps == 4021-ZURE){
		Tama[196].Flg = true;
	}
	if(Control.In_Fps == 4046-ZURE){//3
		Tama[197].Flg = true;
	}
	if(Control.In_Fps == 4059-ZURE){
		Tama[198].Flg = true;
	}
	if(Control.In_Fps == 4072-ZURE){
		Tama[199].Flg = true;
	}
	if(Control.In_Fps == 4085-ZURE){
		Tama[200].Flg = true;
	}


	if(Control.In_Fps == 4097-ZURE){
		Tama[201].Flg = true;
	}
	if(Control.In_Fps == 4109-ZURE){//2
		Tama[202].Flg = true;
	}
	if(Control.In_Fps == 4124-ZURE){
		Tama[203].Flg = true;
	}
	if(Control.In_Fps == 4148-ZURE){
		Tama[204].Flg = true;
	}
	if(Control.In_Fps == 4202-ZURE){
		Tama[205].Flg = true;
	}
	if(Control.In_Fps == 4253-ZURE){
		Tama[206].Flg = true;
	}
	if(Control.In_Fps == 4280-ZURE){
		Tama[207].Flg = true;
	}
	if(Control.In_Fps == 4304-ZURE){
		Tama[208].Flg = true;
	}
	if(Control.In_Fps == 4329-ZURE){
		Tama[209].Flg = true;
	}
	if(Control.In_Fps == 4329){
		End = true;
	}
}


//ロストワン
void    Gakuhu_03Loop(void)
{
	if(Control.In_Fps >= 5960){
	}

	if(Control.In_Fps == 460-ZURE){//145
		Tama[1].Flg = true;
	}
	if(Control.In_Fps == 482-ZURE){
		Tama[2].Flg = true;
	}

	if(Control.In_Fps == 504-ZURE){
		Tama[3].Flg = true;
	}
	if(Control.In_Fps == 515-ZURE){
		Tama[4].Flg = true;
	}
	if(Control.In_Fps == 526-ZURE){
		Tama[5].Flg = true;
	}


	if(Control.In_Fps == 549-ZURE){
		Tama[6].Flg = true;
	}
	if(Control.In_Fps == 571-ZURE){
		Tama[7].Flg = true;
	}

	if(Control.In_Fps == 593-ZURE){
		Tama[8].Flg = true;
	}
	if(Control.In_Fps == 604-ZURE){
		Tama[9].Flg = true;
	}
	if(Control.In_Fps == 615-ZURE){
		Tama[10].Flg = true;
	}


	if(Control.In_Fps == 638-ZURE){
		Tama[11].Flg = true;
	}
	if(Control.In_Fps == 660-ZURE){
		Tama[12].Flg = true;
	}

	if(Control.In_Fps == 682-ZURE){
		Tama[13].Flg = true;
	}
	if(Control.In_Fps == 693-ZURE){
		Tama[14].Flg = true;
	}
	if(Control.In_Fps == 704-ZURE){
		Tama[15].Flg = true;
	}


	if(Control.In_Fps == 727-ZURE){
		Tama[16].Flg = true;
	}
	if(Control.In_Fps == 749-ZURE){//145
		Tama[17].Flg = true;
	}

	if(Control.In_Fps == 771-ZURE){
		Tama[18].Flg = true;
	}
	if(Control.In_Fps == 782-ZURE){
		Tama[19].Flg = true;
	}
	if(Control.In_Fps == 793-ZURE){
		Tama[20].Flg = true;
	}


	if(Control.In_Fps == 816-ZURE){
		Tama[21].Flg = true;
	}
	if(Control.In_Fps == 838-ZURE){
		Tama[22].Flg = true;
	}

	if(Control.In_Fps == 860-ZURE){
		Tama[23].Flg = true;
	}
	if(Control.In_Fps == 871-ZURE){
		Tama[24].Flg = true;
	}
	if(Control.In_Fps == 882-ZURE){//145
		Tama[25].Flg = true;
	}


	if(Control.In_Fps == 905-ZURE){
		Tama[26].Flg = true;
	}
	if(Control.In_Fps == 927-ZURE){
		Tama[27].Flg = true;
	}

	if(Control.In_Fps == 949-ZURE){
		Tama[28].Flg = true;
	}
	if(Control.In_Fps == 960-ZURE){
		Tama[29].Flg = true;
	}
	if(Control.In_Fps == 971-ZURE){
		Tama[30].Flg = true;
	}


	if(Control.In_Fps == 994-ZURE){
		Tama[31].Flg = true;
	}
	if(Control.In_Fps == 1016-ZURE){
		Tama[32].Flg = true;
	}

	if(Control.In_Fps == 1038-ZURE){//145
		Tama[33].Flg = true;
	}
	if(Control.In_Fps == 1049-ZURE){
		Tama[34].Flg = true;
	}
	if(Control.In_Fps == 1060-ZURE){
		Tama[35].Flg = true;
	}


	if(Control.In_Fps == 1083-ZURE){
		Tama[36].Flg = true;
	}
	if(Control.In_Fps == 1105-ZURE){
		Tama[37].Flg = true;
	}


	if(Control.In_Fps == 1127-ZURE){
		Tama[38].Flg = true;
	}
	if(Control.In_Fps == 1138-ZURE){
		Tama[39].Flg = true;
	}
	if(Control.In_Fps == 1149-ZURE){
		Tama[40].Flg = true;
	}
	if(Control.In_Fps == 1160-ZURE){
		Tama[41].Flg = true;
	}

/////////////////////////////////////////////

	if(Control.In_Fps == 1172-ZURE){
		Tama[42].Flg = true;
	}
	if(Control.In_Fps == 1194-ZURE){
		Tama[43].Flg = true;
	}

	if(Control.In_Fps == 1216-ZURE){
		Tama[44].Flg = true;
	}
	if(Control.In_Fps == 1227-ZURE){
		Tama[45].Flg = true;
	}
	if(Control.In_Fps == 1238-ZURE){
		Tama[46].Flg = true;
	}


	if(Control.In_Fps == 1261-ZURE){
		Tama[47].Flg = true;
	}
	if(Control.In_Fps == 1283-ZURE){
		Tama[48].Flg = true;
	}
	if(Control.In_Fps == 1305-ZURE){//145
		Tama[49].Flg = true;
	}
	if(Control.In_Fps == 1316-ZURE){
		Tama[50].Flg = true;
	}
	if(Control.In_Fps == 1327-ZURE){
		Tama[51].Flg = true;
	}

//
	if(Control.In_Fps == 1349-ZURE){
		Tama[52].Flg = true;
	}
	if(Control.In_Fps == 1371-ZURE){
		Tama[53].Flg = true;
	}

	if(Control.In_Fps == 1393-ZURE){
		Tama[54].Flg = true;
	}
	if(Control.In_Fps == 1404-ZURE){
		Tama[55].Flg = true;
	}
	if(Control.In_Fps == 1415-ZURE){
		Tama[56].Flg = true;
	}


	if(Control.In_Fps == 1437-ZURE){
		Tama[57].Flg = true;
	}
	if(Control.In_Fps == 1459-ZURE){
		Tama[58].Flg = true;
	}

	if(Control.In_Fps == 1481-ZURE){
		Tama[59].Flg = true;
	}
	if(Control.In_Fps == 1492-ZURE){
		Tama[60].Flg = true;
	}
	if(Control.In_Fps == 1503-ZURE){
		Tama[61].Flg = true;
	}


	if(Control.In_Fps == 1526-ZURE){
		Tama[62].Flg = true;
	}
	if(Control.In_Fps == 1548-ZURE){//145
		Tama[63].Flg = true;
	}

	if(Control.In_Fps == 1570-ZURE){
		Tama[64].Flg = true;
	}
	if(Control.In_Fps == 1581-ZURE){
		Tama[65].Flg = true;
	}
	if(Control.In_Fps == 1592-ZURE){
		Tama[66].Flg = true;
	}


	if(Control.In_Fps == 1615-ZURE){
		Tama[67].Flg = true;
	}
	if(Control.In_Fps == 1637-ZURE){
		Tama[68].Flg = true;
	}

	if(Control.In_Fps == 1659-ZURE){
		Tama[69].Flg = true;
	}
	if(Control.In_Fps == 1670-ZURE){
		Tama[70].Flg = true;
	}
	if(Control.In_Fps == 1681-ZURE){
		Tama[71].Flg = true;
	}


	if(Control.In_Fps == 1704-ZURE){
		Tama[72].Flg = true;
	}
	if(Control.In_Fps == 1726-ZURE){
		Tama[73].Flg = true;
	}

	if(Control.In_Fps == 1748-ZURE){
		Tama[74].Flg = true;
	}
	if(Control.In_Fps == 1759-ZURE){
		Tama[75].Flg = true;
	}
	if(Control.In_Fps == 1770-ZURE){
		Tama[76].Flg = true;
	}


	if(Control.In_Fps == 1794-ZURE){
		Tama[77].Flg = true;
	}
	if(Control.In_Fps == 1816-ZURE){
		Tama[78].Flg = true;
	}

	if(Control.In_Fps == 1838-ZURE){
		Tama[79].Flg = true;
	}
	if(Control.In_Fps == 1849-ZURE){
		Tama[80].Flg = true;
	}
	if(Control.In_Fps == 1860-ZURE){
		Tama[81].Flg = true;
	}


///////////////////////////////////////////////////


	if(Control.In_Fps == 1883-ZURE){
		Tama[82].Flg = true;
	}
	if(Control.In_Fps == 1905-ZURE){
		Tama[83].Flg = true;
	}
	if(Control.In_Fps == 1927-ZURE){
		Tama[84].Flg = true;
	}
	if(Control.In_Fps == 1949-ZURE){
		Tama[85].Flg = true;
	}
	if(Control.In_Fps == 1971-ZURE){
		Tama[86].Flg = true;
	}
	if(Control.In_Fps == 1993-ZURE){//1
		Tama[87].Flg = true;
	}
	if(Control.In_Fps == 2016-ZURE){
		Tama[88].Flg = true;
	}
	if(Control.In_Fps == 2027-ZURE){
		Tama[89].Flg = true;
	}
	if(Control.In_Fps == 2038-ZURE){
		Tama[90].Flg = true;
	}


	if(Control.In_Fps == 2061-ZURE){
		Tama[91].Flg = true;
	}
	if(Control.In_Fps == 2083-ZURE){
		Tama[92].Flg = true;
	}
	if(Control.In_Fps == 2105-ZURE){
		Tama[93].Flg = true;
	}
	if(Control.In_Fps == 2127-ZURE){
		Tama[94].Flg = true;
	}
	if(Control.In_Fps == 2149-ZURE){
		Tama[95].Flg = true;
	}
	if(Control.In_Fps == 2171-ZURE){
		Tama[96].Flg = true;
	}
	if(Control.In_Fps == 2194-ZURE){
		Tama[97].Flg = true;
	}
	if(Control.In_Fps == 2205-ZURE){
		Tama[98].Flg = true;
	}
	if(Control.In_Fps == 2216-ZURE){
		Tama[99].Flg = true;
	}


	if(Control.In_Fps == 2239-ZURE){
		Tama[100].Flg = true;
	}
	if(Control.In_Fps == 2261-ZURE){
		Tama[101].Flg = true;
	}
	if(Control.In_Fps == 2283-ZURE){
		Tama[102].Flg = true;
	}
	if(Control.In_Fps == 2305-ZURE){
		Tama[103].Flg = true;
	}
	if(Control.In_Fps == 2327-ZURE){
		Tama[104].Flg = true;
	}
	if(Control.In_Fps == 2349-ZURE){
		Tama[105].Flg = true;
	}
	if(Control.In_Fps == 2372-ZURE){
		Tama[106].Flg = true;
	}
	if(Control.In_Fps == 2383-ZURE){
		Tama[107].Flg = true;
	}
	if(Control.In_Fps == 2394-ZURE){
		Tama[108].Flg = true;
	}


	if(Control.In_Fps == 2417-ZURE){
		Tama[109].Flg = true;
	}
	if(Control.In_Fps == 2439-ZURE){
		Tama[110].Flg = true;
	}
	if(Control.In_Fps == 2461-ZURE){
		Tama[111].Flg = true;
	}
	if(Control.In_Fps == 2483-ZURE){
		Tama[112].Flg = true;
	}
	if(Control.In_Fps == 2505-ZURE){
		Tama[113].Flg = true;
	}
	if(Control.In_Fps == 2550-ZURE){
		Tama[114].Flg = true;
	}

////////////////////////////////////////////////////////

	
	if(Control.In_Fps == 2594-ZURE){
		Tama[115].Flg = true;
	}
	if(Control.In_Fps == 2616-ZURE){
		Tama[116].Flg = true;
	}

	if(Control.In_Fps == 2638-ZURE){
		Tama[117].Flg = true;
	}
	if(Control.In_Fps == 2649-ZURE){
		Tama[118].Flg = true;
	}
	if(Control.In_Fps == 2660-ZURE){
		Tama[119].Flg = true;
	}
//1

	if(Control.In_Fps == 2683-ZURE){
		Tama[120].Flg = true;
	}
	if(Control.In_Fps == 2705-ZURE){
		Tama[121].Flg = true;
	}

	if(Control.In_Fps == 2727-ZURE){
		Tama[122].Flg = true;
	}
	if(Control.In_Fps == 2738-ZURE){
		Tama[123].Flg = true;
	}
	if(Control.In_Fps == 2749-ZURE){
		Tama[124].Flg = true;
	}
//2

	if(Control.In_Fps == 2772-ZURE){
		Tama[125].Flg = true;
	}
	if(Control.In_Fps == 2794-ZURE){
		Tama[126].Flg = true;
	}

	if(Control.In_Fps == 2816-ZURE){
		Tama[127].Flg = true;
	}
	if(Control.In_Fps == 2827-ZURE){
		Tama[128].Flg = true;
	}
	if(Control.In_Fps == 2838-ZURE){
		Tama[129].Flg = true;
	}
//3

	if(Control.In_Fps == 2861-ZURE){
		Tama[130].Flg = true;
	}
	if(Control.In_Fps == 2883-ZURE){
		Tama[131].Flg = true;
	}

	if(Control.In_Fps == 2905-ZURE){
		Tama[132].Flg = true;
	}
	if(Control.In_Fps == 2916-ZURE){//145
		Tama[133].Flg = true;
	}
	if(Control.In_Fps == 2927-ZURE){
		Tama[134].Flg = true;
	}
//4ノーフィクション

	if(Control.In_Fps == 2950-1-ZURE){
		Tama[135].Flg = true;
	}
	if(Control.In_Fps == 2972-1-ZURE){
		Tama[136].Flg = true;
	}

	if(Control.In_Fps == 2994-1-ZURE){
		Tama[137].Flg = true;
	}
	if(Control.In_Fps == 3005-1-ZURE){
		Tama[138].Flg = true;
	}
	if(Control.In_Fps == 3016-1-ZURE){
		Tama[139].Flg = true;
	}
//5

	if(Control.In_Fps == 3039-1-ZURE){
		Tama[140].Flg = true;
	}
	if(Control.In_Fps == 3061-1-ZURE){
		Tama[141].Flg = true;
	}
	if(Control.In_Fps == 3083-1-ZURE){
		Tama[142].Flg = true;
	}
	if(Control.In_Fps == 3094-1-ZURE){
		Tama[143].Flg = true;
	}
	if(Control.In_Fps == 3105-1-ZURE){
		Tama[144].Flg = true;
	}
//6

	if(Control.In_Fps == 3128-1-ZURE){
		Tama[145].Flg = true;
	}
	if(Control.In_Fps == 3150-1-ZURE){
		Tama[146].Flg = true;
	}
	if(Control.In_Fps == 3172-1-ZURE){
		Tama[147].Flg = true;
	}
	if(Control.In_Fps == 3183-1-ZURE){
		Tama[148].Flg = true;
	}
	if(Control.In_Fps == 3194-1-ZURE){//145
		Tama[149].Flg = true;
	}
//7

	if(Control.In_Fps == 3217-1-ZURE){
		Tama[150].Flg = true;
	}
	if(Control.In_Fps == 3239-1-ZURE){
		Tama[151].Flg = true;
	}
	if(Control.In_Fps == 3261-1-ZURE){
		Tama[152].Flg = true;
	}
//8

///////////////////////////////////////////////

	if(Control.In_Fps == 3305-ZURE){
		Tama[153].Flg = true;
	}
	if(Control.In_Fps == 3327-ZURE){
		Tama[154].Flg = true;
	}
	if(Control.In_Fps == 3349-ZURE){
		Tama[155].Flg = true;
	}
	if(Control.In_Fps == 3371-ZURE){
		Tama[156].Flg = true;
	}
	if(Control.In_Fps == 3393-ZURE){
		Tama[157].Flg = true;
	}
	if(Control.In_Fps == 3415-ZURE){
		Tama[158].Flg = true;
	}

	if(Control.In_Fps == 3438-ZURE){
		Tama[159].Flg = true;
	}
	if(Control.In_Fps == 3449-ZURE){
		Tama[160].Flg = true;
	}
	if(Control.In_Fps == 3460-ZURE){
		Tama[161].Flg = true;
	}

//

	if(Control.In_Fps == 3483-ZURE){
		Tama[162].Flg = true;
	}
	if(Control.In_Fps == 3505-ZURE){
		Tama[163].Flg = true;
	}
	if(Control.In_Fps == 3527-ZURE){
		Tama[164].Flg = true;
	}
	if(Control.In_Fps == 3549-ZURE){
		Tama[165].Flg = true;
	}
	if(Control.In_Fps == 3571-ZURE){
		Tama[166].Flg = true;
	}
	if(Control.In_Fps == 3593-ZURE){
		Tama[167].Flg = true;
	}

	if(Control.In_Fps == 3615-ZURE){/////////////////////
		Tama[168].Flg = true;
	}
	if(Control.In_Fps == 3626-ZURE){
		Tama[169].Flg = true;
	}
	if(Control.In_Fps == 3637-ZURE){
		Tama[170].Flg = true;
	}

//

	if(Control.In_Fps == 3661-ZURE){
		Tama[171].Flg = true;
	}
	if(Control.In_Fps == 3683-ZURE){
		Tama[172].Flg = true;
	}
	if(Control.In_Fps == 3705-ZURE){
		Tama[173].Flg = true;
	}
	if(Control.In_Fps == 3727-ZURE){
		Tama[174].Flg = true;
	}
	if(Control.In_Fps == 3749-ZURE){
		Tama[175].Flg = true;
	}
	if(Control.In_Fps == 3771-ZURE){
		Tama[176].Flg = true;
	}
	if(Control.In_Fps == 3794-ZURE){
		Tama[177].Flg = true;
	}
	if(Control.In_Fps == 3805-ZURE){
		Tama[178].Flg = true;
	}
	if(Control.In_Fps == 3816-ZURE){
		Tama[179].Flg = true;
	}

//

	if(Control.In_Fps == 3839-ZURE){
		Tama[180].Flg = true;
	}
	if(Control.In_Fps == 3861-ZURE){
		Tama[181].Flg = true;
	}
	if(Control.In_Fps == 3883-ZURE){
		Tama[182].Flg = true;
	}
	if(Control.In_Fps == 3905-ZURE){
		Tama[183].Flg = true;
	}
	if(Control.In_Fps == 3927-ZURE){
		Tama[184].Flg = true;
	}
	if(Control.In_Fps == 3971-ZURE){
		Tama[185].Flg = true;
	}

//////////////////////////////////////////////////////

	if(Control.In_Fps == 4031-ZURE){
		Tama[186].Flg = true;
	}
	if(Control.In_Fps == 4071-ZURE){
		Tama[187].Flg = true;
	}
	if(Control.In_Fps == 4116-ZURE){
		Tama[188].Flg = true;
	}
	if(Control.In_Fps == 4148-ZURE){
		Tama[189].Flg = true;
	}
	if(Control.In_Fps == 4170-ZURE){
		Tama[190].Flg = true;
	}

	if(Control.In_Fps == 4210-ZURE){
		Tama[191].Flg = true;
	}
	if(Control.In_Fps == 4251-ZURE){
		Tama[192].Flg = true;
	}
	if(Control.In_Fps == 4296-ZURE){
		Tama[193].Flg = true;
	}
	if(Control.In_Fps == 4326-ZURE){
		Tama[194].Flg = true;
	}
	if(Control.In_Fps == 4348-ZURE){
		Tama[195].Flg = true;
	}


	if(Control.In_Fps == 4385-ZURE){
		Tama[196].Flg = true;
	}
	if(Control.In_Fps == 4425-ZURE){
		Tama[197].Flg = true;
	}
	if(Control.In_Fps == 4468-ZURE){
		Tama[198].Flg = true;
	}
	if(Control.In_Fps == 4503-ZURE){
		Tama[199].Flg = true;
	}
	if(Control.In_Fps == 4526-ZURE){
		Tama[200].Flg = true;
	}


	if(Control.In_Fps == 4545-ZURE){
		Tama[201].Flg = true;
	}
	if(Control.In_Fps == 4569-ZURE){
		Tama[202].Flg = true;
	}
	if(Control.In_Fps == 4592-ZURE){
		Tama[203].Flg = true;
	}
	if(Control.In_Fps == 4617-ZURE){
		Tama[204].Flg = true;
	}
	if(Control.In_Fps == 4638-ZURE){
		Tama[205].Flg = true;
	}
	if(Control.In_Fps == 4649-ZURE){
		Tama[206].Flg = true;
	}
	if(Control.In_Fps == 4660-ZURE){
		Tama[207].Flg = true;
	}
	if(Control.In_Fps == 4671-ZURE){
		Tama[208].Flg = true;
	}
	if(Control.In_Fps == 4682-ZURE){
		Tama[209].Flg = true;
	}
	if(Control.In_Fps == 4693-ZURE){
		Tama[210].Flg = true;
	}
	if(Control.In_Fps == 4704-ZURE){
		Tama[211].Flg = true;
	}
	if(Control.In_Fps == 4715-ZURE){
		Tama[212].Flg = true;
	}

/////////////////////////////////////////////////////////ここからサビ

//1
	if(Control.In_Fps == 4728-ZURE){
		Tama[213].Flg = true;
	}
	if(Control.In_Fps == 4750-ZURE){
		Tama[214].Flg = true;
	}
	
	if(Control.In_Fps == 4772-ZURE){
		Tama[215].Flg = true;
	}
	if(Control.In_Fps == 4783-ZURE){
		Tama[216].Flg = true;
	}
	if(Control.In_Fps == 4794-ZURE){
		Tama[217].Flg = true;
	}

	if(Control.In_Fps == 4816-ZURE){
		Tama[218].Flg = true;
	}
	if(Control.In_Fps == 4827-ZURE){
		Tama[219].Flg = true;
	}
	if(Control.In_Fps == 4838-ZURE){
		Tama[220].Flg = true;
	}
	if(Control.In_Fps == 4849-ZURE){
		Tama[221].Flg = true;
	}
	if(Control.In_Fps == 4860-ZURE){
		Tama[222].Flg = true;
	}
	if(Control.In_Fps == 4871-ZURE){
		Tama[223].Flg = true;
	}
	if(Control.In_Fps == 4882-ZURE){
		Tama[224].Flg = true;
	}

//2

	if(Control.In_Fps == 4905-ZURE){
		Tama[225].Flg = true;
	}
	if(Control.In_Fps == 4927-ZURE){
		Tama[226].Flg = true;
	}

	if(Control.In_Fps == 4949-ZURE){
		Tama[227].Flg = true;
	}
	if(Control.In_Fps == 4960-ZURE){
		Tama[228].Flg = true;
	}
	if(Control.In_Fps == 4971-ZURE){
		Tama[229].Flg = true;
	}

	if(Control.In_Fps == 4993-ZURE){
		Tama[230].Flg = true;
	}
	if(Control.In_Fps == 5004-ZURE){
		Tama[231].Flg = true;
	}
	if(Control.In_Fps == 5015-ZURE){
		Tama[232].Flg = true;
	}
	if(Control.In_Fps == 5026-ZURE){
		Tama[233].Flg = true;
	}
	if(Control.In_Fps == 5037-ZURE){
		Tama[234].Flg = true;
	}
	if(Control.In_Fps == 5048-ZURE){
		Tama[235].Flg = true;
	}
	if(Control.In_Fps == 5059-ZURE){
		Tama[236].Flg = true;
	}

//3
	if(Control.In_Fps == 5083-ZURE){
		Tama[237].Flg = true;
	}
	if(Control.In_Fps == 5105-ZURE){
		Tama[238].Flg = true;
	}

	if(Control.In_Fps == 5127-ZURE){
		Tama[239].Flg = true;
	}
	if(Control.In_Fps == 5138-ZURE){
		Tama[240].Flg = true;
	}
	if(Control.In_Fps == 5149-ZURE){
		Tama[241].Flg = true;
	}

	if(Control.In_Fps == 5171-ZURE){
		Tama[242].Flg = true;
	}
	if(Control.In_Fps == 5182-ZURE){
		Tama[243].Flg = true;
	}
	if(Control.In_Fps == 5193-ZURE){
		Tama[244].Flg = true;
	}
	if(Control.In_Fps == 5204-ZURE){
		Tama[245].Flg = true;
	}
	if(Control.In_Fps == 5215-ZURE){
		Tama[246].Flg = true;
	}
	if(Control.In_Fps == 5226-ZURE){
		Tama[247].Flg = true;
	}
	if(Control.In_Fps == 5237-ZURE){
		Tama[248].Flg = true;
	}



	if(Control.In_Fps == 5259-ZURE){//245
		Tama[249].Flg = true;
	}

	if(Control.In_Fps == 5304-ZURE){
		Tama[250].Flg = true;
	}

	if(Control.In_Fps == 5348-ZURE){
		Tama[251].Flg = true;
	}
	if(Control.In_Fps == 5372-ZURE){
		Tama[252].Flg = true;
	}

	if(Control.In_Fps == 5395-ZURE){
		Tama[253].Flg = true;
	}
	if(Control.In_Fps == 5406-ZURE){
		Tama[254].Flg = true;
	}
	if(Control.In_Fps == 5417-ZURE){
		Tama[255].Flg = true;
	}
	if(Control.In_Fps == 5428-ZURE){
		Tama[256].Flg = true;
	}


//5
	if(Control.In_Fps == 5439-ZURE){
		Tama[257].Flg = true;
	}
	if(Control.In_Fps == 5461-ZURE){
		Tama[258].Flg = true;
	}

	if(Control.In_Fps == 5483-ZURE){
		Tama[259].Flg = true;
	}
	if(Control.In_Fps == 5494-ZURE){
		Tama[260].Flg = true;
	}
	if(Control.In_Fps == 5505-ZURE){
		Tama[261].Flg = true;
	}


	if(Control.In_Fps == 5528-ZURE){
		Tama[262].Flg = true;
	}
	if(Control.In_Fps == 5539-ZURE){
		Tama[263].Flg = true;
	}
	if(Control.In_Fps == 5550-ZURE){
		Tama[264].Flg = true;
	}
	if(Control.In_Fps == 5561-ZURE){
		Tama[265].Flg = true;
	}
	if(Control.In_Fps == 5572-ZURE){
		Tama[266].Flg = true;
	}
	if(Control.In_Fps == 5583-ZURE){
		Tama[267].Flg = true;
	}
	if(Control.In_Fps == 5594-ZURE){
		Tama[268].Flg = true;
	}


	if(Control.In_Fps == 5617-ZURE){
		Tama[269].Flg = true;
	}
	if(Control.In_Fps == 5639-ZURE){
		Tama[270].Flg = true;
	}

	if(Control.In_Fps == 5661-ZURE){
		Tama[271].Flg = true;
	}
	if(Control.In_Fps == 5672-ZURE){
		Tama[272].Flg = true;
	}
	if(Control.In_Fps == 5683-ZURE){
		Tama[273].Flg = true;
	}


	if(Control.In_Fps == 5705-ZURE){
		Tama[274].Flg = true;
	}
	if(Control.In_Fps == 5716-ZURE){
		Tama[275].Flg = true;
	}
	if(Control.In_Fps == 5727-ZURE){
		Tama[276].Flg = true;
	}
	if(Control.In_Fps == 5738-ZURE){
		Tama[277].Flg = true;
	}
	if(Control.In_Fps == 5749-ZURE){
		Tama[278].Flg = true;
	}
	if(Control.In_Fps == 5760-ZURE){
		Tama[279].Flg = true;
	}
	if(Control.In_Fps == 5771-ZURE){
		Tama[280].Flg = true;
	}


	if(Control.In_Fps == 5794-ZURE){
		Tama[281].Flg = true;
	}
	if(Control.In_Fps == 5816-ZURE){
		Tama[282].Flg = true;
	}

	if(Control.In_Fps == 5838-ZURE){
		Tama[283].Flg = true;
	}
	if(Control.In_Fps == 5849-ZURE){
		Tama[284].Flg = true;
	}
	if(Control.In_Fps == 5860-ZURE){
		Tama[285].Flg = true;
	}


	if(Control.In_Fps == 5882-ZURE){
		Tama[286].Flg = true;
	}
	if(Control.In_Fps == 5893-ZURE){
		Tama[287].Flg = true;
	}
	if(Control.In_Fps == 5904-ZURE){
		Tama[288].Flg = true;
	}
	if(Control.In_Fps == 5915-ZURE){
		Tama[289].Flg = true;
	}
	if(Control.In_Fps == 5926-ZURE){
		Tama[290].Flg = true;
	}
	if(Control.In_Fps == 5937-ZURE){
		Tama[291].Flg = true;
	}
	if(Control.In_Fps == 5948-ZURE){
		Tama[292].Flg = true;
	}


	if(Control.In_Fps == 5971-ZURE){
		Tama[293].Flg = true;
	}
	if(Control.In_Fps == 5994-ZURE){
		Tama[294].Flg = true;
	}
	if(Control.In_Fps == 6018-ZURE){
		Tama[295].Flg = true;
	}


	if(Control.In_Fps == 6061-ZURE){
		Tama[296].Flg = true;
	}
	if(Control.In_Fps == 6084-ZURE){
		Tama[297].Flg = true;
	}
	if(Control.In_Fps == 6105-ZURE){
		Tama[298].Flg = true;
	}
	if(Control.In_Fps == 6129-ZURE){
		Tama[299].Flg = true;
	}

	if(Control.In_Fps == 6129){
		End = true;
	}

}



//走れ
void    Gakuhu_04Loop(void)
{
	if(Control.In_Fps == 512-ZURE){//145
		Tama[1].Flg = true;
	}
	if(Control.In_Fps == 579-ZURE){
		Tama[2].Flg = true;
	}
	if(Control.In_Fps == 645-ZURE){
		Tama[3].Flg = true;
	}
	if(Control.In_Fps == 712-ZURE){
		Tama[4].Flg = true;
	}
	if(Control.In_Fps == 777-ZURE){
		Tama[5].Flg = true;
	}
	if(Control.In_Fps == 846-ZURE){
		Tama[6].Flg = true;
	}
	if(Control.In_Fps == 910-ZURE){
		Tama[7].Flg = true;
	}
	if(Control.In_Fps == 978-ZURE){
		Tama[8].Flg = true;
	}
	if(Control.In_Fps == 1065-ZURE){//145
		Tama[9].Flg = true;
	}
	if(Control.In_Fps == 1158-ZURE){
		Tama[10].Flg = true;
	}
	if(Control.In_Fps == 1244-ZURE){
		Tama[11].Flg = true;
	}
	if(Control.In_Fps == 1335-ZURE){
		Tama[12].Flg = true;
	}
	if(Control.In_Fps == 1422-ZURE){
		Tama[13].Flg = true;
	}
	if(Control.In_Fps == 1520-ZURE){
		Tama[14].Flg = true;
	}
	if(Control.In_Fps == 1600-ZURE){
		Tama[15].Flg = true;
	}
	if(Control.In_Fps == 1688-ZURE){
		Tama[16].Flg = true;
	}
	if(Control.In_Fps == 1736-ZURE){//145
		Tama[17].Flg = true;
	}
	if(Control.In_Fps == 1781-ZURE){
		Tama[18].Flg = true;
	}
	if(Control.In_Fps == 1868-ZURE){
		Tama[19].Flg = true;
	}
	if(Control.In_Fps == 1957-ZURE){
		Tama[20].Flg = true;
	}
	if(Control.In_Fps == 2046-ZURE){
		Tama[21].Flg = true;
	}
	if(Control.In_Fps == 2135-ZURE){
		Tama[22].Flg = true;
	}
	if(Control.In_Fps == 2222-ZURE){
		Tama[23].Flg = true;
	}
	if(Control.In_Fps == 2311-ZURE){
		Tama[24].Flg = true;
	}
	if(Control.In_Fps == 2401-ZURE){//145
		Tama[25].Flg = true;
	}
	if(Control.In_Fps == 2445-ZURE){
		Tama[26].Flg = true;
	}
	if(Control.In_Fps == 2492-ZURE){
		Tama[27].Flg = true;
	}
	if(Control.In_Fps == 2535-ZURE){
		Tama[28].Flg = true;
	}
	if(Control.In_Fps == 2582-ZURE){
		Tama[29].Flg = true;
	}
	if(Control.In_Fps == 2604-ZURE){
		Tama[30].Flg = true;
	}


	if(Control.In_Fps == 2625-ZURE){
		Tama[31].Flg = true;
	}
	if(Control.In_Fps == 2668-ZURE){
		Tama[32].Flg = true;
	}
	if(Control.In_Fps == 2713-ZURE){
		Tama[33].Flg = true;
	}
	if(Control.In_Fps == 2756-ZURE){
		Tama[34].Flg = true;
	}
	if(Control.In_Fps == 2780-ZURE){
		Tama[35].Flg = true;
	}
	if(Control.In_Fps == 2802-ZURE){
		Tama[36].Flg = true;
	}
	if(Control.In_Fps == 2845-ZURE){
		Tama[37].Flg = true;
	}


	if(Control.In_Fps == 2934-ZURE){
		Tama[38].Flg = true;
	}
	if(Control.In_Fps == 3023-ZURE){
		Tama[39].Flg = true;
	}
	if(Control.In_Fps == 3112-ZURE){
		Tama[40].Flg = true;
	}
	if(Control.In_Fps == 3157-ZURE){
		Tama[41].Flg = true;
	}
	if(Control.In_Fps == 3201-ZURE){
		Tama[42].Flg = true;
	}
	if(Control.In_Fps == 3247-ZURE){
		Tama[43].Flg = true;
	}
	if(Control.In_Fps == 3291-ZURE){
		Tama[44].Flg = true;
	}

	if(Control.In_Fps == 3314-ZURE){
		Tama[45].Flg = true;
	}
	if(Control.In_Fps == 3336-ZURE){
		Tama[46].Flg = true;
	}
	if(Control.In_Fps == 3380-ZURE){
		Tama[47].Flg = true;
	}
	if(Control.In_Fps == 3424-ZURE){
		Tama[48].Flg = true;
	}
	if(Control.In_Fps == 3467-ZURE){//145
		Tama[49].Flg = true;
	}
	if(Control.In_Fps == 3490-ZURE){
		Tama[50].Flg = true;
	}
	if(Control.In_Fps == 3512-ZURE){
		Tama[51].Flg = true;
	}

	if(Control.In_Fps == 3556-ZURE){
		Tama[52].Flg = true;
	}
	if(Control.In_Fps == 3602-ZURE){
		Tama[53].Flg = true;
	}
	if(Control.In_Fps == 3646-ZURE){
		Tama[54].Flg = true;
	}
	if(Control.In_Fps == 3667-ZURE){
		Tama[55].Flg = true;
	}
	if(Control.In_Fps == 3689-ZURE){
		Tama[56].Flg = true;
	}
	if(Control.In_Fps == 3735-ZURE){//145
		Tama[57].Flg = true;
	}
	if(Control.In_Fps == 3777-ZURE){
		Tama[58].Flg = true;
	}

	if(Control.In_Fps == 3823-ZURE){
		Tama[59].Flg = true;
	}
	if(Control.In_Fps == 3846-ZURE){
		Tama[60].Flg = true;
	}
	if(Control.In_Fps == 3869-ZURE){
		Tama[61].Flg = true;
	}
	if(Control.In_Fps == 3908-ZURE){
		Tama[62].Flg = true;
	}
	if(Control.In_Fps == 3951-ZURE){//145
		Tama[63].Flg = true;
	}
	if(Control.In_Fps == 3997-ZURE){
		Tama[64].Flg = true;
	}
	if(Control.In_Fps == 4020-ZURE){
		Tama[65].Flg = true;
	}

	if(Control.In_Fps == 4043-ZURE){
		Tama[66].Flg = true;
	}
	if(Control.In_Fps == 4087-ZURE){
		Tama[67].Flg = true;
	}
	if(Control.In_Fps == 4132-ZURE){
		Tama[68].Flg = true;
	}
	if(Control.In_Fps == 4177-ZURE){
		Tama[69].Flg = true;
	}
	if(Control.In_Fps == 4199-ZURE){
		Tama[70].Flg = true;
	}
	if(Control.In_Fps == 4221-ZURE){//145
		Tama[71].Flg = true;
	}
	if(Control.In_Fps == 4267-ZURE){
		Tama[72].Flg = true;
	}

	if(Control.In_Fps == 4313-ZURE){
		Tama[73].Flg = true;
	}
	if(Control.In_Fps == 4354-ZURE){
		Tama[74].Flg = true;
	}
	if(Control.In_Fps == 4377-ZURE){
		Tama[75].Flg = true;
	}
	if(Control.In_Fps == 4400-ZURE){
		Tama[76].Flg = true;
	}
	if(Control.In_Fps == 4443-ZURE){
		Tama[77].Flg = true;
	}
	if(Control.In_Fps == 4488-ZURE){
		Tama[78].Flg = true;
	}
	if(Control.In_Fps == 4534-ZURE){
		Tama[79].Flg = true;
	}
	if(Control.In_Fps == 4556-ZURE){
		Tama[80].Flg = true;
	}
	if(Control.In_Fps == 4577-ZURE){
		Tama[81].Flg = true;
	}
	if(Control.In_Fps == 4598-ZURE){
		Tama[82].Flg = true;
	}
	if(Control.In_Fps == 4619-ZURE){
		Tama[83].Flg = true;
	}

	if(Control.In_Fps == 4619+60*1){
		End = true;
	}

}
//いーあるふぁんくらぶ
void    Gakuhu_05Loop(void)
{
	if(Control.In_Fps == 386-ZURE){//145
		Tama[1].Flg = true;
	}
	if(Control.In_Fps == 412-ZURE){
		Tama[2].Flg = true;
	}
	if(Control.In_Fps == 438-ZURE){
		Tama[3].Flg = true;
	}
	if(Control.In_Fps == 464-ZURE){
		Tama[4].Flg = true;
	}
	if(Control.In_Fps == 489-ZURE){
		Tama[5].Flg = true;
	}
	if(Control.In_Fps == 514-ZURE){
		Tama[6].Flg = true;
	}
	if(Control.In_Fps == 539-ZURE){
		Tama[7].Flg = true;
	}
	if(Control.In_Fps == 550-ZURE){
		Tama[8].Flg = true;
	}
	if(Control.In_Fps == 563-ZURE){//145
		Tama[9].Flg = true;
	}


	if(Control.In_Fps == 590-ZURE){
		Tama[10].Flg = true;
	}
	if(Control.In_Fps == 614-ZURE){
		Tama[11].Flg = true;
	}
	if(Control.In_Fps == 638-ZURE){
		Tama[12].Flg = true;
	}
	if(Control.In_Fps == 663-ZURE){
		Tama[13].Flg = true;
	}
	if(Control.In_Fps == 689-ZURE){
		Tama[14].Flg = true;
	}
	if(Control.In_Fps == 712-ZURE){
		Tama[15].Flg = true;
	}
	if(Control.In_Fps == 736-ZURE){
		Tama[16].Flg = true;
	}
	if(Control.In_Fps == 747-ZURE){//145
		Tama[17].Flg = true;
	}
	if(Control.In_Fps == 760-ZURE){
		Tama[18].Flg = true;
	}

	//
	if(Control.In_Fps == 788-ZURE){
		Tama[19].Flg = true;
	}
	if(Control.In_Fps == 813-ZURE){
		Tama[20].Flg = true;
	}
	if(Control.In_Fps == 838-ZURE){
		Tama[21].Flg = true;
	}
	if(Control.In_Fps == 861-ZURE){
		Tama[22].Flg = true;
	}
	if(Control.In_Fps == 886-ZURE){
		Tama[23].Flg = true;
	}
	if(Control.In_Fps == 911-ZURE){
		Tama[24].Flg = true;
	}
	if(Control.In_Fps == 935-ZURE){//145
		Tama[25].Flg = true;
	}
	if(Control.In_Fps == 947-ZURE){
		Tama[26].Flg = true;
	}
	if(Control.In_Fps == 960-ZURE){
		Tama[27].Flg = true;
	}

	//
	if(Control.In_Fps == 983-ZURE){
		Tama[28].Flg = true;
	}
	if(Control.In_Fps == 1010-ZURE){
		Tama[29].Flg = true;
	}
	if(Control.In_Fps == 1033-ZURE){
		Tama[30].Flg = true;
	}
	if(Control.In_Fps == 1058-ZURE){
		Tama[31].Flg = true;
	}
	if(Control.In_Fps == 1083-ZURE){
		Tama[32].Flg = true;
	}
	if(Control.In_Fps == 1109-ZURE){//145
		Tama[33].Flg = true;
	}
	if(Control.In_Fps == 1134-ZURE){
		Tama[34].Flg = true;
	}
	if(Control.In_Fps == 1147-ZURE){
		Tama[35].Flg = true;
	}
	if(Control.In_Fps == 1159-ZURE){
		Tama[36].Flg = true;
	}

///ういーん

	if(Control.In_Fps == 1183-ZURE){
		Tama[37].Flg = true;
	}
	if(Control.In_Fps == 1284-ZURE){
		Tama[38].Flg = true;
	}
	if(Control.In_Fps == 1332-ZURE){
		Tama[39].Flg = true;
	}

	if(Control.In_Fps == 1380-ZURE){
		Tama[40].Flg = true;
	}
	if(Control.In_Fps == 1406-ZURE){
		Tama[41].Flg = true;
	}
	if(Control.In_Fps == 1432-ZURE){
		Tama[42].Flg = true;
	}

	if(Control.In_Fps == 1481-ZURE){
		Tama[43].Flg = true;
	}
	if(Control.In_Fps == 1506-ZURE){
		Tama[44].Flg = true;
	}
	if(Control.In_Fps == 1533-ZURE){
		Tama[45].Flg = true;
	}

	if(Control.In_Fps == 1580-ZURE){
		Tama[46].Flg = true;
	}
	if(Control.In_Fps == 1604-ZURE){
		Tama[47].Flg = true;
	}
	if(Control.In_Fps == 1632-ZURE){
		Tama[48].Flg = true;
	}

	if(Control.In_Fps == 1679-ZURE){//145
		Tama[49].Flg = true;
	}
	if(Control.In_Fps == 1704-ZURE){
		Tama[50].Flg = true;
	}
	if(Control.In_Fps == 1729-ZURE){
		Tama[51].Flg = true;
	}

	if(Control.In_Fps == 1776-ZURE){
		Tama[52].Flg = true;
	}
	if(Control.In_Fps == 1802-ZURE){
		Tama[53].Flg = true;
	}
	if(Control.In_Fps == 1827-ZURE){
		Tama[54].Flg = true;
	}

	if(Control.In_Fps == 1877-ZURE){
		Tama[55].Flg = true;
	}
	if(Control.In_Fps == 1902-ZURE){
		Tama[56].Flg = true;
	}
	if(Control.In_Fps == 1926-ZURE){//145
		Tama[57].Flg = true;
	}

	if(Control.In_Fps == 1975-ZURE){
		Tama[58].Flg = true;
	}
	if(Control.In_Fps == 1999-ZURE){
		Tama[59].Flg = true;
	}
	if(Control.In_Fps == 2023-ZURE){
		Tama[60].Flg = true;
	}

	if(Control.In_Fps == 2074-ZURE){
		Tama[61].Flg = true;
	}
	if(Control.In_Fps == 2098-ZURE){
		Tama[62].Flg = true;
	}
	if(Control.In_Fps == 2123-ZURE){//145
		Tama[63].Flg = true;
	}

	if(Control.In_Fps == 2174-ZURE){
		Tama[64].Flg = true;
	}
	if(Control.In_Fps == 2198-ZURE){
		Tama[65].Flg = true;
	}

	if(Control.In_Fps == 2224-ZURE){
		Tama[66].Flg = true;
	}
	if(Control.In_Fps == 2247-ZURE){
		Tama[67].Flg = true;
	}
	if(Control.In_Fps == 2273-ZURE){
		Tama[68].Flg = true;
	}
	if(Control.In_Fps == 2298-ZURE){
		Tama[69].Flg = true;
	}
	if(Control.In_Fps == 2323-ZURE){
		Tama[70].Flg = true;
	}
	if(Control.In_Fps == 2335-ZURE){//145
		Tama[71].Flg = true;
	}
	if(Control.In_Fps == 2351-ZURE){
		Tama[72].Flg = true;
	}

	if(Control.In_Fps == 2372-ZURE){
		Tama[73].Flg = true;
	}
	if(Control.In_Fps == 2398-ZURE){
		Tama[74].Flg = true;
	}
	if(Control.In_Fps == 2424-ZURE){
		Tama[75].Flg = true;
	}
	if(Control.In_Fps == 2449-ZURE){
		Tama[76].Flg = true;
	}
	if(Control.In_Fps == 2475-ZURE){
		Tama[77].Flg = true;
	}
	if(Control.In_Fps == 2500-ZURE){
		Tama[78].Flg = true;
	}
	if(Control.In_Fps == 2525-ZURE){
		Tama[79].Flg = true;
	}
	if(Control.In_Fps == 2537-ZURE){
		Tama[80].Flg = true;
	}
	if(Control.In_Fps == 2550-ZURE){
		Tama[81].Flg = true;
	}


	if(Control.In_Fps == 2572-ZURE){
		Tama[82].Flg = true;
	}
	if(Control.In_Fps == 2596-ZURE){
		Tama[83].Flg = true;
	}
	if(Control.In_Fps == 2622-ZURE){
		Tama[84].Flg = true;
	}
	if(Control.In_Fps == 2647-ZURE){
		Tama[85].Flg = true;
	}
	if(Control.In_Fps == 2673-ZURE){
		Tama[86].Flg = true;
	}
	if(Control.In_Fps == 2698-ZURE){//1
		Tama[87].Flg = true;
	}
	if(Control.In_Fps == 2722-ZURE){
		Tama[88].Flg = true;
	}
	if(Control.In_Fps == 2735-ZURE){
		Tama[89].Flg = true;
	}
	if(Control.In_Fps == 2748-ZURE){
		Tama[90].Flg = true;
	}


	if(Control.In_Fps == 2771-ZURE){
		Tama[91].Flg = true;
	}
	if(Control.In_Fps == 2796-ZURE){//2
		Tama[92].Flg = true;
	}
	if(Control.In_Fps == 2822-ZURE){
		Tama[93].Flg = true;
	}
	if(Control.In_Fps == 2846-ZURE){
		Tama[94].Flg = true;
	}
	if(Control.In_Fps == 2873-ZURE){
		Tama[95].Flg = true;
	}
	if(Control.In_Fps == 2897-ZURE){
		Tama[96].Flg = true;
	}
	if(Control.In_Fps == 2919-ZURE){//3
		Tama[97].Flg = true;
	}
	if(Control.In_Fps == 2931-ZURE){
		Tama[98].Flg = true;
	}
	if(Control.In_Fps == 2945-ZURE){
		Tama[99].Flg = true;
	}


	if(Control.In_Fps == 2971-ZURE){
		Tama[100].Flg = true;
	}
	if(Control.In_Fps == 2995-ZURE){
		Tama[101].Flg = true;
	}
	if(Control.In_Fps == 3020-ZURE){
		Tama[102].Flg = true;
	}
	if(Control.In_Fps == 3045-ZURE){//145
		Tama[103].Flg = true;
	}
	if(Control.In_Fps == 3071-ZURE){
		Tama[104].Flg = true;
	}
	if(Control.In_Fps == 3095-ZURE){
		Tama[105].Flg = true;
	}
	if(Control.In_Fps == 3121-ZURE){
		Tama[106].Flg = true;
	}
	if(Control.In_Fps == 3134-ZURE){
		Tama[107].Flg = true;
	}
	if(Control.In_Fps == 3147-ZURE){
		Tama[108].Flg = true;
	}
	if(Control.In_Fps == 3167-ZURE){
		Tama[109].Flg = true;
	}
	if(Control.In_Fps == 3191-ZURE){
		Tama[110].Flg = true;
	}
	if(Control.In_Fps == 3219-ZURE){//145
		Tama[111].Flg = true;
	}
	if(Control.In_Fps == 3242-ZURE){
		Tama[112].Flg = true;
	}
	if(Control.In_Fps == 3267-ZURE){
		Tama[113].Flg = true;
	}
	if(Control.In_Fps == 3290-ZURE){
		Tama[114].Flg = true;
	}
	if(Control.In_Fps == 3316-ZURE){
		Tama[115].Flg = true;
	}
	if(Control.In_Fps == 3327-ZURE){
		Tama[116].Flg = true;
	}
	if(Control.In_Fps == 3341-ZURE){
		Tama[117].Flg = true;
	}
	if(Control.In_Fps == 3365-ZURE){
		Tama[118].Flg = true;
	}
	if(Control.In_Fps == 3393-ZURE){
		Tama[119].Flg = true;
	}
	if(Control.In_Fps == 3419-ZURE){
		Tama[120].Flg = true;
	}
	if(Control.In_Fps == 3442-ZURE){//2607
		Tama[121].Flg = true;
	}
	if(Control.In_Fps == 3467-ZURE){
		Tama[122].Flg = true;
	}
	if(Control.In_Fps == 3491-ZURE){//2633
		Tama[123].Flg = true;
	}
	if(Control.In_Fps == 3517-ZURE){
		Tama[124].Flg = true;
	}
	if(Control.In_Fps == 3529-ZURE){//2658
		Tama[125].Flg = true;
	}
	if(Control.In_Fps == 3541-ZURE){
		Tama[126].Flg = true;
	}
	if(Control.In_Fps == 3566-ZURE){//2682
		Tama[127].Flg = true;
	}
	if(Control.In_Fps == 3592-ZURE){
		Tama[128].Flg = true;
	}


	if(Control.In_Fps == 3617-ZURE){
		Tama[129].Flg = true;
	}
	if(Control.In_Fps == 3640-ZURE){
		Tama[130].Flg = true;
	}
	if(Control.In_Fps == 3664-ZURE){
		Tama[131].Flg = true;
	}
	if(Control.In_Fps == 3689-ZURE){
		Tama[132].Flg = true;
	}
	if(Control.In_Fps == 3714-ZURE){
		Tama[133].Flg = true;
	}
	if(Control.In_Fps == 3726-ZURE){
		Tama[134].Flg = true;
	}
	if(Control.In_Fps == 3738-ZURE){
		Tama[135].Flg = true;
	}
	if(Control.In_Fps == 3762-ZURE){
		Tama[136].Flg = true;
	}
	if(Control.In_Fps == 3810-ZURE){
		Tama[137].Flg = true;
	}

	if(Control.In_Fps == 3835-ZURE){
		Tama[138].Flg = true;
	}
	if(Control.In_Fps == 3885-ZURE){
		Tama[139].Flg = true;
	}
	if(Control.In_Fps == 3910-ZURE){
		Tama[140].Flg = true;
	}
	if(Control.In_Fps == 3935-ZURE){
		Tama[141].Flg = true;
	}
	if(Control.In_Fps == 3960-ZURE){
		Tama[142].Flg = true;
	}
	if(Control.In_Fps == 4010-ZURE){
		Tama[143].Flg = true;
	}
	if(Control.In_Fps == 4035-ZURE){
		Tama[144].Flg = true;
	}

	if(Control.In_Fps == 4083-ZURE){
		Tama[145].Flg = true;
	}
	if(Control.In_Fps == 4108-ZURE){
		Tama[146].Flg = true;
	}
	if(Control.In_Fps == 4132-ZURE){
		Tama[147].Flg = true;
	}
	if(Control.In_Fps == 4157-ZURE){
		Tama[148].Flg = true;
	}
	if(Control.In_Fps == 4208-ZURE){//145
		Tama[149].Flg = true;
	}
	if(Control.In_Fps == 4233-ZURE){
		Tama[150].Flg = true;
	}
	if(Control.In_Fps == 4282-ZURE){
		Tama[151].Flg = true;
	}

	if(Control.In_Fps == 4308-ZURE){
		Tama[152].Flg = true;
	}
	if(Control.In_Fps == 4332-ZURE){
		Tama[153].Flg = true;
	}
	if(Control.In_Fps == 4358-ZURE){
		Tama[154].Flg = true;
	}
	if(Control.In_Fps == 4407-ZURE){
		Tama[155].Flg = true;
	}
	if(Control.In_Fps == 4456-ZURE){
		Tama[156].Flg = true;
	}
	if(Control.In_Fps == 4481-ZURE){//145
		Tama[157].Flg = true;
	}
	if(Control.In_Fps == 4505-ZURE){
		Tama[158].Flg = true;
	}

////サビ

	if(Control.In_Fps == 4558-ZURE){
		Tama[159].Flg = true;
	}
	if(Control.In_Fps == 4582-ZURE){
		Tama[160].Flg = true;
	}
	if(Control.In_Fps == 4606-ZURE){
		Tama[161].Flg = true;
	}
	if(Control.In_Fps == 4617-ZURE){
		Tama[162].Flg = true;
	}
	if(Control.In_Fps == 4631-ZURE){//145
		Tama[163].Flg = true;
	}
	if(Control.In_Fps == 4656-ZURE){
		Tama[164].Flg = true;
	}
	if(Control.In_Fps == 4668-ZURE){
		Tama[165].Flg = true;
	}
	if(Control.In_Fps == 4680-ZURE){
		Tama[166].Flg = true;
	}
	if(Control.In_Fps == 4691-ZURE){
		Tama[167].Flg = true;
	}


	if(Control.In_Fps == 4705-ZURE){
		Tama[168].Flg = true;
	}
	if(Control.In_Fps == 4718-ZURE){
		Tama[169].Flg = true;
	}
	if(Control.In_Fps == 4732-ZURE){
		Tama[170].Flg = true;
	}
	if(Control.In_Fps == 4755-ZURE){
		Tama[171].Flg = true;
	}
	if(Control.In_Fps == 4780-ZURE){
		Tama[172].Flg = true;
	}

	if(Control.In_Fps == 4806-ZURE){
		Tama[173].Flg = true;
	}
	if(Control.In_Fps == 4819-ZURE){
		Tama[174].Flg = true;
	}
	if(Control.In_Fps == 4832-ZURE){
		Tama[175].Flg = true;
	}
	if(Control.In_Fps == 4857-ZURE){
		Tama[176].Flg = true;
	}
	if(Control.In_Fps == 4869-ZURE){
		Tama[177].Flg = true;
	}
	if(Control.In_Fps == 4882-ZURE){
		Tama[178].Flg = true;
	}
	if(Control.In_Fps == 4895-ZURE){
		Tama[179].Flg = true;
	}

	if(Control.In_Fps == 4907-ZURE){
		Tama[180].Flg = true;
	}
	if(Control.In_Fps == 4919-ZURE){
		Tama[181].Flg = true;
	}
	if(Control.In_Fps == 4932-ZURE){
		Tama[182].Flg = true;
	}
	if(Control.In_Fps == 4956-ZURE){
		Tama[183].Flg = true;
	}
	if(Control.In_Fps == 4979-ZURE){
		Tama[184].Flg = true;
	}
	if(Control.In_Fps == 5003-ZURE){
		Tama[185].Flg = true;
	}
	if(Control.In_Fps == 5015-ZURE){
		Tama[186].Flg = true;
	}




	if(Control.In_Fps == 5028-ZURE){//1
		Tama[187].Flg = true;
	}
	if(Control.In_Fps == 5054-ZURE){
		Tama[188].Flg = true;
	}
	if(Control.In_Fps == 5066-ZURE){
		Tama[189].Flg = true;
	}
	if(Control.In_Fps == 5078-ZURE){
		Tama[190].Flg = true;
	}
	if(Control.In_Fps == 5092-ZURE){
		Tama[191].Flg = true;
	}
	if(Control.In_Fps == 5105-ZURE){//2
		Tama[192].Flg = true;
	}
	if(Control.In_Fps == 5119-ZURE){
		Tama[193].Flg = true;
	}
	if(Control.In_Fps == 5131-ZURE){
		Tama[194].Flg = true;
	}
	if(Control.In_Fps == 5153-ZURE){
		Tama[195].Flg = true;
	}
	if(Control.In_Fps == 5178-ZURE){
		Tama[196].Flg = true;
	}
	if(Control.In_Fps == 5202-ZURE){//3
		Tama[197].Flg = true;
	}
	if(Control.In_Fps == 5214-ZURE){
		Tama[198].Flg = true;
	}
	if(Control.In_Fps == 5228-ZURE){
		Tama[199].Flg = true;
	}
	if(Control.In_Fps == 5253-ZURE){
		Tama[200].Flg = true;
	}


	if(Control.In_Fps == 5266-ZURE){
		Tama[201].Flg = true;
	}
	if(Control.In_Fps == 5279-ZURE){//2
		Tama[202].Flg = true;
	}
	if(Control.In_Fps == 5292-ZURE){
		Tama[203].Flg = true;
	}
	if(Control.In_Fps == 5305-ZURE){
		Tama[204].Flg = true;
	}
	if(Control.In_Fps == 5317-ZURE){
		Tama[205].Flg = true;
	}
	if(Control.In_Fps == 5331-ZURE){
		Tama[206].Flg = true;
	}
	if(Control.In_Fps == 5352-ZURE){
		Tama[207].Flg = true;
	}
	if(Control.In_Fps == 5400-ZURE){
		Tama[208].Flg = true;
	}
	if(Control.In_Fps == 5415-ZURE){
		Tama[209].Flg = true;
	}
	if(Control.In_Fps == 5427-ZURE){
		Tama[210].Flg = true;
	}
	if(Control.In_Fps == 5439-ZURE){
		Tama[211].Flg = true;
	}
	if(Control.In_Fps == 5454-ZURE){//2
		Tama[212].Flg = true;
	}
	if(Control.In_Fps == 5478-ZURE){
		Tama[213].Flg = true;
	}
	if(Control.In_Fps == 5505-ZURE){
		Tama[214].Flg = true;
	}
	if(Control.In_Fps == 5528-ZURE){
		Tama[215].Flg = true;
	}

	if(Control.In_Fps == 5528+90){
		End = true;
	}
}
//千本桜
void    Gakuhu_06Loop(void)
{
	if(Control.In_Fps == 287-ZURE){//145
		Tama[1].Flg = true;
	}
	if(Control.In_Fps == 302-ZURE){
		Tama[2].Flg = true;
	}
	if(Control.In_Fps == 331-ZURE){
		Tama[3].Flg = true;
	}
	if(Control.In_Fps == 346-ZURE){
		Tama[4].Flg = true;
	}
	if(Control.In_Fps == 377-ZURE){
		Tama[5].Flg = true;
	}
	if(Control.In_Fps == 392-ZURE){
		Tama[6].Flg = true;
	}
	if(Control.In_Fps == 424-ZURE){
		Tama[7].Flg = true;
	}
	if(Control.In_Fps == 449-ZURE){
		Tama[8].Flg = true;
	}


	if(Control.In_Fps == 470-ZURE){//145
		Tama[9].Flg = true;
	}
	if(Control.In_Fps == 485-ZURE){
		Tama[10].Flg = true;
	}
	if(Control.In_Fps == 519-ZURE){
		Tama[11].Flg = true;
	}
	if(Control.In_Fps == 534-ZURE){
		Tama[12].Flg = true;
	}
	if(Control.In_Fps == 565-ZURE){
		Tama[13].Flg = true;
	}
	if(Control.In_Fps == 589-ZURE){
		Tama[14].Flg = true;
	}
	if(Control.In_Fps == 613-ZURE){
		Tama[15].Flg = true;
	}
	if(Control.In_Fps == 637-ZURE){
		Tama[16].Flg = true;
	}


	if(Control.In_Fps == 659-ZURE){//145
		Tama[17].Flg = true;
	}
	if(Control.In_Fps == 670-ZURE){
		Tama[18].Flg = true;
	}

	if(Control.In_Fps == 705-ZURE){
		Tama[19].Flg = true;
	}
	if(Control.In_Fps == 716-ZURE){
		Tama[20].Flg = true;
	}

	if(Control.In_Fps == 753-ZURE){
		Tama[21].Flg = true;
	}
	if(Control.In_Fps == 764-ZURE){
		Tama[22].Flg = true;
	}

	if(Control.In_Fps == 800-ZURE){
		Tama[23].Flg = true;
	}
	if(Control.In_Fps == 811-ZURE){
		Tama[24].Flg = true;
	}
	if(Control.In_Fps == 822-ZURE){//145
		Tama[25].Flg = true;
	}


	if(Control.In_Fps == 844-ZURE){
		Tama[26].Flg = true;
	}
	if(Control.In_Fps == 859-ZURE){
		Tama[27].Flg = true;
	}
	if(Control.In_Fps == 892-ZURE){
		Tama[28].Flg = true;
	}
	if(Control.In_Fps == 904-ZURE){
		Tama[29].Flg = true;
	}
	if(Control.In_Fps == 937-ZURE){
		Tama[30].Flg = true;
	}
	if(Control.In_Fps == 952-ZURE){
		Tama[31].Flg = true;
	}
	if(Control.In_Fps == 985-ZURE){
		Tama[32].Flg = true;
	}
	if(Control.In_Fps == 998-ZURE){//145
		Tama[33].Flg = true;
	}
	if(Control.In_Fps == 1011-ZURE){
		Tama[34].Flg = true;
	}


	if(Control.In_Fps == 1033-ZURE){
		Tama[35].Flg = true;
	}
	if(Control.In_Fps == 1045-ZURE){
		Tama[36].Flg = true;
	}
	if(Control.In_Fps == 1079-ZURE){
		Tama[37].Flg = true;
	}
	if(Control.In_Fps == 1092-ZURE){
		Tama[38].Flg = true;
	}
	if(Control.In_Fps == 1129-ZURE){
		Tama[39].Flg = true;
	}

	if(Control.In_Fps == 1140-ZURE){
		Tama[40].Flg = true;
	}
	if(Control.In_Fps == 1174-ZURE){
		Tama[41].Flg = true;
	}
	if(Control.In_Fps == 1186-ZURE){
		Tama[42].Flg = true;
	}

	if(Control.In_Fps == 1197-ZURE){
		Tama[43].Flg = true;
	}
	if(Control.In_Fps == 1221-ZURE){
		Tama[44].Flg = true;
	}
	if(Control.In_Fps == 1242-ZURE){
		Tama[45].Flg = true;
	}

	if(Control.In_Fps == 1265-ZURE){
		Tama[46].Flg = true;
	}
	if(Control.In_Fps == 1288-ZURE){
		Tama[47].Flg = true;
	}
	if(Control.In_Fps == 1311-ZURE){
		Tama[48].Flg = true;
	}

	if(Control.In_Fps == 1329-ZURE){//145
		Tama[49].Flg = true;
	}
	if(Control.In_Fps == 1347-ZURE){
		Tama[50].Flg = true;
	}
	if(Control.In_Fps == 1360-ZURE){
		Tama[51].Flg = true;
	}

	if(Control.In_Fps == 1406-ZURE){
		Tama[52].Flg = true;
	}
	if(Control.In_Fps == 1417-ZURE){
		Tama[53].Flg = true;
	}
	if(Control.In_Fps == 1453-ZURE){
		Tama[54].Flg = true;
	}

	if(Control.In_Fps == 1465-ZURE){
		Tama[55].Flg = true;
	}
	if(Control.In_Fps == 1501-ZURE){
		Tama[56].Flg = true;
	}
	if(Control.In_Fps == 1514-ZURE){//145
		Tama[57].Flg = true;
	}

	if(Control.In_Fps == 1548-ZURE){
		Tama[58].Flg = true;
	}
	if(Control.In_Fps == 1559-ZURE){
		Tama[59].Flg = true;
	}
	if(Control.In_Fps == 1572-ZURE){
		Tama[60].Flg = true;
	}

	if(Control.In_Fps == 1596-ZURE){
		Tama[61].Flg = true;
	}
	if(Control.In_Fps == 1607-ZURE){
		Tama[62].Flg = true;
	}
	if(Control.In_Fps == 1640-ZURE){//145
		Tama[63].Flg = true;
	}

	if(Control.In_Fps == 1654-ZURE){
		Tama[64].Flg = true;
	}
	if(Control.In_Fps == 1688-ZURE){
		Tama[65].Flg = true;
	}

	if(Control.In_Fps == 1700-ZURE){
		Tama[66].Flg = true;
	}
	if(Control.In_Fps == 1735-ZURE){
		Tama[67].Flg = true;
	}
	if(Control.In_Fps == 1745-ZURE){
		Tama[68].Flg = true;
	}
	if(Control.In_Fps == 1760-ZURE){
		Tama[69].Flg = true;
	}
	if(Control.In_Fps == 1781-ZURE){
		Tama[70].Flg = true;
	}
	if(Control.In_Fps == 1793-ZURE){//145
		Tama[71].Flg = true;
	}
	if(Control.In_Fps == 1827-ZURE){
		Tama[72].Flg = true;
	}

	if(Control.In_Fps == 1840-ZURE){
		Tama[73].Flg = true;
	}
	if(Control.In_Fps == 1873-ZURE){
		Tama[74].Flg = true;
	}
	if(Control.In_Fps == 1887-ZURE){
		Tama[75].Flg = true;
	}
	if(Control.In_Fps == 1922-ZURE){
		Tama[76].Flg = true;
	}
	if(Control.In_Fps == 1934-ZURE){
		Tama[77].Flg = true;
	}
	if(Control.In_Fps == 1948-ZURE){
		Tama[78].Flg = true;
	}
	if(Control.In_Fps == 1970-ZURE){
		Tama[79].Flg = true;
	}
	if(Control.In_Fps == 1991-ZURE){
		Tama[80].Flg = true;
	}
	if(Control.In_Fps == 2016-ZURE){
		Tama[81].Flg = true;
	}

	if(Control.In_Fps == 2039-ZURE){
		Tama[82].Flg = true;
	}
	if(Control.In_Fps == 2060-ZURE){
		Tama[83].Flg = true;
	}
	if(Control.In_Fps == 2077-ZURE){
		Tama[84].Flg = true;
	}
	if(Control.In_Fps == 2097-ZURE){
		Tama[85].Flg = true;
	}
	if(Control.In_Fps == 2109-ZURE){
		Tama[86].Flg = true;
	}
	if(Control.In_Fps == 2154-ZURE){//1
		Tama[87].Flg = true;
	}
	if(Control.In_Fps == 2179-ZURE){
		Tama[88].Flg = true;
	}
	if(Control.In_Fps == 2204-ZURE){
		Tama[89].Flg = true;
	}
	if(Control.In_Fps == 2226-ZURE){
		Tama[90].Flg = true;
	}


	if(Control.In_Fps == 2249-ZURE){
		Tama[91].Flg = true;
	}
	if(Control.In_Fps == 2272-ZURE){//2
		Tama[92].Flg = true;
	}
	if(Control.In_Fps == 2296-ZURE){
		Tama[93].Flg = true;
	}
	if(Control.In_Fps == 2308-ZURE){
		Tama[94].Flg = true;
	}
	if(Control.In_Fps == 2320-ZURE){
		Tama[95].Flg = true;
	}
	if(Control.In_Fps == 2342-ZURE){
		Tama[96].Flg = true;
	}
	if(Control.In_Fps == 2366-ZURE){//3
		Tama[97].Flg = true;
	}
	if(Control.In_Fps == 2389-ZURE){
		Tama[98].Flg = true;
	}
	if(Control.In_Fps == 2414-ZURE){
		Tama[99].Flg = true;
	}


	if(Control.In_Fps == 2438-ZURE){
		Tama[100].Flg = true;
	}
	if(Control.In_Fps == 2460-ZURE){
		Tama[101].Flg = true;
	}
	if(Control.In_Fps == 2482-ZURE){
		Tama[102].Flg = true;
	}
	if(Control.In_Fps == 2495-ZURE){//145
		Tama[103].Flg = true;
	}
	if(Control.In_Fps == 2508-ZURE){
		Tama[104].Flg = true;
	}
	if(Control.In_Fps == 2530-ZURE){
		Tama[105].Flg = true;
	}
	if(Control.In_Fps == 2553-ZURE){
		Tama[106].Flg = true;
	}
	if(Control.In_Fps == 2577-ZURE){
		Tama[107].Flg = true;
	}
	if(Control.In_Fps == 2599-ZURE){
		Tama[108].Flg = true;
	}
	if(Control.In_Fps == 2623-ZURE){
		Tama[109].Flg = true;
	}
	if(Control.In_Fps == 2646-ZURE){
		Tama[110].Flg = true;
	}
	if(Control.In_Fps == 2670-ZURE){//145
		Tama[111].Flg = true;
	}
	if(Control.In_Fps == 2682-ZURE){
		Tama[112].Flg = true;
	}
	if(Control.In_Fps == 2693-ZURE){
		Tama[113].Flg = true;
	}
	if(Control.In_Fps == 2716-ZURE){
		Tama[114].Flg = true;
	}
	if(Control.In_Fps == 2739-ZURE){
		Tama[115].Flg = true;
	}
	if(Control.In_Fps == 2763-ZURE){
		Tama[116].Flg = true;
	}
	if(Control.In_Fps == 2786-ZURE){
		Tama[117].Flg = true;
	}
	if(Control.In_Fps == 2810-ZURE){
		Tama[118].Flg = true;
	}
	if(Control.In_Fps == 2834-ZURE){
		Tama[119].Flg = true;
	}
	if(Control.In_Fps == 2857-ZURE){
		Tama[120].Flg = true;
	}
	if(Control.In_Fps == 2868-ZURE){//2607
		Tama[121].Flg = true;
	}
	if(Control.In_Fps == 2881-ZURE){
		Tama[122].Flg = true;
	}
	if(Control.In_Fps == 2904-ZURE){//2633
		Tama[123].Flg = true;
	}
	if(Control.In_Fps == 2928-ZURE){
		Tama[124].Flg = true;
	}
	if(Control.In_Fps == 2952-ZURE){//2658
		Tama[125].Flg = true;
	}
	if(Control.In_Fps == 2974-ZURE){
		Tama[126].Flg = true;
	}
	if(Control.In_Fps == 2997-ZURE){//2682
		Tama[127].Flg = true;
	}
	if(Control.In_Fps == 3020-ZURE){
		Tama[128].Flg = true;
	}


	if(Control.In_Fps == 3046-ZURE){
		Tama[129].Flg = true;
	}
	if(Control.In_Fps == 3069-ZURE){
		Tama[130].Flg = true;
	}
	if(Control.In_Fps == 3092-ZURE){
		Tama[131].Flg = true;
	}
	if(Control.In_Fps == 3115-ZURE){
		Tama[132].Flg = true;
	}
	if(Control.In_Fps == 3137-ZURE){
		Tama[133].Flg = true;
	}
	if(Control.In_Fps == 3160-ZURE){
		Tama[134].Flg = true;
	}
	if(Control.In_Fps == 3183-ZURE){
		Tama[135].Flg = true;
	}
	if(Control.In_Fps == 3207-ZURE){
		Tama[136].Flg = true;
	}
	if(Control.In_Fps == 3230-ZURE){
		Tama[137].Flg = true;
	}

	if(Control.In_Fps == 3254-ZURE){
		Tama[138].Flg = true;
	}
	if(Control.In_Fps == 3278-ZURE){
		Tama[139].Flg = true;
	}
	if(Control.In_Fps == 3301-ZURE){
		Tama[140].Flg = true;
	}
	if(Control.In_Fps == 3325-ZURE){
		Tama[141].Flg = true;
	}
	if(Control.In_Fps == 3350-ZURE){
		Tama[142].Flg = true;
	}
	if(Control.In_Fps == 3371-ZURE){
		Tama[143].Flg = true;
	}
	if(Control.In_Fps == 3393-ZURE){
		Tama[144].Flg = true;
	}

	if(Control.In_Fps == 3417-ZURE){
		Tama[145].Flg = true;
	}
	if(Control.In_Fps == 3441-ZURE){
		Tama[146].Flg = true;
	}
	if(Control.In_Fps == 3464-ZURE){
		Tama[147].Flg = true;
	}
	if(Control.In_Fps == 3476-ZURE){
		Tama[148].Flg = true;
	}
	if(Control.In_Fps == 3501-ZURE){//145
		Tama[149].Flg = true;
	}
	if(Control.In_Fps == 3513-ZURE){
		Tama[150].Flg = true;
	}
	if(Control.In_Fps == 3537-ZURE){
		Tama[151].Flg = true;
	}

	if(Control.In_Fps == 3561-ZURE){
		Tama[152].Flg = true;
	}
	if(Control.In_Fps == 3583-ZURE){
		Tama[153].Flg = true;
	}
	if(Control.In_Fps == 3595-ZURE){
		Tama[154].Flg = true;
	}

//サビ

	if(Control.In_Fps == 3653-ZURE){
		Tama[155].Flg = true;
	}
	if(Control.In_Fps == 3669-ZURE){
		Tama[156].Flg = true;
	}
	if(Control.In_Fps == 3687-ZURE){//145
		Tama[157].Flg = true;
	}
	if(Control.In_Fps == 3699-ZURE){
		Tama[158].Flg = true;
	}
	if(Control.In_Fps == 3720-ZURE){
		Tama[159].Flg = true;
	}
	if(Control.In_Fps == 3746-ZURE){
		Tama[160].Flg = true;
	}
	if(Control.In_Fps == 3756-ZURE){
		Tama[161].Flg = true;
	}
	if(Control.In_Fps == 3768-ZURE){
		Tama[162].Flg = true;
	}
	if(Control.In_Fps == 3780-ZURE){//145
		Tama[163].Flg = true;
	}
	if(Control.In_Fps == 3791-ZURE){
		Tama[164].Flg = true;
	}
	if(Control.In_Fps == 3814-ZURE){
		Tama[165].Flg = true;
	}
	if(Control.In_Fps == 3838-ZURE){
		Tama[166].Flg = true;
	}
	if(Control.In_Fps == 3855-ZURE){
		Tama[167].Flg = true;
	}
	if(Control.In_Fps == 3874-ZURE){
		Tama[168].Flg = true;
	}
	if(Control.In_Fps == 3886-ZURE){
		Tama[169].Flg = true;
	}
	if(Control.In_Fps == 3908-ZURE){
		Tama[170].Flg = true;
	}
	if(Control.In_Fps == 3933-ZURE){
		Tama[171].Flg = true;
	}
	if(Control.In_Fps == 3944-ZURE){
		Tama[172].Flg = true;
	}

	if(Control.In_Fps == 3956-ZURE){
		Tama[173].Flg = true;
	}
	if(Control.In_Fps == 3967-ZURE){
		Tama[174].Flg = true;
	}
	if(Control.In_Fps == 3981-ZURE){
		Tama[175].Flg = true;
	}
	if(Control.In_Fps == 4003-ZURE){
		Tama[176].Flg = true;
	}
	if(Control.In_Fps == 4026-ZURE){
		Tama[177].Flg = true;
	}
	if(Control.In_Fps == 4042-ZURE){
		Tama[178].Flg = true;
	}
	if(Control.In_Fps == 4061-ZURE){
		Tama[179].Flg = true;
	}

	if(Control.In_Fps == 4073-ZURE){
		Tama[180].Flg = true;
	}
	if(Control.In_Fps == 4094-ZURE){
		Tama[181].Flg = true;
	}
	if(Control.In_Fps == 4118-ZURE){
		Tama[182].Flg = true;
	}
	if(Control.In_Fps == 4130-ZURE){
		Tama[183].Flg = true;
	}
	if(Control.In_Fps == 4142-ZURE){
		Tama[184].Flg = true;
	}
	if(Control.In_Fps == 4154-ZURE){
		Tama[185].Flg = true;
	}
	if(Control.In_Fps == 4167-ZURE){
		Tama[186].Flg = true;
	}




	if(Control.In_Fps == 4190-ZURE){//1
		Tama[187].Flg = true;
	}
	if(Control.In_Fps == 4214-ZURE){
		Tama[188].Flg = true;
	}
	if(Control.In_Fps == 4236-ZURE){
		Tama[189].Flg = true;
	}
	if(Control.In_Fps == 4260-ZURE){
		Tama[190].Flg = true;
	}
	if(Control.In_Fps == 4283-ZURE){
		Tama[191].Flg = true;
	}
	if(Control.In_Fps == 4305-ZURE){//2
		Tama[192].Flg = true;
	}
	if(Control.In_Fps == 4317-ZURE){
		Tama[193].Flg = true;
	}
	if(Control.In_Fps == 4329-ZURE){
		Tama[194].Flg = true;
	}
	if(Control.In_Fps == 4340-ZURE){
		Tama[195].Flg = true;
	}
	if(Control.In_Fps == 4354-ZURE){
		Tama[196].Flg = true;
	}
	if(Control.In_Fps == 4375-ZURE){//3
		Tama[197].Flg = true;
	}
	if(Control.In_Fps == 4398-ZURE){
		Tama[198].Flg = true;
	}
	if(Control.In_Fps == 4415-ZURE){
		Tama[199].Flg = true;
	}
	if(Control.In_Fps == 4434-ZURE){
		Tama[200].Flg = true;
	}

	if(Control.In_Fps == 4446-ZURE){
		Tama[201].Flg = true;
	}
	if(Control.In_Fps == 4468-ZURE){
		Tama[202].Flg = true;
	}
	if(Control.In_Fps == 4492-ZURE){//145
		Tama[203].Flg = true;
	}
	if(Control.In_Fps == 4505-ZURE){
		Tama[204].Flg = true;
	}
	if(Control.In_Fps == 4516-ZURE){
		Tama[205].Flg = true;
	}
	if(Control.In_Fps == 4527-ZURE){
		Tama[206].Flg = true;
	}
	if(Control.In_Fps == 4539-ZURE){
		Tama[207].Flg = true;
	}
	if(Control.In_Fps == 4562-ZURE){
		Tama[208].Flg = true;
	}
	if(Control.In_Fps == 4585-ZURE){
		Tama[209].Flg = true;
	}
	if(Control.In_Fps == 4602-ZURE){
		Tama[210].Flg = true;
	}
	if(Control.In_Fps == 4622-ZURE){//145
		Tama[211].Flg = true;
	}
	if(Control.In_Fps == 4635-ZURE){
		Tama[212].Flg = true;
	}
	if(Control.In_Fps == 4655-ZURE){
		Tama[213].Flg = true;
	}
	if(Control.In_Fps == 4679-ZURE){
		Tama[214].Flg = true;
	}
	if(Control.In_Fps == 4690-ZURE){
		Tama[215].Flg = true;
	}
	if(Control.In_Fps == 4701-ZURE){
		Tama[216].Flg = true;
	}
	if(Control.In_Fps == 4713-ZURE){
		Tama[217].Flg = true;
	}
	if(Control.In_Fps == 4725-ZURE){
		Tama[218].Flg = true;
	}
	if(Control.In_Fps == 4749-ZURE){
		Tama[219].Flg = true;
	}
	if(Control.In_Fps == 4773-ZURE){
		Tama[220].Flg = true;
	}
	if(Control.In_Fps == 4790-ZURE){//2607
		Tama[221].Flg = true;
	}
	if(Control.In_Fps == 4808-ZURE){
		Tama[222].Flg = true;
	}
	if(Control.In_Fps == 4820-ZURE){//2633
		Tama[223].Flg = true;
	}
	if(Control.In_Fps == 4842-ZURE){
		Tama[224].Flg = true;
	}
	if(Control.In_Fps == 4867-ZURE){//2658
		Tama[225].Flg = true;
	}
	if(Control.In_Fps == 4879-ZURE){
		Tama[226].Flg = true;
	}
	if(Control.In_Fps == 4891-ZURE){//2682
		Tama[227].Flg = true;
	}
	if(Control.In_Fps == 4901-ZURE){
		Tama[228].Flg = true;
	}

	if(Control.In_Fps == 4913-ZURE){
		Tama[229].Flg = true;
	}
	if(Control.In_Fps == 4936-ZURE){
		Tama[230].Flg = true;
	}
	if(Control.In_Fps == 4959-ZURE){
		Tama[231].Flg = true;
	}
	if(Control.In_Fps == 4982-ZURE){
		Tama[232].Flg = true;
	}
	if(Control.In_Fps == 5005-ZURE){
		Tama[233].Flg = true;
	}
	if(Control.In_Fps == 5028-ZURE){
		Tama[234].Flg = true;
	}
	if(Control.In_Fps == 5054-ZURE){
		Tama[235].Flg = true;
	}
	if(Control.In_Fps == 5064-ZURE){
		Tama[236].Flg = true;
	}
	if(Control.In_Fps == 5077-ZURE){
		Tama[237].Flg = true;
	}
	if(Control.In_Fps == 5088-ZURE){
		Tama[238].Flg = true;
	}
	if(Control.In_Fps == 5101-ZURE){
		Tama[239].Flg = true;
	}
	if(Control.In_Fps == 5122-ZURE){
		Tama[240].Flg = true;
	}

	if(Control.In_Fps == 5122+60*1){
		End = true;
	}
}
void	GameLoop(void)
{
	if(KEY_A){
		SceneChange(SENTAKU_SCENE);
	}


	ControlLoop();
	TamaLoop();
	EfeLoop();
	FriendLoop();
	//Gakuhu_01Loop();
	if(Select_Music == 1){
		Gakuhu_02Loop();
	}
	if(Select_Music == 2){
		Gakuhu_03Loop();
	}
	if(Select_Music == 0){
		Gakuhu_04Loop();
	}
	if(Select_Music == 3){
		Gakuhu_05Loop();
	}
	if(Select_Music == 4){
		Gakuhu_06Loop();
	}

	//DEB_TEXT("Select_Music = %d",Select_Music);
	//DEB_TEXT("Control.In_Fps = %d",Control.In_Fps);

	//if(Control.In_Fps >= 4340){
	//	SceneChange(TITLE_SCENE);
	//}

	for(short i=0;i<100;i++){
		if(KEY_Strg){
			if(i == Cnter){
				Deb[i].Flg = true;
			}
		}
	}
	if(KEY_Strg){
			Cnter ++;
		}
	for(short i=0;i<100;i++){
		Deb[i].Cnt ++;
		if(Deb[i].Flg){
			Deb[i].Cnt --;
		}
	}
	for(short i=0;i<100;i++){
		//DEB_TEXT("Friend.T_Flg = %d",Deb[i].Cnt);
		//DEB_TEXT("Friend.T_Flg = %d",Tama[i].E_Flg);
		//DEB_TEXT("Tama[i].Scl = %f",Tama[i].Scl);
	}

	
}
	

//--------------------------------------------------------------------------//1280
//	●	描画		：シーン中の描画、現在のシーンで表示するBMPはここで
//
//--------------------------------------------------------------------------//
// ゲーム画面　描画
void	GameDraw(void){
	
	
	//Gs_DrawBox(0,0,WINW,WINH,ARGB(255,0,0,0),true);//背景
	Gs_DrawLayer(-550,-1400,HAIKEI,0,0,1024,2048,false,ARGB(255,255,255,255),false,0,1.2f,1.2f);//背景

	if(!End){
		Gs_DrawLayer(I(Friend.Px),I(Friend.Py-Friend.Level*10),MONSTER,0,512*Friend.Type,512,512,false,ARGB(255,255,255,255),true,S(Friend.Ang),0.4f+Friend.Level*0.1f,0.4f+Friend.Level*0.1f);//ふれんど
	}
	if(HANA){
		Gs_DrawBox(0,0,WINW,WINH,ARGB(Palp,255,255,255),true);//背景
	}

//フロント

	//たべる
	Gs_DrawBox(0,WINH-368,WINW,WINH-268,ARGB(255,255,200,200),true);//赤
	if(Control.Eat_Flg){
		Gs_DrawBox(0,WINH-368,WINW,WINH-268,ARGB(255,255,140,140),true);//赤
	}
	Gs_DrawLayer(WINW/2-8,WINH/2+164,UI_01,0,128,512,128,false,ARGB(255,255,255,255),true,0,0.425f+Ui[1].Scl,0.425f+Ui[1].Scl);//食べる

	//きたえる
	Gs_DrawBox(0,WINH-268,WINW/2,WINH,ARGB(255,200,255,200),true);//緑
	if(Control.Drill_Flg){
		Gs_DrawBox(0,WINH-268,WINW/2,WINH,ARGB(255,140,255,140),true);//緑
	}
	Gs_DrawLayer(92,WINH-134,UI_01,0,0,512,128,false,ARGB(255,255,255,255),true,0,0.425f+Ui[0].Scl,0.425f+Ui[0].Scl);//きたえる

	//あそぶ
	Gs_DrawBox(WINW/2,WINH-268,WINW,WINH,ARGB(255,200,200,255),true);//青
	if(Control.Enjoy_Flg){
		Gs_DrawBox(WINW/2,WINH-268,WINW,WINH,ARGB(255,140,140,255),true);//青
	}
	Gs_DrawLayer(640-92,WINH-134,UI_01,0,256,512,128,false,ARGB(255,255,255,255),true,0,0.425f+Ui[2].Scl,0.425f+Ui[2].Scl);//きたえる

	//UI
	Gs_DrawLayer(0,0,T_GAGEP,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,1);//
	Gs_DrawLayer(320,0,T_GAGEP,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,1);//
	//Gs_DrawLayer(0,0,T_GAGEP,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//ゲージ下地その1
	//Gs_DrawLayer(320,0,T_GAGEP,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//ゲージ下地その2
	//Gs_DrawLayer(0,32,T_GAGEP,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//ゲージ下地その3
	//Gs_DrawLayer(320,32,T_GAGEP,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//ゲージ下地その4
	for(short i=0;i<80;i++){
		if(Gage[i].gage != 4){//出現させる
			if(Gage[i].gage == RED){
				Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,255,150,150),false,0,0.5f,1);//ゲージ本体
			}
			if(Gage[i].gage == BLUE){
				Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,150,150,255),false,0,0.5f,1);//ゲージ本体
			}
			if(Gage[i].gage == YELLOW){
				Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,245,255,150),false,0,0.5f,1);//ゲージ本体
			}
			if(Gage[i].gage == GREEN){
				Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,120,255,120),false,0,0.5f,1);//ゲージ本体
			}
		}
		//if(Gage[i].gage != 4){//出現させる
		//	if(Gage[i].gage == RED){
		//		Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,255,150,150),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//	if(Gage[i].gage == BLUE){
		//		Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,150,150,255),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//	if(Gage[i].gage == YELLOW){
		//		Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,245,255,150),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//	if(Gage[i].gage == GREEN){
		//		Gs_DrawLayer(8*i,0,T_GAGEH,0,0,16,64,false,ARGB(255,120,255,120),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//}
	}

	for(short i=80;i<160;i++){

		//if(Gage[i].gage != 4){//出現させる
		//	if(Gage[i].gage == RED){
		//		Gs_DrawLayer(8*(i-80),32,T_GAGEH,0,0,16,64,false,ARGB(255,255,150,150),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//	if(Gage[i].gage == BLUE){
		//		Gs_DrawLayer(8*(i-80),32,T_GAGEH,0,0,16,64,false,ARGB(255,150,150,255),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//	if(Gage[i].gage == YELLOW){
		//		Gs_DrawLayer(8*(i-80),32,T_GAGEH,0,0,16,64,false,ARGB(255,245,255,150),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//	if(Gage[i].gage == GREEN){
		//		Gs_DrawLayer(8*(i-80),32,T_GAGEH,0,0,16,64,false,ARGB(255,120,255,120),false,0,0.5f,0.5f);//ゲージ本体
		//	}
		//}
	}

	Gs_DrawLayer(0,0,T_GAGE,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,1);//食べるゲージその1
	Gs_DrawLayer(320,0,T_GAGE,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,1);//食べるゲージその2

	//Gs_DrawLayer(0,0,T_GAGE,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//食べるゲージその1
	//Gs_DrawLayer(320,0,T_GAGE,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//食べるゲージその2

	//Gs_DrawLayer(0,32,T_GAGE,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//食べるゲージその3
	//Gs_DrawLayer(320,32,T_GAGE,0,0,640,64,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//食べるゲージその4


	Gs_DrawLayer(WINW/2-320,WINH/7+24,GAGE,0,0,128,512,false,ARGB(255,255,255,255),false,0,0.75f,0.75f);//ゲージ左下地
	Gs_DrawLayer(94,547,GAGE,128,0,126,I(Control.Drill),false,ARGB(255,255,255,255),false,180,0.75f,0.75f);//ゲージ左
	Gs_DrawLayer(WINW/2-320,WINH/7+24,GAGE,256,0,128,512,false,ARGB(192,255,255,255),false,0,0.75f,0.75f);//ゲージ左下地

	Gs_DrawLayer(WINW/2+224,WINH/7+24,GAGE2,0,0,128,512,false,ARGB(255,255,255,255),false,0,0.75f,0.75f);//ゲージ左下地
	Gs_DrawLayer(97+544,547,GAGE2,128,0,126,I(Control.Enjoy),false,ARGB(255,255,255,255),false,180,0.75f,0.75f);//ゲージ左
	Gs_DrawLayer(WINW/2+224,WINH/7+25,GAGE2,256,0,128,512,false,ARGB(192,255,255,255),false,0,0.75f,0.75f);//ゲージ左下地

	//Gs_DrawLayer(WINW/2+230,WINH/7+24,GAGE,0,0,64,256,false,ARGB(255,255,255,255),false,0,1.5f,1.5f);//ゲージ右下地
	//Gs_DrawLayer(WINW/2+326,521+24,GAGE,64*2,0,64,I(Control.Enjoy),false,ARGB(255,255,255,255),false,0,1.5f,1.5f);//ゲージ右

	//SceneChange(TITLE_SCENE);

	Gs_DrawLayer(WINW/2,WINH-134,TAMA2,0,0,256,256,false,ARGB(255,156,156,156),true,0,1.05f,1.05f);//判定部分
	for(short i=0;i<TAMAMAX;i++){
		if(Tama[i].Scl >= 0.84f && Tama[i].Scl <= 1.06f && !Tama[i].Miss_Flg && !Tama[i].TamaP_Flg){
			Gs_DrawLayer(WINW/2,WINH-134,TAMA2,0,0,256,256,false,ARGB(200,245,255,150),true,0,1.05f,1.05f);//黄色部分
		}
	}
	Gs_DrawLayer(WINW/2,WINH-134,TAMA2,0,0,256,256,false,ARGB(255,96,96,96),true,0,0.85f,0.85f);//判定部分

	//for(short i=0;i<TAMAMAX;i++){
	Gs_DrawLayer(WINW/2,WINH-134,SEN,0,0,256,256,false,ARGB(32,0,0,0),true,0,0.97f,0.97f);
	for(short i=TAMAMAX-1;i>-1;i--){
		if(Tama[i].TamaP_Flg && !Tama[i].Miss_Flg){
			Gs_DrawLayer(I(Tama[i].Px),I(Tama[i].Py),TAMA,256*Tama[i].Color,0,256,256,false,ARGB(255,255,255,255),true,0,Tama[i].Scl,Tama[i].Scl);//リズム玉
		}

		if(!Tama[i].Miss_Flg && Tama[i].Flg && !Tama[i].TamaP_Flg){//もしもTama[i].Flgがtrueならリズム玉が出現
			Gs_DrawLayer(I(Tama[i].Px),I(Tama[i].Py),TAMA,256*Tama[i].Color,0,256,256,false,ARGB(30,255,255,255),true,0,Tama[i].Scl,Tama[i].Scl);//リズム玉
			Gs_DrawLayer(I(Tama[i].Px),I(Tama[i].Py),SEN,256*Tama[i].Color,0,256,256,false,ARGB(255,255,255,255),true,0,Tama[i].Scl,Tama[i].Scl);//リズム玉
		}
	}
	Gs_DrawBox(0,0,WINW,WINH,ARGB(Black,0,0,0),true);//赤
	if(End){
		Gs_DrawLayer(I(Friend.Px)+I(Plus.Px),I(Friend.Py-Friend.Level*10)+I(Plus.Py),MONSTER,0,512*Friend.Type,512,512,false,ARGB(255,255,255,255),true,0,0.4f+Friend.Level*0.1f+(Plus.Scl),0.4f+Friend.Level*0.1f+(Plus.Scl));//ふれんど
	}

	//星
	for(short i=0;i<EFEMAX;i++){
		if(Efe[i].Flg){
			if(Green){
				Gs_DrawLayer(I(Efe[i].Px),I(Efe[i].Py),STAR,0,0,128,128,false,ARGB(Efe[i].alp,30,255,30),true,0,0.2f,0.2f);//緑星
			}
			if(Blue){
				Gs_DrawLayer(I(Efe[i].Px),I(Efe[i].Py),STAR,0,0,128,128,false,ARGB(Efe[i].alp,150,150,255),true,0,0.2f,0.2f);//青星
			}
			if(Red){
				Gs_DrawLayer(I(Efe[i].Px),I(Efe[i].Py),STAR,0,0,128,128,false,ARGB(Efe[i].alp,255,150,150),true,0,0.2f,0.2f);//赤星
			}
		}
	}
	if(End){
		Gs_DrawLayer(WINW/2,PPy,MOZIZI,0,0,1024,128,false,ARGB(255,255,255,255),true,0,1.5f,1.5f);//育成完了
		//Gs_DrawLayer(PPx,640,MOZIZI,0,128,1024,128,false,ARGB(255,255,255,255),true,0,1.f,1.f);//育成完了
	}

	if(Mouse.Px >= 20 && Mouse.Px <= 620 && Mouse.Py >= 20 && Mouse.Py <= 940){
		Gs_DrawLayer(Mouse.Px,Mouse.Py+32,YUBI_M,0,0,256,256,false,ARGB(64,255,255,255),true,0,0.25f,0.25f);//指
	}



}
//--------------------------------------------------------------------------//
//	●	ロード		：シーンに入る時、そのシーンで必要なデータの読み込み
//
//--------------------------------------------------------------------------//
short	GameLoad(void)
{
	HAIKEI  = Gs_LoadBMP("DATA/BMP/haikei4.PNG");
	MOZIZI  = Gs_LoadBMP("DATA/BMP/newfriend.PNG");

	MONSTER  =  Gs_LoadBMP("DATA/BMP/monster.PNG");

	TAMA  = Gs_LoadBMP("DATA/BMP/melodyball.PNG");
	SEN  = Gs_LoadBMP("DATA/BMP/rizumu.PNG");

	TAMA2  = Gs_LoadBMP("DATA/BMP/ball.PNG");
	GAGE  = Gs_LoadBMP("DATA/BMP/s_gage.PNG");
	GAGE2  = Gs_LoadBMP("DATA/BMP/su_gage.PNG");//ストレスゲージ
	UI_01  = Gs_LoadBMP("DATA/BMP/mozi_UI.PNG");
	STAR  = Gs_LoadBMP("DATA/BMP/star.PNG");
	T_GAGEP  = Gs_LoadBMP("DATA/BMP/taberuv.PNG");
	T_GAGEH  = Gs_LoadBMP("DATA/BMP/sikakukun.PNG");
	T_GAGE  = Gs_LoadBMP("DATA/BMP/taberu.PNG");
	YUBI_M  = Gs_LoadBMP("DATA/BMP/yubi.PNG");

	BGM_01     = Gs_LoadWAVE("DATA/SOUND/bgm_01.wav",false);
	BGM_02     = Gs_LoadWAVE("DATA/SOUND/Plastic Sky.wav",false);
	BGM_03     = Gs_LoadWAVE("DATA/SOUND/ロストワンの号哭.wav",false);
	BGM_04    = Gs_LoadWAVE("DATA/SOUND/走れ.wav",false);
	BGM_05    = Gs_LoadWAVE("DATA/SOUND/いーあるふぁんくらぶ.wav",false);
	BGM_06    = Gs_LoadWAVE("DATA/SOUND/千本桜.wav",false);

	SUZU     = Gs_LoadWAVE("DATA/SOUND/suzu.wav",false);

	return		GAME_SCENE;
}
//--------------------------------------------------------------------------//
//	●	後処理		：現在のシーンから抜ける時、データの破棄
//
//--------------------------------------------------------------------------//
void	GameExit(void)
{
	Gs_ReleaseBMP(HAIKEI);

	Gs_ReleaseBMP(MONSTER);

	Gs_ReleaseBMP(TAMA);
	Gs_ReleaseBMP(SEN);
	Gs_ReleaseBMP(TAMA2);
	Gs_ReleaseBMP(T_GAGE);
	Gs_ReleaseBMP(T_GAGEH);
	Gs_ReleaseBMP(T_GAGEP);
	Gs_ReleaseBMP(GAGE);
	Gs_ReleaseBMP(GAGE2);
	Gs_ReleaseBMP(UI_01);
	Gs_ReleaseBMP(STAR);
	Gs_ReleaseBMP(YUBI_M);
	Gs_ReleaseBMP(MOZIZI);
	Gs_ReleaseSOUND(SUZU);
	Gs_ReleaseSOUND(BGM_01);
	Gs_ReleaseSOUND(BGM_02);
	Gs_ReleaseSOUND(BGM_03);
	Gs_ReleaseSOUND(BGM_04);
	Gs_ReleaseSOUND(BGM_05);
	Gs_ReleaseSOUND(BGM_06);


}

//****************************************************************************
//	★　シーンテーブル：各シーンで呼び出される関数の登録
//****************************************************************************

SCENE_TBL	GameSceneTbl = {
		GameLoad,
		GameInit,
		GameLoop,
		GameDraw,		
		GameExit
};

//======================================================================================//
//							EOP															//
//======================================================================================//
