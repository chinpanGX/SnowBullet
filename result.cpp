/*-----------------------------
	result.cpp
------------------------------*/

#include "main.h"
#include "game.h"
#include "enemy.h"
#include "input.h"
#include "scene.h"
#include "sprite.h"
#include "score_draw.h"
#include "texture.h"
#include "fade.h"
#include "sound.h"

#define RANKING_MAX 5

double ranking[RANKING_MAX];

void  ranking_sort(int score);

//�X�^�e�B�b�N�ϐ�
static bool g_bEnd = false;		//true�ŉ�ʑJ��
static double count[5];
//int i = 0;

//���U���g�̏�����
void Result_Initialize(void)
{
	ranking_sort(Enemy_GetKillCount());	//	�l�̑��
	Fade_Start(false, 90, D3DCOLOR_RGBA(0, 0, 0, 0));
	g_bEnd = false;
	PlaySound(SOUND_LABEL_BGM002);	//���U���gBGM�Đ�
}

//���U���g�̏I������
void Result_Finalize(void)
{

}

//���U���g�̍X�V
void Result_Update(void)
{	
	if (!g_bEnd)
	{
		//���U���g����^�C�g���֑J��
		if (Keyboard_IsTrigger(DIK_SPACE) || GamePad_IsTrigger(0, BUTTON_Y))
		{
			Fade_Start(true, 30, D3DCOLOR_RGBA(0, 0, 0, 0));
			g_bEnd = true;
			StopSound(SOUND_LABEL_BGM002);	//���U���gBGM��~
		}
	}
	else
	{
		if (!Fade_IsFade())
		{
			Scene_Change(SCENE_INDEX_TITLE);
		}
	}
}

//���U���g�̕`��
void Result_Draw(void)
{
	//killcount = Enemy_GetKillCount();//�ŏI�L�������Q�b�^�[���g���đ��
	Sprite_Draw(TEXTURE_INDEX_RESULT, 0.0f, 0.0f);
	
	/*---------------------------------------------------------------------------------
memo	
		���Ԃ͊֌W�Ȃ��A�����������ق����`�悳���
		
		�z��ɂȂ��Ă��邪�A�`�悪��ԏ�̔ԍ������`�悳��Ȃ��H
		���������z��ɂȂ��Ă��Ȃ��̂��H

	--------------------------------------------------------------------------------*/
	//ranking_sort(Enemy_GetKillCount());	
	
	//ranking_sort(100);
	//count = ranking_sort(15.0);

	for (int i = 0; i < RANKING_MAX; i++)	//	�`����܉�J��Ԃ�
	{
		Score_Draw(SCREEN_WIDTH / 2 - 48, SCREEN_HEIGHT / 3 + i * 32, ranking[i], 3, true);	
		//Score_Draw(SCREEN_WIDTH / 2 - 48, SCREEN_HEIGHT / 3 + 64, ranking[i+1], 3, true);
	}	
}


void ranking_sort(int score)
{
	for (int i = 0; i < RANKING_MAX; i++) 
	{//1�ʂ����r
		//���̏��ʂ���������΂��ꂪ����̏��ʂƂȂ�
		//(1�ʂ�荂�����1�ʁA1�ʂ��Ⴍ2�ʂ�荂�����2�ʁA�c)
		if (score > ranking[i])
		{
			for (int j = RANKING_MAX - 1; j > i; j--) //����̏��ʈȍ~�̃X�R�A�����炷
			{
				ranking[j] = ranking[j - 1]; 
			}
			ranking[i] = score;//����̃X�R�A���L��
			break;//�ȍ~�̏��ʂ͒��ׂȂ�
		}
	}
}
