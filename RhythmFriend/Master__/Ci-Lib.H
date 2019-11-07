//======================================================================================//
//																						//
//					Commonness　Define													//
//																						//
//======================================================================================//
#define			DIRECTINPUT_VERSION		0x0800

#pragma			warning( disable : 4312 )
#pragma			warning( disable : 4311 )
#pragma			warning( disable : 4819 )
#pragma			warning( disable : 4800 )
#pragma			warning( disable : 4996 )


//#define		VC6

#ifdef	VC6
	#define		vsprintf_s		vsprintf
	#define		sprintf_s		sprintf
	#define		strcat_s		strcat
#endif

#include		<d3d9.h>
#include		<d3dx9.h>
#include		<ddraw.h>
#include		<dInput.h>

//------------------------------------------------------------------------------//
//	★	環境設定
//------------------------------------------------------------------------------//
//#define		KEY_SCAN					// Mouse&Key Scan MODE
//#define		KEY_PLAY					// Mouse&Key Play MODE

#define		KEY_DISP	OFF				// Mouse&Key Play DISP ON/OFF


//#define		PAD4_USE					// 4Pad Use
//#define		FULLSCREEN					// Full Screen Switch
//#define		MOUSE_DISP					// Mouse Disp On
#define		DEBUG_DISP					// Debug Mode
//#define		DX3_PLUS
//------------------------------------------------------------------------------//
//	★	共用                                                                    //
//------------------------------------------------------------------------------//
typedef	unsigned long	u_long;
typedef	unsigned short	u_short;
typedef	unsigned char	u_char;

enum		{OFF,	ON};
enum		{SMALL,	LARGE};
enum		{EVEN,	ODD};
enum		{NG,	OK};
enum		{NO,	YES};
enum		{DEAD,	ALIVE};

#define		NULL		0
#define     ESC			0x1b			//Escape Key Code 

//------------------------------------------------------------------------------//
//	★	Wide Use Macro                                                          //
//------------------------------------------------------------------------------//
#define		PAI				((double)(3.14159265358979323846264338328))  // π 円周率

// 回転Ｘ座標・回転Ｙ座標
#define		ROUND_X(Angle,Length,Center)	(float)(cos((Angle)*PAI*2/360)*(Length)+(Center))
#define		ROUND_Y(Angle,Length,Center)	(float)(sin((Angle)*PAI*2/360)*(Length)+(Center))

#define		RAD(x)			(((double)(x))*((double)((2.0*PAI)/360.0)))     // 角度→ラジアン
#define		DEG(x)			(((double)(x))*((double)(360.0/(2.0*PAI))))     // ラジアン→角度
#define		xRAD(x)			D3DXToRadian(x)									// 角度→ラジアン
#define		xDEG(x)			D3DXToDegree(x)									// ラジアン→角度

#define		OBLIQ(x,y)		F( sqrt(pow(double(x),2)+pow(double(y),2)) )	// 三角形斜辺の長さ
#define		ATANG(x,y)		F(DEG( atan2(F(y),F(x)) ))						// ｘ、ｙから角度（タンジェント）

// Ａ点から見たＢ点の角度（０～３５９）
#define		DIR_AtoB(ax,ay,bx,by)	REMF(ATANG((bx-ax), (by-ay))+360,360 )	

// Ａ点とＢ点の距離
#define		LEN_AtoB(ax,ay,bx,by)	OBLIQ( (bx-ax),(by-ay) )


#define		REMF(a,b)		(F(a)-( F(b * (I(a)/I(b))) ))		// 少数点の余剰演算 
#define		SWAP(a,b)		{	a = b-a; b = b-a; a = a+b; }	// aとｂの入れ替え
#define		ABS(x)			(((x)>=0)?(x):-(x))
#define		SGN(a)			((a)? (a)/abs(a) :0)				// (a=0)...0, (a>0)...1,(a<0)...-1
#define		FSGN(a)			((a)? (a)/fabs(a):0)				// (a=0)...0, (a>0)...1,(a<0)...-1
#define		CLIP(a,b)		((a)/(b))*(b)						// bの商算余剰切捨て
#define		LIMIT(a,min,max)	{	if (a<min)	a=min;	if (a>max)	a=max;	}


#define		F(x)			((float)(x))
#define		I(x)			((int)(x))
#define		S(x)			((short)(x))
#define		IF(x)			I(F(x))
#define		IB(x)			((x) ? true:false)



