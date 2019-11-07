//======================================================================================//
//																						//
//				Ｇａｍｅ　Ｄｅｆｉｎｅ													//
//																						//
//======================================================================================//

//------------------------------------------------------------------//
//	★	ゲーム個別の定義
//------------------------------------------------------------------//
#define		TITLE_NAME			"リズムふれんど"

//------------------------------------------------------------------//
//	★	シーン番号の定義
//------------------------------------------------------------------//

enum {
	GAME_SCENE,						//  ゲーム
	TITLE_SCENE,                    //  タイトル
	RESULT_SCENE,                   //  リザルト
	SENTAKU_SCENE,                  //  選択画面
	TETYOU_SCENE,                  //  選択画面
	SETTEI_SCENE,                  //  選択画面

	SCENE_MAX
};

//#define		START_SCENE		TITLE_SCENE		//　起動直後のシーン
//#define		START_SCENE		GAME_SCENE
#define		START_SCENE		TITLE_SCENE
//#define		START_SCENE		RESULT_SCENE


//------------------------------------------------------------------//
//	★	グローバルデータ(シーン共用データ)構造体
//     シーン間共用(受け渡す）データを追加登録する
//     ※各シーンのInit部、Exit部のみ参照、書き換えできる領域とする
//------------------------------------------------------------------//
typedef	struct {

	short	Mode;
	short	Param[8];

	int		score;
	int		time;

}GLOBAL_DATA;

//☆グローバルデータ 共用化宣言　（DataTbl.cpp）
extern	GLOBAL_DATA		GamePublic;


//======================================================================================//
//	アプリケーション　共用宣言															//
//======================================================================================//

#define   KEY_S      Gs_GetKEY(kKEYSPACE)
#define   KEY_Strg   Gs_GetKEYtrg(kKEYSPACE)
#define   KEY_Spul   Gs_GetKEYpul(kKEYSPACE)

#define   KEY_A      Gs_GetKEY(kKEY_A)
#define   KEY_Atrg   Gs_GetKEYtrg(kKEY_A)
#define   KEY_Apul   Gs_GetKEYpul(kKEY_A)

#define   KEY_B      Gs_GetKEY(kKEY_B)
#define   KEY_Btrg   Gs_GetKEYtrg(kKEY_B)
#define   KEY_Bpul   Gs_GetKEYpul(kKEY_B)

#define   KEY_X      Gs_GetKEY(kKEY_C)
#define   KEY_Xtrg   Gs_GetKEYtrg(kKEY_C)
#define   KEY_Xpul   Gs_GetKEYpul(kKEY_C)

#define   KEY_Y      Gs_GetKEY(kKEY_X)
#define   KEY_Ytrg   Gs_GetKEYtrg(kKEY_X)
#define   KEY_Ypul   Gs_GetKEYpul(kKEY_X)

#define   KEY_L      Gs_GetKEY(kKEY_Y)
#define   KEY_Ltrg   Gs_GetKEYtrg(kKEY_Y)
#define   KEY_Lpul   Gs_GetKEYpul(kKEY_Y)

#define   KEY_R      Gs_GetKEY(kKEY_Z)
#define   KEY_Rtrg   Gs_GetKEYtrg(kKEY_Z)
#define   KEY_Rpul   Gs_GetKEYpul(kKEY_Z)

///////キー入力///////


#define   KEY_WLEFT       (_lMOUSE & Mouse.WClk) && !(_rMOUSE & Mouse.WClk)
#define   KEY_WLEFTtrg    (_lMOUSE & Mouse.WClk) && !(_rMOUSE & Mouse.WClk)
#define   KEY_WRIGHT      (_rMOUSE & Mouse.WClk) && !(_lMOUSE & Mouse.WClk)
#define   KEY_WRIGHTtrg   (_rMOUSE & Mouse.WClk) && !(_lMOUSE & Mouse.WClk)

#define   KEY_RIGHT      (_rMOUSE & Mouse.Now) && !(_lMOUSE & Mouse.Now)
#define   KEY_RIGHTtrg   (_rMOUSE & Mouse.Trg) && !(_lMOUSE & Mouse.Trg)
#define   KEY_RIGHTpul   (_rMOUSE & Mouse.Pul) && !(_lMOUSE & Mouse.Pul)

#define   KEY_LEFT       (_lMOUSE & Mouse.Now) && !(_rMOUSE & Mouse.Now)
#define   KEY_LEFTtrg    (_lMOUSE & Mouse.Trg) && !(_rMOUSE & Mouse.Trg)
#define   KEY_LEFTpul    (_lMOUSE & Mouse.Pul) && !(_rMOUSE & Mouse.Pul)

#define   KEY_UP         (_lMOUSE & Mouse.Now) && (_rMOUSE & Mouse.Now)
#define   KEY_UPtrg      (_lMOUSE & Mouse.Trg) && (_rMOUSE & Mouse.Now)
#define   KEY_UPpul      (_lMOUSE & Mouse.Pul) && (_rMOUSE & Mouse.Pul)

#define   KEY_DOWN       (_lMOUSE & Mouse.Trg)
#define   KEY_DOWNtrg    (_lMOUSE & Mouse.Trg)
#define   KEY_DOWNpul    (_lMOUSE & Mouse.Trg)

#define   KEY_ANYtrg      Gs_GetKEYtrg(kKEYRIGHT) ||  Gs_GetKEYtrg(kKEYLEFT)  ||  Gs_GetKEYtrg(kKEYDOWN) ||  Gs_GetKEYtrg(kKEYUP)

//======================================================================================//
//	Ｅｎｄ																				//
//======================================================================================//
