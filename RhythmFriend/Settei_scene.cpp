//======================================================================================//
//
//							�Q�[���v���O����
//
//======================================================================================//
#include		<windows.h>
#include		<stdio.h>
#include		<math.h>
#include		"MASTER/Ci-Lib.H"
#include		"Game.H"

//****************************************************************************
//	��	��`
//
//****************************************************************************

GsTEXTURE	YUBI_Se;
GsTEXTURE	YOMIKOMI_Se;
GsSOUND    Se_BGM;

//****************************************************************************
//	��	�����g�p�@�ϐ�
//
//****************************************************************************

typedef struct{
	short  Fps;
	short  MiniFps;
	bool   Start;
	bool   Flg;
	bool  alp_Flg[320];//�e�L�X�g�؂�ւ����G�t�F�N�g
	short alp[320];
	short  Cnt;//�؂�ւ��G�t�F�N�g���̃J�E���g
}CONTROLSe;

CONTROLSe    ControlSe;

//--------------------------------------------------------------------------//
//	��	������		�F�V�[���Ɉڂ鎞���A�ϐ��̏�����
//
//--------------------------------------------------------------------------//
void	SetteiInit(void)
{
	Gs_PlayBGM(Se_BGM);
	for(short i=0;i<320;i++){
		ControlSe.alp_Flg[i] = false;
		ControlSe.alp[i] = 255;
	}
	ControlSe.Fps = 0;
}

//--------------------------------------------------------------------------//
//	��	��������	�F�V�[�����̏����A���݂̃V�[���ōs������
//
//--------------------------------------------------------------------------//
// �Q�[�����C�����[�v
void	SetteiLoop(void)
{
	ControlSe.Fps += 8;
	for(short i=0;i<320;i++){
		if(ControlSe.Fps >= 320 - i){
			ControlSe.alp_Flg[i] = true;
		}
		if(ControlSe.alp_Flg[i]){
			ControlSe.alp[i] -= 10;
		}
		LIMIT(ControlSe.alp[i],0,255);
	}

	if(MsAREA(0,0,200,50)){
		if(Mouse.Trg & _lMOUSE){
			SceneChange(TITLE_SCENE);
		}
	}
}              
//--------------------------------------------------------------------------//
//	��	�`��		�F�V�[�����̕`��A���݂̃V�[���ŕ\������BMP�͂�����
//
//--------------------------------------------------------------------------//
// �Q�[����ʁ@�`��
void	SetteiDraw(void){
	Gs_DrawBox(0,0,WINW,WINH,ARGB(255,255,255,204),true);//�w�i
	Gs_DrawBox(0,0,WINW,WINH,ARGB(200,192,192,192),true);//�w�i

	for(short i=0;i<320;i++){
		Gs_DrawBox(i,0,i+1,WINH,ARGB(ControlSe.alp[i],240,240,240),true);//��
		Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(ControlSe.alp[i],240,240,240),true);//��
	}

	if(Mouse.Px >= 20 && Mouse.Px <= 620 && Mouse.Py >= 20 && Mouse.Py <= 940){
		Gs_DrawLayer(Mouse.Px,Mouse.Py+32,YUBI_Se,0,0,256,256,false,ARGB(255,255,255,255),true,0,0.25f,0.25f);//�w
	}
}
//--------------------------------------------------------------------------//
//	��	���[�h		�F�V�[���ɓ��鎞�A���̃V�[���ŕK�v�ȃf�[�^�̓ǂݍ���
//
//--------------------------------------------------------------------------//
short	SetteiLoad(void)
{
	YUBI_Se  = Gs_LoadBMP("DATA/BMP/yubi.PNG");
	YOMIKOMI_Se  = Gs_LoadBMP("DATA/BMP/yomikomi.PNG");
	Se_BGM      = Gs_LoadWAVE("DATA/SOUND/SETTEIBGM.wav",true);
	return		SETTEI_SCENE;
}

//--------------------------------------------------------------------------//
//	��	�㏈��		�F���݂̃V�[�����甲���鎞�A�f�[�^�̔j��
//
//--------------------------------------------------------------------------//
void	SetteiExit(void)
{
	Gs_ReleaseBMP(YUBI_Se);
	Gs_ReleaseBMP(YOMIKOMI_Se);
	Gs_ReleaseSOUND(Se_BGM);
}

//****************************************************************************
//	���@�V�[���e�[�u���F�e�V�[���ŌĂяo�����֐��̓o�^
//****************************************************************************

SCENE_TBL	SetteiSceneTbl = {
		SetteiLoad,
		SetteiInit,
		SetteiLoop,
		SetteiDraw,
		SetteiExit
};

//======================================================================================//
//							EOP															//
//============================================================