// 簡易関数マクロ
#define		ARGB(a,r,g,b)	(((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))

#define		Gs_SaveFILE(fn,n)			Gs_FileDataWrite(fn,(char *)&n,sizeof(n))
#define		Gs_LoadFILE(fn,n)			Gs_FileDataLoad(fn,(char *)&n,sizeof(n))

#define		Gs_DrawTex(x,y,id,sx,sy,w,h)	Gs_DrawDirectLayer(x,y,id,sx,sy,w,h,OFF,ARGB(255,255,255,255))

//------------------------------------------------------------------------------//
//	★	デバッグ関数　定義                                                       //
//------------------------------------------------------------------------------//
#ifdef DEBUG_DISP
#define		DEB_INIT				Gs_DebugInit
#define		DEB_DRAW				Gs_DebugDraw
#define		DEB_TEXT				Gs_DebugOut
#define		DEB_XY					Gs_DebugOutXY
#else
#define		DEB_INIT				//
#define		DEB_DRAW				//
#define		DEB_TEXT				//
#define		DEB_XY					//
#endif

//------------------------------------------------------------------------------//
//	★	ゲーム共通の定義                                                        //
//------------------------------------------------------------------------------//
#define		WINW					(640)							// ウィンドウ　Width
#define		WINH					(960)							// ウィンドウ　Height

#define		FRAME_LATE				60								// １フレームタイム６０
#define		FRAME_LATE30			30

#define		FONT_SIZE				64								//	TEXT文字サイズ
#define		FONT_TYPE				4

#define		TRANS_COLOR				ARGB(0xff,0x00,0x00,0x00)		// ヌケ色指定
//#define		BACK_COLOR				ARGB(0xff,21,70,10)				// 背景色
#define		BACK_COLOR				ARGB(0xff,0x00,0x00,0x00)		// 背景色

#define		BETA_COLOR				ARGB(0xff,0xff,0xff,0xff)		// 元色
#define		HALF_COLOR				ARGB(0x80,0xff,0xff,0xff)		// 半透明色
#define		RED_COLOR				ARGB(0xff,0xff,0x00,0x00)		// 赤明色
#define		GREEN_COLOR				ARGB(0xff,0x00,0xff,0x00)		// 緑明色
#define		BLUE_COLOR				ARGB(0xff,0x00,0x00,0xff)		// 青明色

typedef		short					GsTEXTURE;
typedef		short					GsSOUND;
typedef		u_long					GsCOLOR;

typedef		LPDIRECT3DTEXTURE9		TEX9;

// FVF
#define D3DFVF_VERTEX \
	(D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX2|D3DFVF_TEXCOORDSIZE2(0)|D3DFVF_TEXCOORDSIZE2(1))

#define D3DFVF_SWORDTRAIL_VERTEX \
	(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1)

struct MYVERTEX {
	
	float X, Y, Z, RHW;			// 変換済み頂点
	D3DCOLOR Diffuse;			// ディフューズ色
	float U0, V0;				// テクスチャ座標
	float U1, V1;
};

//------------------------------------------------------------------------------//
//	★	Class macro                                                             //
//------------------------------------------------------------------------------//
#define		HEAD_START(name)	class name{protected:
#define		HEAD_PROG(name)		private:
#define		HEAD_END(name)		};

//------------------------------------------------------------------------------//
//	★	キー、パッドフラグ用定数                                                //
//------------------------------------------------------------------------------//

enum {PAD_1P,PAD_2P,PAD_3P,PAD_4P};

#ifdef PAD4_USE
enum {eIDEV0,eIDEV1,eIDEV2,eIDEV3,eIDEVNUM};
#else
enum {eIDEV0,eIDEVNUM};
#endif

#define		kKEYNONE		0x0000

#define		kKEYUP			0x0001
#define		kKEYDOWN		0x0002
#define		kKEYRIGHT		0x0004
#define		kKEYLEFT		0x0008

#define		kPADSTART		0x0010
#define		kPADSELECT		0x0020
#define		kPAD_A			0x0040
#define		kPAD_B			0x0080
#define		kPAD_C			0x0100
#define		kPAD_X			0x0200
#define		kPAD_Y			0x0400
#define		kPAD_Z			0x0800
#define		kPAD_L			0x1000
#define		kPAD_R			0x2000

