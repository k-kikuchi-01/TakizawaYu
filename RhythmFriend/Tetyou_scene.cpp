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
//	��	�����g�p�@�ϐ�
//
//****************************************************************************

typedef struct{
	short  Fps;
	short  MiniFps;
	bool   Start;
	bool   alp_Flg[320];//�e�L�X�g�؂�ւ����G�t�F�N�g
	short  alp[320];
	short  Cnt;//�؂�ւ��G�t�F�N�g���̃J�E���g
	bool   Flg;
	bool   Back;
	short  B_Time;//�߂鎞��
	bool   B_Flg;
}CONTROLT;

typedef struct{
	bool   S_Flg;//���̃t���O�����ƍ��ڂ�����
	bool   Flg;
}CONTROLM;

typedef struct{
	short  Px,Py;
}FRIEND2;

typedef struct{
	short  type;//�ӂ��ǂ̎��
	short  page;//���݂̃y�[�W
}BOOK;

CONTROLT    ControlT;
CONTROLM    ControlM[11];
BOOK        Book;
FRIEND2      FRiend;

//--------------------------------------------------------------------------//
//	��	������		�F�V�[���Ɉڂ鎞���A�ϐ��̏�����
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
//	��	��������	�F�V�[�����̏����A���݂̃V�[���ōs������
//
//--------------------------------------------------------------------------//
// �Q�[�����C�����[�v
void	TetyouLoop(void)
{
	//DEB_TEXT("Px = %d",Mouse.Px);
	//DEB_TEXT("Py = %d",Mouse.Py);


	if(ControlT.Flg){
		//�y�[�W�߂���(��)
		if(KEY_LEFTtrg){
			Book.page ++;
			Gs_PlaySE(PERA);
			Gs_PlaySE(PERA);
		}
		if(Book.page >= 4){
			Book.page = 0;
			//Book.type ++;
		}
	}//�⑫�F���̏������ɍs��Ȃ��Ƃ��������Ȃ�



	//���菉�߂̃G�t�F�N�g
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


	//�߂鎞�̏���
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

	


	//�ڎ�
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
		if(ControlM[i].S_Flg){//�J�[�\�������炩�̖ڎ��̏�ɒu���Ă�����
			if(KEY_LEFTtrg){
				Gs_PlaySE(PERA);
				Gs_PlaySE(PERA);
				ControlM[i].Flg = true;
				ControlT.Flg = true;//���炩�̃y�[�W���J�����t���O
			}
			if(ControlM[i].Flg){
			//�ӂ���
				if(FRiend.Py <= WINH/2-50){
					FRiend.Py += 20;
				}
				Book.type = i;
			}
		}
	}//�������{�^������������Ή����������X�^�[�̐������J�n

	for(short i=0;i<11;i++){
		//DEB_TEXT("ControlT.Flg[i] = %d",ControlM[i].Flg);
	}

}              
//--------------------------------------------------------------------------//
//	��	�`��		�F�V�[�����̕`��A���݂̃V�[���ŕ\������BMP�͂�����
//
//--------------------------------------------------------------------------//
// �Q�[����ʁ@�`
void	TetyouDraw(void){

	Gs_DrawBox(0,0,WINW,WINH,ARGB(255,255,255,204),true);//�w�i
	//Gs_DrawBox(0,0,WINW,WINH,ARGB(255,0,0,0),true);//�w�i

	if(ControlT.Flg){
		Gs_DrawLayer(0,50,MEISYOU,Book.type*640,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//�ӂ��ǂ̖��́A���n
		Gs_DrawLayer(0,50,SETUMEI,640*Book.page,Book.type*960,640,960,false,ARGB(255,255,255,255),false,0,1,1);//������
		if(Book.type >= 4){
			Gs_DrawLayer(0,50,SETUMEI2,640*Book.page,(Book.type-4)*960,640,960,false,ARGB(255,255,255,255),false,0,1,1);//������
		}
		if(Book.type >= 8){
			Gs_DrawLayer(0,50,SETUMEI3,640*Book.page,(Book.type-8)*960,640,960,false,ARGB(255,255,255,255),false,0,1,1);//������
		}
	}



	//if(!ControlT.Flg[0] && !ControlT.Flg[1] && !ControlT.Flg[2] && !ControlT.Flg[3] && !ControlT.Flg[4] && !ControlT.Flg[5] && !ControlT.Flg[6] && !ControlT.Flg[7] && !ControlT.Flg[8] && !ControlT.Flg[9] && !ControlT.Flg[10]){
	for(short i=0;i<11;i++){
		if(!ControlT.Flg){
			Gs_DrawLayer(0,50,HATENA,Book.type*640,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//�ӂ��ǂ̖��́A���n
			Gs_DrawLayer(0,0,SITAZI,0,0,640,960,false,ARGB(255,245,245,245),false,0,1,1);//������

			for(short j=0;j<11;j++){
				if(ControlM[j].S_Flg){
					Gs_DrawBox(0,80+j*80,640,158+j*80,ARGB(255,255,255,170),true);
				}
			}//�I����������
			Gs_DrawLayer(0,0,MOKUZI,0,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//������
		}
	}


	if(!ControlT.Flg){
		if(!ControlT.Back){
			Gs_DrawLayer(32,14,UI_MOZI_T,0,50,512,50,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//���C�����j���[�֖߂�
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(192,255,255,255),true,0,0.25f,0.25f);//���
		}else{
			Gs_DrawLayer(32,14,UI_MOZI_T,0,50,512,50,false,ARGB(192,255,255,255),false,0,0.55f,0.55f);//���C�����j���[�֖߂�
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//���
		}
	}
	if(ControlT.Flg){
		if(!ControlT.Back){
			Gs_DrawLayer(32,14,UI_MOZI_T,0,100,512,50,false,ARGB(128,255,255,255),false,0,0.5f,0.5f);//�ڎ��֖߂�
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(192,255,255,255),true,0,0.25f,0.25f);//���
		}else{
			Gs_DrawLayer(32,14,UI_MOZI_T,0,100,512,50,false,ARGB(192,255,255,255),false,0,0.55f,0.55f);//�ڎ��֖߂�
			Gs_DrawLayer(18,26,T_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//���
		}

		if(Book.type == 0){
			Gs_DrawLayer(FRiend.Px-10,FRiend.Py,MONSTERs,512*Book.page,0,512,512,false,ARGB(255,255,255,255),true,0,0.4f,0.4f);//������
		}
		if(Book.type == 1){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-24,MONSTERs,512*Book.page,512*2,512,512,false,ARGB(255,255,255,255),true,0,0.5f,0.5f);//������
		}
		if(Book.type == 2){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-24,MONSTERs,512*Book.page,512*1,512,512,false,ARGB(255,255,255,255),true,0,0.5f,0.5f);//������
		}
		if(Book.type == 3){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*5,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//������
		}
		if(Book.type == 4){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*6,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//������
		}
		if(Book.type == 5){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*3,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//������
		}
		if(Book.type == 6){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-72,MONSTERs,512*Book.page,512*4,512,512,false,ARGB(255,255,255,255),true,0,0.6f,0.6f);//������
		}
		if(Book.type == 7){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*9,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//������
		}
		if(Book.type == 8){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*10,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//������
		}
		if(Book.type == 9){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*7,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//������
		}
		if(Book.type == 10){
			Gs_DrawLayer(FRiend.Px,FRiend.Py-96,MONSTERs,512*Book.page,512*8,512,512,false,ARGB(255,255,255,255),true,0,0.7f,0.7f);//������
		}

		//Gs_DrawLayer(450,580,T_YAZIRUSI,0,0,128,128,true,ARGB(255,255,255,255),true,0,0.3f,0.3f);//���
		//Gs_DrawLayer(600,580,T_YAZIRUSI,0,0,128,128,false,ARGB(255,255,255,255),true,0,0.3f,0.3f);//���
	}



	for(short i=0;i<320;i++){
		Gs_DrawBox(i,0,i+1,WINH,ARGB(ControlT.alp[i],240,255,240),true);//��
		Gs_DrawBox(639-i,0,639-i+1,WINH,ARGB(ControlT.alp[i],240,255,240),true);//��
	}

	if(Mouse.Px >= 20 && Mouse.Px <= 620 && Mouse.Py >= 20 && Mouse.Py <= 940){
		Gs_DrawLayer(Mouse.Px,Mouse.Py+32,YUBI_T,0,0,256,256,false,ARGB(255,255,255,255),true,0,0.25f,0.25f);//�w
	}

	if(ControlT.B_Flg){
		Gs_DrawLayer(0,0,YOMIKOMI_T,0,0,640,960,false,ARGB(255,255,255,255),false,0,1,1);//�ǂݍ���
	}
}
//--------------------------------------------------------------------------//
//	��	���[�h		�F�V�[���ɓ��鎞�A���̃V�[���ŕK�v�ȃf�[�^�̓ǂݍ���
//
//--------------------------------------------------------------------------//
short	TetyouLoad(void)
{
	YUBI_T  = Gs_LoadBMP("DATA/BMP/yubi.PNG");
	MEISYOU  = Gs_LoadBMP("DATA/BMP/meisyo.PNG");
	SETUMEI  = Gs_LoadBMP("DATA/BMP/setumei.PNG");
	SETUMEI2  = Gs_LoadBMP("DATA/BMP/setumei2.PNG");
	SETUMEI3  = Gs_LoadBMP("DATA/BMP/setumei3.PNG");
	MOKUZI  = Gs_LoadBMP("DATA/BMP/�}��.PNG");
	HATENA  = Gs_LoadBMP("DATA/BMP/�}�ӁH.PNG");
	SITAZI  = Gs_LoadBMP("DATA/BMP/�}�Ӊ��n.PNG");

	MONSTERs  =  Gs_LoadBMP("DATA/BMP/monster.PNG");

	UI_MOZI_T  = Gs_LoadBMP("DATA/BMP/UImozi.PNG");
	T_YAZIRUSI  = Gs_LoadBMP("DATA/BMP/���.PNG");

	YOMIKOMI_T  = Gs_LoadBMP("DATA/BMP/yomikomi.PNG");
	TETYOU_BGM      = Gs_LoadWAVE("DATA/SOUND/t_bgm.wav",true);
	PERA      = Gs_LoadWAVE("DATA/SOUND/pera.wav",false);
	SENTAKU_SE2 = Gs_LoadWAVE("DATA/SOUND/sentaku_se.wav",false);
	B_SE = Gs_LoadWAVE("DATA/SOUND/back_se.wav",false);
	return		TETYOU_SCENE;
}

//--------------------------------------------------------------------------//
//	��	�㏈��		�F���݂̃V�[�����甲���鎞�A�f�[�^�̔j��
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
//	���@�V�[���e�[�u���F�e�V�[���ŌĂяo�����֐��̓o�^
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