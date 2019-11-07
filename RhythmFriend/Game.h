//======================================================================================//
//																						//
//				�f�������@�c����������													//
//																						//
//======================================================================================//

//------------------------------------------------------------------//
//	��	�Q�[���ʂ̒�`
//------------------------------------------------------------------//
#define		TITLE_NAME			"���Y���ӂ���"

//------------------------------------------------------------------//
//	��	�V�[���ԍ��̒�`
//------------------------------------------------------------------//

enum {
	GAME_SCENE,						//  �Q�[��
	TITLE_SCENE,                    //  �^�C�g��
	RESULT_SCENE,                   //  ���U���g
	SENTAKU_SCENE,                  //  �I�����
	TETYOU_SCENE,                  //  �I�����
	SETTEI_SCENE,                  //  �I�����

	SCENE_MAX
};

//#define		START_SCENE		TITLE_SCENE		//�@�N������̃V�[��
//#define		START_SCENE		GAME_SCENE
#define		START_SCENE		TITLE_SCENE
//#define		START_SCENE		RESULT_SCENE


//------------------------------------------------------------------//
//	��	�O���[�o���f�[�^(�V�[�����p�f�[�^)�\����
//     �V�[���ԋ��p(�󂯓n���j�f�[�^��ǉ��o�^����
//     ���e�V�[����Init���AExit���̂ݎQ�ƁA���������ł���̈�Ƃ���
//------------------------------------------------------------------//
typedef	struct {

	short	Mode;
	short	Param[8];

	int		score;
	int		time;

}GLOBAL_DATA;

//���O���[�o���f�[�^ ���p���錾�@�iDataTbl.cpp�j
extern	GLOBAL_DATA		GamePublic;


//======================================================================================//
//	�A�v���P�[�V�����@���p�錾															//
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

///////�L�[����///////


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
//	�d����																				//
//======================================================================================//