#define		kKEYSPACE		0x0010
#define		kKEYENTER		0x0020
#define		kKEY_A			0x0040
#define		kKEY_B			0x0080
#define		kKEY_C			0x0100
#define		kKEY_X			0x0200
#define		kKEY_Y			0x0400
#define		kKEY_Z			0x0800

#define		kKEY_F1			0x10000000
#define		kKEY_F2			0x20000000
#define		kKEY_F3			0x40000000
#define		kKEYESC			0x80000000

//------------------------------------------------------------------------------//
//	★	マウス管理構造体              　　                                      //
//------------------------------------------------------------------------------//
#define		_lMOUSE			0x0001
#define		_rMOUSE			0x0002
#define		MS_WHEEL_UNIT	120
typedef struct {
	short		Px,Py;
	short		Ox,Oy;
	short		Dx,Dy;
	u_short		Now;
	u_short		Old;
	u_short		Trg;
	u_short		Pul;
	u_short		WClk;
	short		Scr;
	long		Wx,Wy;
} MS_PRIM;

// マウス情報
extern		MS_PRIM		Mouse;

// マウス領域チェック　マクロ
#define		MsAREA(x,y,w,h)		(x<Mouse.Px && x+w>Mouse.Px && y<Mouse.Py && y+h>Mouse.Py)

//------------------------------------------------------------------------------//
//	★	Window管理構造体              　　                                      //
//------------------------------------------------------------------------------//
typedef struct {
	short		Sx,Sy;
	short		Ex,Ey;
	short		Width;
	short		Height;
	short		DispX;
	short		DispY;
} WIN_PRIM;

// Window情報
extern		WIN_PRIM	mWin;

#define		AREA_WIN	1
#define		AREA_FULL	2
#define		AREA_FREE	3

//------------------------------------------------------------------------------//
//	★	シーン管理構造体  　　                                                  //
//------------------------------------------------------------------------------//
// SCENE TBL STRUCT
typedef struct {
	short	(*Load)(void);
	void	(*Init)(void);
	void	(*Task)(void);
	void	(*Draw)(void);
	void	(*Exit)(void);
} SCENE_TBL;

// シーン登録用定義文
#define		SCENE_ENTRY(no,call)	\
	extern		SCENE_TBL	call;	\
	SceneTbl[no]		= &(call)

//------------------------------------------------------------------------------//
//	★	Gs LIB			  　　                                                  //
//------------------------------------------------------------------------------//

#define		GsBMPSTART				8
#define		GsBMPMAX				256
#define		GsCHRMAX				(256*8)


// BMP Management
typedef struct {
	LPDIRECT3DTEXTURE9		Bmp;
	BOOL					Flg;
} BMPHANG;

// CHAR Management
typedef struct {
	u_short					Stt;
	int						No;
	RECT					Rect;
	float					ScaleX;
	float					ScaleY;
	int						Rot;
	COLORREF				GsRGB;
	u_char					Alpha;
} CHRHANG;


//======================================================================================//
//					ＣＩ　ＬＩＢ　Ｈｅａｄｅｒ											//
//======================================================================================//
extern		HWND			hWnd;				// ウィンドゥ　ハンドル
extern		HINSTANCE		hInst;
extern		char			Font_Type[32][32];

extern		HWND			GsHwnd;
extern		BMPHANG			GsBmpHang[GsBMPMAX];


//------------------------------------------------------------------//
//	★	ＧｓＬｉｂ　Ｅｘｔｅｒｎ宣言                                //
//------------------------------------------------------------------//
// マスター制御
extern		BOOL		Gs_Init(HWND hWnd);
extern		BOOL		Gs_Exit(void);

// パッド関連
extern		BOOL		Gs_PADInfo(int id);
extern		BOOL		Gs_CheckPAD(void);
extern		bool		Gs_GetPAD(int id,int x);
extern		bool		Gs_GetPADtrg(int id,int x);
extern		bool		Gs_GetPADpul(int id,int x);

// キーボード関連
extern		BOOL		Gs_CheckKEY(void);
extern		bool		Gs_GetKEY(int x);
extern		bool		Gs_GetKEYtrg(int x);
extern		bool		Gs_GetKEYpul(int x);
extern		bool		Gs_GetVKEY(int x);

extern		void		Gs_MousePosSet(int x,int y);
extern		void		Gs_MouseArea(int sw);
extern		void		Gs_MouseMoveArea(int sx,int sy,int w,int h);

// ファイル操作
extern		bool		Gs_FileDataWrite(char *fn,char *dst,int num);
extern		bool		Gs_FileDataLoad(char *fn,char *dst,int num);
extern		bool		Gs_TextWriteFILE(char *fn,char* Format,...);

extern		bool		Gs_ClearFILE(char *fn);
extern		bool		Gs_FindFILE(char *fn);

// 描画系
extern		void		Gs_BeginDraw(void);
extern		void		Gs_EndDraw(void);
extern		void		Gs_DrawClear(void);
extern		void		Gs_DrawScene(void);
extern		void		Gs_DrawPresent(void);
extern		void		Gs_Begin2D(void);
extern		void		Gs_End2D(void);
extern		void		Gs_Begin3D(void);
extern		void		Gs_End3D(void);
extern		void		Gs_BeginAlPha(void);
extern		void		Gs_EndAlPha(void);
extern		void		Gs_Begin2DFront(void);
extern		void		Gs_Begin2DBack(void);
extern		void		Gs_DrawClear2(void);

extern		GsTEXTURE	Gs_LoadBMP(LPCTSTR fName);
extern		BOOL		Gs_ReleaseBMP(int ID);

extern		void		Gs_DrawDirectLayer(int px,int py,GsTEXTURE no,int x1,int y1,int w,int h,bool rev,GsCOLOR argb);
extern		void		Gs_DrawLayer(int px,int py,GsTEXTURE no,int x1,int y1,int w,int h,bool rev,GsCOLOR argb,short gen,short rot,float scx,float scy);
extern		void		Gs_DrawLine(int sx,int sy,int len,float ang,GsCOLOR argb);
extern		void		Gs_DrawLineTo(int sx,int sy,int ex,int ey,GsCOLOR argb,int size);
extern		void		Gs_DrawBox(int sx,int sy,int ex,int ey,GsCOLOR argb,bool nuri);
extern		void		Gs_DrawPixel(int x,int y,GsCOLOR argb,int sc);
extern		void		Gs_DrawCircle(int x,int y,float r,int sz,GsCOLOR argb,bool nuri);
extern		void		Gs_DrawArc(int x,int y,float r,int line,GsCOLOR argb,float s_ang,float e_ang);

extern		BOOL		Gs_DrawText(int x,int y,char *str,GsCOLOR argb);
extern		BOOL		Gs_FontSet(int no,int size);

extern		void		Gs_FadeDraw(void);
extern		BOOL		Gs_FadeCheck(void);
extern		void		Gs_FadeOut(COLORREF col,float time);
extern		void		Gs_FadeIn(COLORREF col,float time);


//------------------------------------------------------------------//
//	★	ＷＬｉｂ　Ｅｘｔｅｒｎ宣言									//
//------------------------------------------------------------------//
extern		DWORD		SyncTime(void);
extern		BOOL		SyncWait(DWORD t);
extern		DWORD		SyncFPS(void);
extern		void		System_FPS(int x,int y);

extern		char		*FindCHR(char *str,char code);
extern		char		*FindSTR(char *str,char *key);

extern		void		System_Message(HWND hwnd, char* Format,...);
extern		int			System_MessageYN(HWND hwnd, char* Format,...);

extern		void		System_MakeWindow(char *cLass,char *tItle,int sTyle,int SizeX,int SizeY);

extern		void		Gs_DebugInit(void);
extern		void		Gs_DebugDraw(void);
extern		void		Gs_DebugOut(char* Format,...);
extern		void		Gs_DebugOutXY(int x,int y,char* Format,...);
extern		void		Gs_SystemText(int x,int y,GsCOLOR argb,char* Format,...);

extern		void		Gs_KeyScanSTART(void);
extern		void		Gs_KeyScanSTOP(void);
extern		void		Gs_KeyScanTask(void);
extern		void		Gs_KeyDataPLAY(void);
extern		void		Gs_KeyDataSTOP(void);

//------------------------------------------------------------------//
//	★	ＳＣＥＮＥ　Ｅｘｔｅｒｎ宣言                                //
//------------------------------------------------------------------//
extern		int			SceneMode(void);							
extern		int			SceneOld(void);
extern		BOOL		SceneCheck(void);
extern		BOOL		SceneExport(void);
extern		void		SceneChange(int no);
extern		void		SceneBack(void);
extern		void		SceneStart(void);
extern		void		SceneInit(void);
extern		void		SceneExit(int x);
extern		BOOL		SceneExitCheck(void);
extern		int			SceneDraw(int n);
extern		void		SceneClear(bool x);
extern		BOOL		SceneClearCheck(void);

extern		void		_NOP	(void);
extern		bool		_NOPR	(void);

//------------------------------------------------------------------//
//	★	ＭＵＳＩＣ　Ｅｘｔｅｒｎ宣言                                //
//------------------------------------------------------------------//
extern		GsSOUND		Gs_LoadWAVE(char *fname,bool rept);
extern		GsSOUND		Gs_LoadMIDI(char *fname,bool rept);
extern		void		Gs_ReleaseSOUND(GsSOUND no);

extern		void		Gs_PlayBGM(GsSOUND no);
extern		void		Gs_PlaySE(GsSOUND no);
extern		void		Gs_PlaySE(GsSOUND no,int vol);		// 音量指定つきSE
extern		void		Gs_StopMUSIC(GsSOUND no);
extern		void		Gs_GetMasterVol(void);
extern		short		Gs_SetMasterVol(int vol);			// マスター（全体）ボリューム指定(-10000 ～ 1500の範囲）
extern		short		Gs_SetBGMVol(int vol);				// BGMボリューム指定(-10000 ～ 1500の範囲）
extern		short		Gs_SetSEVol(int no,int vol);		
extern		short		Gs_GetBGMInfo(void);				// 演奏中ならON


//======================================================================================//
//					３ＤＸ　ＬＩＢ　Ｈｅａｄｅｒ										//
//======================================================================================//
#ifdef DX3_PLUS

//------------------------------------------------------------------//
//	 for 3D
//------------------------------------------------------------------//
#define		SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#define		SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define		SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }

#define		MRYPR					D3DXMatrixRotationYawPitchRoll
#define		VTFC					D3DXVec3TransformCoord

typedef	struct	DIRECTION{
	float Yaw; 
	float Pit; 
	float Rol;
}DIR3;

typedef		short					GsMESH;
typedef		short					GsSKINMESH;

typedef		D3DXVECTOR3				VEC3;
typedef		D3DXMATRIX				MAT;
typedef		D3DXQUATERNION			QUA;
typedef		LPDIRECT3DSURFACE9      SF9;
typedef		D3DMATERIAL9			MAT9;
typedef		D3DCOLORVALUE			COLV;
typedef		D3DXPLANE				PLN;


extern		LPDIRECT3D9				lpD3D;							//Direct3Dオブジェクト
extern		LPDIRECT3DDEVICE9		lpD3DDev;						//Direct3DDeviceオブジェクト
extern		LPD3DXSPRITE			lpSprite;						//スプライトオブジェクト
extern		LPDIRECT3DSURFACE9		lpRenderTarget;					//オリジナルのレンダリングターゲット
extern		LPDIRECT3DSURFACE9		lpDepthStencil;					//オリジナルのZバッファ
extern      LPDIRECT3DVERTEXBUFFER9	lpVertexBuffer;
extern      D3DVIEWPORT9			Viewport;


//汎用モデル用データ
typedef struct {
	D3DXVECTOR3		Pos;
	D3DXVECTOR3		Ofs;
	DIR3			Rot;
	D3DXMATRIX		Mat;
}MODEL_PARAM;

#define		ROT3(y,x,z)		{y,x,z}

extern		GsMESH					SYS_BILB;


//------------------------------------------------------------------//
//	  3D FUNCTION EXTERN
//------------------------------------------------------------------//

extern		void		Gs_Init3D(void);
extern		void		Gs_Exit3D(void);
extern		void		Gs_3DSetUP(void);

extern		GsMESH		Gs_LoadXMesh(char *fName,char *path);
extern		BOOL		Gs_ReleaseMesh(int i);

extern		void		Gs_DrawDirectModel(float x,float y,float z,GsMESH id);
extern		void		Gs_DrawScaleModel(float x,float y,float z,GsMESH id,float scx,float scy,float scz);
extern		void		Gs_DrawModel(float x,float y,float z,GsMESH id,float scx,float scy,float scz,float yaw,float pitch,float roll);
extern		void		Gs_DrawModel(VEC3 pos,GsMESH id,float scx,float scy,float scz,DIR3 r);
extern		void		Gs_DrawModelAxis(float x,float y,float z,GsMESH id,float scx,float scy,float scz,float yaw,float pitch,float roll,float cnt,float ax,float ay,float az);
extern		void		Gs_DrawModelDx(MODEL_PARAM mod,GsMESH id);

extern		void		Gs_DrawTexture( LPDIRECT3DBASETEXTURE9 texture, float x, float y, float w, float h,float u0, float v0, float u1, float v1, D3DCOLOR diffuse);
extern		void		Gs_DrawBillboard(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy);
extern		void		Gs_DrawBillboardYPR(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy,float yaw,float pit,float rol);
extern		void		Gs_DrawMapboard(float x,float y,float z,GsTEXTURE Tid,short sx,short sy,short w,short h,float scx,float scy,float yaw,float pit,float rol);
extern		void		Gs_BillboardTexSize(int w,int h);

extern		void		Gs_DrawLayer3D(float px,float py,float pz,GsTEXTURE no,int x1,int y1,int w,int h,bool rev,GsCOLOR argb,short gen,short rot,float scx,float scy);
extern		void		Gs_DrawLine3D(float sx,float sy,float sz,float ex,float ey,float ez);

//
extern		void		Gs_CamLookAt(float x,float y, float z);
extern		void		Gs_CamPosSet(float x,float y, float z);
extern		void		Gs_CamPosture(float x,float y, float z);
extern		void		Gs_CamPrjSet(float prj,float asp, float nr,float fr);

extern		void		Gs_LightPosSet(short id,bool sw,float x,float y, float z);

//
extern		void		Gs_DrawView(void	(*draw)(void),int x,int y,int w,int h,VEC3 eye,VEC3 look);
extern		void		SetViewMatrix(VEC3 eye,VEC3 look);
extern		HRESULT		ChangeViewport(DWORD dwX,DWORD dwY,DWORD dwWidth,DWORD dwHeight);


//skinmesh
extern		GsSKINMESH		Gs_LoadSKINMESH(char* fName, char path[]);
extern		void			Gs_DrawSKINMESH(GsSKINMESH no, VEC3 Pos, MAT mRot, float Trans, float Scx, float Scy, float Scz);
extern		void			Gs_UpdateSKINMESH(GsSKINMESH no, VEC3 Pos, MAT mRot, float Scx, float Scy, float Scz);
extern		bool			Gs_ReleaseSKINMESH(GsSKINMESH no);
extern		void			Gs_DrawFRAME(GsSKINMESH no, VEC3 Pos, MAT mRot, float Trans, float Scx, float Scy, float Scz, char *FrameName);
extern		LPD3DXFRAME*    Gs_FindFRAME(GsSKINMESH no, char *FrameName);
extern		D3DXVECTOR3*	Gs_GetVERTEX(LPD3DXFRAME* pFrame);
extern		void			Gs_SetFrameDF(GsSKINMESH no, char *FrameName, bool isdrawF);
extern		void			Gs_GetFrameMat(GsSKINMESH no, char *FrameName, MAT* pMat);

//アニメション管理
extern		void			Gs_AdvanceANMTIME(GsSKINMESH no, double time);
extern		float			Gs_GetANMTIME(GsSKINMESH no);
extern		void			Gs_ResetANMTIME(GsSKINMESH no);
extern		void			Gs_SetANMTRACK(GsSKINMESH no, DWORD track, bool enable );
extern		void			Gs_SetANMWEIGHT(GsSKINMESH no, DWORD track, float weight);
extern		void			Gs_SetANMSPEED(GsSKINMESH no, DWORD track, float speed);
extern		void			Gs_SetTRACKTIME(GsSKINMESH no, DWORD track, double time);


//3D演算系
extern		void			Gs_MoveOBJECT(VEC3* pos, MAT mRot, VEC3 mDist);
extern		void			Gs_MatSLERP(MAT* mat, MAT* mat1, MAT* mat2, float ratio);
extern		void			Gs_MatAddYPR(MAT *mat, float yaw,float pit,float rol);
extern		void			Gs_MatGetYPR(DIR3 *pDir, MAT mat);
extern		float			Gs_CheckHeight(GsMESH ms,float x,float y, float z);
extern		float			Gs_CheckPitch(GsMESH id,float x,float y, float z,float yaw,float pit,float rol);
extern		float			Gs_CheckRoll(GsMESH id,float x,float y, float z,float yaw,float pit,float rol);
extern		void			Gs_LinkChild(MODEL_PARAM *pChild,MODEL_PARAM *pParent);
extern		D3DXVECTOR3		MakeVec(float dx,float dy,float dz,float yaw,float pitch,float roll);

#endif

//======================================================================================//
//	Ｅｎｄ																				//
//======================================================================================//
